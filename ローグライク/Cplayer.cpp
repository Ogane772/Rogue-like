///=============================================================================
//
//	Playerクラス
//
//=============================================================================

//=============================================================================
//	インクルードファイル
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
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
#include "save_load.h"
#include "CTexture.h"
#include "CCamera.h"
#include "common.h"
#include "map.h"
#include "CTrap.h"
#include "Attack.h"
#include "scene.h"
#include "collision.h"
#include "sound.h"
#include "CWepon.h"
#include "CEnemy.h"
#include <iostream>
#include <random>



#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define PURASU_UP (2)//装備の+値1当たりの上昇値
#define MAX_HP_CHECK if (m_Hp > m_MaxHp){m_Hp = m_MaxHp;}//最大値を超えたら戻す
#define MAX_ONAKA_CHECK if (m_Onaka > m_MaxOnaka){m_Onaka = m_MaxOnaka;}//最大値を超えたら戻す
#define HEAL_KURAYAMIANDPOIZUN (30)//目隠しと毒回復ターン数
#define BAISOKU_TURN (20)//倍速ターン数（倍ターンを数えるので実質5ターン）
#define DONSOKU_TURN (10)//鈍足ターン数(半分減るので実質5ターン)
#define HEAL_SLEEP (3)//睡眠回復ターン数
#define ITEM_EFFECT_TURN (50)//持続アイテムのターン数
#define LV_DATA_MAX (50)
#define PLAYER_CSV_NAME "CSV/PLAYER_CSV.csv"

bool trigger = false;
//=============================================================================
//	静的変数
//=============================================================================
CPlayer::Player_LvData CPlayer::m_PlayerLvData[LV_DATA_MAX] = { 0 };
enum Animation
{
	IDLE,
	WALK,
	NOANIME,
	ATTACK_2,
	ATTACK_3,
	NOANIME2,
	GET,
	DEATH_ANIME,
	CLEAR,
	WALK2,
	ATTACK_1,
	ATTACK_4,
	MAX_ANIME,
};
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
	Finalize();
}

//=============================================================================
// 初期化処理
//=============================================================================
void CPlayer::Initialize(void)
{
	int i;
	int end_check = 0;//CSVの行の終わりか確認
	//プレイヤーの経験値テーブル読み込み
	for (i = 0; i < LV_DATA_MAX; i++)
	{//2行目から読むので+1する
		end_check = CSV_PlayerLv_Load(&m_PlayerLvData[i], i + 1);
		if (m_PlayerLvData[i].lv == 0)
		{
			break;
		}
	}

	strcpy_s(name, MAX_NAME, "ロビン");
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
	m_GridFlag = false;
	m_NanameFlag = false;
	m_EnemyBack = false;
	m_Hold = false;
	m_Back = false;
	m_BackFrame = 0;
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
	player_wepon[WEPON_NUMBER].wepon_sikibetu_number = CWepon::TYPE_POIZUN_ATTACK;
	m_WeponStock[SHELD_NUMBER] = 1;
	player_wepon[SHELD_NUMBER].wepon_str = 0;
	player_wepon[SHELD_NUMBER].wepon_def = 1;
	player_wepon[SHELD_NUMBER].wepon_type = CWepon::TYPE_SHELD;
	player_wepon[SHELD_NUMBER].wepon_purasu_number = 0;
	player_wepon[SHELD_NUMBER].wepon_sikibetu_number = 0;
	m_WeponStock[RING_NUMBER] = 2;
	player_wepon[RING_NUMBER].wepon_str = 1;
	player_wepon[RING_NUMBER].wepon_def = 1;
	player_wepon[RING_NUMBER].wepon_type = CWepon::TYPE_RING;
	player_wepon[RING_NUMBER].wepon_purasu_number = 0;
	player_wepon[RING_NUMBER].wepon_sikibetu_number = 0;
	m_AutoHell = PLAYER_HEAL;
	
	
	m_ItemStock[0] = CObject::TYPE_YAKUSOU;
	m_ItemStock[1] = 3;
	m_ItemStock[2] = 4; 
	m_ItemStock[3] = 5;
	m_ItemStock[4] = 6;
	
	m_ItemStock[5] = CObject::TYPE_WARP;
	m_ItemStock[6] = CObject::TYPE_WARP;
	m_ItemStock[7] = CObject::TYPE_WARP;
	m_ItemStock[8] = CObject::TYPE_WARP;
	
	m_TrapCheck = 0;
	m_TrapType = 0;
	m_AllWeponStock = 3;
	m_EnemyAttackCount = 0;
	rival_flag = false;
	enter_flag = false;
	turbo = false;
	m_ItemOn = false;
	m_ItemTips = false;
	m_NextItemPage = false;
	m_DarkFlag = false;
	m_Baisoku_Flag = false;
	m_Donsoku_Flag = false;
	m_Gekika = false;
	m_SkillAtaack = false;
	m_Condition = NORMAL_CONDITION;
	m_Add_cursor = 0;
	m_CursorNumber = 0;
	m_TimeCursorNumber = 0;
	m_TurnCount = 0;
	m_Lv = m_PlayerLvData[0].lv;
	m_Gold = 100;
	charatype = 0;
	m_Wait_frame = 0;
	m_OnakaTurn = DEFFAULT_ONAKA_TURN;
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
	m_HealFlag = false;
	m_EnemyOn = false;
	m_EnemyWarp = false;
	m_EnemyDeath = false;
	m_item_turncount = 0;
	// プレイヤーの初期ステータスを決める
	if (CStage::Stage_GetLevel() == 1)
	{//戦いの記憶の変数タスのやっといて
		m_MaxHp = (float)m_PlayerLvData[0].maxhp;
		m_Hp = m_MaxHp;
	//	m_Hp = 10;
		m_Str = m_PlayerLvData[0].str;
		m_Str = 25;
		m_Str2 = 1.0f;
		m_Def = m_PlayerLvData[0].def;
		m_Exp = m_PlayerLvData[0].lvup_exp;
		m_MaxOnaka = MAX_ONAKA;
		//m_MaxOnaka = 21;
		m_Onaka = m_MaxOnaka;
	}
	m_Colision.radius = PLAYER_RADIUS;
	m_EnemyColision.radius = PLAYER_ENERADIUS;


	// 逆かも
	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);
	D3DXVec3Normalize(&m_Right, &m_Right);

	D3DXVec3Cross(&m_Up, &m_Front, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	char animefile[TEXTURE_FILENAME_MAX] = {};
	strcpy_s(animefile, C3DObj::Get_AnimeFileName(MODELL_ANIME_PLAYER));

	SkinMesh.InitThing(m_pD3DDevice, &anime_model, animefile);

	//モデル情報取得
	//Thing_Anime_model = GetAnimeModel();
	for (i = 0; i < (signed)anime_model.pAnimController->GetNumAnimationSets(); i++)
	{//AnimSetにアニメーション情報格納
		anime_model.pAnimController->GetAnimationSet(i, &pAnimSet[i]);
	}
	//アニメーション情報初期化
	TrackDesc.Weight = 1;
	TrackDesc.Enable = true;
	TrackDesc.Position = 0;//アニメーションタイムリセット
	TrackDesc.Speed = 0.005f;//モーションスピード
	anime_model.pAnimController->SetTrackDesc(0, &TrackDesc);//アニメ情報セット
	anime_model.pAnimController->SetTrackAnimationSet(0, pAnimSet[IDLE]);//初期アニメーションセット
	m_AnimationType = IDLE;
	Animation_Change(IDLE, 0.008f);

	GamePadInit();
}

int CPlayer::CSV_PlayerLv_Load(Player_LvData* lvdata, const int num)
{
	int cnt = 0;
	char buf[128];
	int size = 128;
	int data[20] = { 0 };
	FILE* file = NULL;
	if((fopen_s(&file, PLAYER_CSV_NAME, "r")) != 0)
	{
		MessageBox(NULL, "PLAYER_CSVを読み込めませんでした\nCSVを保存しなおせば直る可能性があります", "エラー", MB_OK);
		exit(1);
		return -1;
	}
	//データを読み込む
	while (fgets(buf, size, file) != NULL && num > ++cnt);
	fscanf(file, "%d,%d,%d,%d,%d", &lvdata->lv, &lvdata->lvup_exp,&lvdata->maxhp, &lvdata->str, &lvdata->def);

	fclose(file);



	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}

void CPlayer::Finalize(void)
{
	AnimeModel_Finalize(&anime_model);
	//delete this;
}

