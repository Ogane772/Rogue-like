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
#include "sound.h"
#include "CObject.h"
#include "userinterface.h"
#include "stage.h"
#include "debug_font.h"
#include "bilboard.h"
#include "exp.h"
#include "gamepad.h"
#include "gamepad.h"
#include "CTexture.h"
#include "CCamera.h"
#include "common.h"
#include "map.h"
#include "Attack.h"
#include "scene.h"
#include "collision.h"
#include "sound.h"
#include "CWepon.h"
#include "CEnemy.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define SPEED (0.2f)
#define PURASU_UP (2)//装備の+値1当たりの上昇値
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
CPlayer::Player_LvData CPlayer::m_PlayerLvData[]
{   //最初のみ初期値を書く
	//LV LVUPEXP HP STR DEF
	{ 1 , 0, PLAYER_MAXHP, PLAYER_STR, 0 },
	//以後足す分だけ書く
	{ 2 , 2, 5, 2, 0 },
	{ 3 , 6, 10, 2, 0 },
	{ 4 , 15, 5, 2, 0 },
	{ 5 , 30, 10, 2, 0 },
	{ 6 , 500, 5, 2, 0 },
};
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

Sphere CPlayer::m_Colision;		// 当たり判定
Sphere CPlayer::m_EnemyColision;	// エネミーとの当たり判定専用
CPlayer::JUDGE CPlayer::m_Judge_player;
bool CPlayer::g_bIsFade;
int CPlayer::m_Turn;
bool CPlayer::goladder;
D3DXVECTOR3 CPlayer::start_m_Position;
int CPlayer::start_m_Mapz;
int CPlayer::start_m_Mapx;
CPlayer::PLAYER_WEPON CPlayer::player_wepon[3];
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
	int i;
	strcpy_s(name, MAX_NAME, "シレン");
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
	m_Judge_player.HitItem = false;

	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = 0;
	}
	for (i = 0; i < MAX_ITEM; i++)
	{
		m_ItemStock[i] = 0;
	}
	for (i = 0; i < MAX_WEPON + MAX_ITEM; i++)
	{
		m_ItemStockType[i] = 0;
		m_AllItemStock[i] = 0;
	}
	m_Wmode = false;
	m_WeponStock[WEPON_NUMBER] = 0;
	player_wepon[WEPON_NUMBER].wepon_str = 1;
	player_wepon[WEPON_NUMBER].wepon_def = 0;
	player_wepon[WEPON_NUMBER].wepon_type = CWepon::TYPE_SWORD;
	player_wepon[WEPON_NUMBER].wepon_purasu_number = 0;
	m_WeponStock[SHELD_NUMBER] = 1;
	player_wepon[SHELD_NUMBER].wepon_str = 0;
	player_wepon[SHELD_NUMBER].wepon_def = 1;
	player_wepon[SHELD_NUMBER].wepon_type = CWepon::TYPE_SHELD;
	player_wepon[SHELD_NUMBER].wepon_purasu_number = 0;
	m_WeponStock[RING_NUMBER] = 2;
	player_wepon[RING_NUMBER].wepon_str = 1;
	player_wepon[RING_NUMBER].wepon_def = 1;
	player_wepon[RING_NUMBER].wepon_type = CWepon::TYPE_RING;
	player_wepon[RING_NUMBER].wepon_purasu_number = 0;

	m_ItemStock[0] = CObject::TYPE_YAKUSOU;
	m_ItemStock[1] = CObject::TYPE_YAKUSOU;
	m_ItemStock[2] = CObject::TYPE_YAKUSOU; 
	m_ItemStock[3] = CObject::TYPE_YAKUSOU;
	m_ItemStock[4] = CObject::TYPE_YAKUSOU;
	
	m_ItemStock[5] = CObject::TYPE_YAKUSOU;

	
	m_AllWeponStock = 3;//本当は装備取得時に足す
	rival_flag = false;
	enter_flag = false;
	turbo = false;
	m_ItemOn = false;
	m_ItemTips = false;
	m_NextItemPage = false;
	m_Add_cursor = 0;
	m_CursorNumber = 0;
	m_TimeCursorNumber = 0;
	m_Lv = m_PlayerLvData[0].lv;
	m_Gold = 100;
	charatype = 0;
	m_Wait_frame = 0;
	m_Position = D3DXVECTOR3(2.5f, 0.0f, 2.5f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	walkpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&m_Front, &m_Front);

	m_Turn = PLAYER_KEY_INPUT;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	attack_endframe = ATTACK_END;
	nanawalk = 0;
	goladder = false;
	g_bIsFade = false;
	m_NoItem = false;
	// プレイヤーの初期ステータスを決める
	if (CStage::Stage_GetLevel() == 1)
	{
		m_MaxHp = (float)m_PlayerLvData[0].maxhp;
		m_Hp = m_MaxHp;
		m_Str = m_PlayerLvData[0].str;
		m_Def = m_PlayerLvData[0].def;
		m_Exp = m_PlayerLvData[0].lvup_exp;
		m_MaxOnaka = MAX_ONAKA;
		m_Onaka = m_MaxOnaka;
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

	Normal_model = GetNormalModel(MODELL_KISHI);
	
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
	PlayerVsWall(&m_Judge_player, &m_Colision);
	
	// カメラの振り向き　回転行列
	D3DXMATRIX mtxR;        // 1フレームに回転する角度
	D3DXMATRIX mtxM;
	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	switch (m_Turn)
	{
	case PLAYER_FIRST:
		Player_First();
		break;
	case PLAYER_STANDBY:
		Player_Standby();
		break;
	case PLAYER_KEY_INPUT:
		Player_KeyInput();
		break;
	case PLAYER_WINDOW:
		Player_WindowMode();
		break;
	case PLAYER_WINDOW_STATUS:
		if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_BACK))
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber = CURSOL_LEFT_TOP;
			m_Turn = PLAYER_WINDOW;
		}
		break;
	case PLAYER_WINDOW_ASIMOTO:
		m_NoItem = false;
		m_Turn = PLAYER_TURN_END;
		break;
	case PLAYER_WINDOW_ITEM:
		Player_SelectItem();
		break;
	case PLAYER_SERECT_UI:
		Player_SelectUi();
		break;
	case PLAYER_DESTROY:
		Player_Destroy();
		break;
	case PLAYER_ITEM_EFFECT:
		Player_ItemEffect();
		break;
	case PLAYER_ITEM_WAIT:
		m_Wait_frame++;
		if (m_Wait_frame == WAIT_FRAME)
		{
			m_Wait_frame = 0;
			m_Judge_player.HitItem = false;
			m_Turn = PLAYER_TURN_END;
		}
		break;
	case PLAYER_ITEM_ASIMOTO:
		Player_ItemAsimoto();
		break;
	case PLAYER_MOVE:
		if (!turbo)
		{
			Player_Move();
		}
		else
		{
			Player_TurboMove();
		}
		break;
	case PLAYER_MOVE_END:
		m_NoItem = false;
	case PLAYER_ACT_END:
		m_Turn = PLAYER_TURN_END;
		break;
	case PLAYER_ACT:
		Player_Act();
		break;
	case PLAYER_TURN_END:
		m_NextItemPage = false;
		m_Add_cursor = 0;
		m_Judge_player.HitItem = false;
		break;
	}
	Player_CollisionUpdate();
	if (!m_NoItem)
	{
		Player_Check();
	}
}
//=============================================================================
// 描画
//=============================================================================

