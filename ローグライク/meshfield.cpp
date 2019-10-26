#include <d3dx9.h>
#include "common.h"
#include "CTexture.h"
#include "CLight.h"
#include "meshfield.h"
//#define MAX_TYOU (36)
#define SAFE_RELEASE(x) {if(x){(x)->Release();(x)=NULL;}}


#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

CMeshField::MESH CMeshField::g_mesh[MESH_MAX];
LPDIRECT3DVERTEXBUFFER9 CMeshField::g_pVertexBuffer;
LPDIRECT3DINDEXBUFFER9 CMeshField::g_pIndexBuffer;
int CMeshField::gTexture;
D3DXVECTOR3 CMeshField::Rpos;

void CMeshField::MeshField_Init(void)
{
	Rpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < MESH_MAX; i++)
	{
		g_mesh[i].muse = false;
		g_mesh[i].tex = -1;
		g_mesh[i].pVBuffer = NULL;
		g_mesh[i].pIBuffer = NULL;
		g_mesh[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_mesh[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_mesh[i].widthmax = 0;
		g_mesh[i].heightmax = 0;
		g_mesh[i].widthcube = 0;
		g_mesh[i].heightcube = 0;
	}
}

void CMeshField::MeshField_Create(int tex, int width, int height, int wcube, int hcube, D3DXVECTOR3 pos)
{
	for (int i = 0; i < MESH_MAX; i++)
	{
		if (!g_mesh[i].muse)
		{
			g_mesh[i].muse = true;
			g_mesh[i].tex = tex;
			g_mesh[i].widthmax = width;
			g_mesh[i].heightmax = height;
			g_mesh[i].widthcube = wcube;
			g_mesh[i].heightcube = hcube;
			g_mesh[i].pos = pos;

			m_pD3DDevice->CreateVertexBuffer(		//
				sizeof(Vertex3D) * ((g_mesh[i].widthcube + 1) * (g_mesh[i].heightcube + 1)),// 借りたい頂点の容量	//
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX3D,
				D3DPOOL_MANAGED,
				&g_mesh[i].pVBuffer,
				NULL);	//

			m_pD3DDevice->CreateIndexBuffer(		//28
				sizeof(WORD) * ((2 + g_mesh[i].widthcube * 2) * g_mesh[i].heightcube + (g_mesh[i].heightcube - 1) * 2),// 	//sizeof(WORD) * (2 + WIDTH_CUBE * 2) * HEIGHT_CUBE + (HEIGHT_CUBE - 1) * 2	←
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_mesh[i].pIBuffer,
				NULL);

			Vertex3D *pV;
			g_mesh[i].pVBuffer->Lock(0, 0,/* ここに0を入れると全部借りるという意味)*/(void**)&pV, 0); // ロックをかける  pVに仮想アドレスが入るアンロック掛けたらもう使えない
			for (int y = 0; y <= g_mesh[i].heightcube; y++)
			{
				for (int x = 0; x <= g_mesh[i].widthcube; x++)
				{
					// バーテックス
					pV[y * g_mesh[i].widthcube + x + y].position = D3DXVECTOR3((float)(x * (g_mesh[i].widthmax / g_mesh[i].widthcube) - g_mesh[i].widthmax / 2), (float)g_mesh[i].pos.y, (float)(g_mesh[i].heightmax / 2 - (g_mesh[i].heightmax / g_mesh[i].heightcube) * y));
					pV[y * g_mesh[i].widthcube + x + y].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					pV[y * g_mesh[i].widthcube + x + y].color = D3DCOLOR_RGBA(255, 255, 255, 255);
					pV[y * g_mesh[i].widthcube + x + y].texcoord = D3DXVECTOR2(1.0f * (x % 2), 1.0f * (y % 2));
				}
			}

			g_mesh[i].pVBuffer->Unlock();

			LPWORD pIndex;
			g_mesh[i].pIBuffer->Lock(0, 0, (void**)&pIndex, 0);

			for (int y = 0; y < g_mesh[i].heightcube; y++)
			{
				if (y > 0)
				{
					// 右にどうち

					pIndex[y * (g_mesh[i].widthcube * 2 + 2) + y + y - 2] = pIndex[y * (g_mesh[i].widthcube * 2 + 2) + y + y - 3];
					// 左二度打ち
					pIndex[y * (g_mesh[i].widthcube * 2 + 2) + y + y - 1] = (y + 1) * (g_mesh[i].widthcube + 1);
				}

				for (int x = 0; x < (g_mesh[i].widthcube + 1) * 2; x++)
				{
					// 奇数の場合														  
					if (x % 2 == 1)
					{
						if (y <= 0)
						{
							pIndex[x] = x / 2;
						}
						else
						{
							pIndex[y * (g_mesh[i].widthcube * 2 + 2) + x + y + y] = y * g_mesh[i].widthcube + (x + 1) / 2 + y - 1;
						}
					}
					else // 偶数の場合
					{
						if (y <= 0)
						{
							pIndex[x] = g_mesh[i].widthcube + x / 2 + 1;
						}
						else
						{
							pIndex[y * (g_mesh[i].widthcube * 2 + 2) + x + y + y] = (y + 1) * (g_mesh[i].widthcube + 1) + x / 2;
						}
					}
				}
			}
			g_mesh[i].pIBuffer->Unlock();
			break;
		}
	}
}

void CMeshField::MeshField_Finalize(void)
{
	for (int i = 0; i < MESH_MAX; i++)
	{
		if (g_mesh[i].muse)
		{
			SAFE_RELEASE(g_mesh[i].pVBuffer);
			SAFE_RELEASE(g_mesh[i].pIBuffer);
		}
	}
}

void CMeshField::MeshField_Draw(void)
{

	//Set_Light();
	int i = 0;
	for (i = 0; i < MESH_MAX; i++)
	{
		if (g_mesh[i].muse)
		{
			// ワールド座標変換行列の作成と設定
			D3DXMATRIX mtxWorld;

			D3DXMatrixIdentity(&mtxWorld);

			// わーやーフレーム
			//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			m_pD3DDevice->SetFVF(FVF_VERTEX3D);

			m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
			
			m_pD3DDevice->SetTexture(0, Texture_GetTexture(g_mesh[i].tex));

			m_pD3DDevice->SetStreamSource(0, g_mesh[i].pVBuffer, 0, sizeof(Vertex3D)); //第一引数パイプライン番号, 住所Buffer, 
			m_pD3DDevice->SetIndices(g_mesh[i].pIBuffer);// テクスチャ使わない場合は値をNULLにする
			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (g_mesh[i].widthcube + 1) * (g_mesh[i].heightcube + 1), 0, (2 + g_mesh[i].widthcube * 2) * g_mesh[i].heightcube + (g_mesh[i].heightcube - 1) * 2 - 2);
		}
	}
}

