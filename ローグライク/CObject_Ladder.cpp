#include "CObject.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CObject_Ladder.h"
#include "C3DObj.h"
//===================================
// グローバル変数
//===================================
//=============================================================================
//	生成
//=============================================================================

CObjectLadder::CObjectLadder(int x, int z) :CObject(TYPE_LADDER), C3DObj(C3DObj::TYPE_OBJECT)
{
	Initialize(x, z);
}

//=============================================================================
//	破棄
//=============================================================================

CObjectLadder::~CObjectLadder()
{

}

void CObjectLadder::Initialize(int x, int z)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	type = TYPE_LADDER;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CObjectLadder::Finalize(void)
{

}

void CObjectLadder::Update(void)
{

}

void CObjectLadder::Draw(void)
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
		CBilboard::BilBoard3_Draw(CTexture::TEX_LADDER, m_Position);
	}
}

int CObjectLadder::Damage(int str)
{
	return 0;
}

void CObjectLadder::Damage(void)
{

}