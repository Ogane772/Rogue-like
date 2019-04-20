///=============================================================================
//
//	Playerクラス
//
//=============================================================================

//=============================================================================
//	インクルードファイル
//=============================================================================
#include "Cplayer.h"
#include "input.h"
#include "fade.h"
#include "CObject.h"
#include "userinterface.h"
#include "stage.h"
#include "debug_font.h"
#include "bilboard.h"
#include "exp.h"
#include "gamepad.h"
#include "gamepad.h"
#include "exp.h"
#include "CTexture.h"
#include "CCamera.h"
#include "common.h"
#include "map.h"
#include "Attack.h"
#include "scene.h"
#include "collision.h"
#include "sound.h"
#include "CEnemy.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.2f)
#define COASTER_SPEED (0.4f)
#define MP_ADD (5)//1フレーム辺りのMP回復量
#define PLAYER_SAIZ (0.5f)
#define MPSTOCK_INIT (9)
#define ANGLE (3)
#define CORSTER_START (80)//コースターが来る時間
#define SLANT (45)
bool trigger = false;
//=============================================================================
//	静的変数
//=============================================================================
enum PlayerAnime
{
	PLAYER_WALK,
	PLAYER_IDLE,
	PLAYER_SET,
	PLAYER_DAMAGE,
	PLAYER_JUMP,
	PLAYER_COASTER,
	PLAYER_WHEEL,
	PLAYER_DOWN,
};

static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;
static int doubleflag;	//キー二つ押しを格納

CPlayer *CPlayer::m_pPlayer;
Sphere CPlayer::m_Colision;		// 当たり判定
Sphere CPlayer::m_EnemyColision;	// エネミーとの当たり判定専用
CPlayer::JUDGE CPlayer::m_Judge_player;
bool CPlayer::g_bIsFade;
CPlayer::PLAYERTURN CPlayer::turn;
bool CPlayer::goladder;
D3DXVECTOR3 CPlayer::start_m_Position;
int CPlayer::start_m_Mapz;
int CPlayer::start_m_Mapx;
//=============================================================================
//  生成
//=============================================================================
CPlayer::CPlayer() :C3DObj(C3DObj::TYPE_PLAYER)
{
	Initialize();
}

CPlayer *CPlayer::PlayerCreate(void)
{
	CPlayer *m_pPlayer = new CPlayer;
	return m_pPlayer;
}

void CPlayer::Player_Check(void)
{
	int i;
	C3DObj *object;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		object = CObject::Get_Object(i);
		//オブジェクトの判定
		if (object)
		{
			if (object->alive)
			{
				if (m_Mapz == object->m_Mapz && m_Mapx == object->m_Mapx)
				{
					if (object->m_ObjectType == CObject::TYPE_LADDER)
						m_Judge_player.HitLadder = true;
				}
			}
		}
	}
	
}

//=============================================================================
// 破棄
//=============================================================================
CPlayer::~CPlayer()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
void CPlayer::Initialize(void)
{
	m_Judge_player.HitTop = false;
	m_Judge_player.HitBottom = false;
	m_Judge_player.HitRight = false;
	m_Judge_player.HitLeft = false;
	m_Judge_player.HitTopLeft = false;
	m_Judge_player.HitBottomRight = false;
	m_Judge_player.HitTopRight = false;
	m_Judge_player.HitBottomLeft = false;
	m_Judge_player.HitEnemy = false;
	m_Judge_player.HitEnemyTop = false;
	m_Judge_player.HitEnemyBottom = false;
	m_Judge_player.HitEnemyRight = false;
	m_Judge_player.HitEnemyLeft = false;
	m_Judge_player.HitEnemyTopLeft = false;
	m_Judge_player.HitEnemyBottomRight = false;
	m_Judge_player.HitEnemyTopRight = false;
	m_Judge_player.HitEnemyBottomLeft = false;
	m_Judge_player.Hitnaname = false;
	m_Judge_player.HitLadder = false;

	rival_flag = false;
	enter_flag = false;
	m_Lv = 1;
	m_Gold = 100;
	charatype = 0;
	m_Position = D3DXVECTOR3(2.5f, 0.0f, 2.5f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	walkpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&m_Front, &m_Front);

	turn = PLAYER_KEY_INPUT;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	attack_endframe = ATTACK_END;
	nanawalk = 0;
	goladder = false;
	g_bIsFade = false;
	// プレイヤーの初期ステータスを決める
	if (CStage::Stage_GetLevel() == 1)
	{
		m_MaxHp = PLAYER_MAXHP;
		m_Hp = m_MaxHp;
		//m_Str = PLAYER_STR;
		m_Str = 999;
	}
	m_Colision.radius = PLAYER_RADIUS;
	m_EnemyColision.radius = PLAYER_ENERADIUS;
	// 外積
	//D3DXVec3Cross(&g_right, &g_front, &g_up);
	// 逆かも
	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);
	D3DXVec3Normalize(&m_Right, &m_Right);

	D3DXVec3Cross(&m_Up, &m_Front, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	Normal_model = GetNormalModel(MODELL_PLAYER);
	
	
}

