#include "CEnemy.h"
#include <d3dx9.h>
#include "CEnemy_Srime.h"
#include "input.h"
#include "CPlayer.h"
#include "sound.h"
#include "userinterface.h"
#include "debug_font.h"
#include "Attack.h"
#include "save_load.h"
#include "map.h"
#include "collision.h"
#include "bilboard.h"
#include "exp.h"
#include "C2DObj.h"
#include "C3DObj.h"
#define MAXHP (20)
#define STR (4)
//=============================================================================
//	生成
//=============================================================================


CEnemy_Srime::CEnemy_Srime(int x, int z, ENEMY_Data enemy_data) :CEnemy(TYPE_SRIME), C3DObj(C3DObj::TYPE_ENEMY)
{
	Initialize(x, z, enemy_data);
}

//=============================================================================
//	破棄
//=============================================================================

CEnemy_Srime::~CEnemy_Srime()
{
	NormalModel_Finalize(&Normal_model);
	//delete this;
}


void CEnemy_Srime::Initialize(int x, int z, ENEMY_Data enemy_data)
{
	strcpy_s(name, MAX_NAME, enemy_data.enemy_name);
	m_EnemyIndex = Get_EnemyIndex(TYPE_ALL);
	//Normal_model = GetNormalModel(MODELL_ENEMY_1);
	C3DObj::InitNormalModelLoad(&Normal_model, "asset/model/piel_wepon.x");
	TurnCount = 0;
	m_Condition = NORMAL_CONDITION;
	get_turbo = false;
	m_WeponType = enemy_data.wepon_type;
	add_time = 0;
	alive = true;
	rival_flag = false;
	map_drawflag = false;
	m_Position = D3DXVECTOR3(-247.5f + x * 5, 0.0f, 247.5f - z * 5);
	m_EnemyMyColision.position = m_Position;
	m_EnemyMyColision.radius = ENEMY_RADIUS;
	enemyturn = ENEMY_WAIT;
	m_Type = TYPE_SRIME;
	m_MaxHp = enemy_data.Hp;
	m_Hp = m_MaxHp;
	m_Str = enemy_data.str;
	m_Str2 = 1.0f;
	m_Def = enemy_data.def;
	m_Exp = enemy_data.exp;
	m_Gold = enemy_data.gold;
	m_Goway = false;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	nanawalk = 0;
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&m_Front, &m_Front);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);
	D3DXVec3Normalize(&m_Right, &m_Right);

	D3DXVec3Cross(&m_Up, &m_Front, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	// マップ位置を登録
	m_Mapx = x;
	m_Mapz = z;

	m_Goalx = x;
	m_Goalz = z;

	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

}