void CPlayer::Draw(void)
{
	DrawDX_Normal(m_mtxWorld, &Normal_model);
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);	
	DebugFont_Draw(600, 120, "m_CursorNumber = %d", m_CursorNumber);
	DebugFont_Draw(300, 160, "m_AddCursor = %d", m_Add_cursor);
	/*
	DebugFont_Draw(200, 120, "m_Wait_frame = %d", m_Wait_frame);
	DebugFont_Draw(200, 20, "m_Judge_player.HitItem = %d", m_Judge_player.HitItem);
	DebugFont_Draw(200, 200, "player.pos x = %f player.pos z = %f", m_Position.x, m_Position.z);
	DebugFont_Draw(200, 50, "turn = %d", m_Turn);
	//DebugFont_Draw(200, 250, "m_NoItem  = %d", m_NoItem);
	DebugFont_Draw(200, 250, "Map_GetData(m_Mapz, m_Mapx).have = %d", CMap::Map_GetData(m_Mapz, m_Mapx).have);
	
	/*
	DebugFont_Draw(200, 20, "attackframe = %d", attackframe);
	
	DebugFont_Draw(200, 150, "walkf = %d", walkf);
	DebugFont_Draw(200, 100, "player.pos x = %f player.pos z = %f", m_Position.x, m_Position.z);
	DebugFont_Draw(200, 200, "m_Colision x = %f m_Colision z = %f", m_Colision.position.x, m_Colision.position.z);
	DebugFont_Draw(200, 250, "turbo = %d", turbo);*/
}

void CPlayer::Player_SetPos(int z, int x)
{
	start_m_Position = D3DXVECTOR3(-247.5f + x * 5, 0.0f, 247.5f - z * 5);
	m_Colision.position = start_m_Position;
	m_EnemyColision.position = start_m_Position;
	start_m_Mapz = z;
	start_m_Mapx = x;
	m_Turn = PLAYER_FIRST;
}

void CPlayer::Player_First(void)
{
	m_Position = start_m_Position;
	m_Mapz = start_m_Mapz;
	m_Mapx = start_m_Mapx;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	nanawalk = 0;
	goladder = false;
	g_bIsFade = false;
	m_Turn = PLAYER_KEY_INPUT;
}

void CPlayer::Player_Standby(void)
{
	if (m_Hp <= 0)
	{
		m_Turn = PLAYER_DESTROY;
	}
	else
	{
		// 自然回復 (MAXHP / 100)
		if ((int)m_Hp < (int)m_MaxHp)
		{
			m_Hp += PLAYER_HEAL;
		}
		m_Turn = PLAYER_KEY_INPUT;
	}
}