//=============================================================================
//  更新
//=============================================================================
void CPlayer::Update(void)
{
	//XBOXコントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}

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
	case PLAYER_WORPFIRST:
		Player_WorpIn();
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
		if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::B_BUTTON)))
		{
			trigger = false;
		}
		if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_BACK) || JoyDevice_IsPress(CONTROLLER::A_BUTTON) && !trigger || JoyDevice_IsPress(CONTROLLER::B_BUTTON) && !trigger)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber = CURSOL_LEFT_TOP;
			m_Turn = PLAYER_WINDOW;
			trigger = true;
		}
		break;
	case PLAYER_WINDOW_ASIMOTO:
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
	case PLAYER_ENEMYTURN_WAIT:
		m_Wait_frame++;
		if (rival_flag)
		{
			if (m_Wait_frame == WAIT_FRAME + 20)
			{
				m_EnemyDeath = false;
				m_Wait_frame = 0;
				m_Turn = PLAYER_KEY_INPUT;
			}
		}
		else
		{
			if (m_Wait_frame == WAIT_FRAME + 10)
			{
				m_EnemyDeath = false;
				m_Wait_frame = 0;
				m_Turn = PLAYER_KEY_INPUT;
			}
		}
		break;
	case PLAYER_TIME_WAIT:
		m_Wait_frame++;
		if (m_Wait_frame == WAIT_FRAME)
		{
			m_Wait_frame = 0;
			m_Turn = PLAYER_STANDBY;
		}
		break;
	case PLAYER_RANGEHIT_WAIT:
		int i;
		C3DObj *enemy;
		if (m_EnemyAttackCount > 0)
		{
			for (i = 0; i < MAX_GAMEOBJ; i++)
			{
				enemy = CEnemy::Get_Enemy(i);
				if (enemy && enemy->Get_RangeHit())
				{
					break;
				}
			}
			if (m_Wait_frame == 0 && enemy)
			{
				int week_check;
				int damage;
				enemy->Set_RangeHit(false);
				week_check = CAttack::Attack_WeekCheck(player_wepon[WEPON_NUMBER].wepon_type, enemy->m_WeponType);
				damage = CAttack::Attack_DamegeKeisan(week_check, enemy, m_Str, m_Str2);
				CAttack::Skill_PlayerEffect(enemy, player_wepon[WEPON_NUMBER].wepon_sikibetu_number, damage, week_check);

			}
			m_Wait_frame++;
			//レベルアップしなければ通常フレーム待つ
			if (m_Wait_frame == ATTACK_END && !rival_flag)
			{
				m_Wait_frame = 0;
				m_EnemyAttackCount -= 1;
				if(m_EnemyAttackCount > 0)
				{//当たった敵がいなくなるまで繰り返す
					m_Turn = PLAYER_RANGEHIT_WAIT;
				}
				else if(m_EnemyAttackCount == 0)
				{
					m_SkillAtaack = false;
					m_Turn = PLAYER_ACT_END;
				}
			}
			//レベルアップしたら更に待つ
			else if(m_Wait_frame == WAIT_FRAME + 100 && rival_flag || m_Wait_frame == WAIT_FRAME + 100 && !rival_flag)
			{
				m_Wait_frame = 0;
				m_EnemyAttackCount -= 1;

				if (m_EnemyAttackCount > 0)
				{//当たった敵がいなくなるまで繰り返す
					m_Turn = PLAYER_RANGEHIT_WAIT;
				}
				else if (m_EnemyAttackCount == 0)
				{
					m_SkillAtaack = false;
					m_Turn = PLAYER_ACT_END;
				}
			}
		}
		
		break;
	case PLAYER_CONDITION_HEAL_WAIT:
		if (m_Wait_frame == 0)
		{//状態異常回復ログ
			CUserinterface::UI_TextCreateCondition(CUserinterface::PLAYERCHARA, CUserinterface::CONDITION_HEAL, m_Condition);
		}
		m_Wait_frame++;
		if (m_Wait_frame == WAIT_FRAME)
		{
			m_Condition = NORMAL_CONDITION;
			m_Wait_frame = 0;
			m_Turn = PLAYER_KEY_INPUT;
		}
		break;
	case PLAYER_SLEEP_WAIT:
		if (m_Wait_frame == 0)
		{//睡眠状態ログ
			CUserinterface::UI_TextCreateCondition(CUserinterface::PLAYERCHARA, CUserinterface::SLEEP_WAIT, m_Condition);
		}
		m_Wait_frame++;
		if (m_Wait_frame == WAIT_FRAME)
		{
			m_Wait_frame = 0;
			m_Turn = PLAYER_TURN_END;
		}
		break;
	case PLAYER_WARP_WAIT:
		if (m_Wait_frame <= 20)
		{
			m_Position.y += 1;
		}
		if (m_Wait_frame == 21)
		{
			CMap::WorpPlayerSet(m_Mapz, m_Mapx);
			Player_WorpIn();
			Player_CollisionUpdate();
		}
		if (m_Wait_frame > 20 && m_Wait_frame < WAIT_FRAME - 5)
		{
			m_Position.y -= 1;
			if (m_Position.y < 0)
			{
				m_Position.y = 0.0f;
			}
		}	
		if (m_Wait_frame == WAIT_FRAME - 5)
		{
			m_Position.y = 0.0f;
			m_Wait_frame = 0;
			//アイテム使用のワープならアイテム削除	
			if (m_CursorNumber >= 3 && !m_NextItemPage || m_NextItemPage)
			{
				m_ItemStock[m_CursorNumber - m_AllWeponStock + m_Add_cursor] = 0;
				Player_ItemSort();
			}
			m_Turn = PLAYER_TURN_END;
		}
		else
		{
			m_Wait_frame++;
		}
		break;
	case PLAYER_ITEM_ASIMOTO:
		Player_ItemAsimoto();
		break;
	case PLAYER_TRAP_EFFECT:
		Player_TrapEffect();
		break;
	case PLAYER_ENEMY_BACK_WAIT:
		if (!m_EnemyBack)
		{//m_EnemyBackがfalseになるまで待機
			if (rival_flag)
			{
				//ここでUIモードにしてキー入力待ちを行い、エンターを押したら倒したログを出す
				m_SkillAtaack = false;
				m_Turn = PLAYER_SERECT_UI;
			}
			else
			{
				m_Wait_frame++;
				if (m_Wait_frame == WAIT_FRAME)
				{
					m_Condition = NORMAL_CONDITION;
					m_Wait_frame = 0;
					m_SkillAtaack = false;
					m_Turn = PLAYER_ACT_END;
				}
			}
		}
		break;
	case PLAYER_MOVE:
		Player_BackCheck();
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
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->walk_count += 1;
	case PLAYER_ACT_END:
		Animation_Change(IDLE, 0.008f);
		m_Turn = PLAYER_TURN_END;
		Player_Check();
		break;
	case PLAYER_ACT:
		Player_Act();
		break;
	case PLAYER_TURN_END:
		Animation_Change(IDLE, 0.008f);
		m_NextItemPage = false;
		m_Add_cursor = 0;
		m_Judge_player.HitItem = false;
		//敵ターン中の時は倍速、鈍足処理を行わない
		if (m_Condition == DONSOKU_CONDITION || m_Condition == BAISOKU_CONDITION)
		{
			m_Turn = PLAYER_TURN_CONPLETE;
		}
		if (m_Condition == DONSOKU_CONDITION && !m_Donsoku_Flag)
		{
			m_Donsoku_Flag = true;
		}
		else if (m_Condition == DONSOKU_CONDITION && m_Donsoku_Flag)
		{
			m_Donsoku_Flag = false;
		}
		if (m_Condition == BAISOKU_CONDITION && !m_Baisoku_Flag)
		{
			m_Baisoku_Flag = true;
			m_Turn = PLAYER_STANDBY;
		}
		break;
	case PLAYER_TURN_CONPLETE:
		m_Baisoku_Flag = false;
		break;
	}
	Player_CollisionUpdate();
	if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 2)
	{
		//m_DarkFlag = true;
	}
	else
	{
		m_DarkFlag = false;
	}
	if (m_Hp < 0)
	{
		m_Hp = 0;
	}

}
//=============================================================================
// 描画
//=============================================================================

void CPlayer::Draw(void)
{
	DrawDX_Anime(m_mtxWorld, &anime_model);
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);	
	//DebugFont_Draw(0, 550, "PmapX = %d,PmapZ = %d", m_Mapx, m_Mapz);
	//DebugFont_Draw(0, 400, "c.pos x = %f c.pos y = %f c.pos z = %f", m_EnemyColision.position.x, m_EnemyColision.position.y, m_EnemyColision.position.z);
	//DebugFont_Draw(0, 450, "c2.pos x = %f c2.pos y = %f c2.pos z = %f", m_Colision.position.x, m_Colision.position.y, m_Colision.position.z);
	//DebugFont_Draw(200, 200, "m_Judge_player.HitLadder = %d", m_Judge_player.HitLadder);
	//DebugFont_Draw(200, 200, "m_Baisoku_Flag = %d", m_Baisoku_Flag);
	//DebugFont_Draw(200, 50, "m_TurnCount = %d", m_TurnCount);
	//DebugFont_Draw(0, 250, "m_EnemyDeath = %d", m_EnemyDeath);
	DebugFont_Draw(0, 150, "m_CursorNumber= %d", m_CursorNumber);
	DebugFont_Draw(0, 200, "turn = %d", m_Turn);
	//DebugFont_Draw(0, 200, "p.pos x = %f p.pos y = %f p.pos z = %f", m_Position.x, m_Position.y, m_Position.z);
	DebugFont_Draw(0, 300, "m_Wait_frame = %d", m_Wait_frame);
	//DebugFont_Draw(200, 20, "m_Judge_player.HitEnemyTop = %d", m_Judge_player.HitEnemyTop);
	//DebugFont_Draw(600, 120, "wepon_type = %d", player_wepon[WEPON_NUMBER].wepon_type);
	//DebugFont_Draw(600, 120, "m_CursorNumber = %d", m_CursorNumber);
	//DebugFont_Draw(300, 160, "m_AddCursor = %d", m_Add_cursor);
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

void CPlayer::Player_SetWorpPos(int z, int x)
{
	start_m_Position = D3DXVECTOR3(-247.5f + x * 5, 0.0f, 247.5f - z * 5);
	m_Colision.position = start_m_Position;
	m_EnemyColision.position = start_m_Position;
	start_m_Mapz = z;
	start_m_Mapx = x;
	m_Turn = PLAYER_WORPFIRST;
}

void CPlayer::Player_First(void)
{
	//階移動時は全ての状態をリセットする
	SetTurnCount(0);
	m_Condition = C3DObj::NORMAL_CONDITION;
	m_Str2 = 1.0f;
	m_Position = D3DXVECTOR3(start_m_Position.x,m_Position.y,start_m_Position.z);
	m_Mapz = start_m_Mapz;
	m_Mapx = start_m_Mapx;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	nanawalk = 0;
	goladder = false;
	g_bIsFade = false;
	m_EnemyOn = false;
	m_Gekika = false;
	m_Turn = PLAYER_STANDBY;
}