void CPlayer::Finalize(void)
{
	NormalModel_Finalize(&Normal_model);
	delete this;
}

//=============================================================================
//  更新
//=============================================================================
void CPlayer::Update(void)
{
	int i;
	PlayerVsWall(&m_Judge_player, &m_Colision);
	
	// カメラの振り向き　回転行列
	D3DXMATRIX mtxR;        // 1フレームに回転する角度
	D3DXMATRIX mtxM;
	//D3DXMATRIX mtxB;
	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;


	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;


	
	//	if ( turn == PLAYER_KEY_INPUT||  turn == PLAYER_MOVE) // デバッグ用
	switch (turn)
	{
	case PLAYER_FIRST:
		m_Position = start_m_Position;
		m_Mapz = start_m_Mapz;
		m_Mapx = start_m_Mapx;
		m_Angle = 0.0f;
		walkf = 0;
		attackframe = 0;
		nanawalk = 0;
		goladder = false;
		g_bIsFade = false;
		turn = PLAYER_KEY_INPUT;
		break;
	case PLAYER_STANDBY:
		if (m_Hp <= 0)
		{
			turn = PLAYER_DESTROY;
		}
		else
		{
			// 自然回復 (MAXHP / 100)
			if ((int)m_Hp < (int)m_MaxHp)
			{
				m_Hp += m_MaxHp / 500;
			}
			turn = PLAYER_KEY_INPUT;
		}
		break;
	case PLAYER_KEY_INPUT:
		if ( goladder)
		{
			 turn = PLAYER_SERECT_UI;
			break;
		}
		//足踏み
		if (Keyboard_IsPress(DIK_O))
		{
			turn = PLAYER_TURN_END;
		}
		//===================================================
		// ミニマップ表示(デバッグ用)
		//===================================================
//		if (Keyboard_IsTrigger(DIK_M))
//		{
		
//		}
		//===================================================
		// 方向転換
		//===================================================
		// 上方向
		if (Keyboard_IsPress(DIK_UP))
		{
			AngleChange(0.0f);
		}

		// 左方向4.8
		if (Keyboard_IsPress(DIK_LEFT))
		{
			AngleChange(4.8f);
		}

		//	下方向3.2
		if (Keyboard_IsPress(DIK_DOWN))
		{
			AngleChange(3.2f);
		}

		//	右方向1.6
		if (Keyboard_IsPress(DIK_RIGHT))
		{
			AngleChange(1.6f);
		}

		// 左上方向
		if (Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_LEFT))
		{
			AngleChange(5.6f);
		}

		// 左下方向
		if (Keyboard_IsPress(DIK_DOWN) && Keyboard_IsPress(DIK_LEFT))
		{
			AngleChange(4.0f);
		}

		// 右下方向
		if (Keyboard_IsPress(DIK_DOWN) && Keyboard_IsPress(DIK_RIGHT))
		{
			AngleChange(2.4f);
		}

		// 右上方向
		if (Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_RIGHT))
		{
			AngleChange(0.8f);
		}

		//===================================================
		// 攻撃
		//===================================================
		if (Keyboard_IsTrigger(DIK_SPACE))
		{
			turn = PLAYER_ACT;
			attackframe = 0;
			attack_endframe = NOATTACK_END;
			break;
		}

		//===================================================
		// 移動
		//===================================================
		if (!m_Judge_player.HitTop)
		{
			// 上移動
			if (Keyboard_IsPress(DIK_W) && !Keyboard_IsPress(DIK_LSHIFT))
			{
				Player_MoveChenge(DIRE_TOP);

				Player_Move();
				break;
			}
		}

		if (!m_Judge_player.HitLeft)
		{
			// 左移動
			if (Keyboard_IsPress(DIK_A) && !Keyboard_IsPress(DIK_LSHIFT))
			{
				Player_MoveChenge(DIRE_LEFT);
				Player_Move();
				break;
			}
		}

		// 下移動3.2
		if (!m_Judge_player.HitBottom)
		{
			if (Keyboard_IsPress(DIK_S) && !Keyboard_IsPress(DIK_LSHIFT))
			{
				Player_MoveChenge(DIRE_BOTTOM);
				Player_Move();
				break;
			}
		}

		//右移動1.6
		if (!m_Judge_player.HitRight)
		{
			if (Keyboard_IsPress(DIK_D) && !Keyboard_IsPress(DIK_LSHIFT))
			{
				Player_MoveChenge(DIRE_RIGHT);
				Player_Move();
				break;
			}
		}

		if (!m_Judge_player.HitTopLeft)
		{
			// 左上移動
			if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_A) && Keyboard_IsPress(DIK_LSHIFT))
			{
				if (CMap::Map_GetData( m_Mapz - 1,  m_Mapx - 1).type != 0)
				{
					Player_MoveChenge(DIRE_TOP_LEFT);
					Player_Move();
					break;
				}
			}
		}

		if (!m_Judge_player.HitBottomLeft)
		{
			// 左下移動
			if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_A) && Keyboard_IsPress(DIK_LSHIFT))
			{
				if (CMap::Map_GetData( m_Mapz + 1,  m_Mapx - 1).type != 0)
				{
					Player_MoveChenge(DIRE_BOTTOM_LEFT);
					Player_Move();
					break;
				}
			}
		}

		if (!m_Judge_player.HitBottomRight)
		{
			// 右下移動
			if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_D) && Keyboard_IsPress(DIK_LSHIFT))
			{
				if (CMap::Map_GetData( m_Mapz + 1,  m_Mapx + 1).type != 0)
				{
					Player_MoveChenge(DIRE_BOTTOM_RIGHT);
					Player_Move();
					break;
				}
			}
		}

		if (!m_Judge_player.HitTopRight)
		{
			// 右上移動
			if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_D) && Keyboard_IsPress(DIK_LSHIFT))
			{
				if (CMap::Map_GetData( m_Mapz - 1,  m_Mapx + 1).type != 0)
				{
					Player_MoveChenge(DIRE_TOP_RIGHT);
					Player_Move();
					break;
				}
			}
		}

		break;

	case PLAYER_SERECT_UI:
		if (rival_flag)
		{
			if (Keyboard_IsTrigger(DIK_RETURN))
			{
				enter_flag = true;
			}
		}
		else
		{
			if (Keyboard_IsTrigger(DIK_UP))
			{
				goladder = true;
			}
			if (Keyboard_IsTrigger(DIK_DOWN))
			{
				goladder = false;
			}
			if (goladder)
			{
				CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::GOLADDER);
			}
			if (!goladder)
			{
				CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::NOTGOLADDER);
			}
			if (Keyboard_IsTrigger(DIK_RETURN))
			{
				if (goladder)
				{
					// ネクストシーンへ
					if (!g_bIsFade)
					{
						Fade_Start(true, 90, 0, 0, 0);
						g_bIsFade = true;
					}
				}
				else
				{
					turn = PLAYER_KEY_INPUT;
				}
				CUserinterface::UI_Delete();
			}

			if (g_bIsFade)
			{
				CUserinterface::UI_Delete();
				// フェードが終わったら
				if (!Fade_IsFade())
				{
					g_bIsFade = false;
					// 最上階だったらリザルトへ
					if (CStage::Stage_GetLevel() == STAGE_LEVELMAX)
					{
						Scene_Change(SCENE_INDEX_REZULT);
					}
					// 次の階へ
					else
					{
						Scene_Change(SCENE_INDEX_STAGE);
					}
				}
			}
		}
		break;
	case PLAYER_DESTROY:
		Player_Destroy();
		break;
	case PLAYER_MOVE:
		Player_Move();
		break;
	case PLAYER_MOVE_END:
	case PLAYER_ACT_END:
		 turn = PLAYER_TURN_END;
		break;

	case PLAYER_ACT:
		Player_Act();
		break;
	}

	// マップ位置の更新
	 m_Mapz = (int)(( m_Colision.position.z - 247.5f) / -5);
	 m_Mapx = (int)(( m_Colision.position.x + 247.5f) / 5);
	 m_Judge_player.HitTop = false;
	 m_Judge_player.HitBottom = false;
	 m_Judge_player.HitRight = false;
	 m_Judge_player.HitLeft = false;
	 m_Judge_player.HitTopLeft = false;
	 m_Judge_player.HitBottomRight = false;
	 m_Judge_player.HitTopRight = false;
	 m_Judge_player.HitBottomLeft = false;
	 m_Judge_player.HitEnemy = false;
	 m_Judge_player.HitEnemyTop = false;
	 m_Judge_player.HitEnemyBottom = false;
	 m_Judge_player.HitEnemyRight = false;
	 m_Judge_player.HitEnemyLeft = false;
	 m_Judge_player.HitEnemyTopLeft = false;
	 m_Judge_player.HitEnemyBottomRight = false;
	 m_Judge_player.HitEnemyTopRight = false;
	 m_Judge_player.HitEnemyBottomLeft = false;
	 m_Judge_player.Hitnaname = false;
	 m_Judge_player.HitLadder = false;
	 C3DObj *enemy;
	 //エネミーの当たり判定初期化
	 for (i = 0; i < MAX_GAMEOBJ; i++)
	 {
		 enemy = CEnemy::Get_Enemy(i);
		 if (enemy)
		 {
			 enemy->m_Judge_enemy.HitTop = false;
			 enemy->m_Judge_enemy.HitBottom = false;
			 enemy->m_Judge_enemy.HitRight = false;
			 enemy->m_Judge_enemy.HitLeft = false;
			 enemy->m_Judge_enemy.HitTopLeft = false;
			 enemy->m_Judge_enemy.HitBottomRight = false;
			 enemy->m_Judge_enemy.HitTopRight = false;
			 enemy->m_Judge_enemy.HitBottomLeft = false;
			 enemy->m_Judge_enemy.HitEnemy = false;
			 enemy->m_Judge_enemy.HitEnemyTop = false;
			 enemy->m_Judge_enemy.HitEnemyBottom = false;
			 enemy->m_Judge_enemy.HitEnemyRight = false;
			 enemy->m_Judge_enemy.HitEnemyLeft = false;
			 enemy->m_Judge_enemy.HitEnemyTopLeft = false;
			 enemy->m_Judge_enemy.HitEnemyBottomRight = false;
			 enemy->m_Judge_enemy.HitEnemyTopRight = false;
			 enemy->m_Judge_enemy.HitEnemyBottomLeft = false;
			 enemy->m_Judge_enemy.Hitnaname = false;
			 //壁とプレイヤーの当たり判定
			 CEnemy::EnemyVsWall(&enemy->m_Judge_enemy, &enemy->m_EnemyMyColision);
			 C3DObj::Collision_AnimeVSAnime(&m_Judge_player, &m_EnemyColision, &enemy->m_Judge_enemy,&enemy->m_EnemyMyColision);
		 }
	 }
	 Player_Check();
	
}
//=============================================================================
// 描画
//=============================================================================