void CPlayer::Player_KeyInput(void)
{
	turbo = false;
	//次の階選択画面
	if (goladder)
	{
		m_Turn = PLAYER_SERECT_UI;
		return;
	}
	//足踏み
	if (Keyboard_IsPress(DIK_O))
	{
		m_NoItem = false;
		m_Turn = PLAYER_TURN_END;
	}
	//倍速モード
	if (Keyboard_IsPress(DIK_RSHIFT))
	{
		turbo = true;
	}
	//コマンドウィンドウを開く
	if (Keyboard_IsTrigger(DIK_P))
	{
		PlaySound(CURSOR_SE);
		m_CursorNumber = 0;
		m_Turn = PLAYER_WINDOW;
	}
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
		m_Turn = PLAYER_ACT;
		attackframe = 0;
		attack_endframe = NOATTACK_END;
		return;
	}

	//===================================================
	// 移動
	//===================================================
	if (!m_Judge_player.HitTop)
	{
		// 上移動
		if (Keyboard_IsPress(DIK_W) && !Keyboard_IsPress(DIK_LSHIFT))
		{
			if (!turbo)
			{
				Player_MoveChenge(DIRE_TOP);
				Player_Move();
			}
			else
			{
				Player_TurboMoveChenge(DIRE_TOP);
				Player_TurboMove();
			}
			return;
		}
	}

	if (!m_Judge_player.HitLeft)
	{
		// 左移動
		if (Keyboard_IsPress(DIK_A) && !Keyboard_IsPress(DIK_LSHIFT))
		{
			if (!turbo)
			{
				Player_MoveChenge(DIRE_LEFT);
				Player_Move();
			}
			else
			{
				Player_TurboMoveChenge(DIRE_LEFT);
				Player_TurboMove();
			}
			return;
		}
	}

	// 下移動3.2
	if (!m_Judge_player.HitBottom)
	{
		if (Keyboard_IsPress(DIK_S) && !Keyboard_IsPress(DIK_LSHIFT))
		{
			if (!turbo)
			{
				Player_MoveChenge(DIRE_BOTTOM);
				Player_Move();
			}
			else
			{
				Player_TurboMoveChenge(DIRE_BOTTOM);
				Player_TurboMove();
			}
			return;
		}
	}

	//右移動1.6
	if (!m_Judge_player.HitRight)
	{
		if (Keyboard_IsPress(DIK_D) && !Keyboard_IsPress(DIK_LSHIFT))
		{
			if (!turbo)
			{
				Player_MoveChenge(DIRE_RIGHT);
				Player_Move();
			}
			else
			{
				Player_TurboMoveChenge(DIRE_RIGHT);
				Player_TurboMove();
			}
			return;
		}
	}

	if (!m_Judge_player.HitTopLeft)
	{
		// 左上移動
		if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_A) && Keyboard_IsPress(DIK_LSHIFT))
		{
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx - 1).type != 0)
			{
				if (!turbo)
				{
					Player_MoveChenge(DIRE_TOP_LEFT);
					Player_Move();
				}
				else
				{
					Player_TurboMoveChenge(DIRE_TOP_LEFT);
					Player_TurboMove();
				}
				return;
			}
		}
	}

	if (!m_Judge_player.HitBottomLeft)
	{
		// 左下移動
		if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_A) && Keyboard_IsPress(DIK_LSHIFT))
		{
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx - 1).type != 0)
			{
				if (!turbo)
				{
					Player_MoveChenge(DIRE_BOTTOM_LEFT);
					Player_Move();
				}
				else
				{
					Player_TurboMoveChenge(DIRE_BOTTOM_LEFT);
					Player_TurboMove();
				}
				return;
			}
		}
	}

	if (!m_Judge_player.HitBottomRight)
	{
		// 右下移動
		if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_D) && Keyboard_IsPress(DIK_LSHIFT))
		{
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx + 1).type != 0)
			{
				if (!turbo)
				{
					Player_MoveChenge(DIRE_BOTTOM_RIGHT);
					Player_Move();
				}
				else
				{
					Player_TurboMoveChenge(DIRE_BOTTOM_RIGHT);
					Player_TurboMove();
				}
				return;
			}
		}
	}

	if (!m_Judge_player.HitTopRight)
	{
		// 右上移動
		if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_D) && Keyboard_IsPress(DIK_LSHIFT))
		{
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx + 1).type != 0)
			{
				if (!turbo)
				{
					Player_MoveChenge(DIRE_TOP_RIGHT);
					Player_Move();
				}
				else
				{
					Player_TurboMoveChenge(DIRE_TOP_RIGHT);
					Player_TurboMove();
				}
				return;
			}
		}
	}
}

