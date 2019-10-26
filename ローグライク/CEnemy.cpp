//////////////////////////////////////////////////
////
////	エネミークラス
////
//////////////////////////////////////////////////


//=============================================================================
//	インクルードファイル
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "CEnemy.h"
#include "CEnemy_Gen.h"
#include "CEnemy_Knight.h"
#include "CEnemy_Piel.h"
#include "CEnemy_Dwarf.h"
#include "CEnemy_Haniwa.h"
#include "CEnemy_Sekizou.h"
#include "Cplayer.h"
#include "map.h"
#include "sound.h"
#include "CWepon.h"
#include "wall.h"
#include "bilboard.h"
#include "save_load.h"
#include "Attack.h"
#include "exp.h"
#define _CRTDBG_MAP_ALLOC
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define ENEMY_HITSTOP (30)//ヒットストップの長さ
#define ENEMY_MAX (30)
#define ENEMY_CSV_NAME "CSV/ENEMY_CSV.csv"
#define ENEMY_ON_TURN (50) //エネミー出現ターン数
#define ENEMY_COND_ICON_Y (4)
//=============================================================================
//	静的変数
//=============================================================================

//	エネミー移動用構造体
int CEnemy::m_EnemyNum[TYPE_MAXENEMY] = {};
int CEnemy::m_EnemyEnableNum = 0;
int CEnemy::m_EnemyMaxNum = 0;
int CEnemy::TurnCount = 0;
int CEnemy::m_ENEMYDATA_MAX = 0;
bool CEnemy::attackflag = false;
CEnemy::ENEMY_Data CEnemy::m_EnemyData[ENEMY_MAX] = { 0 };
//=============================================================================
//	生成
//=============================================================================

CEnemy::CEnemy()
{
	m_EnemyNum[TYPE_ALL]++;
}

CEnemy::CEnemy(int EnemyType)
{
	m_Type = EnemyType;
	m_EnemyIndex = m_EnemyNum[TYPE_ALL];
	m_EnemyNum[EnemyType]++;
	m_EnemyMaxNum++;
}


//=============================================================================
//	破棄
//=============================================================================

CEnemy::~CEnemy()
{
	m_EnemyMaxNum--;
	m_EnemyNum[TYPE_ALL]--;
	//SkinMesh.cHierarchy.DestroyFrame(Thing.pFrameRoot);
	//SkinMesh.cHierarchy.DestroyMeshContainer()
}

void CEnemy::EnemyDataInit(void)
{
	int i = 0;
	int end_check = 0;//CSVの行の終わりか確認
	for (i = 0; i < ENEMY_MAX; i++)
	{//2行目から読むので+1する
		end_check = CSV_EnemyLoad(&m_EnemyData[i], i + 1);
		if (m_EnemyData[i].wepon_type == 0)
		{
			break;
		}
		m_ENEMYDATA_MAX++;
	}
}