void CPlayer::Player_WorpIn(void)
{
	m_Position = D3DXVECTOR3(start_m_Position.x, m_Position.y, start_m_Position.z);
	m_Mapz = start_m_Mapz;
	m_Mapx = start_m_Mapx;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	nanawalk = 0;
	m_Turn = PLAYER_WARP_WAIT;
}


void CPlayer::Player_Standby(void)
{
	int poizun_damage = 0;
	m_TurnCount++;
	if (m_Condition == POIZUN_CONDITION)
	{
		poizun_damage = (int)(m_MaxHp / 50);
		if (poizun_damage < 1)
		{
			poizun_damage = 1;
		}
		m_Hp -= poizun_damage;
		m_Turn = PLAYER_KEY_INPUT;
		Animation_Change(IDLE, 0.008f);
	}
	if (m_Onaka == 0)
	{
		m_Hp -= 1;
	}
	if (m_Hp <= 0)
	{
		m_Turn = PLAYER_DESTROY;
	}
	else if(m_Condition != POIZUN_CONDITION)
	{
		//HP自動回復
		//指輪が癒しの指輪だったらそっちも適用
		if (player_wepon[RING_NUMBER].wepon_sikibetu_number == CWepon::TYPE_IYASI_RING)
		{
			switch (player_wepon[RING_NUMBER].wepon_purasu_number)
			{
			case 0:
				m_Hp += m_MaxHp / (m_AutoHell - PLAYER_RING_HEAL);
				break;
			case 1:
				m_Hp += m_MaxHp / (m_AutoHell - (PLAYER_RING_HEAL * 2));
				break;
			case 2:
				m_Hp += m_MaxHp / (m_AutoHell - (PLAYER_RING_HEAL * 3));
				break;
			case 3:
				m_Hp += m_MaxHp / (m_AutoHell - (PLAYER_RING_HEAL * 4));
				break;
			case 4:
				m_Hp += m_MaxHp / (m_AutoHell - (PLAYER_RING_HEAL * 5));
				break;
			case 5:
				m_Hp += m_MaxHp / (m_AutoHell - (PLAYER_RING_HEAL * 6));
				break;
			}
		}
		else
		{
			m_Hp += m_MaxHp / m_AutoHell;
		}
		if (m_Hp > m_MaxHp)
		{
			m_Hp = m_MaxHp;
		}
		m_Turn = PLAYER_KEY_INPUT;
		Animation_Change(IDLE, 0.008f);
	}
	if (m_Condition == SLEEP_CONDITION)
	{
		m_Turn = PLAYER_SLEEP_WAIT;
	}
	if (m_Condition == DONSOKU_CONDITION && !m_Donsoku_Flag)
	{
		m_Turn = PLAYER_TURN_END;
	}
	if (m_Condition != NORMAL_CONDITION && m_TurnCount == HEAL_KURAYAMIANDPOIZUN || m_Condition == SLEEP_CONDITION && m_TurnCount == HEAL_SLEEP)
	{
		m_Turn = PLAYER_CONDITION_HEAL_WAIT;
	}
	if (m_Condition == BAISOKU_CONDITION && m_TurnCount == BAISOKU_TURN)
	{
		m_Baisoku_Flag = false;
		m_Turn = PLAYER_CONDITION_HEAL_WAIT;
	}
	if (m_Condition == DONSOKU_CONDITION && m_TurnCount == DONSOKU_TURN)
	{
		m_Donsoku_Flag = false;
		m_Turn = PLAYER_CONDITION_HEAL_WAIT;
	}
	if (m_HealFlag)
	{
		m_item_turncount++;
		if (m_item_turncount == ITEM_EFFECT_TURN)
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::FREE_MESSAGE, "道具の効果が切れた");
			m_item_turncount = 0;
			m_HealFlag = false;
			m_AutoHell += PLAYER_HIGH_HEAL;
			m_Turn = PLAYER_TIME_WAIT;
		}
	}
}

void CPlayer::Player_KeyInput(void)
{
	if (m_EnemyDeath)
	{
		m_Turn = PLAYER_ENEMYTURN_WAIT;
		return;
	}
	if (rival_flag)
	{
		return;
	}
	//以下のキーが押されてなければボタンを押していい状態にする
	if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::Y_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::X_BUTTON)))
	{
		trigger = false;
	}
	if (Keyboard_IsRelease(DIK_UP) || Keyboard_IsRelease(DIK_DOWN) || Keyboard_IsRelease(DIK_LEFT) || Keyboard_IsRelease(DIK_RIGHT))
	{
		m_Hold = false;
	}
	m_Hold = false;
	turbo = false;
	m_NanameFlag = false;
	//グリッドモード取得
	if (Keyboard_IsTrigger(DIK_Q) || pJoyDevice && JoyDevice_IsTrigger(CONTROLLER::L_BUTTON))
	{
		if(!m_GridFlag)
		m_GridFlag = true;
		else
		m_GridFlag = false;
	}
	
	//斜め移動モード取得
	if (Keyboard_IsPress(DIK_LSHIFT) || pJoyDevice && JoyDevice_IsPress(CONTROLLER::L_BUTTON))
	{
		m_NanameFlag = true;
	}
	if (Keyboard_IsRelease(DIK_LSHIFT) || pJoyDevice && !JoyDevice_IsPress(CONTROLLER::L_BUTTON))
	{
		m_NanameFlag = false;
	}
	//次の階選択画面
	if (goladder)
	{
		m_Turn = PLAYER_SERECT_UI;
		return;
	}
	//足踏み
	if (Keyboard_IsPress(DIK_O) || JoyDevice_IsPress(CONTROLLER::R_BUTTON))
	{
		m_Turn = PLAYER_MOVE_END;
	}
	//倍速モード
	if (Keyboard_IsPress(DIK_RSHIFT) || JoyDevice_IsPress(CONTROLLER::B_BUTTON))
	{
		turbo = true;
	}
	//コマンドウィンドウを開く
	if (Keyboard_IsTrigger(DIK_P) || JoyDevice_IsPress(CONTROLLER::X_BUTTON) && !trigger)
	{
		PlaySound(CURSOR_SE);
		m_CursorNumber = 0;
		m_Turn = PLAYER_WINDOW;
		trigger = true;
	}
	//===================================================
	// 方向転換
	//===================================================
	// 上方向
	if (Keyboard_IsPress(DIK_UP) || js.lRx >= -2 && js.lRx <= 2 && js.lRy == -6)
	{
		AngleChange(UP_ANGLE);
	}

	// 左方向4.8
	if (Keyboard_IsPress(DIK_LEFT) || js.lRy >= -2 && js.lRy <= 2 && js.lRx == -6)
	{
		AngleChange(LEFT_ANGLE);
	}

	//	下方向3.2
	if (Keyboard_IsPress(DIK_DOWN) || js.lRx >= -2 && js.lRx <= 2 && js.lRy == 6)
	{
		AngleChange(DOWN_ANGLE);
	}

	//	右方向1.6
	if (Keyboard_IsPress(DIK_RIGHT) || js.lRy >= -2 && js.lRy <= 2 && js.lRx == 6)
	{
		AngleChange(RIGHT_ANGLE);
	}

	// 左上方向
	if (Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_LEFT) || js.lRx >= -6 && js.lRx <= -3 && js.lRy >= -5 && js.lRy <= -3)
	{
		AngleChange(LEFT_TOP_ANGLE);
	}

	// 左下方向
	if (Keyboard_IsPress(DIK_DOWN) && Keyboard_IsPress(DIK_LEFT) || js.lRx >= -6 && js.lRx <= -3 && js.lRy >= 3 && js.lRy <= 5)
	{
		AngleChange(LEFT_DOWN_ANGLE);
	}

	// 右下方向
	if (Keyboard_IsPress(DIK_DOWN) && Keyboard_IsPress(DIK_RIGHT) || js.lRx >= 3 && js.lRx <= 6 && js.lRy >= 3 && js.lRy <= 6)
	{
		AngleChange(RIGHT_DOWN_ANGLE);
	}

	// 右上方向
	if (Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_RIGHT) || js.lRx >= 3 && js.lRx <= 6 && js.lRy >= -6 && js.lRy <= -3)
	{
		AngleChange(RIGHT_TOP_ANGLE);
	}

	//===================================================
	// 攻撃
	//===================================================
	if (Keyboard_IsTrigger(DIK_SPACE) || JoyDevice_IsPress(CONTROLLER::A_BUTTON) && !trigger)
	{
		m_Turn = PLAYER_ACT;
		m_SkillAtaack = false;
		attackframe = 0;
		attack_endframe = NOATTACK_END;
		Animation_Change(ATTACK_1, 0.017f);
		trigger = true;
		return;
	}
	//武器スキル攻撃
	if (Keyboard_IsTrigger(DIK_L) || JoyDevice_IsPress(CONTROLLER::Y_BUTTON) && !trigger)
	{
		if (SkillHpCheck())
		{
			m_Turn = PLAYER_ACT;
			m_SkillAtaack = true;
			attackframe = 0;
			attack_endframe = NOATTACK_END;
			Animation_Change(ATTACK_1, 0.017f);
			return;
		}
		else
		{
			PlaySound(CURSOR_NO_SE);
		}
		trigger = true;
	}
	//===================================================
	// 移動
	//===================================================
	if (!m_Judge_player.HitTop)
	{
		// 上移動
		if (Keyboard_IsPress(DIK_W) && !m_NanameFlag || (js.lX >= -2 && js.lX <= 2) && js.lY == -6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(UP_BUTTON)) && !m_NanameFlag)
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
			m_Hold = true;
			Animation_Change(WALK, 0.04f);
			return;
		}
	}
	else if (m_Judge_player.HitTop)
	{
		if (Keyboard_IsPress(DIK_W) && !m_NanameFlag || (js.lX >= -2 && js.lX <= 2) && js.lY == -6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(UP_BUTTON)) && !m_NanameFlag)
		{
			AngleChange(UP_ANGLE);
		}
	}
	if (!m_Judge_player.HitLeft)
	{
		// 左移動
		if (Keyboard_IsPress(DIK_A) && !m_NanameFlag || (js.lY >= -2 && js.lY <= 2) && js.lX == -6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(LEFT_BUTTON)) && !m_NanameFlag)
		{
			m_Hold = true;
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
			Animation_Change(WALK, 0.04f);
			return;
		}
	}
	else if (m_Judge_player.HitLeft)
	{
		if (Keyboard_IsPress(DIK_A) && !m_NanameFlag || (js.lY >= -2 && js.lY <= 2) && js.lX == -6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(LEFT_BUTTON)) && !m_NanameFlag)
		{
			AngleChange(LEFT_ANGLE);
		}
	}
	// 下移動3.2
	if (!m_Judge_player.HitBottom)
	{
		if (Keyboard_IsPress(DIK_S) && !m_NanameFlag || (js.lX >= -2 && js.lX <= 2) && js.lY == 6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && !m_NanameFlag)
		{
			m_Hold = true;
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
			Animation_Change(WALK, 0.04f);
			return;
		}
	}
	else if (m_Judge_player.HitBottom)
	{
		if (Keyboard_IsPress(DIK_S) && !m_NanameFlag || (js.lX >= -2 && js.lX <= 2) && js.lY == 6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && !m_NanameFlag)
		{
			AngleChange(DOWN_ANGLE);
		}
	}
	//右移動1.6
	if (!m_Judge_player.HitRight)
	{
		if (Keyboard_IsPress(DIK_D) && !m_NanameFlag || (js.lY >= -2 && js.lY <= 2) && js.lX == 6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(RIGHT_BUTTON)) && !m_NanameFlag)
		{
			m_Hold = true;
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
			Animation_Change(WALK, 0.04f);
			return;
		}
	}
	else if(m_Judge_player.HitRight)
	{
		if (Keyboard_IsPress(DIK_D) && !m_NanameFlag || (js.lY >= -2 && js.lY <= 2) && js.lX == 6 && !m_NanameFlag || (JoyDevice_IsCrossTrigger(RIGHT_BUTTON)) && !m_NanameFlag)
		{
			AngleChange(RIGHT_ANGLE);
		}
	}

	if (!m_Judge_player.HitTopLeft)
	{
		// 左上移動
		if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_A) && m_NanameFlag || (js.lX >= -6 && js.lX <= -3) && (js.lY >= -5 && js.lY <= -3) || (JoyDevice_IsCrossTrigger(LEFT_TOP_BUTTON)) && m_NanameFlag)
		{
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx - 1).type != 0)
			{
				m_Hold = true;
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
				Animation_Change(WALK, 0.04f);
				return;
			}
		}
	}
	else if (m_Judge_player.HitTopLeft)
	{
		if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_A) && m_NanameFlag || (js.lX >= -6 && js.lX <= -3) && (js.lY >= -5 && js.lY <= -3) || (JoyDevice_IsCrossTrigger(LEFT_TOP_BUTTON)) && m_NanameFlag)
		{
			AngleChange(LEFT_TOP_ANGLE);
		}
	}
	if (!m_Judge_player.HitBottomLeft)
	{
		// 左下移動
		if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_A) && m_NanameFlag || (js.lX >= -6 && js.lX <= -3) && (js.lY >= 3 && js.lY <= 5) || (JoyDevice_IsCrossTrigger(LEFT_DOWN_BUTTON)) && m_NanameFlag)
		{
			m_Hold = true;
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
				Animation_Change(WALK, 0.04f);
				return;
			}
		}
	}
	else if (m_Judge_player.HitBottomLeft)
	{
		if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_A) && m_NanameFlag || (js.lX >= -6 && js.lX <= -3) && (js.lY >= 3 && js.lY <= 5) || (JoyDevice_IsCrossTrigger(LEFT_DOWN_BUTTON)) && m_NanameFlag)
		{
			AngleChange(LEFT_DOWN_ANGLE);
		}
	}

	if (!m_Judge_player.HitBottomRight)
	{
		// 右下移動
		if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_D) && m_NanameFlag || (js.lX >= 3 && js.lX <= 6) && (js.lY >= 3 && js.lY <= 6) || (JoyDevice_IsCrossTrigger(RIGHT_DOWN_BUTTON)) && m_NanameFlag)
		{
			m_Hold = true;
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
				Animation_Change(WALK, 0.04f);
				return;
			}
		}
	}
	else if (m_Judge_player.HitBottomRight)
	{
		if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_D) && m_NanameFlag || (js.lX >= 3 && js.lX <= 6) && (js.lY >= 3 && js.lY <= 6) || (JoyDevice_IsCrossTrigger(RIGHT_DOWN_BUTTON)) && m_NanameFlag)
		{
			AngleChange(RIGHT_DOWN_ANGLE);
		}
	}

	if (!m_Judge_player.HitTopRight)
	{
		// 右上移動
		if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_D) && m_NanameFlag || (js.lX >= 3 && js.lX <= 6) && (js.lY >= -6 && js.lY <= -3) || (JoyDevice_IsCrossTrigger(RIGHT_TOP_BUTTON)) && m_NanameFlag)
		{
			m_Hold = true;
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
				Animation_Change(WALK, 0.04f);
				return;
			}
		}
	}
	else if (m_Judge_player.HitTopRight)
	{
		if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_D) && m_NanameFlag || (js.lX >= 3 && js.lX <= 6) && (js.lY >= -6 && js.lY <= -3) || (JoyDevice_IsCrossTrigger(RIGHT_TOP_BUTTON)) && m_NanameFlag)
		{
			AngleChange(RIGHT_TOP_ANGLE);
		}
	}
}