void CPlayer::Player_WindowMode(void)
{
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		PlaySound(CURSOR_SE);
		if (m_CursorNumber == CURSOL_LEFT_TOP)
		{
			m_CursorNumber = 0;
			m_Turn = PLAYER_WINDOW_ITEM;
		}
		if (m_CursorNumber == CURSOL_LEFT_DOWN)
		{
			m_Turn = PLAYER_WINDOW_ASIMOTO;
		}
		if (m_CursorNumber == CURSOL_RIGHT_TOP)
		{
			m_Turn = PLAYER_WINDOW_STATUS;
		}
	}
	if (Keyboard_IsTrigger(DIK_P) || Keyboard_IsTrigger(DIK_BACK))
	{
		PlaySound(CURSOR_SE);
		m_Turn = PLAYER_KEY_INPUT;
	}
	if (Keyboard_IsTrigger(DIK_W))
	{
		if (m_CursorNumber == CURSOL_LEFT_DOWN || m_CursorNumber == CURSOL_RIGHT_DOWN)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber -= 2;
		}
	}
	if (Keyboard_IsTrigger(DIK_D))
	{
		if (m_CursorNumber == CURSOL_LEFT_TOP || m_CursorNumber == CURSOL_LEFT_DOWN)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber++;
		}
	}
	if (Keyboard_IsTrigger(DIK_A))
	{
		if (m_CursorNumber == CURSOL_RIGHT_TOP || m_CursorNumber == CURSOL_RIGHT_DOWN)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber--;
		}
	}
	if (Keyboard_IsTrigger(DIK_S))
	{
		if (m_CursorNumber == CURSOL_LEFT_TOP || m_CursorNumber == CURSOL_RIGHT_TOP)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber += 2;
		}
	}
}

void CPlayer::Player_SelectUi(void)
{
	if (rival_flag)
	{
		if (Keyboard_IsTrigger(DIK_RETURN))
		{
			enter_flag = true;
		}
	}
	else
	{
		if (Keyboard_IsTrigger(DIK_UP) || Keyboard_IsTrigger(DIK_W))
		{
			PlaySound(CURSOR_SE);
			goladder = true;
		}
		if (Keyboard_IsTrigger(DIK_DOWN) || Keyboard_IsTrigger(DIK_S))
		{
			PlaySound(CURSOR_SE);
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
					PlaySound(FLOOR_SE);
				}
			}
			else
			{
				m_Turn = PLAYER_KEY_INPUT;
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
}

void CPlayer::Player_SelectItem(void)
{
	int i;
	int stock = 0;//アイテムが何個あるか数える
	//取得タイプと番号を見てどのアイテムを選んでるか取得する　例）m_AllItemStock[カーソル位置]
	//所持アイテムの整列
	for (i = 0; i < MAX_WEPON; i++)
	{
		if (m_WeponStock[i] != -1)
		{
			m_ItemStockType[stock] = WEPON;
			m_AllItemStock[stock] = m_WeponStock[i];
			stock++;
		}
	}
	for (i = 0; i < MAX_ITEM; i++)
	{
		if (m_ItemStock[i] != 0)
		{
			m_ItemStockType[stock] = ITEM;
			m_AllItemStock[stock] = m_ItemStock[i];
			stock++;
		}
	}
	//もし2ページ目があるならページを切り替えれるようにする
	//アイテムの説明以外の時はカーソル操作可能
	if (!m_ItemTips)
	{
		if (!m_ItemOn && m_WeponStock[2] != 0 && m_ItemStock[5] != 0)
		{
			if (Keyboard_IsTrigger(DIK_A) || Keyboard_IsTrigger(DIK_D))
			{
				m_CursorNumber = 0;
				PlaySound(CURSOR_SE);
				m_NextItemPage = !m_NextItemPage;
				if (m_NextItemPage)
				{
					m_Add_cursor = ((MAX_ITEM + MAX_WEPON) / 2);
				}
				else if (!m_NextItemPage)
				{
					m_Add_cursor = 0;
				}
			}
		}
		if (Keyboard_IsTrigger(DIK_W))
		{
			PlaySound(CURSOR_SE);
			//アイテム選択時じゃないとき
			if (!m_ItemOn)
			{
				m_CursorNumber--;
				if (!m_NextItemPage)
				{			
					if (m_CursorNumber < 0)
					{
						if (m_WeponStock[2] != 0 && m_ItemStock[5] != 0)
						{
							m_CursorNumber = ((MAX_ITEM + MAX_WEPON) / 2) - 1;
						}
						else
						{
							m_CursorNumber = stock - 1;
						}
					}
				}
				else if (m_NextItemPage)
				{
					if (m_CursorNumber < 0)
					{
						m_CursorNumber = stock - 1 - ((MAX_ITEM + MAX_WEPON) / 2);
					}
				}
			}
			else if (m_ItemOn)
			{
				if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == WEPON)
				{
					//選択アイテムが装備の時は何もしない
				}
				else if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == ITEM)
				{
					m_TimeCursorNumber--;
					if (m_TimeCursorNumber < 0)
					{
						m_TimeCursorNumber = 2;
					}
				}
			}
		}
		if (Keyboard_IsTrigger(DIK_S))
		{
			PlaySound(CURSOR_SE);
			if (!m_ItemOn)
			{
				m_CursorNumber++;
				if (!m_NextItemPage)
				{
					if (m_CursorNumber > stock - 1 || m_CursorNumber > ((MAX_ITEM + MAX_WEPON) / 2) - 1)
					{
						m_CursorNumber = 0;
					}
				}
				else if (m_NextItemPage)
				{
					if (m_CursorNumber > stock - 1 - ((MAX_ITEM + MAX_WEPON) / 2) /*|| m_CursorNumber > (MAX_ITEM + MAX_WEPON)*/)
					{
						m_CursorNumber = 0;
					}
				}
			}
			else if (m_ItemOn)
			{
				if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == WEPON)
				{
					//選択アイテムが装備の時は何もしない
				}
				else if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == ITEM)
				{
					m_TimeCursorNumber++;
					if (m_TimeCursorNumber > TIPS_ITEM)
					{
						m_TimeCursorNumber = EFFECT_ITEM;
					}
				}
			}
		}
	}
	if (Keyboard_IsTrigger(DIK_RETURN))
	{
		if (!m_ItemOn)
		{
			PlaySound(CURSOR_SE);
			m_ItemOn = true;
			//右のカーソル初期位置設定
			if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == WEPON)
			{
				m_TimeCursorNumber = TIPS_WEPON;
			}
			else if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == ITEM)
			{
				m_TimeCursorNumber = EFFECT_ITEM;
			}
		}
		else if (m_ItemOn && !m_ItemTips)
		{		
			//対応するTimeCursorの処理
			if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == WEPON)
			{
				PlaySound(CURSOR_SE);
				if (m_TimeCursorNumber == TIPS_WEPON)//アイテム説明
				{
					m_ItemTips = true;
				}
			}
			else if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == ITEM)
			{
				PlaySound(CURSOR_SE);
				if (m_TimeCursorNumber == EFFECT_ITEM)
				{
					CUserinterface::UI_Delete();
					m_ItemOn = false;
					m_Turn = PLAYER_ITEM_EFFECT;
				}
				if (m_TimeCursorNumber == ASIMOTO_ITEM)
				{
					CUserinterface::UI_Delete();
					m_ItemOn = false;
					m_Turn = PLAYER_ITEM_ASIMOTO;
				}
				if (m_TimeCursorNumber == TIPS_ITEM)
				{
					m_ItemTips = true;
				}
			}
		}
		else if (m_ItemOn && m_ItemTips)
		{//１つ前の状態に戻る
			PlaySound(CURSOR_SE);
			m_ItemTips = false;
			if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == WEPON)
			{
				m_TimeCursorNumber = TIPS_WEPON;
			}
			else if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == ITEM)
			{
				m_TimeCursorNumber = TIPS_ITEM;
			}
		}
	}
	if (Keyboard_IsTrigger(DIK_BACK))//戻る処理
	{
		if (!m_ItemOn)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber = CURSOL_LEFT_TOP;
			m_Turn = PLAYER_WINDOW;
		}
		else if(m_ItemOn && !m_ItemTips)
		{
			PlaySound(CURSOR_SE);
			m_TimeCursorNumber = 0;
			m_ItemOn = false;
		}
		else if (m_ItemOn && m_ItemTips)
		{
			PlaySound(CURSOR_SE);
			m_ItemTips = false;
			if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == WEPON)
			{
				m_TimeCursorNumber = 0;
			}
			else if (m_ItemStockType[m_CursorNumber + m_Add_cursor] == ITEM)
			{
				m_TimeCursorNumber = 2;
			}
		}
	}
}

