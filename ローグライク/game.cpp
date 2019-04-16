
//=============================================================================
//	インクルードファイル
//=============================================================================
#define NOMINMAX

#include "CLight.h"
#include "CCamera.h"
#include "game.h"
#include "meshField.h"
#include "Cplayer.h"
#include "bilboard.h"
#include "exp.h"
#include "CEnemy.h"
#include "fade.h"
#include "scene.h"
#include "userInterface.h"
#include "common.h"
#include "input.h"
#include "exp.h"
#include "sound.h"
#include "debug_font.h"
#include <crtdbg.h>
#include "fade.h"
#include <time.h>
#include "map.h"
#include "wall.h"
//=============================================================================
//	定数定義
//=============================================================================
#define FADE_WAIT (255.0f)
#define FADE_SPEED (8)//4の倍数


//=============================================================================
//	グローバル宣言
//=============================================================================
static bool g_bend;				//	フェードインアウトフラグ
static bool g_gameend;
static C2DObj *m_fade;

static int FaDe_counterX;

static float fade_in_u;
static float fade_in_v;
static float fade_u;
static float fade_v;
static float fade_wait;
static bool Fadraw;
static bool fade_ioFlg;
static bool Fwait_flg;
//=============================================================================
//	初期化処理
//=============================================================================
void Game_Initialize(void)
{
	CPlayer::PlayerCreate();				//	プレイヤー生成		
	CLight::Light_Create();					//	ライト生成
	srand(time(NULL));
	int g_Dblock = -1;
	int g_TexReef = -1;
	//_TexReef = CTexture::Texture_SetLoadFile("./asset/texture/kusa.png", 100, 100);
	//g_Dblock = Texture_SetLoadFile("./asset/texture/block.png", 100, 100);
	CBilboard::BilBoard_Initialize();
	CBilboard::Shadow_Init();
	CMeshField::MeshField_Init();
	CWall::MeshWall_Init();
	Exp_Init();
	CUserinterface::UI_Initialize();
	//Number_Initialize(3);
	CMap::Map_Initialize();
	CMeshField::MeshField_Create(CTexture::TEX_KUSA, 500, 500, 100, 100, D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 1面  = 5.0f * 5.0f

																					// マップ生成
	CMap::Map_Create();
	//XModel_Initialize("asset/model/sraim4x.x" , 0, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 1);	// タイプ1 = スライム回転
	//XModel_Initialize("asset/model/sraimv2.x", 1, D3DXVECTOR3(0.0f, 5.5f, 0.0f), 1);

	//XModel_Initialize("asset/model/sraimg3.x", 2, D3DXVECTOR3(0.0f, 5.5f, 0.0f), 1);


	Fade_Start(false, 30, 0, 0, 0);

	g_bend = false;
	g_gameend = false;

	CCamera::Camera_Create();
}

//=============================================================================
//	終了処理
//=============================================================================

void Game_Finalize(void)
{
	CMeshField::MeshField_Finalize();
	CBilboard::Shadow_Finalize();
	C3DObj::DeleteAll();			//	3Dオブジェクト全消去
	CGameObj::DeleteAll2D();			//	2Dオブジェクト全消去

	_CrtDumpMemoryLeaks();
}

//=============================================================================
//	更新処理
//=============================================================================

void Game_Update(void)
{
	C3DObj::UpdateAll();	//	3Dオブジェクト更新
	CGameObj::UpdateAll();	//	2Dオブジェクト更新
	CBilboard::BilBoard_Update();
	CUserinterface::UI_Update();
	CEnemy::EnemyTurnEnd();
	Exp_Update();
}

//=============================================================================
//	描画処理
//=============================================================================

void Game_Draw(void)
{
	C3DObj::DrawAll();		//	3Dオブジェクト描画
	CGameObj::DrawAll();	//	2Dオブジェクト描画
	CMeshField::MeshField_Draw();
	CWall::MeshWall_Draw();
	Exp_Draw();
	CUserinterface::UI_Draw();
}


void Game_End(void)
{
	g_gameend = true;
}

bool Get_Gameend(void)
{
	return g_gameend;
}
