//////////////////////////////////////////////////
////
////	�J�����N���X
////
//////////////////////////////////////////////////

//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================
#include <crtdbg.h>
#include "CCamera.h"
#include "Cplayer.h"
#include "common.h"
#include "input.h"
#include "C3DObj.h"
#include "gamepad.h"
#include "debug_font.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	�萔��`
//=============================================================================


#define ASPECT ((float)WINDOW_WIDTH / WINDOW_HIGHT)
#define ATLENGTH (14.0f)
#define COSLENGTH (0.5f)
#define CAMERA_SPEED (0.00f)


//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

D3DXVECTOR3 CCamera::m_Right;
D3DXMATRIX CCamera::m_mtxView;
CCamera *CCamera::m_pCamera;
D3DXVECTOR3 CCamera::m_CameraPos;
static DIJOYSTATE2 js;
static LPDIRECTINPUTDEVICE8 pJoyDevice;
static HRESULT hr;

bool r = false;
bool l = false;
//=============================================================================
//	����
//=============================================================================
CCamera::CCamera() :CGameObj(CGameObj::TYPE_CAMERA)
{
	Camera_Initialize();
}



//=============================================================================
//	�j��
//=============================================================================
CCamera::~CCamera()
{
	//Camera_Finalize();
}



//	������
void CCamera::Camera_Initialize(void)
{
	//g_Cposition = D3DXVECTOR3(5.0f, 5.0f, -10.0f);
	m_CameraPos = D3DXVECTOR3(0.0f, 0.0f, -6.0f);

	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXVec3Normalize(&m_Front, &m_Front);

	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// �O��
	//D3DXVec3Cross(&g_right, &g_front, &g_up);
	// �t����
	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);
	D3DXVec3Normalize(&m_Right, &m_Right);

	D3DXVec3Cross(&m_Up, &m_Front, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);
	m_AtLength = 0.1f;
	m_At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	view = false;
}

//	�I������
void CCamera::Finalize(void)
{
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
	}
}



//	�J�����N���X�쐬
CCamera* CCamera::Camera_Create(void)
{
	m_pCamera = new CCamera;
	return m_pCamera;
}

//	�X�V
void CCamera::Update(void)
{
	// �J�����̐U������@��]�s��
	D3DXMATRIX mtxR;        // 1�t���[���ɉ�]����p�x
	C3DObj *getplayer = CPlayer::Get_Player();
							//g_Came_at = (g_front * g_length) + g_Cposition;
							// �U�����ȊO�̓J�������v���C���[�ɒǏ]
	if (getplayer->Get_PlayerTurn() != CPlayer::PLAYER_ACT)
	{
		m_At = (m_Front * m_AtLength) + getplayer->Get_Position();
		m_CameraPos = getplayer->Get_Position();

		if (!view)
		{

			if (Keyboard_IsTrigger(DIK_V))
			{
				// �r���[���[�h�I��
				view = true;
			}

			m_CameraPos.y += 15.0f;
			m_CameraPos.z -= 8.0f;
		}
		else
		{
			if (Keyboard_IsTrigger(DIK_V))
			{
				// �r���[���[�h�I�t
				view = false;
			}
			m_CameraPos.y += 200.0f;
			m_CameraPos.z -= 50.0f;
		}
	}

	// �r���[���W�ϊ��s��̍쐬�Ɛݒ�
	// �J�������W
	D3DXVECTOR3 eye(5.0f, 5.0f, -10.0f);
	// ���Ă�ꏊ(�����_)
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
	// �J�����̏�����x�N�g��
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);


	//g_front = D3DXVECTOR3(0, -0.1f, 1.0f);
	//D3DXVec3TransformNormal(&g_front, &g_front);

	D3DXMatrixLookAtLH(&m_mtxView, &m_CameraPos, &m_At, &up);

	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�������W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxProjection;				// ����p120���ɂ�����								near�ɂ�0�ȉ��֎~ �ǂݍ��ދ���
	if (view)
	{
		D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 230.0f);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 25.0f);
	}
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

}

//	�`��
void CCamera::Draw(void)
{

}

//	�f�o�b�O�`��
void  CCamera::DebugDraw(void)
{
	DebugFont_Draw(400, 10, "%f\n,%f\n,%f\n,", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
}

void CCamera::Dalete_Camera(void)
{
	CGameObj *pcamera = CGameObj::Get(CGameObj::TYPE_CAMERA);
	if (pcamera)
	{
		delete pcamera;
	}
}