void CPlayer::Player_ItemEffect(void)
{
	if (m_Wait_frame == 0)
	{
		CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_EFFECT, m_AllItemStock[m_CursorNumber + m_Add_cursor]);
	}
	if (m_Wait_frame == ITEM_EFFECT_FRAME)
	{
		switch (m_AllItemStock[m_CursorNumber + m_Add_cursor])
		{
		case CObject::TYPE_YAKUSOU:
			m_Hp += 30;
			if (m_Hp > m_MaxHp)//最大値を超えたら戻す
			{
				m_Hp = m_MaxHp;
			}
			break;
		}
	}
	if (m_Wait_frame == WAIT_ITEM_FRAME)
	{
		//使ったアイテムを削除
		m_ItemStock[m_CursorNumber - m_AllWeponStock + m_Add_cursor] = 0;
		Player_ItemSort();
		m_Wait_frame = 0;
		m_Turn = PLAYER_TURN_END;
	}
	else
	{
		m_Wait_frame++;
	}
}

void CPlayer::Player_ItemAsimoto(void)
{
	if (m_Wait_frame == 0)
	{
		if (CMap::Map_GetData(m_Mapz, m_Mapx).have == CMap::NOTHAVE)
		{
			//足元に何もないときのみ置ける
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_ASIMOTO, m_AllItemStock[m_CursorNumber + m_Add_cursor]);
			CObject::Create(m_AllItemStock[m_CursorNumber + m_Add_cursor], m_Mapx, m_Mapz);
			//置いたアイテムを削除
			m_ItemStock[m_CursorNumber - m_AllWeponStock + m_Add_cursor] = 0;
			Player_ItemSort();
			m_NoItem = true;
			CMap::Map_SetHaveData(m_Mapz, m_Mapx, CMap::HAVEITEM);
		}
		else
		{//置けないメッセージ
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::NOITEM_ASIMOTO, m_AllItemStock[m_CursorNumber + m_Add_cursor]);
		}
	}
	if (m_Wait_frame == WAIT_FRAME)
	{
		m_Wait_frame = 0;
		m_Turn = PLAYER_TURN_END;
	}
	else
	{
		m_Wait_frame++;
	}
}