void CPlayer::Draw(void)
{
	DrawDX_Normal(m_mtxWorld, &Normal_model);
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);	
	DebugFont_Draw(200, 20, "attackframe = %d", attackframe);
	DebugFont_Draw(200, 50, "turn = %d", turn);
}

void CPlayer::Player_SetPos(int z, int x)
{
	start_m_Position = D3DXVECTOR3(-247.5f + x * 5, 0.0f, 247.5f - z * 5);
	m_Colision.position = start_m_Position;
	m_EnemyColision.position = start_m_Position;
	start_m_Mapz = z;
	start_m_Mapx = x;
	turn = PLAYER_FIRST;
}

/*
C3DObj *CPlayer::Get_Player(void)
{
	int i;
	C3DObj *pplayer;

	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		pplayer = C3DObj::Get(i);
		if (pplayer->Get_3DObjType() == C3DObj::TYPE_PLAYER)
		{
			return pplayer;
		}
	}
	return 0;
}*/

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Player_Move(void)
{
	if (walkf <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecplayer * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf++;
		if (walkf == 5)
			turn = PLAYER_MOVE_END;
	}

	if (walkf >= 96)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecplayer * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf--;
		if (walkf == 95)
		{
			turn = PLAYER_MOVE_END;
		}
	}
	if (walkf == 56)
	{
		Player_lefttopMove();
	}
	if (walkf == 40)
	{
		Player_leftbottomMove();
	}
	if (walkf == 24)
	{
		Player_rightbottomMove();
	}
	if (walkf == 8)
	{
		Player_righttopMove();
	}
}

