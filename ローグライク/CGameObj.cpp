//////////////////////////////////////////////////
////
////	�Q�[���I�u�W�F�N�g�N���X
////
//////////////////////////////////////////////////

//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================

#include "CGameObj.h"
#include "common.h"
#include "debug_font.h"

//=============================================================================
//	�萔��`
//=============================================================================


//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

int CGameObj::m_FrameCount = 0;
int CGameObj::m_GameObjNum = 0;
LPDIRECT3D9 CGameObj::m_pD3D = NULL;
LPDIRECT3DDEVICE9 CGameObj::m_pD3DDevice = NULL;

CGameObj *CGameObj::pGameObj[MAX_GAMEOBJ];
//=============================================================================
//	����
//=============================================================================

CGameObj::CGameObj()
{

}
CGameObj::CGameObj(int type)
{
	// ���[�N�Ɏ������g(this)���i�[
	for (m_GameObjIndex = 0; m_GameObjIndex < MAX_GAMEOBJ; m_GameObjIndex++)
	{
		if (pGameObj[m_GameObjIndex] == NULL)
		{
			pGameObj[m_GameObjIndex] = this;
			m_GameObjType = type;
			m_GameObjNum++;
			break;
		}
	}
	// ���[�N�i�[���s
	if (m_GameObjIndex >= MAX_GAMEOBJ)
	{
		m_GameObjIndex = -1;
	}
}
void CGameObj::FrameCountReset(void)
{
	m_FrameCount = 0;
}



//=============================================================================
//	�j��
//=============================================================================

CGameObj::~CGameObj()
{

	m_TimeKeep = 0;
	if (m_GameObjIndex >= 0)
	{
		m_GameObjNum--;
		pGameObj[m_GameObjIndex] = NULL;
	}

}


//=============================================================================
//	�f�o�C�X����
//=============================================================================

bool CGameObj::Device_Initialize(HWND hWnd)
{
	// Direct3D�C���^�[�t�F�[�X�̎擾
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL) {
		// Direct3D�C���^�[�t�F�[�X�̎擾�Ɏ��s
		MessageBox(hWnd, "Direct3D�C���^�[�t�F�[�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	// �f�o�C�X�̐����ɗ��p������\���̂̍쐬
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = (UINT)(WINDOW_WIDTH);                       // �o�b�N�o�b�t�@�̉���				
	d3dpp.BackBufferHeight = (UINT)(WINDOW_HEIGHT);					    // �o�b�N�o�b�t�@�̏c��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // �o�b�N�o�b�t�@�̃t�H�[�}�b�g�w��
	d3dpp.BackBufferCount = 1;                                  // �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // �X���b�v�G�t�F�N�g�i�X���b�v���@�j�̐ݒ�		
	d3dpp.Windowed = TRUE;                                      // �E�B���h�E���[�h or �t���X�N���[��
	d3dpp.EnableAutoDepthStencil = TRUE;                        // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̎g�p				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // �[�x�o�b�t�@�E�X�e���V���o�b�t�@�̃t�H�[�}�b�g�w��
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // �t���X�N���[�����̃��t���b�V�����[�g�̎w��
																// d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       // ���t���b�V�����[�g��Present�����̊֌W
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;       // ���t���b�V�����[�g��Present�����̊֌W

																	  // Direct3D�f�o�C�X�̎擾
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) {
		// �f�o�C�X�̍쐬�Ɏ��s
		MessageBox(hWnd, "Direct3D�f�o�C�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
		return false;
	}

	// �T���v���[�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// �A���t�@�u�����h�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���_�J���[�ƃe�N�X�`���̃u�����h�ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// Z�o�b�t�@�L��
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// ���C�g��L�� 
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	return true;
}

//=============================================================================
//	�f�o�C�X�j��
//=============================================================================

void CGameObj::Device_Finalize(void)
{
	// Direct3D�f�o�C�X�̊J��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3D�C���^�[�t�F�[�X�̊J��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

void CGameObj::DebugDraw(void)
{
	DebugFont_Draw(10, 500, "FrameCount\n%d\nTIME\n%d.%02d", m_FrameCount, m_FrameCount / 60, m_FrameCount % 100);

}


//=============================================================================
// �S�I�u�W�F�N�g�X�V
//=============================================================================
void CGameObj::UpdateAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// �|�����[�t�B�Y���ɂ���Ĕh���N���X��Update()���Ă΂��
		if (pGameObj[i])
		{
			pGameObj[i]->Update();
		}
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�`��
//=============================================================================
void CGameObj::DrawAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// �|�����[�t�B�Y���ɂ���Ĕh���N���X��Draw()���Ă΂��
		if (pGameObj[i])
		{
			pGameObj[i]->Draw();
		}
	}
}



//=============================================================================
// �S�I�u�W�F�N�g�폜
//=============================================================================
void CGameObj::DeleteAll2D()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (pGameObj[i])
		{
			delete pGameObj[i];
			//pGameObj[i] = NULL;
		}
	}
}


CGameObj *CGameObj::Get(int type)
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (pGameObj[i])
		{
			if (pGameObj[i]->m_GameObjType == type)
			{
				return pGameObj[i];
			}
		}
	}
	return 0;
}