void CPlayer::Player_WindowMode(void)
{

	//何もキーが押されてなければボタンを押していい状態にする
	if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::B_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::X_BUTTON)) && !(JoyDevice_IsCrossTrigger(LEFT_BUTTON)) && !(JoyDevice_IsCrossTrigger(RIGHT_BUTTON)) && !(JoyDevice_IsCrossTrigger(UP_BUTTON)) && !(JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && (js.lRx >= -1 && js.lRx <= 1 && js.lRy >= -1 && js.lRy <= 1))
	{
		trigger = false;
	}
	if (Keyboard_IsTrigger(DIK_RETURN) || JoyDevice_IsTrigger(CONTROLLER::A_BUTTON) && !trigger)
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
		trigger = true;
	}
	if (Keyboard_IsTrigger(DIK_P) || Keyboard_IsTrigger(DIK_BACK) || JoyDevice_IsTrigger(CONTROLLER::B_BUTTON) && !trigger || JoyDevice_IsTrigger(CONTROLLER::X_BUTTON) && !trigger)
	{
		PlaySound(CURSOR_SE);
		m_Turn = PLAYER_KEY_INPUT;
		trigger = true;
	}
	if (Keyboard_IsTrigger(DIK_W) || JoyDevice_IsCrossTrigger(UP_BUTTON) && !trigger && (js.lX >= -2 && js.lX <= 2) && js.lY == -6 && !trigger)
	{
		if (m_CursorNumber == CURSOL_LEFT_DOWN || m_CursorNumber == CURSOL_RIGHT_DOWN)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber -= 2;
			trigger = true;
		}
	}
	if (Keyboard_IsTrigger(DIK_D) || JoyDevice_IsCrossTrigger(RIGHT_BUTTON) && !trigger && (js.lY >= -2 && js.lY <= 2) && js.lX == 6 && !trigger)
	{
		if (m_CursorNumber == CURSOL_LEFT_TOP || m_CursorNumber == CURSOL_LEFT_DOWN)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber++;
			trigger = true;
		}
	}
	if (Keyboard_IsTrigger(DIK_A) || JoyDevice_IsCrossTrigger(LEFT_BUTTON) && !trigger && (js.lY >= -2 && js.lY <= 2) && js.lX == -6 && !trigger)
	{
		if (m_CursorNumber == CURSOL_RIGHT_TOP || m_CursorNumber == CURSOL_RIGHT_DOWN)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber--;
			trigger = true;
		}
	}
	if (Keyboard_IsTrigger(DIK_S) || JoyDevice_IsCrossTrigger(DOWN_BUTTON) && !trigger && (js.lX >= -2 && js.lX <= 2) && js.lY == 6 && !trigger)
	{
		if (m_CursorNumber == CURSOL_LEFT_TOP || m_CursorNumber == CURSOL_RIGHT_TOP)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber += 2;
			trigger = true;
		}
	}
}