void CPlayer::Player_MoveChenge(int MoveType)
{
	switch (MoveType)
	{
	case DIRE_TOP:
		walkf = 0;
		m_Mapz -= 1;
		m_Angle = 0.0f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += m_Front * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;

		m_EnemyColision.position = m_Position;


		m_Position -= m_Front * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;


		vecplayer = m_Front;
		turn = PLAYER_MOVE;
		break;
	case DIRE_LEFT:
		m_Angle = 4.8f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= m_Right * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;
		m_Mapx -= 1;

		m_Position += m_Right * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 100;
		vecplayer = m_Right;
		turn = PLAYER_MOVE;
		break;
	case DIRE_BOTTOM:
		m_Angle = 3.2f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= m_Front * 5.0f;
		m_Mapz += 1;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;

		m_Position += m_Front * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 100;
		vecplayer = m_Front;
		turn = PLAYER_MOVE;
		break;
	case DIRE_RIGHT:
		m_Angle = 1.6f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += m_Right * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;
		m_Mapx += 1;
		m_Position -= m_Right * 5.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 0;
		vecplayer = m_Right;
		turn = PLAYER_MOVE;
		break;
	case DIRE_TOP_LEFT:
		m_Angle = 5.6f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += m_Front * 5.0f;
		m_Position -= m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;
		// m_Colision.position =  walkpos;

		m_Position -= m_Front * 5.0f;
		m_Position += m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 56;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		turn = PLAYER_MOVE;
		break;
	case DIRE_BOTTOM_LEFT:
		m_Angle = 4.0f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= m_Front * 5.0f;
		m_Position -= m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;
		// m_Colision.position =  walkpos;

		m_Position += m_Front * 5.0f;
		m_Position += m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 40;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		turn = PLAYER_MOVE;
		break;
	case DIRE_BOTTOM_RIGHT:
		m_Angle = 2.4f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= m_Front * 5.0f;
		m_Position += m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;

		m_Position += m_Front * 5.0f;
		m_Position -= m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 24;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		turn = PLAYER_MOVE;
		break;
	case DIRE_TOP_RIGHT:
		m_Angle = 0.8f;

		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += m_Front * 5.0f;
		m_Position += m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// 当たり判定の設定
		m_Colision.position = m_Position;
		m_EnemyColision.position = m_Position;
		// m_Colision.position =  walkpos;

		m_Position -= m_Front * 5.0f;
		m_Position -= m_Right * 5.0f;
		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf = 8;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		turn = PLAYER_MOVE;
		break;
	}
}