void CEnemy::Create(int enemy_type, int x, int z)
{
	switch (enemy_type)
	{
	case TYPE_GEN:
	{
		CEnemy_Gen *penemy_gen = new CEnemy_Gen(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_KNIGHT:
	{
		CEnemy_Knight *penemy_knight = new CEnemy_Knight(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_PIEL:
	{
		CEnemy_Piel *penemy_piel = new CEnemy_Piel(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_DWARF:
	{
		CEnemy_Dwarf *penemy_dwarf = new CEnemy_Dwarf(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_HANIWA:
	{
		CEnemy_Haniwa *penemy_haniwa = new CEnemy_Haniwa(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_SEKIZOU:
	{
		CEnemy_Sekizou *penemy_sekizou = new CEnemy_Sekizou(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	}
}

void CEnemy::Enemy_Finalize(int Index)
{

}

void CEnemy::EnemyTurnEnd(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int i;
	int turnend_count;//ターンエンドカウント
	int eposX;
	int eposZ;
	attackflag = false;
	C3DObj *getplayer;
	C3DObj *enemy;
	turnend_count = 0;
	getplayer = CPlayer::Get_Player();
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{	
		enemy = CEnemy::Get_Enemy(i);
		if (!enemy)
		{
			//誰もエネミーがいなくてもターン終了処理をする
			if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION && m_EnemyMaxNum == 0 && i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_END
				|| getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION && m_EnemyMaxNum == 0 && i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_CONPLETE)
			{
				TurnCount++;
				if (!getplayer->Get_PlayerHealFlag())
				{
					if (TurnCount % getplayer->Get_OnakaTurn() == 0 && TurnCount != 0)
					{
						getplayer->Player_OnakaDown();
					}
				}
				else
				{
					if (TurnCount % getplayer->Get_OnakaTurn() / 2 == 0 && TurnCount != 0)
					{
						getplayer->Player_OnakaDown();
					}
				}
				CPlayer::Player_NextTurn();
				if (TurnCount % ENEMY_ON_TURN == 0 && TurnCount != 0)
				{
					do
					{
						eposX = mt() % MAX_MAPWIDTH;
						eposZ = mt() % MAX_MAPHEIGHT;
					} while (CMap::Map_GetData(eposZ, eposX).type != 1);

					CMap::MapEnemySingleSet();
				}
			}
		}
		// エネミーがいない場合はコンティニュー
		if (!enemy)
		{
			continue;
		}
		if (enemy->Get_EnemyTurn() == ENEMY_TURN_END)
		{
			turnend_count++;
		}
		// 敵全員のターンが終了
		if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION && turnend_count == m_EnemyMaxNum && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_END
			|| getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION && turnend_count == m_EnemyMaxNum && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_CONPLETE)
		{
			TurnCount++;
			CPlayer::Player_NextTurn();
			if (!getplayer->Get_PlayerHealFlag())
			{
				if (TurnCount % getplayer->Get_OnakaTurn() == 0 && TurnCount != 0)
				{
					getplayer->Player_OnakaDown();
				}
			}
			else
			{
				if (TurnCount % getplayer->Get_OnakaTurn() / 2 == 0 && TurnCount != 0)
				{
					getplayer->Player_OnakaDown();
				}
			}
			// 数ターンごとにランダムで敵を生成
			if (TurnCount % ENEMY_ON_TURN == 0 && TurnCount != 0)
			{
				do
				{
					eposX = mt() % MAX_MAPWIDTH;
					eposZ = mt() % MAX_MAPHEIGHT;
				} while (CMap::Map_GetData(eposZ, eposX).type != 1);

				CMap::MapEnemySingleSet();
			}
		}
	}

}

void CEnemy::EnemyVsWall(JUDGE *enemy_judge, Sphere *m_EnemyMyColision)
{
	int i;
	int c;
	for (i = 0; i < WALL_MAX; i++)
	{
		if (CWall::Wall_GetData(i).wuse)
		{
			for (c = 0; c < WALL_WIDTH_MAX; c++)
			{
				// エネミーと壁の当たり判定
				if (Collision_IsHitVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
				{

					if (Collision_IsHitTopVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitTop = true;
						enemy_judge->HitTopLeft = true;
						enemy_judge->HitTopRight = true;
					}

					if (Collision_IsHitBottomVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitBottom = true;
						enemy_judge->HitBottomLeft = true;
						enemy_judge->HitBottomRight = true;
					}

					if (Collision_IsHitRightVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitRight = true;
						enemy_judge->HitTopRight = true;
						enemy_judge->HitBottomRight = true;
					}

					if (Collision_IsHitLeftVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitLeft = true;
						enemy_judge->HitTopLeft = true;
						enemy_judge->HitBottomLeft = true;
					}

				}
			}
		}
	}
}

C3DObj *CEnemy::Get_Enemy(int index)
{
	C3DObj *penemy = C3DObj::Get(index);
	if (penemy)
	{
		if (penemy->Get_3DObjType() == C3DObj::TYPE_ENEMY)
		{
			return penemy;
		}
	}
	return NULL;
}

void CEnemy::DeleteAllEnemy(void)
{
	int i;
	C3DObj *enemy;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);

		if (enemy)
		{
			delete enemy;
		}
	}
}

int CEnemy::CSV_EnemyLoad(ENEMY_Data* enemydata, const int num)
{
	int cnt = 0;
	char buf[128];
	int size = 128;
	int data[20] = { 0 };
	FILE* file = NULL;

	if ((file = fopen(ENEMY_CSV_NAME, "r")) == NULL)
	{
		MessageBox(NULL, "ENEMY_CSVを読み込めませんでした\nCSVを保存しなおせば直る可能性があります", "エラー", MB_OK);
		exit(1);
		return -1;
	}
	//データを読み込む
	while (fgets(buf, size, file) != NULL && num > ++cnt);
	fscanf(file, "%d,%d,%[^,],%f,%d,%d,%d,%d,%d,%d,%d",&enemydata->enemy_type, &enemydata->wepon_type, enemydata->enemy_name , &enemydata->Hp, &enemydata->str, &enemydata->def, &enemydata->exp, &enemydata->gold, &enemydata->first_floor, &enemydata->end_floor, &enemydata->enemychance);

	fclose(file);



	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}

void CEnemy::Enemy_Update(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();

	//吹き飛んでるときは更新しない
	if (m_Back == false)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;


		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
	}

	if (m_Warp == true)
	{
		Enemy_WarpMove();
	}
	else if (m_Back == true)
	{
		Enemy_BackMove();
	}
	else if (m_Back == false)
	{
		if (m_Condition == SLEEP_CONDITION)
		{
			if (m_TurnCount < ENEMY_SLEEP_HEALTURN)
			{
				enemyturn = ENEMY_TURN_END;
			}
		}
		else if (m_Condition != SLEEP_CONDITION)
		{
			switch (getplayer->Get_PlayerTurn())
			{
			case CPlayer::PLAYER_KEY_INPUT:
			case CPlayer::PLAYER_SERECT_UI:
			case CPlayer::PLAYER_WINDOW:
			case CPlayer::PLAYER_ITEM_WAIT:
				// なにもしない
				break;

			case CPlayer::PLAYER_MOVE:
			case CPlayer::PLAYER_WARP_WAIT:
				//プレイヤーが移動中にオブジェクトに当たった時は殴らない
				//プレイヤーが複数攻撃中も
				if (enemyturn == ENEMY_ACTION && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WARP_WAIT)
					enemyturn = ENEMY_TURN_END;

				if (getplayer->m_Judge_player.HitItem || getplayer->Get_PlayerTurn() == CPlayer::PLAYER_RANGEHIT_WAIT)
				{
					if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_MOVE)
						break;
				}
			case CPlayer::PLAYER_ACT_END:
			case CPlayer::PLAYER_MOVE_END:
			case CPlayer::PLAYER_TURN_END:
				if (!m_Judge_player.HitItem)
				{
					if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION)
					{
						Enemy_AI();
					}
				}
				break;
			case CPlayer::PLAYER_TURN_CONPLETE:
				if (!m_Judge_player.HitItem)
				{
					if (getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION)
					{
						Enemy_AI();
					}
				}
				break;
			}
		}
	}
	// AI処理
	//壁判定

}

void CEnemy::Enemy_Draw(void)
{
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);
	DrawDX_Anime(m_mtxWorld, &anime_model);
	//DrawDX_Normal(m_mtxWorld, &Normal_model);
	if (m_Str2 >= CHARGE_BUFF)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + 4, m_Position.z), CTexture::TEX_PLAYER_CHARGE_ICON);
	}
	if (m_Condition == POIZUN_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_POIZUN_ICON);
	}
	if (m_Condition == SLEEP_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_SLEEP_ICON);
	}
	if (m_Condition == KURAYAMI_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_MEKAKUSI_ICON);
	}
	if (m_Condition == MAHUJI_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_MAHUJI_ICON);
	}
}

