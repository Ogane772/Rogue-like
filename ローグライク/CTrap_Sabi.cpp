#include "CTrap.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CTrap_Sabi.h"
#include "C3DObj.h"
#include "debug_font.h"
//===================================
// グローバル変数
//===================================
//=============================================================================
//	生成
//=============================================================================

CTrapSabi::CTrapSabi(int x, int z, int trap_type) :CTrap(trap_type), C3DObj(C3DObj::TYPE_TRAP)
{
	Initialize(x, z, trap_type);
}

//=============================================================================
//	破棄
//=============================================================================

CTrapSabi::~CTrapSabi()
{

}

void CTrapSabi::Initialize(int x, int z, int trap_type)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = trap_type;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
	m_WanaCheck = false;
	m_WanaEffectOn = false;
}

void CTrapSabi::Finalize(void)
{

}

void CTrapSabi::Update(void)
{

}

void CTrapSabi::Draw(void)
{
	// ワールド座標変換行列の作成と設定
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	if (alive && m_WanaCheck)
	{
		D3DXMatrixIdentity(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslotation, m_Position.x, m_Position.y, m_Position.z);
		CBilboard::BilBoard_ChangeSizeColor(4.0f, 3.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
		CBilboard::BilBoard3_Draw(CTexture::TEX_SABITRAP_ICON, m_Position);
	}
}

