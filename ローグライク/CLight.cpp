//////////////////////////////////////////////////
////
////	���C�g�N���X
////
//////////////////////////////////////////////////

//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================

#include "CLight.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//=============================================================================
//	�萔��`
//=============================================================================


//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

CLight *CLight::m_pLight;


//=============================================================================
//	����
//=============================================================================
CLight::CLight():CGameObj(CGameObj::TYPE_LIGHT)
{
	Light_Initialize();
}

//=============================================================================
//	�j��
//=============================================================================
CLight::~CLight()
{
	//Light_Finalize();
}



void CLight::Update(void)
{
	
}

void CLight::Draw(void)
{
	Light_Set();
}

//	���C�g������
void CLight::Light_Initialize(void)
{
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// �@���x�N�g����1�ɂ���
}

CLight* CLight::Light_Create(void)
{
	m_pLight = new CLight;
	return m_pLight;
}

void CLight::Finalize(void)
{
	if (m_pLight != NULL)
	{
		delete m_pLight;
	}
}

//	���C�g�Z�b�g
void CLight::Light_Set(void)
{

	// ���s����(�f�B���N�V���i���f�B�t�F�[�Y)
	D3DLIGHT9 dirLight;

	ZeroMemory(&dirLight, sizeof(dirLight));

	dirLight.Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̌���
	D3DXVECTOR3 vecDir(0.0f, 0.0f, 1.0f);
	// ���K��
	D3DXVec3Normalize((D3DXVECTOR3*)&dirLight.Direction, &vecDir);//(1�ɂ������̂��i�[���邽�߂̃x�N�g��, ����1�ɂ������x�N�g��)

																  // �J���[
	dirLight.Diffuse.r = 1.0f;
	dirLight.Diffuse.g = 1.0f;
	dirLight.Diffuse.b = 1.0f;
	dirLight.Diffuse.a = 1.0f;

	// �A���r�G���g���C�g(����)					// �Â߂�����
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(8, 0, 24, 255));
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(128, 128, 128, 255));
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);	// D3DMCS_COLOR1 �L���[�u�p
																			// D3DMCS_MATERIAL �}�e���A���p
																			// ���C�g�����Ă���` 4�܂ł���
	m_pD3DDevice->SetLight(0, &dirLight);

	// 0�ԃ��C�g��L���ɂ���
	m_pD3DDevice->LightEnable(0, true);
}









