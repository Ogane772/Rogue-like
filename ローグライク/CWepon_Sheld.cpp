#include "CWepon.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CWepon_Sheld.h"
//===================================
// �O���[�o���ϐ�
//===================================
//=============================================================================
//	����
//=============================================================================

CWeponSheld::CWeponSheld(int x, int z) :CWepon(TYPE_SHELD), C3DObj(C3DObj::TYPE_WEPON)
{
	Initialize(x, z);
}

//=============================================================================
//	�j��
//=============================================================================

CWeponSheld::~CWeponSheld()
{

}

void CWeponSheld::Initialize(int x, int z)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = TYPE_SHELD;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CWeponSheld::Finalize(void)
{

}

void CWeponSheld::Update(void)
{

}

void CWeponSheld::Draw(void)
{
	// ���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	if (alive)
	{
		D3DXMatrixIdentity(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslotation, m_Position.x, m_Position.y, m_Position.z);
		CBilboard::BilBoard_ChangeSizeColor(5.0f, 4.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
		CBilboard::BilBoard3_Draw(CTexture::TEX_SHELD_ICON, m_Position);
	}
}

bool CWeponSheld::Damage(int str, float angle)
{
	return 0;
}
