#include "CWepon.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CWepon_Ax.h"
//===================================
// グローバル変数
//===================================
//=============================================================================
//	生成
//=============================================================================

CWeponAx::CWeponAx(int x, int z) :CWepon(TYPE_AX), C3DObj(C3DObj::TYPE_WEPON)
{
	Initialize(x, z);
}

//=============================================================================
//	破棄
//=============================================================================

CWeponAx::~CWeponAx()
{

}

void CWeponAx::Initialize(int x, int z)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = TYPE_AX;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CWeponAx::Finalize(void)
{

}

void CWeponAx::Update(void)
{

}

void CWeponAx::Draw(void)
{
	// ワールド座標変換行列の作成と設定
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	if (alive)
	{
		D3DXMatrixIdentity(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslotation, m_Position.x, m_Position.y, m_Position.z);
		CBilboard::BilBoard_ChangeSizeColor(5.0f, 4.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
		CBilboard::BilBoard3_Draw(CTexture::TEX_AX_ICON, m_Position);
	}
}

bool CWeponAx::Damage(int str, float angle)
{
	return 0;
}