void CEnemy::Enemy_AI(void)
{
	int i = 0;
	int e = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
	std::random_device rd;
	std::mt19937 mt(rd());
	get_turbo = getplayer->Get_TurboMode();
	// ターンが終わってる敵の数をカウント

	// ワンフレームに攻撃できる敵をカウント
	// 攻撃待ちカウント
	int enemyturncount = 0;
	// プレイヤーが移動するか移動以外の行動が終わった場合エネミーも移動する
	switch (enemyturn)
	{
	case ENEMY_WAIT:
		// 現在地の更新
		//m_Mapz = (m_Position.z - 247.5f) / -5;
		//m_Mapx = (m_Position.x + 247.5f) / 5;
		C3DObj *enemy;
		C3DObj *enemy2;


		for (i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{
				if (i != MAX_GAMEOBJ - 1)
				{
					for (e = 0; e < MAX_GAMEOBJ; e++)
					{
						if (i != e)
						{
							enemy2 = CEnemy::Get_Enemy(e);
							if (enemy2)
							{
								if (enemy->m_Mapx == enemy2->m_Mapx && enemy->m_Mapz == enemy2->m_Mapz - 1)
								{
									enemy->m_Judge_enemy.HitBottom = true;
									enemy2->m_Judge_enemy.HitTop = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx && enemy->m_Mapz == enemy2->m_Mapz + 1)
								{
									enemy->m_Judge_enemy.HitTop = true;
									enemy2->m_Judge_enemy.HitBottom = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx + 1 && enemy->m_Mapz == enemy2->m_Mapz)
								{
									enemy->m_Judge_enemy.HitLeft = true;
									enemy2->m_Judge_enemy.HitRight = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx - 1 && enemy->m_Mapz == enemy2->m_Mapz)
								{
									enemy->m_Judge_enemy.HitRight = true;
									enemy2->m_Judge_enemy.HitLeft = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx - 1 && enemy->m_Mapz == enemy2->m_Mapz - 1)
								{
									enemy->m_Judge_enemy.HitBottomRight = true;
									enemy2->m_Judge_enemy.HitTopLeft = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx - 1 && enemy->m_Mapz == enemy2->m_Mapz + 1)
								{
									enemy->m_Judge_enemy.HitTopRight = true;
									enemy2->m_Judge_enemy.HitBottomLeft = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx + 1 && enemy->m_Mapz == enemy2->m_Mapz + 1)
								{
									enemy->m_Judge_enemy.HitTopLeft = true;
									enemy2->m_Judge_enemy.HitBottomRight = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx + 1 && enemy->m_Mapz == enemy2->m_Mapz - 1)
								{
									enemy->m_Judge_enemy.HitBottomLeft = true;
									enemy2->m_Judge_enemy.HitTopRight = true;
								}
							}
						}
					}
				}

			}
		}
		// 通路での斜め攻撃防止
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type != 2 || !m_Judge_enemy.Hitnaname)
		{
			// プレイヤーが通路にいる場合斜め攻撃しない
			if (!m_Judge_enemy.Hitnaname || CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2)
			{
				// 1マス先にプレイヤーがいる場合攻撃する
				if (m_Judge_enemy.HitEnemy)
				{
					enemyturn = ENEMY_ACTION;
					attackframe = 0;
					break;
				}
			}
		}

		//===================================================
		// エネミーAI
		//===================================================

		//===================================================
		// 目的地を決める
		//===================================================
		// まず自分がいる足場を調べる					// フロア
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 1)
		{
			// プレイヤーが同じフロアにいるか調べる
			if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 1 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group
				|| CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 3 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group)
			{
				// プレイヤーの位置を目的地にする
				m_Goalz = getplayer->m_Mapz;
				m_Goalx = getplayer->m_Mapx;
			}	// 目的地に着いた場合
			else if (m_Mapz == m_Goalz&&
				m_Mapx == m_Goalx)
			{
				// フロア内の目的地を決める
				do
				{
					m_Goalz = mt() % MAX_MAPHEIGHT;
					m_Goalx = mt() % MAX_MAPWIDTH;
				} while (CMap::Map_GetData(m_Goalz, m_Goalx).type != 1 &&
					CMap::Map_GetData(m_Goalz, m_Goalx).type != 3 ||
					CMap::Map_GetData(m_Goalz, m_Goalx).Group != CMap::Map_GetData(m_Mapz, m_Mapx).Group ||
					m_Goalz == m_Mapz && m_Goalx == m_Mapx);
			}
		}

		// まず自分がいる足場を調べる					// フロア出入口
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 3)
		{
			// スイッチ
			if (!m_Goway)
				m_Goway = true;
			else
				m_Goway = false;

			// プレイヤーが同じフロアにいるか調べる
			if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 1 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group
				|| CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 3 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group)
			{
				// プレイヤーの位置を目的地にする
				m_Goalz = getplayer->m_Mapz;
				m_Goalx = getplayer->m_Mapx;
			}
			else
			{
				// 通路のほうに進む
				if (m_Goway)
				{
					// 目的地を通路へ
					// 上の通路
					if (CMap::Map_GetData(m_Mapz - 1, m_Mapx).type == 2 && !m_Judge_enemy.HitTop)
					{
						m_Goalz = m_Mapz - 1;
						m_Goalx = m_Mapx;
					}
					// 下の通路
					if (CMap::Map_GetData(m_Mapz + 1, m_Mapx).type == 2 && !m_Judge_enemy.HitBottom)
					{
						m_Goalz = m_Mapz + 1;
						m_Goalx = m_Mapx;
					}
					// 左の通路
					if (CMap::Map_GetData(m_Mapz, m_Mapx - 1).type == 2 && !m_Judge_enemy.HitLeft)
					{
						m_Goalz = m_Mapz;
						m_Goalx = m_Mapx - 1;
					}
					// 右の通路
					if (CMap::Map_GetData(m_Mapz, m_Mapx + 1).type == 2 && !m_Judge_enemy.HitRight)
					{
						m_Goalz = m_Mapz;
						m_Goalx = m_Mapx + 1;
					}

				}
				// フロアのほうに進む
				else
				{
					// フロア内の目的地を決める
					do
					{
						m_Goalz = mt() % MAX_MAPHEIGHT;
						m_Goalx = mt() % MAX_MAPWIDTH;
					} while (CMap::Map_GetData(m_Goalz, m_Goalx).type != 1 &&
						CMap::Map_GetData(m_Goalz, m_Goalx).type != 3 ||
						CMap::Map_GetData(m_Goalz, m_Goalx).Group != CMap::Map_GetData(m_Mapz, m_Mapx).Group ||
						m_Goalz == m_Mapz && m_Goalx == m_Mapx);
				}
			}
		}

		// まず自分がいる足場を調べる					// 通路
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 2)
		{
			// 後ろに戻らず進み続ける
			// 上に進んでる場合
			if (m_Angle == 0.0f)
			{
				if (!m_Judge_enemy.HitTop)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
				// 左へ
				else if (!m_Judge_enemy.HitLeft)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
				else if (!m_Judge_enemy.HitRight)// 右へ
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
				else if (!m_Judge_enemy.HitBottom)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
			}

			// 右に進んでる場合
			if (m_Angle == 1.6f)
			{
				if (!m_Judge_enemy.HitRight)// 右へ
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
				// 
				else if (!m_Judge_enemy.HitTop)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitBottom)// 右へ
				{
					m_Goalz = m_Mapz + 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitLeft)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
			}

			// 下に進んでる場合
			if (m_Angle == 3.2f)
			{
				if (!m_Judge_enemy.HitBottom)
				{
					m_Goalz = m_Mapz + 1;
					m_Goalx = m_Mapx;
				}
				// 左へ
				else if (!m_Judge_enemy.HitRight)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
				else if (!m_Judge_enemy.HitLeft)// 右へ
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
				else if (!m_Judge_enemy.HitTop)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
			}

			// 左に進んでる場合
			if (m_Angle == 4.8f)
			{
				if (!m_Judge_enemy.HitLeft)// 右へ
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
				// 左へ
				else if (!m_Judge_enemy.HitBottom)
				{
					m_Goalz = m_Mapz + 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitTop)// 右へ
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitRight)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
			}
		}

		// 1マス先にプレイヤーがいない場合移動する
		//===================================================
		// 上移動
		//===================================================
		if (m_Goalz < m_Mapz && m_Goalx == m_Mapx)
		{
			if (!m_Judge_enemy.HitTop)
			{
				Enemy_TopMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopLeft)
			{
				Enemy_TopLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopRight)
			{
				Enemy_TopRightMoveCheck();
				break;
			}
		}

		//===================================================
		// 下移動
		//===================================================
		if (m_Goalz > m_Mapz && m_Goalx == m_Mapx)
		{
			if (!m_Judge_enemy.HitBottom)
			{
				Enemy_BottomMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomRight)
			{
				Enemy_BottomRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomLeft)
			{
				Enemy_BottomLeftMoveCheck();
				break;
			}
		}

		//===================================================
		// 右移動
		//===================================================
		if (m_Goalz == m_Mapz && m_Goalx > m_Mapx)
		{
			if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopRight)
			{
				Enemy_TopRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomRight)
			{
				Enemy_BottomRightMoveCheck();
				break;
			}
		}

		//===================================================
		// 左移動
		//===================================================
		if (m_Goalz == m_Mapz && m_Goalx < m_Mapx)
		{
			if (!m_Judge_enemy.HitLeft)
			{
				Enemy_LeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomLeft)
			{
				Enemy_BottomLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopLeft)
			{
				Enemy_TopLeftMoveCheck();
				break;
			}
		}

		//===================================================
		// 左上移動
		//===================================================
		if (m_Goalz < m_Mapz && m_Goalx < m_Mapx)
		{
			if (!m_Judge_enemy.HitTopLeft)
			{
				Enemy_TopLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitLeft)
			{
				Enemy_LeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTop)
			{
				Enemy_TopMoveCheck();
				break;
			}
		}

		//===================================================
		// 左下移動
		//===================================================
		if (m_Goalz > m_Mapz && m_Goalx < m_Mapx)
		{
			if (!m_Judge_enemy.HitBottomLeft)
			{
				Enemy_BottomLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottom)
			{
				Enemy_BottomMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitLeft)
			{
				Enemy_LeftMoveCheck();
				break;
			}
		}

		//===================================================
		// 右下移動
		//===================================================
		if (m_Goalz > m_Mapz && m_Goalx > m_Mapx)
		{
			if (!m_Judge_enemy.HitBottomRight)
			{
				Enemy_BottomRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
		}

		//===================================================
		// 右上移動
		//===================================================
		if (m_Goalz < m_Mapz && m_Goalx > m_Mapx)
		{
			if (!m_Judge_enemy.HitTopRight)
			{
				Enemy_TopRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTop)
			{
				Enemy_TopMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
		}

		enemyturn = ENEMY_MOVE_END;
		if (m_TimeWarp)
		{
			m_Warp = true;
		}
		break;
		//===================================================
		// 移動中
		//===================================================
	case ENEMY_MOVE:
		if (get_turbo)
		{
			Enemy_TurboMove();
		}
		else
		{
			Enemy_Move();
		}
		break;

		//===================================================
		// 移動完了
		//===================================================
	case ENEMY_MOVE_END:
		enemyturn = ENEMY_TURN_END;
		Animation_Change(IDLE, m_IdleAnimeTime);
		
		break;

		//===================================================
		// 攻撃中
		//===================================================
	case ENEMY_ACTION:
		if (attackflag)	// 1フレームに2体以上同時に攻撃させない
			break;

		attackflag = true;

		//if (enemyturn == ENEMY_MOVE)
		//{
		//	enemyturncount++;
		//}
		if (enemyturncount == 0)
		{
			// エネミー用攻撃関数
			Enemy_Act();
		}
		break;

		//===================================================
		// 攻撃完了
		//===================================================
	case ENEMY_ACT_END:
		enemyturn = ENEMY_TURN_END;
		Animation_Change(IDLE, 0.005f);
		
		break;
	}
	m_Oldmz = m_Mapz;
	m_Oldmx = m_Mapx;
	m_Mapz = (int)(m_EnemyMyColision.position.z - 247.5f) / -5;
	m_Mapx = (int)(m_EnemyMyColision.position.x + 247.5f) / 5;
	CMap::MapEnemyPosSet(m_Mapz, m_Mapx, m_Oldmz, m_Oldmx);
}

