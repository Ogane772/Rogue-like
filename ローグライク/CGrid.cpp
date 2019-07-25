#include "C3DObj.h"
#include "CGameObj.h"
#include "Cplayer.h"
#include "CGrid.h"
#include <d3dx9.h>
#include "common.h"
//#include "sprite.h"
#define MAX_TYOU (36)

static D3DXVECTOR3 Cpos(2.0f, 0.0f, -5.0f);



#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE)
CGrid::Vertex3d CGrid::gLine[GRID_MAX + GRID_MAX];


void CGrid::Init()
{
	
	for (int i = 0; i < GRID_MAX + GRID_MAX; i++)
	{
		if (i < GRID_MAX)
		{
			if (i % 2 == 0)
			{
				gLine[i].position = D3DXVECTOR3(i / 2 * 5.0f, 0.2f, 0.0f);
			}
			else
			{
				gLine[i].position = D3DXVECTOR3(i / 2 * 5.0f, 0.2f, -25.0f);
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				gLine[i].position = D3DXVECTOR3(0.0f, 0.2f, (i - GRID_MAX) / 2 * -5.0f);
			}
			else
			{
				gLine[i].position = D3DXVECTOR3(25.0f, 0.2f, (i - GRID_MAX) / 2 * -5.0f);
			}
		}
		
		
		gLine[i].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		gLine[i].color = D3DCOLOR_RGBA(255,255, 255, 255);
	}

}

void CGrid::Update(int z, int x)
{
	for (int i = 0; i < GRID_MAX + GRID_MAX; i++)
	{
		if (i < GRID_MAX)
		{
			if (i % 2 == 0)
			{
				gLine[i].position = D3DXVECTOR3((i / 2 * 5.0f) + (-260.0f + x * 5), 0.1f, 260.0f - z * 5);
			}
			else
			{
				gLine[i].position = D3DXVECTOR3(i / 2 * 5.0f + (-260.0f + x * 5), 0.1f, -25.0f + (260.0f - z * 5));
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				gLine[i].position = D3DXVECTOR3(-260.0f + x * 5, 0.1f, (i - GRID_MAX) / 2 * -5.0f + (260.0f - z * 5));
			}
			else
			{
				gLine[i].position = D3DXVECTOR3(25.0f + (-260.0f + x * 5), 0.1f, (i - GRID_MAX) / 2 * -5.0f + (260.0f - z * 5));
			}
		}

	}
}

void CGrid::Draw(void)
{

	// ワールド座標変換行列の作成と設定
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;

	D3DXMatrixIdentity(&mtxWorld);

	// ワールド返還行列に拡大成分が入っていると法線も影響を受ける(Direct3Dデバイス)
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	m_pD3DDevice->SetFVF(FVF_VERTEX3D);

	//MyDirect3D_GetDevice()->SetTexture(0, Texture_GetTexture(texture_index));
	D3DXMatrixIdentity(&mtxWorld);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST, GRID_MAX, &gLine[0], sizeof(Vertex3d));
	//MyDirect3D_GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, &b[0], sizeof(Vertex3D));
	//Y軸回転

}

void CGrid::Finalize()
{

}

void CGrid::Grid_Draw(void)
{

	C3DObj *getplayer = CPlayer::Get_Player();
	//斜め移動モードの時4方の斜めに矢印表示
	if (getplayer->Get_GridFlag())
	{
		Update(getplayer->m_Mapz, getplayer->m_Mapx);
		
	}
	else
	{
		for (int i = 0; i < GRID_MAX + GRID_MAX; i++)
		{
			gLine[i].position.y = -1.0f;
		}
	}

	Draw();
}