void CEnemy_Srime::Update(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
		
	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;


	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	switch (getplayer->Get_PlayerTurn())
	{
	case CPlayer::PLAYER_KEY_INPUT:
	case CPlayer::PLAYER_SERECT_UI:
	case CPlayer::PLAYER_WINDOW:
	case CPlayer::PLAYER_ITEM_WAIT:
		// なにもしない
		break;

	case CPlayer::PLAYER_MOVE:
		//プレイヤーが移動中にオブジェクトに当たった時は殴らない
		if (getplayer->m_Judge_player.HitItem)
		{
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
	// AI処理
	//壁判定
	
}

void CEnemy_Srime::Draw(void)
{
	
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);
	DrawDX_Normal(m_mtxWorld, &Normal_model);
	if (m_Str2 >= CHARGE_BUFF)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, m_Position, CTexture::TEX_PLAYER_CHARGE_ICON);
	}
	//DebugFont_Draw(200, 150, "エネミーモード%d", enemyturn);
	//DebugFont_Draw(200, 250, "Attackfream = %d", attackframe);
	//DebugFont_Draw(0, 300, "Angle = %f", m_Angle);
	//DebugFont_Draw(0, 300, "エネミー nanawalk = %d", nanawalk);
	//DebugFont_Draw(0, 330, "エネミー walk = %d", walkf);
	/*
	DebugFont_Draw(200, 20, "エネミー出現数%d", m_EnemyNum[TYPE_SRIME]);
	DebugFont_Draw(200, 60, "m_Judge_enemy.HitTop = %d", m_Judge_enemy.HitTop);
	DebugFont_Draw(200, 100, "m_Judge_enemy.HitBottom  = %d", m_Judge_enemy.HitBottom);
	DebugFont_Draw(200, 140, "m_Judge_enemy.HitRight   = %d", m_Judge_enemy.HitRight);
	DebugFont_Draw(200, 180, "m_Judge_enemy.HitLeft   = %d", m_Judge_enemy.HitLeft);
	DebugFont_Draw(200, 220, "m_Judge_enemy.HitTopLeft   = %d", m_Judge_enemy.HitTopLeft);
	DebugFont_Draw(200, 260, "m_Judge_enemy.HitBottomRight   = %d", m_Judge_enemy.HitBottomRight);
	DebugFont_Draw(200, 300, "m_Judge_enemy.HitBottomLeft   = %d", m_Judge_enemy.HitBottomLeft);
	DebugFont_Draw(200, 340, "m_Judge_enemy.HitEnemy   = %d", m_Judge_enemy.HitEnemy);
	DebugFont_Draw(200, 380, "m_Judge_enemy.HitEnemyTop   = %d", m_Judge_enemy.HitEnemyTop);
	DebugFont_Draw(200, 420, "m_Judge_enemy.HitEnemyBottom   = %d", m_Judge_enemy.HitEnemyBottom);
	DebugFont_Draw(200, 460, "m_Judge_enemy.HitEnemyRight   = %d", m_Judge_enemy.HitEnemyRight);
	DebugFont_Draw(200, 500, "m_Judge_enemy.HitEnemyLeft   = %d", m_Judge_enemy.HitEnemyLeft);
	DebugFont_Draw(200, 540, "m_Judge_enemy.HitEnemyTopLeft   = %d", m_Judge_enemy.HitEnemyTopLeft);
	DebugFont_Draw(200, 580, "m_Judge_enemy.HitEnemyBottomRight   = %d", m_Judge_enemy.HitEnemyBottomRight);
	DebugFont_Draw(200, 620, "m_Judge_enemy.HitEnemyTopRight   = %d", m_Judge_enemy.HitEnemyTopRight);
	DebugFont_Draw(200, 660, "m_Judge_enemy.HitEnemyBottomLeft   = %d", m_Judge_enemy.HitEnemyBottomLeft);
	DebugFont_Draw(200, 700, "m_Judge_enemy.Hitnaname   = %d", m_Judge_enemy.Hitnaname);
	DebugFont_Draw(200, 740, "Attackfream = %d", attackframe);
	//DebugFont_Draw(200, 150, "エネミーモード%d", enemyturn);
	*/
}

void CEnemy_Srime::Enemy_AI(void)
{
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

		// 
		for (int i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);

			if (enemy)
			{
				if (i != MAX_GAMEOBJ - 1)
				{
					for (int e = 0; e < MAX_GAMEOBJ; e++)
					{
						enemy2 = CEnemy::Get_Enemy(e);
						if (enemy2)
							C3DObj::Collision_EnemyVSEnemy(&enemy->m_Judge_enemy, &enemy->m_EnemyMyColision, &enemy2->m_Judge_enemy, &enemy2->m_EnemyMyColision);

					}
				}
			}
		}

		for (int i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{
				if (enemy->m_Mapx == m_Mapx && enemy->m_Mapz == m_Mapz - 1)
					m_Judge_enemy.HitBottom = true;

				if (enemy->m_Mapx == m_Mapx && enemy->m_Mapz == m_Mapz + 1)
					m_Judge_enemy.HitTop = true;

				if (enemy->m_Mapx == m_Mapx + 1 && enemy->m_Mapz == m_Mapz)
					m_Judge_enemy.HitRight = true;

				if (enemy->m_Mapx == m_Mapx - 1 && enemy->m_Mapz == m_Mapz)
					m_Judge_enemy.HitLeft = true;

				if (enemy->m_Mapx == m_Mapx - 1 && enemy->m_Mapz == m_Mapz - 1)
					m_Judge_enemy.HitTopLeft = true;

				if (enemy->m_Mapx == m_Mapx - 1 && enemy->m_Mapz == m_Mapz + 1)
					m_Judge_enemy.HitBottomLeft = true;

				if (enemy->m_Mapx == m_Mapx + 1 && enemy->m_Mapz == m_Mapz + 1)
					m_Judge_enemy.HitBottomRight = true;

				if (enemy->m_Mapx == m_Mapx + 1 && enemy->m_Mapz == m_Mapz - 1)
					m_Judge_enemy.HitTopRight = true;

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
		break;
	}
}

