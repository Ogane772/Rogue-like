#include "CWepon.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CWepon_BigSword.h"
//===================================
// グローバル変数
//===================================
//=============================================================================
//	生成
//=============================================================================

CWeponBigSword::CWeponBigSword(int x, int z) :CWepon(TYPE_BIGSWORD), C3DObj(C3DObj::TYPE_WEPON)
{
	Initialize(x, z);
}

//=============================================================================
//	破棄
//=============================================================================

CWeponBigSword::~CWeponBigSword()
{

}

void CWeponBigSword::Initialize(int x, int z)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = TYPE_BIGSWORD;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CWeponBigSword::Finalize(void)
{

}

void CWeponBigSword::Update(void)
{

}

void CWeponBigSword::Draw(void)
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
		CBilboard::BilBoard3_Draw(CTexture::TEX_BIGSWORD_ICON, m_Position);
	}
}
