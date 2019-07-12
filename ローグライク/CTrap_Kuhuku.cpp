#include "CTrap.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CTrap_Kuhuku.h"
#include "C3DObj.h"
#include "debug_font.h"
//===================================
// グローバル変数
//===================================
//=============================================================================
//	生成
//=============================================================================

CTrapKuhuku::CTrapKuhuku(int x, int z, int trap_type) :CTrap(trap_type), C3DObj(C3DObj::TYPE_TRAP)
{
	Initialize(x, z, trap_type);
}

//=============================================================================
//	破棄
//=============================================================================

CTrapKuhuku ::~CTrapKuhuku()
{

}

void CTrapKuhuku::Initialize(int x, int z, int trap_type)
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

void CTrapKuhuku::Finalize(void)
{

}

void CTrapKuhuku::Update(void)
{

}

void CTrapKuhuku::Draw(void)
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
		CBilboard::BilBoard3_Draw(CTexture::TEX_KUHUKUTRAP_ICON, m_Position);
	}
}