void CPlayer::Player_CollisionUpdate(void)
{
	int i;
	// マップ位置の更新
	m_Mapz = (int)((m_Colision.position.z - 247.5f) / -5);
	m_Mapx = (int)((m_Colision.position.x + 247.5f) / 5);
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
			enemy->m_Judge_enemy.HitItem = false;
			//壁とプレイヤーの当たり判定
			CEnemy::EnemyVsWall(&enemy->m_Judge_enemy, &enemy->m_EnemyMyColision);
			C3DObj::Collision_AnimeVSAnime(&m_Judge_player, &m_EnemyColision, &enemy->m_Judge_enemy, &enemy->m_EnemyMyColision);
		}
	}
}

void CPlayer::Player_TurboMove(void)
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
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
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
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
	if (walkf == 56)
	{
		Player_TurbolefttopMove();
	}
	if (walkf == 40)
	{
		Player_TurboleftbottomMove();
	}
	if (walkf == 30)
	{
		Player_TurborightbottomMove();
	}
	if (walkf == 24)
	{
		Player_TurborighttopMove();
	}
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Player_Move(void)
{
	if (walkf <= WALK_COUNT)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecplayer * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf++;
		if (walkf == WALK_COUNT + 1)
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}

	if (walkf >= 96)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecplayer * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		walkf--;
		if (walkf == 95)
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
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
	if (walkf == 30)
	{
		Player_rightbottomMove();
	}
	if (walkf == 24)
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
		m_Turn = PLAYER_MOVE;
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

		walkf = 100 + WALK_COUNT - 4;
		vecplayer = m_Right;
		m_Turn = PLAYER_MOVE;
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

		walkf = 100 + WALK_COUNT - 4;
		vecplayer = m_Front;
		m_Turn = PLAYER_MOVE;
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
		m_Turn = PLAYER_MOVE;
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
		//m_Mapx -= 1;
		//m_Mapz += 1;
		m_Turn = PLAYER_MOVE;
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
		//m_Mapx -= 1;
		//m_Mapz -= 1;
		m_Turn = PLAYER_MOVE;
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

		walkf = 30;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		//m_Mapx += 1;
		//m_Mapz -= 1;
		m_Turn = PLAYER_MOVE;
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

		walkf = 24;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		//m_Mapx += 1;
		//m_Mapz += 1;
		m_Turn = PLAYER_MOVE;
		break;
	}
}

void CPlayer::Player_TurboMoveChenge(int MoveType)
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
		m_Turn = PLAYER_MOVE;
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
		m_Turn = PLAYER_MOVE;
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
		m_Turn = PLAYER_MOVE;
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
		m_Turn = PLAYER_MOVE;
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
		//m_Mapx -= 1;
		//m_Mapz += 1;
		m_Turn = PLAYER_MOVE;
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
		//m_Mapx -= 1;
		//m_Mapz -= 1;
		m_Turn = PLAYER_MOVE;
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

		walkf = 30;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		//m_Mapx += 1;
		//m_Mapz -= 1;
		m_Turn = PLAYER_MOVE;
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

		walkf = 24;
		nanawalk = 0;
		vecplayer = m_Front;
		vec2player = m_Right;
		//m_Mapx += 1;
		//m_Mapz += 1;
		m_Turn = PLAYER_MOVE;
		break;
	}
}

void CPlayer::Player_lefttopMove(void)
{
	if (nanawalk <= (WALK_COUNT * 2) + 1)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecplayer * 0.25f;

		m_Position -= vec2player * 0.25f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT * 2)+ 2)
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_righttopMove(void)
{
	if (nanawalk <= (WALK_COUNT * 2) + 1)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecplayer * 0.25f;

		m_Position += vec2player * 0.25f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT * 2) + 2)
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_leftbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT * 2) + 1)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecplayer * 0.25f;

		m_Position -= vec2player * 0.25f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT * 2) + 2)
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_rightbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT * 2) + 1)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecplayer * 0.25f;

		m_Position += vec2player * 0.25f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT * 2) + 2)
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_TurbolefttopMove(void)
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
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_TurborighttopMove(void)
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
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_TurboleftbottomMove(void)
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
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
	}
}

void CPlayer::Player_TurborightbottomMove(void)
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
		{
			if (m_Judge_player.HitItem)
			{
				m_Turn = PLAYER_ITEM_WAIT;
			}
			else
			{
				m_Turn = PLAYER_MOVE_END;
			}
		}
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
		if (m_Wmode)
		{
			CAttack::Attack_PlayerUpdate(charatype, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str);
		}
		else
		{
			CAttack::Attack_PlayerUpdate(charatype, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[SHELD_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str);
		}
	}
	if (attackframe == attack_endframe)
	{
		if (rival_flag)
		{
			//ここでUIモードにしてキー入力待ちを行い、エンターを押したら倒したログを出す
			m_Turn = PLAYER_SERECT_UI;
		}
		else
		{
			m_Turn = PLAYER_ACT_END;
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
	m_Turn = PLAYER_STANDBY;
	//プレイヤーをSTANDBYに戻すと同時にエネミー全てをWAITにする
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);
		if (enemy)
		{
			if (enemy->Get_EnemyTurn() != CEnemy::ENEMY_ACTION)
			{
				enemy->Set_EnemyTurn(CEnemy::ENEMY_WAIT);
			}
		}
	}
}