void CPlayer::Player_SelectUi(void)
{
	if (rival_flag)
	{
	//	if (Keyboard_IsTrigger(DIK_RETURN))
	//	{
	//		enter_flag = true;
	//	}
	}
	else
	{
		if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) &&  !(JoyDevice_IsCrossTrigger(UP_BUTTON)) && !(JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && (js.lRx >= -1 && js.lRx <= 1 && js.lRy >= -1 && js.lRy <= 1))
		{
			trigger = false;
		}
		if (Keyboard_IsTrigger(DIK_UP) || Keyboard_IsTrigger(DIK_W) || (JoyDevice_IsCrossTrigger(UP_BUTTON)) && !trigger && (js.lX >= -2 && js.lX <= 2) && js.lY == -6 && !trigger)
		{
			PlaySound(CURSOR_SE);
			goladder = true;
			trigger = true;
		}
		if (Keyboard_IsTrigger(DIK_DOWN) || Keyboard_IsTrigger(DIK_S) || (JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && !trigger && (js.lX >= -2 && js.lX <= 2) && js.lY == 6 && !trigger)
		{
			PlaySound(CURSOR_SE);
			goladder = false;
			trigger = true;
		}
		if (goladder)
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::GOLADDER);
		}
		if (!goladder)
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::NOTGOLADDER);
		}
		if (Keyboard_IsTrigger(DIK_RETURN) || (JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !trigger)
		{
			trigger = true;
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
					SAVE_COUNT *save;
					save = Get_SaveCount();
					save->floor_count += 1;
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
	if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::B_BUTTON)) && !(JoyDevice_IsCrossTrigger(LEFT_BUTTON)) && !(JoyDevice_IsCrossTrigger(RIGHT_BUTTON)) && !(JoyDevice_IsCrossTrigger(UP_BUTTON)) && !(JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && (js.lRx >= -1 && js.lRx <= 1 && js.lRy >= -1 && js.lRy <= 1))
	{
		trigger = false;
	}
	//もし2ページ目があるならページを切り替えれるようにする
	//アイテムの説明以外の時はカーソル操作可能
	if (!m_ItemTips)
	{
		if (!m_ItemOn && m_WeponStock[2] != 0 && m_ItemStock[5] != 0)
		{
			if (Keyboard_IsTrigger(DIK_A) || Keyboard_IsTrigger(DIK_D) || (JoyDevice_IsCrossTrigger(LEFT_BUTTON)) && !trigger || (JoyDevice_IsCrossTrigger(RIGHT_BUTTON)) && !trigger && (js.lY >= -2 && js.lY <= 2) && js.lX == 6 && !trigger && (js.lY >= -2 && js.lY <= 2) && js.lX == -6 && !trigger)
			{
				trigger = true;
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
		if (Keyboard_IsTrigger(DIK_W) || (JoyDevice_IsCrossTrigger(UP_BUTTON)) && !trigger && (js.lX >= -2 && js.lX <= 2) && js.lY == -6 && !trigger)
		{
			trigger = true;
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
		if (Keyboard_IsTrigger(DIK_S) || (JoyDevice_IsCrossTrigger(DOWN_BUTTON)) && !trigger && (js.lX >= -2 && js.lX <= 2) && js.lY == 6 && !trigger)
		{
			trigger = true;
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
	if (Keyboard_IsTrigger(DIK_RETURN) || (JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !trigger)
	{
		trigger = true;
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
	if (Keyboard_IsTrigger(DIK_BACK) || (JoyDevice_IsTrigger(CONTROLLER::B_BUTTON) && !trigger))//戻る処理
	{
		trigger = true;
		if (!m_ItemOn)
		{
			PlaySound(CURSOR_SE);
			m_CursorNumber = CURSOL_LEFT_TOP;
			m_Add_cursor = 0;
			m_Turn = PLAYER_WINDOW;
			m_TimeCursorNumber = 0;
			m_NextItemPage = false;
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
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 2);
	if (m_Wait_frame == 0)
	{
		if (m_AllItemStock[m_CursorNumber + m_Add_cursor] != CObject::TYPE_KENMASOU)
		{
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_EFFECT, m_AllItemStock[m_CursorNumber + m_Add_cursor]);
		}
		else if (m_AllItemStock[m_CursorNumber + m_Add_cursor] == CObject::TYPE_KENMASOU)
		{//研磨草なら装備の+値を上げる
			int wepon_number = random(mt);
			switch (wepon_number)
			{
			case WEPON_NUMBER:
				//+値が5だったら再抽選
				if (player_wepon[0].wepon_purasu_number == 5 && player_wepon[1].wepon_purasu_number == 5 && player_wepon[2].wepon_purasu_number == 5)
				{
					CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_EFFECT, m_AllItemStock[m_CursorNumber + m_Add_cursor], "しかし効果がなかった！");
				}
				else
				{
					if (player_wepon[wepon_number].wepon_purasu_number < 5)
					{
						player_wepon[wepon_number].wepon_purasu_number += 1;
						//両手持ちなら補正値倍
						if (m_Wmode)
						{
							player_wepon[wepon_number].wepon_str += (PURASU_UP + PURASU_UP);
						}
						else if (!m_Wmode)
						{
							player_wepon[wepon_number].wepon_str += PURASU_UP;
						}
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_EFFECT, m_AllItemStock[m_CursorNumber + m_Add_cursor], "武器の+値が1増えた！");
					}
					else
					{
						m_Wait_frame--;
					}
				}
				break;
			case SHELD_NUMBER:
				if (player_wepon[wepon_number].wepon_purasu_number < 5)
				{
					player_wepon[wepon_number].wepon_purasu_number += 1;
					player_wepon[wepon_number].wepon_def += PURASU_UP;
					CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_EFFECT, m_AllItemStock[m_CursorNumber + m_Add_cursor], "盾の+値が1増えた！");
				}
				else
				{
					m_Wait_frame--;
				}
				break;
			case RING_NUMBER:
				if (player_wepon[wepon_number].wepon_purasu_number < 5)
				{
					player_wepon[wepon_number].wepon_purasu_number += 1;
					//指輪の能力値が0同士じゃなければ能力値を上げる
					if (player_wepon[wepon_number].wepon_str != 0 && player_wepon[wepon_number].wepon_def != 0)
					{
						if (player_wepon[wepon_number].wepon_str > player_wepon[wepon_number].wepon_def)
						{
							player_wepon[wepon_number].wepon_str += PURASU_UP;
						}
						if (player_wepon[wepon_number].wepon_def > player_wepon[wepon_number].wepon_str)
						{
							player_wepon[wepon_number].wepon_def += PURASU_UP;
						}
						if (player_wepon[wepon_number].wepon_str == player_wepon[wepon_number].wepon_def)
						{
							player_wepon[wepon_number].wepon_str += PURASU_UP;
							player_wepon[wepon_number].wepon_def += PURASU_UP;
						}
					}
					CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_EFFECT, m_AllItemStock[m_CursorNumber + m_Add_cursor], "指輪の+値が1増えた！");
				}
				else
				{
					m_Wait_frame--;
				}
				break;
			}
			
		}
	}
	if (m_Wait_frame == ITEM_EFFECT_FRAME)
	{
		switch (m_AllItemStock[m_CursorNumber + m_Add_cursor])
		{
		case CObject::TYPE_YAKUSOU:
			m_Hp += 30;
			MAX_HP_CHECK;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_ZAXTUSOU:
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_BAIKA:
			m_Str2 = CHARGE_BUFF;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_DOKUKESI:
			if (m_Condition == C3DObj::POIZUN_CONDITION)
			{
				m_Condition = C3DObj::NORMAL_CONDITION;
			}
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_PATTIRI:
			if (m_Condition == C3DObj::KURAYAMI_CONDITION)
			{
				m_Condition = C3DObj::NORMAL_CONDITION;
			}
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_IYASI:
			m_HealFlag = true;
			m_AutoHell -= PLAYER_HIGH_HEAL;
			m_item_turncount = 0;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_GEKIKA:
			m_Gekika = true;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_WARP:
			m_Wait_frame = 0;
			m_Turn = PLAYER_WARP_WAIT;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_SIAWASE:
			//最大レベルの時は無効にすること
			for (;;)
			{//次のレベルまで経験値を1にする
				if (m_PlayerLvData[m_Lv].lvup_exp - m_Exp > 1)
				{
					m_Exp++;
				}
				else
				{
					break;
				}
			}
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_TOKU_YAKUSOU:
			m_Hp += 70;
			MAX_HP_CHECK;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_PUTI_KAKI:
			m_Onaka += 30;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_BIG_KAKI:
			m_Onaka += 50;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_KASOKU:
			m_Condition = BAISOKU_CONDITION;
			SetTurnCount(0);
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_YOSOKU:
			m_EnemyOn = true;
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_TANKEN:
			int i;
			C3DObj *object;
			C3DObj *wepon;
			C3DObj *trap;
			for (i = 0; i < MAX_GAMEOBJ; i++)
			{
				object = CObject::Get_Object(i);
				//オブジェクトの判定
				if (object)
				{
					if (object->alive)
					{
						object->Set_MapDrawFlag(true);
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
						wepon->Set_MapDrawFlag(true);
					}
				}
			}
			for (i = 0; i < MAX_GAMEOBJ; i++)
			{
				trap = CTrap::Get_Trap(i);
				//装備の判定
				if (trap)
				{
					if (trap->alive)
					{
						trap->Set_MapDrawFlag(true);
					}
				}
			}
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		case CObject::TYPE_KENMASOU:
			//研磨草のみフレーム０で処理を行う
			m_Onaka += 5;
			MAX_ONAKA_CHECK;
			break;
		}
	}
	if (m_Wait_frame == WAIT_ITEM_FRAME)
	{
		//使ったアイテムを削除
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->item_count += 1;
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

void CPlayer::Player_TrapEffect(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 2);
	std::random_device rd2;
	std::mt19937 mt2(rd2());
	std::uniform_int_distribution<int> random_trap_check(1, 100);
	int delete_item = 0;
	//罠避けの指輪なら+値下げる
	if (m_Wait_frame == 0)
	{
		if (player_wepon[RING_NUMBER].wepon_sikibetu_number == CWepon::TYPE_WANAYOKE_RING)
		{
			//罠避けの指輪だったら+値を下げ罠を無効にする
			//+値が0だったらただの指輪にする
		
			player_wepon[RING_NUMBER].wepon_purasu_number -= 1;
			if (player_wepon[RING_NUMBER].wepon_purasu_number == -1)
			{
				CUserinterface::UI_TextCreateTrapRog(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType, "罠避けの指輪が効果を無効にした！\n指輪が壊れた！");
				m_WeponStock[RING_NUMBER] = 2;
				player_wepon[RING_NUMBER].wepon_str = 1;
				player_wepon[RING_NUMBER].wepon_def = 1;
				player_wepon[RING_NUMBER].wepon_type = CWepon::TYPE_RING;
				player_wepon[RING_NUMBER].wepon_purasu_number = 0;
				player_wepon[RING_NUMBER].wepon_sikibetu_number = 0;
			}
			else
			{
				CUserinterface::UI_TextCreateTrapRog(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType, "罠避けの指輪が効果を無効にした！");
			}
		} 
		else
		{
			m_TrapCheck = random_trap_check(mt2);
			if (m_TrapType < CTrap::TYPE_ITEMDELETE_TRAP)
			{
				if (m_TrapCheck <= TRAP_OK)
				{
					CUserinterface::UI_TextCreateTrap(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType);
				}
				else
				{
					CUserinterface::UI_TextCreateTrap(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_NOEFFECT, m_TrapType);
				}
			}
			else if (m_TrapType == CTrap::TYPE_ITEMDELETE_TRAP)
			{
				if (m_TrapCheck <= TRAP_OK)
				{
					std::random_device rd;
					std::mt19937 mt(rd());
					std::uniform_int_distribution<int> random(0, MAX_ITEM);
					for (;;)
					{//アイテムが入ってる場所を検索
						delete_item = random(mt);
						//アイテムが無くてもbreak;
						if (m_ItemStock[delete_item] != 0 || m_ItemStock[0] == 0)
						{
							break;
						}
					}
					CUserinterface::UI_TextCreateDeleteItem(CUserinterface::PLAYERCHARA, CUserinterface::ITEM_DELETE, m_ItemStock[delete_item], CTrap::Get_TrapName(CTrap::TYPE_ITEMDELETE_TRAP), "を踏んでしまった！");//アイテム削除用
					m_ItemStock[delete_item] = 0;
					Player_ItemSort2();
				}
				else
				{
					CUserinterface::UI_TextCreateTrap(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_NOEFFECT, m_TrapType);
				}
			}
			else if (m_TrapType == CTrap::TYPE_SABIRU_TRAP)
			{//錆びる罠なら装備の+値をさげる
				int wepon_number = random(mt);
				switch (wepon_number)
				{
				case WEPON_NUMBER:
					//+値が0だったら再抽選
					if (player_wepon[wepon_number].wepon_purasu_number > 0)
					{
						player_wepon[wepon_number].wepon_purasu_number -= 1;
						//両手持ちなら補正値倍
						if (m_Wmode)
						{
							player_wepon[wepon_number].wepon_str -= (PURASU_UP + PURASU_UP);
						}
						else if (!m_Wmode)
						{
							player_wepon[wepon_number].wepon_str -= PURASU_UP;
						}
						CUserinterface::UI_TextCreateTrapRog(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType, "武器の+値が1減った");
					}
					else
					{
						m_Wait_frame--;
					}
					break;
				case SHELD_NUMBER:
					if (player_wepon[wepon_number].wepon_purasu_number > 0)
					{
						player_wepon[wepon_number].wepon_purasu_number -= 1;
						player_wepon[wepon_number].wepon_def -= PURASU_UP;
						CUserinterface::UI_TextCreateTrapRog(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType, "盾の+値が1減った");
					}
					else
					{
						m_Wait_frame--;
					}
					break;
				case RING_NUMBER:
					if (player_wepon[0].wepon_purasu_number == 0 && player_wepon[1].wepon_purasu_number == 0 && player_wepon[2].wepon_purasu_number == 0)
					{
						CUserinterface::UI_TextCreateTrapRog(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType, "しかし効果がなかった！");
					}
					else
					{
						if (player_wepon[wepon_number].wepon_purasu_number > 0)
						{
							player_wepon[wepon_number].wepon_purasu_number -= 1;
							//指輪の能力値が0同士じゃなければ能力値を上げる
							if (player_wepon[wepon_number].wepon_str != 0 && player_wepon[wepon_number].wepon_def != 0)
							{
								if (player_wepon[wepon_number].wepon_str > player_wepon[wepon_number].wepon_def)
								{
									player_wepon[wepon_number].wepon_str -= PURASU_UP;
								}
								if (player_wepon[wepon_number].wepon_def > player_wepon[wepon_number].wepon_str)
								{
									player_wepon[wepon_number].wepon_def -= PURASU_UP;
								}
								if (player_wepon[wepon_number].wepon_str == player_wepon[wepon_number].wepon_def)
								{
									player_wepon[wepon_number].wepon_str -= PURASU_UP;
									player_wepon[wepon_number].wepon_def -= PURASU_UP;
								}
							}
							CUserinterface::UI_TextCreateTrapRog(CUserinterface::PLAYERCHARA, CUserinterface::TRAP_EFFECT, m_TrapType, "指輪の+値が1減った");
						}
						else
						{
							m_Wait_frame--;
						}
					}
					break;
				}

			}
		}
	}
	if (m_Wait_frame == ITEM_EFFECT_FRAME)
	{
		if (m_TrapCheck <= TRAP_OK)
		{
			switch (m_TrapType)
			{
			case CTrap::TYPE_DAMAGE:
				Exp_Set(HIT, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
				m_Hp -= m_MaxHp / 8;
				MAX_HP_CHECK;
				break;
			case CTrap::TYPE_KUHUKU:
				m_Onaka -= 15;
				MAX_ONAKA_CHECK;
				break;
			case CTrap::TYPE_WARP_TRAP:
				m_Wait_frame = 0;
				m_Turn = PLAYER_WARP_WAIT;
				break;
			case CTrap::TYPE_SLEEP_TRAP:
				SetTurnCount(0);
				SetCondition(SLEEP_CONDITION);
				break;
			case CTrap::TYPE_POIZUN_TRAP:
				SetTurnCount(0);
				SetCondition(POIZUN_CONDITION);
				break;
			case CTrap::TYPE_KUYARAMI_TRAP:
				SetTurnCount(0);
				SetCondition(KURAYAMI_CONDITION);
				break;
			case CTrap::TYPE_DONSOKU_TRAP:
				SetTurnCount(0);
				SetCondition(DONSOKU_CONDITION);
				break;
			case CTrap::TYPE_ITEMDELETE_TRAP:
				//アイテム削除は0フレーム目で行う
				break;
			}
		}
	}
	if (m_Wait_frame == WAIT_ITEM_FRAME)
	{
		m_TrapType = 0;
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
	m_Oldmz = m_Mapz;
	m_Oldmx = m_Mapx;
	m_Mapz = (int)((m_Colision.position.z - 247.5f) / -5);
	m_Mapx = (int)((m_Colision.position.x + 247.5f) / 5);
	CMap::MapPlayerPosSet(m_Mapz, m_Mapx, m_Oldmz, m_Oldmx);
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
	m_GridFlag = false;
	m_NanameFlag = false;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
	m_GridFlag = false;
	m_NanameFlag = false;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
		m_Angle = UP_ANGLE;

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
		m_Angle = LEFT_ANGLE;

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
		m_Angle = DOWN_ANGLE;

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
		m_Angle = RIGHT_ANGLE;

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
		m_Angle = LEFT_TOP_ANGLE;

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
		m_Angle = LEFT_DOWN_ANGLE;

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
		m_Angle = RIGHT_DOWN_ANGLE;

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
		m_Angle = RIGHT_TOP_ANGLE;

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
		m_Angle = UP_ANGLE;

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
		m_Angle = LEFT_ANGLE;

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
		m_Angle = DOWN_ANGLE;

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
		m_Angle = RIGHT_ANGLE;

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
		m_Angle = LEFT_TOP_ANGLE;

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
		m_Angle = LEFT_DOWN_ANGLE;

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
		m_Angle = RIGHT_DOWN_ANGLE;

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
		m_Angle = RIGHT_TOP_ANGLE;

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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
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
			m_Turn = PLAYER_MOVE_END;
		}
	}
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

void CPlayer::Player_Act(void)
{
	m_GridFlag = false;
	m_NanameFlag = false;
	if (attackframe < 8)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		// 左に攻撃
		if (m_Angle == LEFT_ANGLE)
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
		if (m_Angle == UP_ANGLE)
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
		if (m_Angle == RIGHT_ANGLE)
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
		if (m_Angle == DOWN_ANGLE)
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
		if (m_Angle == RIGHT_TOP_ANGLE)
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
		if (m_Angle == RIGHT_DOWN_ANGLE)
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
		if (m_Angle == LEFT_DOWN_ANGLE)
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
		if (m_Angle == LEFT_TOP_ANGLE)
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
		//両手持ちかどうかで渡す装備攻撃力を変える
		if (m_Wmode)
		{//スキルフラグみること	
			if (m_SkillAtaack)
			{
				if (player_wepon[WEPON_NUMBER].wepon_sikibetu_number == CWepon::TYPE_THREE_ATTACK)
				{
					m_EnemyAttackCount = CAttack::Attack_PlayerThreeRenge(player_wepon[WEPON_NUMBER].wepon_sikibetu_number, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
				}
				if (player_wepon[WEPON_NUMBER].wepon_sikibetu_number == CWepon::TYPE_TURANUKI_ATTACK)
				{
					m_EnemyAttackCount = CAttack::Attack_PlayerTuranukiSkill(player_wepon[WEPON_NUMBER].wepon_sikibetu_number, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
				}
				if(player_wepon[WEPON_NUMBER].wepon_sikibetu_number < CWepon::TYPE_THREE_ATTACK)
				{
					CAttack::Attack_PlayerSkill(player_wepon[WEPON_NUMBER].wepon_sikibetu_number, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
				}
			}
			else
			{
				CAttack::Attack_PlayerUpdate(charatype, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
			}
			Player_TrapCheck();
		}
		else
		{
			if (m_SkillAtaack)
			{
				if (player_wepon[WEPON_NUMBER].wepon_sikibetu_number == CWepon::TYPE_THREE_ATTACK)
				{
					m_EnemyAttackCount = CAttack::Attack_PlayerThreeRenge(player_wepon[WEPON_NUMBER].wepon_sikibetu_number, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
				}
				if (player_wepon[WEPON_NUMBER].wepon_sikibetu_number == CWepon::TYPE_TURANUKI_ATTACK)
				{
					m_EnemyAttackCount = CAttack::Attack_PlayerTuranukiSkill(player_wepon[WEPON_NUMBER].wepon_sikibetu_number, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
				}
				if (player_wepon[WEPON_NUMBER].wepon_sikibetu_number < CWepon::TYPE_THREE_ATTACK)
				{
					CAttack::Attack_PlayerSkill(player_wepon[WEPON_NUMBER].wepon_sikibetu_number, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
				}
			}
			else
			{
				CAttack::Attack_PlayerUpdate(charatype, m_Angle, m_Str + player_wepon[WEPON_NUMBER].wepon_str + player_wepon[RING_NUMBER].wepon_str, m_Str2);
			}
			Player_TrapCheck();
		}
		m_Str2 = 1.0f;
	}
	if (attackframe == attack_endframe)
	{//敵は吹っ飛び中以外かワープ移動以外ならターン処理を行う

		if (m_EnemyWarp)
		{
			m_Turn = PLAYER_TIME_WAIT;
		}
		else if (m_EnemyBack)
		{
			m_Turn = PLAYER_ENEMY_BACK_WAIT;
		}
		else if (!m_EnemyBack)
		{
			//敵を複数攻撃してたらUI消費ターンにする
			if (m_EnemyAttackCount > 0)
			{
				Player_SkillHpDown();
				m_Turn = PLAYER_RANGEHIT_WAIT;
			}
			else
			{
				if (rival_flag)
				{
					//ここでUIモードにしてキー入力待ちを行い、エンターを押したら倒したログを出す
					m_SkillAtaack = false;
					m_Turn = PLAYER_SERECT_UI;
				}
				else
				{
					m_SkillAtaack = false;
					m_Turn = PLAYER_ACT_END;
				}
			}
		}

	}
}

void CPlayer::Player_Destroy(void)
{
	// ゲームオーバーへ画面遷移
	CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::DESTROY);
	if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)))
	{
		trigger = false;
	}
	if (Keyboard_IsTrigger(DIK_RETURN) || Keyboard_IsTrigger(DIK_SPACE) || JoyDevice_IsTrigger(CONTROLLER::A_BUTTON) && !trigger)
	{
		// ネクストシーンへ
		if (!g_bIsFade)
		{
			trigger = true;
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
			// リザルトへ
			SAVE_COUNT *save;
			save = Get_SaveCount();
			save->death_count += 1;
			Save();
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
	m_Judge_player.HitLadder = false;
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
				enemy->Enemy_ConditionCount();
				//毒のエネミーのダメージ処理
				if (enemy->Get_Condition() == POIZUN_CONDITION)
				{
					enemy->Enemy_PoizunDamageStart();
				}			
			}
		}
	}
}

bool CPlayer::Damage(int str, float angle, int week_type)
{
	// 後にダメージエフェクトを作成
	if (str > 0)
	{//week_typeでエフェクトを変える
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
		PlaySound(PLAYERDAMAGE_SE);
	}
	else//攻撃がミスの時
	{
		PlaySound(MISS_SE);
	}

	//攻撃を受けた方に向く
	
	if (angle == 4.8f)
	{
		m_Angle = RIGHT_ANGLE;
	}
	if (angle == 0.0f)
	{
		m_Angle = DOWN_ANGLE;
	}
	if (angle == 1.6f)
	{
		m_Angle = LEFT_ANGLE;
	}
	if (angle == 3.2f)
	{
		m_Angle = UP_ANGLE;
	}
	if (angle == 0.8f)
	{
		m_Angle = LEFT_DOWN_ANGLE;
	}
	if (angle == 2.4f)
	{
		m_Angle = LEFT_TOP_ANGLE;
	}
	if (angle == 4.0f)
	{
		m_Angle = 4.0f;
	}
	if (angle == 5.6f)
	{
		m_Angle = RIGHT_DOWN_ANGLE;
	}
	
	m_Hp -= str;
	if (m_Hp <= 0)
	{
		m_Hp = 0;
	}
	SAVE_COUNT *save;
	save = Get_SaveCount();
	save->damage_on_count += str;
	// 体力が0以下で倒れる
	if (m_Hp <= 0)
	{
		return DEATH;
	}
	return NORMAL;
}

void CPlayer::Player_OnakaDown(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(1, 3);
	int number = 0;
	m_Onaka -= 1;
	if (m_Onaka <= 0)
	{
		m_Onaka = 0;
	}
	if (m_Onaka == 20)
	{
		CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::FREE_MESSAGE, "おなかが、減ってきた・・・");
		m_Turn = PLAYER_TIME_WAIT;
	}
	if (m_Onaka == 10)
	{
		CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::FREE_MESSAGE, "ハラペコで目が回ってきた・・・");
		m_Turn = PLAYER_TIME_WAIT;
	}
	if (m_Onaka == 0)
	{
		number = random(mt);
		switch (number)
		{
		case 1:
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::FREE_MESSAGE, "だめだ！もう倒れそうだ！");
			break;
		case 2:
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::FREE_MESSAGE, "早く・・・なにか食べないと・・・");
			break;
		case 3:
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::FREE_MESSAGE, "飢え死にしてしまう！");
			break;
		}
		m_Turn = PLAYER_TIME_WAIT;
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

int CPlayer::WeponPurasuSet(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(1, 100);
	int number = random(mt);
	if (number <= 25)
	{
		return 0;
	}
	else if (number >= 26 && number <= 50)
	{
		return 1;
	}
	else if (number >= 51 && number <= 70)
	{
		return 2;
	}
	else if (number >= 71 && number <= 85)
	{
		return 3;
	}
	else if (number >= 86 && number <= 95)
	{
		return 4;
	}
	else if (number >= 96 && number <= 100)
	{
		return 5;
	}
	return 0;
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

void CPlayer::Player_BackCheck(void)
{
	int i;
	C3DObj *object;
	C3DObj *wepon;
	C3DObj *trap;
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
					m_Judge_player.HitItem = true;
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
					m_Judge_player.HitItem = true;
				}
			}
		}
	}
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		trap = CTrap::Get_Trap(i);
		//ワナの判定
		if (trap)
		{
			if (trap->alive)
			{//効果が発動してない罠のみ効果発動
			 //if (trap->GetWanaEffectOn() == false)
			 //{
				if (m_Mapz == trap->m_Mapz && m_Mapx == trap->m_Mapx)
				{
					m_Judge_player.HitItem = true;
				}
				//}
			}
		}
	}
}

