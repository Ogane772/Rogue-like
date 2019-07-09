#include "CWepon.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CWepon_Ring.h"
//===================================
// �O���[�o���ϐ�
//===================================
//=============================================================================
//	����
//=============================================================================

CWeponRing::CWeponRing(int x, int z) :CWepon(TYPE_RING), C3DObj(C3DObj::TYPE_WEPON)
{
	Initialize(x, z);
}

//=============================================================================
//	�j��
//=============================================================================

CWeponRing::~CWeponRing()
{

}

void CWeponRing::Initialize(int x, int z)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = TYPE_RING;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CWeponRing::Finalize(void)
{

}

void CWeponRing::Update(void)
{

}

void CWeponRing::Draw(void)
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
		CBilboard::BilBoard3_Draw(CTexture::TEX_RING_ICON, m_Position);
	}
}