bool CPlayer::Damage(int str, float angle)
{
	// 後にダメージエフェクトを作成
	if (str > 0)
	{
		Exp_Set(HIT, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		PlaySound(PLAYERDAMAGE_SE);
	}
	else//攻撃がミスの時
	{
		PlaySound(MISS_SE);
	}
	//攻撃を受けた方に向く
	if (angle == 4.8f)
	{
		m_Angle = 1.6f;
	}
	if (angle == 0.0f)
	{
		m_Angle = 3.2f;
	}
	if (angle == 1.6f)
	{
		m_Angle = 4.8f;
	}
	if (angle == 3.2f)
	{
		m_Angle = 0.0f;
	}
	if (angle == 0.8f)
	{
		m_Angle = 4.0f;
	}
	if (angle == 2.4f)
	{
		m_Angle = 5.6f;
	}
	if (angle == 4.0f)
	{
		m_Angle = 0.8f;
	}
	if (angle == 5.6f)
	{
		m_Angle = 2.4f;
	}
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

void CPlayer::Player_OnakaDown(void)
{
	m_Onaka -= 1;
	if (m_Onaka <= 0)
	{
		m_Onaka = 0;
	}
}

bool CPlayer::ExpGoldCheck(int exp, int gold)
{
	//一時的にレベル確保
	int time_lv = m_Lv;
	m_Exp += exp;
	m_Gold += gold;
	//お金が最大値を超えたら戻す
	if (m_Gold > MAX_GOLD)
	{
		m_Gold = MAX_GOLD;
	}
	for (;;)
	{//レベルアップが止まるまで無限ループ
		if (m_Exp >= m_PlayerLvData[m_Lv].lvup_exp)
		{
			m_MaxHp += m_PlayerLvData[m_Lv].maxhp;
			m_Hp += m_PlayerLvData[m_Lv].maxhp;
			m_Str += m_PlayerLvData[m_Lv].str;
			m_Def += m_PlayerLvData[m_Lv].def;
			m_Lv = m_PlayerLvData[m_Lv].lv;
		}
		else//レベルアップしなかったら終了
		{
			break;
		}
	}
	//レベルアップしていたらtrueを返す
	if (time_lv != m_Lv)
	{
		PlaySound(LVUP_SE);
		return true;
	}
	return false;
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

void CPlayer::Player_Check(void)
{
	int i, j;
	C3DObj *object;
	C3DObj *wepon;
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
					{
						m_Judge_player.HitLadder = true;
					}//梯子以外のアイテム
					if (object->m_ObjectType != CObject::TYPE_LADDER)
					{
						m_Judge_player.HitItem = true;
						if (m_ItemStock[MAX_ITEM - 1] != 0) 
						{
							//アイテムを持てないときの処理
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::NOITEM, object->m_ObjectType);
						}
						else
						{
							for (j = 0; j < MAX_ITEM; j++)
							{
								if (m_ItemStock[j] == 0)
								{//アイテムに空きがあったら格納
									CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM, object->m_ObjectType);
									m_ItemStock[j] = object->m_ObjectType;
									CMap::Map_SetHaveData(m_Mapz, m_Mapx, CMap::NOTHAVE);
									object->C3DObj_delete();
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		wepon = CWepon::Get_Wepon(i);
		//装備の判定
		if (wepon)
		{
			if (wepon->alive)
			{
				if (m_Mapz == wepon->m_Mapz && m_Mapx == wepon->m_Mapx)
				{
					if (wepon->m_WeponType == CWepon::TYPE_SWORD)
					{
						Player_WeponChenge(CWepon::TYPE_SWORD, WEPON_NUMBER);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_REIPIA)
					{
						Player_WeponChenge(CWepon::TYPE_REIPIA, WEPON_NUMBER);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_MEISU)
					{
						Player_WeponChenge(CWepon::TYPE_MEISU, WEPON_NUMBER);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_BIGSWORD)
					{
						Player_W_WeponChenge(CWepon::TYPE_BIGSWORD);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_RANCE)
					{
						Player_W_WeponChenge(CWepon::TYPE_RANCE);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_AX)
					{
						Player_W_WeponChenge(CWepon::TYPE_AX);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_SHELD)
					{
						Player_WeponChenge(CWepon::TYPE_SHELD, SHELD_NUMBER);
						wepon->C3DObj_delete();
					}
					if (wepon->m_WeponType == CWepon::TYPE_RING)
					{
						Player_WeponChenge(CWepon::TYPE_RING, RING_NUMBER);
						wepon->C3DObj_delete();
					}
				}
			}
		}
	}
}

void CPlayer::Player_WeponChenge(int wepon_type, int wepondata_number)
{
	int j;
	int WEPONDATA_MAX = CWepon::Get_WEPONDATAMAX();
	int floor_wepon[MAX_WEPON_NUMBER] = { -1 };//武器番号に0があるため-1で初期化
	int floor_wepon_count = 0;
	m_Judge_player.HitItem = true;
	for (j = 0; j < WEPONDATA_MAX; j++)
	{//武器データから対応する武器タイプと出現フロアで出る武器を抽出
		if (CWepon::Get_Wepon_Data(j)->type == wepon_type &&
			CWepon::Get_Wepon_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CWepon::Get_Wepon_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			floor_wepon[floor_wepon_count] = j;
			floor_wepon_count++;
		}
	}
	if (floor_wepon[0] != -1)//武器が存在したら
	{
		//武器番号格納
		m_WeponStock[wepondata_number] = floor_wepon[rand() % floor_wepon_count];
		//+の値決定
		player_wepon[wepondata_number].wepon_purasu_number = rand() % 6;
		//プレイヤー武器データに格納str def type 
		if (CWepon::TYPE_SWORD == wepon_type || CWepon::TYPE_REIPIA == wepon_type || CWepon::TYPE_MEISU == wepon_type)
		{
			player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str + (player_wepon[wepondata_number].wepon_purasu_number * PURASU_UP);
			player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def;
			player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
		}
		else if(CWepon::TYPE_SHELD == wepon_type || CWepon::TYPE_RING == wepon_type)
		{
			player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str;
			player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def + (player_wepon[wepondata_number].wepon_purasu_number * PURASU_UP);
			player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
		}
		
		//↓+値とplayer_wepondata_numberを渡す
		if (m_Wmode)
		{
			if (CWepon::TYPE_SWORD == wepon_type || CWepon::TYPE_REIPIA == wepon_type || CWepon::TYPE_MEISU == wepon_type)
			{
				CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::WEPON_NORMAL_GETCHENGE, m_WeponStock[wepondata_number], player_wepon[wepondata_number].wepon_purasu_number);
				m_Wmode = false;
			}
			else
			{
				if (CWepon::TYPE_SHELD == wepon_type)
				{
					CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::SHELD_W_GET, m_WeponStock[wepondata_number], player_wepon[wepondata_number].wepon_purasu_number);
				}
				else
				{
					CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::WEPON_GET, m_WeponStock[wepondata_number], player_wepon[wepondata_number].wepon_purasu_number);
				}
			}
		}
		else
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::WEPON_GET, m_WeponStock[wepondata_number], player_wepon[wepondata_number].wepon_purasu_number);
		}		
		CMap::Map_SetHaveData(m_Mapz, m_Mapx, CMap::NOTHAVE);
	}
}