void CPlayer::Player_Check(void)
{
	int i, j;
	C3DObj *object;
	C3DObj *wepon;
	C3DObj *trap;
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
							m_Turn = PLAYER_TIME_WAIT;
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
									m_Turn = PLAYER_ITEM_WAIT;
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
					m_Turn = PLAYER_ITEM_WAIT;
				}
			}
		}
	}
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		trap = CTrap::Get_Trap(i);
		//ワナの判定
		if (trap)
		{
			if (trap->alive)
			{//効果が発動してない罠のみ効果発動
				//if (trap->GetWanaEffectOn() == false)
				//{
					if (m_Mapz == trap->m_Mapz && m_Mapx == trap->m_Mapx)
					{
						trap->SetWanaEffectOn(true);
						trap->SetWanaCheck(true);
						m_TrapType = trap->Get_Type();
						m_Turn = PLAYER_TRAP_EFFECT;
					}
				//}
			}
		}
	}
}

void CPlayer::Player_TrapCheck(void)
{
	int i;
	C3DObj *trap;
	int move_z = 0;
	int move_x = 0;
	if (m_Angle == UP_ANGLE)
	{
		move_z = -1;
	}
	if (m_Angle == LEFT_ANGLE)
	{
		move_x = -1;
	}
	if (m_Angle == DOWN_ANGLE)
	{
		move_z = 1;
	}
	if (m_Angle == RIGHT_ANGLE)
	{
		move_x = 1;
	}
	if (m_Angle == LEFT_TOP_ANGLE)
	{
		move_x = -1;
		move_z = -1;
	}
	if (m_Angle == LEFT_DOWN_ANGLE)
	{
		move_x = -1;
		move_z = 1;
	}
	if (m_Angle == RIGHT_TOP_ANGLE)
	{
		move_x = 1;
		move_z = -1;
	}
	if (m_Angle == RIGHT_DOWN_ANGLE)
	{
		move_x = 1;
		move_z = 1;
	}
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		trap = CTrap::Get_Trap(i);
		//ワナの判定
		if (trap)
		{
			if (trap->alive)
			{//当たっていたら罠を可視化
				if (trap->GetWanaEffectOn() == false)
				{
					if (m_Mapz + move_z == trap->m_Mapz && m_Mapx + move_x == trap->m_Mapx)
					{
						trap->SetWanaCheck(true);
					}
				}
			}
		}
	}
}

