#include "CWepon.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CWepon_Reipia.h"
//===================================
// グローバル変数
//===================================
//=============================================================================
//	生成
//=============================================================================

CWeponReipia::CWeponReipia(int x, int z) :CWepon(TYPE_REIPIA), C3DObj(C3DObj::TYPE_WEPON)
{
	Initialize(x, z);
}

//=============================================================================
//	破棄
//=============================================================================

CWeponReipia::~CWeponReipia()
{

}

void CWeponReipia::Initialize(int x, int z)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = TYPE_REIPIA;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CWeponReipia::Finalize(void)
{

}

void CWeponReipia::Update(void)
{

}

void CWeponReipia::Draw(void)
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
		CBilboard::BilBoard3_Draw(CTexture::TEX_REIPIA_ICON, m_Position);
	}
}