void CPlayer::Player_W_WeponChenge(int wepon_type)
{
	int j;
	int WEPONDATA_MAX = CWepon::Get_WEPONDATAMAX();
	int floor_wepon[MAX_WEPON_NUMBER] = { -1 };//武器番号に0があるため-1で初期化
	int floor_wepon_count = 0;
	m_Judge_player.HitItem = true;
	for (j = 0; j < WEPONDATA_MAX; j++)
	{//武器データから対応する武器タイプと出現フロアで出る武器を抽出
		if (CWepon::Get_Wepon_Data(j)->type == wepon_type &&
			CWepon::Get_Wepon_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CWepon::Get_Wepon_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			floor_wepon[floor_wepon_count] = j;
			floor_wepon_count++;
		}
	}
	if (floor_wepon[0] != -1)//武器が存在したら
	{
		//武器番号格納
		m_WeponStock[WEPON_NUMBER] = floor_wepon[rand() % floor_wepon_count];
		//+の値決定
		player_wepon[WEPON_NUMBER].wepon_purasu_number = rand() % 6;
		//プレイヤー武器データに格納str def type 
		player_wepon[WEPON_NUMBER].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->str + (player_wepon[WEPON_NUMBER].wepon_purasu_number * (PURASU_UP * 2));
		player_wepon[WEPON_NUMBER].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->def;
		player_wepon[WEPON_NUMBER].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->type;
		
		
		//↓+値とplayer_WEPON_NUMBERを渡す
		if (!m_Wmode)
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::WEPON_W_GETCHENGE, m_WeponStock[WEPON_NUMBER], player_wepon[WEPON_NUMBER].wepon_purasu_number);
			m_Wmode = true;
		}
		else
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::WEPON_GET, m_WeponStock[WEPON_NUMBER], player_wepon[WEPON_NUMBER].wepon_purasu_number);
		}
		CMap::Map_SetHaveData(m_Mapz, m_Mapx, CMap::NOTHAVE);
	}
}

void CPlayer::Player_ItemSort(void)
{
	int i;
	for (i = m_CursorNumber - m_AllWeponStock + m_Add_cursor; i < MAX_ITEM - 1; i++)
	{
		m_ItemStock[i] = m_ItemStock[i + 1];
	}
}