void CPlayer::Player_WeponChenge(int wepon_type, int wepondata_number)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);
	int j;
	int WEPONDATA_MAX = CWepon::Get_WEPONDATAMAX();
	int floor_wepon_count[100] = { 0 };//ドロップ確率を代入
	int kakuritu_start = 0;
	int k;
	m_Judge_player.HitItem = true;
	/*
	for (j = 0; j < WEPONDATA_MAX; j++)
	{//武器データから対応する武器タイプと出現フロアで出る武器を抽出
		if (CWepon::Get_Wepon_Data(j)->type == wepon_type &&
			CWepon::Get_Wepon_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CWepon::Get_Wepon_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			floor_wepon[floor_wepon_count] = j;
			floor_wepon_count++;
		}
	}*/
	for (j = 0; j < WEPONDATA_MAX; j++)
	{//武器データから対応する武器タイプと出現フロアで出る武器を抽出
		if (CWepon::Get_Wepon_Data(j)->type == wepon_type &&
			CWepon::Get_Wepon_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CWepon::Get_Wepon_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			for (k = kakuritu_start; k < kakuritu_start + CWepon::Get_Wepon_Data(j)->weponchance; k++)
			{
				if (floor_wepon_count[k] == 0)
				{//武器ごとの出現確率を代入
					floor_wepon_count[k] = j;
				}
			}
			kakuritu_start += CWepon::Get_Wepon_Data(j)->weponchance;
		}
	}
	//もしも100％埋まってなかったら空きに最初に代入した武器番号を入れる
	for (k = 0; k < 100; k++)
	{
		if (floor_wepon_count[k] == 0)
		{
			floor_wepon_count[k] = floor_wepon_count[0];
		}
	}
	if (floor_wepon_count[0] != -1)//武器が存在したら
	{
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->wepon_chenge_count += 1;
		//武器番号格納
		m_WeponStock[wepondata_number] = floor_wepon_count[random(mt)];
		//+の値決定
		player_wepon[wepondata_number].wepon_purasu_number = WeponPurasuSet();
		//プレイヤー武器データに格納str def type 
		if (CWepon::TYPE_SWORD == wepon_type || CWepon::TYPE_REIPIA == wepon_type || CWepon::TYPE_MEISU == wepon_type)
		{
			player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str + (player_wepon[wepondata_number].wepon_purasu_number * PURASU_UP);
			player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def;
			player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
			player_wepon[wepondata_number].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->kantei_type;
		}
		else if(CWepon::TYPE_SHELD == wepon_type)
		{
			player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str;
			player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def + (player_wepon[wepondata_number].wepon_purasu_number * PURASU_UP);
			player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
			player_wepon[wepondata_number].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->kantei_type;
		}
		else if (CWepon::TYPE_RING == wepon_type)
		{//指輪は上昇能力が高いほうに補正値を足す（同時の場合は両方を補正値の半分足す）0の時は足さない
			if (CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str == 0 && CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def == 0)
			{
				player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str;
				player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def;
				player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
				player_wepon[wepondata_number].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->kantei_type;
			}
			else
			{
				if (CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str > CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def)
				{
					player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str + (player_wepon[wepondata_number].wepon_purasu_number * PURASU_UP);
					player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def;
					player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
					player_wepon[wepondata_number].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->kantei_type;
				}
				if (CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def > CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str)
				{
					player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str;
					player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def + (player_wepon[wepondata_number].wepon_purasu_number * PURASU_UP);
					player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
					player_wepon[wepondata_number].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->kantei_type;
				}
				if (CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str == CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def)
				{
					player_wepon[wepondata_number].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->str + (player_wepon[wepondata_number].wepon_purasu_number * (PURASU_UP / 2));
					player_wepon[wepondata_number].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->def + (player_wepon[wepondata_number].wepon_purasu_number * (PURASU_UP / 2));
					player_wepon[wepondata_number].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->type;
					player_wepon[wepondata_number].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[wepondata_number])->kantei_type;
				}
			}
			//指輪が腹持ちの指輪なら腹持ちターン変更
			if (player_wepon[wepondata_number].wepon_sikibetu_number == CWepon::TYPE_HARAMOTI_RING)
			{
				m_OnakaTurn = (DEFFAULT_ONAKA_TURN * 2) + (2 * player_wepon[wepondata_number].wepon_purasu_number);
			}
			else if (player_wepon[wepondata_number].wepon_sikibetu_number != CWepon::TYPE_HARAMOTI_RING)
			{//腹持ちの指輪以外ならお腹が減るターン数をデフォルトにする
				m_OnakaTurn = DEFFAULT_ONAKA_TURN;
			}
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
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 99);
	int j;
	int WEPONDATA_MAX = CWepon::Get_WEPONDATAMAX();
	int floor_wepon_count[100] = { 0 };//ドロップ確率を代入
	int kakuritu_start = 0;
	int k;
	m_Judge_player.HitItem = true;
	for (j = 0; j < WEPONDATA_MAX; j++)
	{//武器データから対応する武器タイプと出現フロアで出る武器を抽出
		if (CWepon::Get_Wepon_Data(j)->type == wepon_type &&
			CWepon::Get_Wepon_Data(j)->first_floor <= CStage::Stage_GetLevel() &&
			CWepon::Get_Wepon_Data(j)->end_floor >= CStage::Stage_GetLevel())
		{
			for (k = kakuritu_start; k < kakuritu_start + CWepon::Get_Wepon_Data(j)->weponchance; k++)
			{
				if (floor_wepon_count[k] == 0)
				{
					floor_wepon_count[k] = j;
				}
			}
			kakuritu_start += CWepon::Get_Wepon_Data(j)->weponchance;
		}
	}
	//もしも100％埋まってなかったら空きに最初に代入した武器番号を入れる
	for (k = 0; k < 100; k++)
	{
		if (floor_wepon_count[k] == 0)
		{
			floor_wepon_count[k] = floor_wepon_count[0];
		}
	}
	if (floor_wepon_count[0] != -1)//武器が存在したら
	{
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->wepon_chenge_count += 1;
		//武器番号格納
		m_WeponStock[WEPON_NUMBER] = floor_wepon_count[random(mt)];
		//+の値決定
		player_wepon[WEPON_NUMBER].wepon_purasu_number = WeponPurasuSet();
		//プレイヤー武器データに格納str def type 
		player_wepon[WEPON_NUMBER].wepon_str = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->str + (player_wepon[WEPON_NUMBER].wepon_purasu_number * (PURASU_UP * 2));
		player_wepon[WEPON_NUMBER].wepon_def = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->def;
		player_wepon[WEPON_NUMBER].wepon_type = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->type;
		player_wepon[WEPON_NUMBER].wepon_sikibetu_number = CWepon::Get_Wepon_Data(m_WeponStock[WEPON_NUMBER])->kantei_type;
		
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

void CPlayer::Player_ItemSort2(void)
{
	int i;
	for (i = 0; i < MAX_ITEM - 1; i++)
	{
		if (m_ItemStock[i] == 0)
		{
			m_ItemStock[i] = m_ItemStock[i + 1];
			m_ItemStock[i + 1] = 0;
		}
	}
}

bool CPlayer::SkillHpCheck(void)
{
	switch (player_wepon[WEPON_NUMBER].wepon_sikibetu_number)
	{
	case CWepon::TYPE_HIGHATTACK_SKILL:
	case CWepon::TYPE_BACKATTACK_SKILL:
	case CWepon::TYPE_POIZUN_ATTACK:
	case CWepon::TYPE_KURAYAMI_ATTACK:
	case CWepon::TYPE_MAHUJI_ATTACK:
	case CWepon::TYPE_YOROIKUDAKI_SKILL:
		if (m_Hp >= SMOLL_SKILL_HP)
		{
			return true;
		}
		return false;
		break;
	case CWepon::TYPE_WARP_SKILL:
	case CWepon::TYPE_THREE_ATTACK:
	case CWepon::TYPE_TURANUKI_ATTACK:
		if (m_Hp >= MIDDLE_SKILL_HP)
		{
			return true;
		}
		return false;
		break;
	case CWepon::TYPE_SLEEP_ATTACK:
		if (m_Hp >= BIG_SKILL_HP)
		{
			return true;
		}
		return false;
		break;
	}
	return false;
}

void CPlayer::Player_SkillHpDown(void)
{
	switch (player_wepon[WEPON_NUMBER].wepon_sikibetu_number)
	{
	case CWepon::TYPE_HIGHATTACK_SKILL:
	case CWepon::TYPE_POIZUN_ATTACK:
	case CWepon::TYPE_KURAYAMI_ATTACK:
	case CWepon::TYPE_MAHUJI_ATTACK:
	case CWepon::TYPE_YOROIKUDAKI_SKILL:
		m_Hp -= SMOLL_SKILL_HP;
		break;
	case CWepon::TYPE_BACKATTACK_SKILL:
		m_EnemyBack = true;
		m_Hp -= SMOLL_SKILL_HP;
		break;
	case CWepon::TYPE_THREE_ATTACK:
	case CWepon::TYPE_TURANUKI_ATTACK:
		m_Hp -= MIDDLE_SKILL_HP;
		break;
	case CWepon::TYPE_WARP_SKILL:
		m_EnemyWarp = true;
		m_Hp -= MIDDLE_SKILL_HP;
		break;
	case CWepon::TYPE_SLEEP_ATTACK:
		m_Hp -= BIG_SKILL_HP;
		break;
	}
}

int CPlayer::Player_SkillHpGet(void)
{
	switch (player_wepon[WEPON_NUMBER].wepon_sikibetu_number)
	{
	case CWepon::TYPE_HIGHATTACK_SKILL:
	case CWepon::TYPE_BACKATTACK_SKILL:
	case CWepon::TYPE_POIZUN_ATTACK:
	case CWepon::TYPE_KURAYAMI_ATTACK:
	case CWepon::TYPE_YOROIKUDAKI_SKILL:
		return (int)SMOLL_SKILL_HP;
	case CWepon::TYPE_WARP_SKILL:
	case CWepon::TYPE_THREE_ATTACK:
	case CWepon::TYPE_TURANUKI_ATTACK:
		return (int)MIDDLE_SKILL_HP;
	case CWepon::TYPE_SLEEP_ATTACK:
		return (int)BIG_SKILL_HP;
	}
	return 0;
}

int CPlayer::ItemSnatch(void)
{
	int delete_item = 0;
	int item_type = 0;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, MAX_ITEM);
	for (;;)
	{//アイテムが入ってる場所を検索
		delete_item = random(mt);
		//アイテムが無くてもbreak;
		if (m_ItemStock[delete_item] != 0 || m_ItemStock[0] == 0)
		{
			break;
		}
	}
	
	//アイテム番号を受け取りその値を返す
	item_type = m_ItemStock[delete_item];
	m_ItemStock[delete_item] = 0;
	Player_ItemSort2();

	return item_type;
}