void CEnemy_Srime::Enemy_Act(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 2);
	int attack_number = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
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
		//attack_number = random(mt);
		attack_number = 0;
		switch (attack_number)
		{
		case 0:
			//CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Angle);
			if (getplayer->Get_Condition() == NORMAL_CONDITION)
			{
				CAttack::Attack_EnemySkill(CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			}
			else
			{
				CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str,m_Str2, m_Angle);
			}
			//m_Str2 = CHARGE_BUFF;
			break;
		case 1:
			//CAttack::Attack_EnemySkill(CAttack::HIGH_ATTACK_SKILL, m_WeponType, m_Type, m_Str, m_Angle, ESCAPE_CHECK_OK);
			CAttack::Attack_EnemySkill(CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			//m_Str2 = CHARGE_BUFF;
			break;
		case 2:
			CAttack::Attack_EnemySkill(CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			//m_Str2 = CHARGE_BUFF;
			break;
		}
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
	}
}

/*高速の時*/

void CEnemy_Srime::Enemy_TurboMove(void)
{
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
	
	if (walkf== 56)
	Enemy_TurbolefttopMove();
	if (walkf== 40)
	Enemy_TurboleftbottomMove();
	if (walkf== 30)
	Enemy_TurborightbottomMove();
	if (walkf== 24)
	Enemy_TurborighttopMove();
}

void CEnemy_Srime::Enemy_Move(void)
{
	if (walkf<= WALK_COUNT)
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
		if (walkf== WALK_COUNT + 1)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf>= 96)
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
		if (walkf== 95)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf== 56)
		Enemy_lefttopMove();
	if (walkf== 40)
		Enemy_leftbottomMove();
	if (walkf== 30)
		Enemy_rightbottomMove();
	if (walkf== 24)
		Enemy_righttopMove();
}

void CEnemy_Srime::Enemy_lefttopMove(void)
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_righttopMove(void)
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_leftbottomMove(void)
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_rightbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT) )
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_TurbolefttopMove(void)
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

void CEnemy_Srime::Enemy_TurborighttopMove(void)
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

void CEnemy_Srime::Enemy_TurboleftbottomMove(void)
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

void CEnemy_Srime::Enemy_TurborightbottomMove(void)
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

void CEnemy_Srime::Enemy_LeftMoveCheck(void)
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
	m_Mapx -= 1;

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

void CEnemy_Srime::Enemy_RightMoveCheck(void)
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
	m_Mapx += 1;

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

void CEnemy_Srime::Enemy_BottomMoveCheck(void)
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
	m_Mapz += 1;

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

void CEnemy_Srime::Enemy_TopMoveCheck(void)
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
	m_Mapz -= 1;
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

void CEnemy_Srime::Enemy_TopLeftMoveCheck(void)
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
	m_Mapz -= 1;
	m_Mapx -= 1;

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

void CEnemy_Srime::Enemy_TopRightMoveCheck(void)
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
	m_Mapz -= 1;
	m_Mapx += 1;

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

void CEnemy_Srime::Enemy_BottomLeftMoveCheck(void)
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
	m_Mapz += 1;
	m_Mapx -= 1;

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
void CEnemy_Srime::Enemy_BottomRightMoveCheck(void)
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
	m_Mapz += 1;
	m_Mapx += 1;

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

bool CEnemy_Srime::Damage(int str, float angle, int week_type)
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

void CEnemy_Srime::Enemy_Destroy(void)
{
	alive = false;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Type = CUserinterface::CHARATYPENONE;
	delete this;
}