void CEnemy::Enemy_Act(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (attackframe == 0)
	{
		Animation_Change(ATTACK, m_AttackAnimeTime);
	}
	if (attackframe < 8)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		// 左に攻撃
		if (m_Judge_enemy.HitEnemyLeft)
		{
			m_Angle = 4.8f;
			if (attackframe < 4)
			{
				m_Position -= m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 上に攻撃
		if (m_Judge_enemy.HitEnemyTop)
		{
			m_Angle = 0.0f;
			if (attackframe < 4)
			{
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 右に攻撃
		if (m_Judge_enemy.HitEnemyRight)
		{
			m_Angle = 1.6f;
			if (attackframe < 4)
			{
				m_Position += m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 下に攻撃
		if (m_Judge_enemy.HitEnemyBottom)
		{
			m_Angle = 3.2f;
			if (attackframe < 4)
			{
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 右上に攻撃
		if (m_Judge_enemy.HitEnemyTopRight)
		{
			m_Angle = 0.8f;
			if (attackframe < 4)
			{
				m_Position += m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 右下に攻撃
		if (m_Judge_enemy.HitEnemyBottomRight)
		{
			m_Angle = 2.4f;
			if (attackframe < 4)
			{
				m_Position += m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 左下に攻撃
		if (m_Judge_enemy.HitEnemyBottomLeft)
		{
			m_Angle = 4.0f;
			if (attackframe < 4)
			{
				m_Position -= m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// 左上に攻撃
		if (m_Judge_enemy.HitEnemyTopLeft)
		{
			m_Angle = 5.6f;
			if (attackframe < 4)
			{
				m_Position -= m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}
	}
	attackframe++;
	if (attackframe == 5)
	{
		EnemySkill();
		//攻撃スキルのみ↓を書く
		//m_Str2 = 1.0;
		// 向いてる方向、攻撃力、攻撃したキャラの名前を渡す
		//	CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Angle);
		//	CAttack::Attack_EnemySkill(CAttack::HIGH_ATTACK_SKILL, m_WeponType, m_Type, m_Str, m_Angle);
	}
	//プレイヤーが死んだら表示時間延長
	if (attackframe == 20 && getplayer->Get_Hp() <= 0)
	{
		add_time = 25;
	}
	if (attackframe == 55 + add_time)
	{
		add_time = 0;
		enemyturn = ENEMY_ACT_END;
		attackframe = 0;
		if (getplayer->Get_Hp() <= 0)
		{
			getplayer->Set_PlayerTurn(CPlayer::PLAYER_DESTROY);
		}
		else
		{
			if (m_TimeWarp)
			{
				m_Warp = true;
			}
		}
	}
}

/*高速の時*/

void CEnemy::Enemy_TurboMove(void)
{
	if (walkf == 0 || walkf == 56 || walkf == 24 || walkf == 30 || walkf == 40 || walkf == 100)
	{
		Animation_Change(WALK, m_WalkAnimeTime);
	}
	if (walkf <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		//g_player.col.position = g_player.pos;
		walkf++;
		if (walkf == 5)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf >= 96)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		//g_player.col.position = g_player.pos;
		walkf--;
		if (walkf == 95)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf == 56)
		Enemy_TurbolefttopMove();
	if (walkf == 40)
		Enemy_TurboleftbottomMove();
	if (walkf == 30)
		Enemy_TurborightbottomMove();
	if (walkf == 24)
		Enemy_TurborighttopMove();
}

void CEnemy::Enemy_Move(void)
{
	//0,56,24,30,40,105(turbo=100)
	if (walkf == 0 || walkf == 56 || walkf == 24 || walkf == 30 || walkf == 40 || walkf == 105)
	{
		Animation_Change(WALK, m_WalkAnimeTime);
	}
	if (walkf <= WALK_COUNT)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		//g_player.col.position = g_player.pos;
		walkf++;
		if (walkf == WALK_COUNT + 1)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf >= 96)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		//g_player.col.position = g_player.pos;
		walkf--;
		if (walkf == 95)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf == 56)
		Enemy_lefttopMove();
	if (walkf == 40)
		Enemy_leftbottomMove();
	if (walkf == 30)
		Enemy_rightbottomMove();
	if (walkf == 24)
		Enemy_righttopMove();
}

void CEnemy::Enemy_lefttopMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 0.5f;

		m_Position -= vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_righttopMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 0.5f;

		m_Position += vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_leftbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 0.5f;

		m_Position -= vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_rightbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 0.5f;

		m_Position += vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurbolefttopMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 1.0f;

		m_Position -= vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurborighttopMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 1.0f;

		m_Position += vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurboleftbottomMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 1.0f;

		m_Position -= vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurborightbottomMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 1.0f;

		m_Position += vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_LeftMoveCheck(void)
{
	m_Angle = 4.8f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;

	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	if (get_turbo)
	{
		walkf = 100;
	}
	else
	{
		walkf = 100 + WALK_COUNT - 4;
	}
	vecenemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_RightMoveCheck(void)
{
	m_Angle = 1.6f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;


	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 0;
	vecenemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BottomMoveCheck(void)
{
	m_Angle = 3.2f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;

	m_Position += m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	if (get_turbo)
	{
		walkf = 100;
	}
	else
	{
		walkf = 100 + WALK_COUNT - 4;
	}
	vecenemy = m_Front;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_TopMoveCheck(void)
{

	m_Angle = 0.0f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;
	//g_player.col.position = g_player.walkpos;

	m_Position -= m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 0;
	vecenemy = m_Front;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}

}

void CEnemy::Enemy_TopLeftMoveCheck(void)
{
	m_Angle = 5.6f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;


	m_Position -= m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 56;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_TopRightMoveCheck(void)
{
	m_Angle = 0.8f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;


	m_Position -= m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 24;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BottomLeftMoveCheck(void)
{
	m_Angle = 4.0f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;


	m_Position += m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 40;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BottomRightMoveCheck(void)
{
	m_Angle = 2.4f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// 当たり判定の設定
	m_EnemyMyColision.position = m_Position;

	m_Position += m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 30;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BackMove(void)
{//後で吹っ飛び先にモンスターがいたときの処理も書くこと
	C3DObj *getplayer;
	getplayer = CPlayer::Get_Player();
	if (getplayer && this)
	{
		//下向いてたら上に飛ぶ
		if (getplayer->Get_Angle() == 3.2f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapz = m_Mapz - 1;
				}
				m_Position.z += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 3.2f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz - 1, m_Mapx).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.z -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//上向いてたら下に飛ぶ
		else if (getplayer->Get_Angle() == 0.0f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapz = m_Mapz + 1;
				}
				m_Position.z -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 0.0f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz + 1, m_Mapx).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.z += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//左向いてたら右に飛ぶ
		else if (getplayer->Get_Angle() == 4.8f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz, m_Mapx + 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx + 1;
				}
				m_Position.x += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 4.8f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz, m_Mapx + 1).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.x -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//右向いてたら左に飛ぶ
		else if (getplayer->Get_Angle() == 1.6f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz, m_Mapx - 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx - 1;
				}
				m_Position.x -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 1.6f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz, m_Mapx - 1).type == 0)
			{//壁にぶつかったら止める
			 //フレームが0（つまり壁際）だったら移動しない
				if (m_BackFrame != 0)
				{
					m_Position.x += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//右上向いてたら左下に飛ぶ
		else if (getplayer->Get_Angle() == 0.8f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx - 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx - 1;
					m_Mapz = m_Mapz + 1;
				}
				m_Position.x -= 0.5f;
				m_Position.z -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 0.8f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz + 1, m_Mapx - 1).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.x += 0.5f;
					m_Position.z += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//左上向いてたら右下に飛ぶ
		else if (getplayer->Get_Angle() == 5.6f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx + 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx + 1;
					m_Mapz = m_Mapz + 1;
				}
				m_Position.x += 0.5f;
				m_Position.z -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 5.6f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz + 1, m_Mapx + 1).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.x -= 0.5f;
					m_Position.z += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//左下向いてたら右上に飛ぶ
		else if (getplayer->Get_Angle() == 4.0f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx + 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx + 1;
					m_Mapz = m_Mapz - 1;
				}
				m_Position.x += 0.5f;
				m_Position.z += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 4.0f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz - 1, m_Mapx + 1).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.x -= 0.5f;
					m_Position.z -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//右下向いてたら左上に飛ぶ
		else if (getplayer->Get_Angle() == 2.4f)
		{//壁にぶつかるまで飛ぶ
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx - 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx - 1;
					m_Mapz = m_Mapz - 1;
				}
				m_Position.x -= 0.5f;
				m_Position.z += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 2.4f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz - 1, m_Mapx - 1).type == 0)
			{//壁にぶつかったら止める
				if (m_BackFrame != 0)
				{
					m_Position.x += 0.5f;
					m_Position.z -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
	}
}

void CEnemy::Enemy_WarpMove(void)
{
	if (m_BackFrame <= 20)
	{
		m_Position.y += 1;
	}
	if (m_BackFrame == 21)
	{
		CMap::WorpEnemySet(this, m_Mapz, m_Mapx);
	}
	if (m_BackFrame> 20 && m_BackFrame < WAIT_FRAME - 5)
	{
		m_Position.y -= 1;
		if (m_Position.y < 0)
		{
			m_Position.y = 0.0f;
		}
	}
	if (m_BackFrame == WAIT_FRAME - 5)
	{
		m_Position.y = 0.0f;
		m_BackFrame = 0;
		m_Warp = false;
		m_TimeWarp = false;
	}
	else
	{
		m_BackFrame++;
	}
}

bool CEnemy::Damage(int str, float angle, int week_type)
{
	// 後にダメージエフェクトを作成
	if (str > 0)
	{
		Sleep(HITSTOP);
		if (week_type == CUserinterface::NORMAL_TYPE)
		{
			Exp_Set(HIT, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		}
		if (week_type == CUserinterface::WEEK_TYPE)
		{
			Exp_Set(HIT_WEEK, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		}
		if (week_type == CUserinterface::RESIST_TYPE)
		{
			Exp_Set(HIT_RESIST, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		}
		PlaySound(ENEMYDAMAGE_SE);
	}
	else//攻撃がミスの時
	{
		PlaySound(MISS_SE);
	}
	//攻撃を受けた方に向く
	if (angle == LEFT_ANGLE)
	{
		m_Angle = 1.6f;
	}
	if (angle == UP_ANGLE)
	{
		m_Angle = 3.2f;
	}
	if (angle == RIGHT_ANGLE)
	{
		m_Angle = 4.8f;
	}
	if (angle == DOWN_ANGLE)
	{
		m_Angle = 0.0f;
	}
	if (angle == LEFT_DOWN_ANGLE)
	{
		m_Angle = 0.8f;
	}
	if (angle == RIGHT_DOWN_ANGLE)
	{
		m_Angle = 5.6f;
	}
	if (angle == RIGHT_TOP_ANGLE)
	{
		m_Angle = 4.0f;
	}
	if (angle == LEFT_TOP_ANGLE)
	{
		m_Angle = 2.4f;
	}
	// 後にダメージ計算式を作成
	m_Hp -= str;
	// 体力が0以下で倒れる
	if (m_Hp <= 0)
	{
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->enemy_death_count += 1;
		Enemy_Destroy();
		return DEATH;
	}
	return NORMAL;
}

bool CEnemy::PoizunDamage(int str)
{
	//毒エフェクト再生すること
	// 後にダメージ計算式を作成
	m_Hp -= str;
	// 体力が0以下で倒れる
	if (m_Hp <= 0)
	{
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->enemy_death_count += 1;
		Enemy_Destroy();
		return DEATH;
	}
	return NORMAL;
}

void CEnemy::Enemy_Destroy(void)
{
	Enemy_ItemDrop();
	alive = false;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Type = CUserinterface::CHARATYPENONE;
	delete this;
}

void CEnemy::Enemy_SetWorpPos(int pposZ, int pposX)
{
	m_Position = D3DXVECTOR3(-247.5f + pposX * 5, 0.0f, 247.5f - pposZ * 5);
	m_EnemyMyColision.position = m_Position;
	m_EnemyMyColision.radius = ENEMY_RADIUS;
	// マップ位置を登録
	m_Mapx = pposX;
	m_Mapz = pposZ;

	m_Goalx = pposX;
	m_Goalz = pposZ;

	//下を向かせる
	m_Angle = 3.2f;
	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;
}

void CEnemy::Enemy_PoizunDamageStart(void)
{
	if (m_Condition == POIZUN_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_POIZUN_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
		Enemy_PoizunDamage();
	}
}

void CEnemy::Enemy_PoizunDamage(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	//これがないとプレイヤーに経験値入らず
	//毒で死んだらUIも出すこと
	//もしプレイヤーのエネミーデスがtrueだったらHP1で耐えるようにすること（1ターンに1体以上殺さない）
	if (!getplayer->Get_EnemyDeath())
	{
		if (m_Hp - ENEMY_POINZU_DAMAGE <= 0)
		{
			getplayer->SetEnemyDeath(true);
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ENEMY_POIZUN_DEATH, CUserinterface::ENEMY, 0, m_Type);
		}
	}
	//ダメージ処理も毒用のに変えること
	getplayer->Set_RivalFlag(PoizunDamage((int)ENEMY_POINZU_DAMAGE));
}

void CEnemy::Enemy_ConditionCount(void)
{
	if (m_Condition == SLEEP_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_SLEEP_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
	}
	if (m_Condition == KURAYAMI_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_KURAYAMI_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
	}
	if (m_Condition == MAHUJI_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_TOKUGI_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
	}
}

void CEnemy::Enemy_ItemDrop(void)
{
	//アイテムを持ってたらその場にアイテムを置く
	if (set_item != 0)
	{
		CMap::MapItemPosSet(set_item, m_Mapx, m_Mapz);
	}
}