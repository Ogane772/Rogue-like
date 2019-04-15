
//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================
#define NOMINMAX

#include "CLight.h"
#include "CCamera.h"
#include "game.h"
#include "meshField.h"
#include "Cplayer.h"
#include "bilboard.h"
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
#include "effect.h"
#include "wall.h"
//=============================================================================
//	�萔��`
//=============================================================================
#define FADE_WAIT (255.0f)
#define FADE_SPEED (8)//4�̔{��


//=============================================================================
//	�O���[�o���錾
//=============================================================================
static bool g_bend;				//	�t�F�[�h�C���A�E�g�t���O
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
//	����������
//=============================================================================
void Game_Initialize(void)
{
	DebugFont_Initialize();
	CPlayer::PlayerCreate();				//	�v���C���[����		
	CLight::Light_Create();					//	���C�g����
	srand(time(NULL));
	int g_Dblock = -1;
	int g_TexReef = -1;
	g_TexReef = CTexture::Texture_SetLoadFile("./asset/texture/kusa.png", 100, 100);
	//g_Dblock = Texture_SetLoadFile("./asset/texture/block.png", 100, 100);
	CBilboard::BilBoard_Initialize();
	CBilboard::Shadow_Init();
	CMeshField::MeshField_Init();
	CWall::MeshWall_Init();
	CEffect::Effect_Initialize();
	CUserinterface::UI_Initialize();
	//Number_Initialize(3);
	CMap::Map_Initialize();
	CMeshField::MeshField_Create(g_TexReef, 500, 500, 100, 100, D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// 1��  = 5.0f * 5.0f

																					// �}�b�v����
	CMap::Map_Create();
	//XModel_Initialize("asset/model/sraim4x.x" , 0, D3DXVECTOR3(0.0f, 0.5f, 0.0f), 1);	// �^�C�v1 = �X���C����]
	//XModel_Initialize("asset/model/sraimv2.x", 1, D3DXVECTOR3(0.0f, 5.5f, 0.0f), 1);

	//XModel_Initialize("asset/model/sraimg3.x", 2, D3DXVECTOR3(0.0f, 5.5f, 0.0f), 1);

	if (CTexture::Texture_Load() > 0)
	{
		return;
	}

	Fade_Start(false, 30, 0, 0, 0);

	g_bend = false;
	g_gameend = false;

	CCamera::Camera_Create();
}

//=============================================================================
//	�I������
//=============================================================================

void Game_Finalize(void)
{
	CMeshField::MeshField_Finalize();
	CEffect::Effect_Finalize();
	CBilboard::Shadow_Finalize();
	C3DObj::DeleteAll();			//	3D�I�u�W�F�N�g�S����
	CGameObj::DeleteAll2D();			//	2D�I�u�W�F�N�g�S����

	_CrtDumpMemoryLeaks();
}

//=============================================================================
//	�X�V����
//=============================================================================

void Game_Update(void)
{
	C3DObj::UpdateAll();	//	3D�I�u�W�F�N�g�X�V
	CGameObj::UpdateAll();	//	2D�I�u�W�F�N�g�X�V
	CBilboard::BilBoard_Update();
	CEffect::Effect_Update();
	CUserinterface::UI_Update();
	CEnemy::EnemyTurnEnd();
}

//=============================================================================
//	�`�揈��
//=============================================================================

void Game_Draw(void)
{
	C3DObj::DrawAll();		//	3D�I�u�W�F�N�g�`��
	CGameObj::DrawAll();	//	2D�I�u�W�F�N�g�`��
	CMeshField::MeshField_Draw();
	CWall::MeshWall_Draw();
	CEffect::Effect_Draw();
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