void CPlayer::Player_lefttopMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecplayer * 1.0f;

		m_Position -= vec2player * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			turn = PLAYER_MOVE_END;
	}
}

void CPlayer::Player_righttopMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecplayer * 1.0f;

		m_Position += vec2player * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			turn = PLAYER_MOVE_END;
	}
}

void CPlayer::Player_leftbottomMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecplayer * 1.0f;

		m_Position -= vec2player * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			turn = PLAYER_MOVE_END;
	}
}

void CPlayer::Player_rightbottomMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecplayer * 1.0f;

		m_Position += vec2player * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			turn = PLAYER_MOVE_END;
	}
}
//=============================================================================
// 移動処理（ゲームパッド）
//=============================================================================

void CPlayer::Player_GamepadMove(void)
{

}
//=============================================================================
// 方向変更
//=============================================================================

void CPlayer::AngleChange(float Angle)
{
	m_Angle = Angle;

	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;
}

//=============================================================================
// 方向変更(ゲームパッド)
//=============================================================================

void CPlayer::ControllerAngleChange(int index)
{

}

void CPlayer::Player_Act(void)
{
	if (attackframe < 8)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		// 左に攻撃
		if (m_Angle == 4.8f)
		{
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
		if (m_Angle == 0.0f)
		{
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
		if (m_Angle == 1.6f)
		{
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
		if (m_Angle == 3.2f)
		{
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
		if (m_Angle == 0.8f)
		{
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
		if (m_Angle == 2.4f)
		{
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
		if (m_Angle == 4.0f)
		{
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
		if (m_Angle == 5.6f)
		{
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
		// 向いてる方向、攻撃力、攻撃したキャラの名前を渡す
		CAttack::Attack_PlayerUpdate(charatype, m_Angle, rand() % m_Str + 5);
	}
	if (attackframe == attack_endframe)
	{
		if (rival_flag)
		{
			//ここでUIモードにしてキー入力待ちを行い、エンターを押したら倒したログを出す
			turn = PLAYER_SERECT_UI;
		}
		else
		{
			turn = PLAYER_ACT_END;
		}
	}
}

void CPlayer::Player_Destroy(void)
{
	// ゲームオーバーへ画面遷移
	CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::DESTROY);
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE))
	{
		// ネクストシーンへ
		if (!g_bIsFade)
		{
			Fade_Start(true, 90, 0, 0, 0);
			g_bIsFade = true;
		}

	}

	if (g_bIsFade)
	{
		CUserinterface::UI_Delete();
		// フェードが終わったら
		if (!Fade_IsFade())
		{
			g_bIsFade = false;
			// タイトルへ
			Scene_Change(SCENE_INDEX_REZULT);

		}
	}
}

void CPlayer::Player_NextTurn(void)
{
	int i;
	if (m_Judge_player.HitLadder)
	{
		goladder = true;
		CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::GOLADDER);
	}
	C3DObj *enemy;
	turn = PLAYER_STANDBY;
	//プレイヤーをSTANDBYに戻すと同時にエネミー全てをWAITにする
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);
		if (enemy)
		{
			if (enemy->enemyturn != ENEMY_ACTION)
			{
				enemy->enemyturn = ENEMY_WAIT;
			}
		}
	}
}

bool CPlayer::Damage(int str)
{
	// 後にダメージエフェクトを作成
	Exp_Set(HIT, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);

	// 後にダメージ計算式を作成
	m_Hp -= str;
	if (m_Hp <= 0)
	{
		m_Hp = 0;
	}
	// 体力が0以下で倒れる
	if (m_Hp <= 0)
	{
		return DEATH;
	}
	return NORMAL;
}

bool CPlayer::JoyDevice_IsTrigger(int nKey)
{
	return (js.rgbButtons[nKey] & 0x80) ? true : false;
}

C3DObj* CPlayer::Get_Player(void)
{
	int i;
	C3DObj *pplayer;

	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		pplayer = C3DObj::Get(i);
		if (pplayer->Get_3DObjType() == C3DObj::TYPE_PLAYER)
		{
			return pplayer;
		}
	}
	return 0;
}
