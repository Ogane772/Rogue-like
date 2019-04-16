#include <d3dx9.h>
#include "common.h"
#include "CTexture.h"
#include "collision.h"
#include "wall.h"
//#define MAX_TYOU (36)

#define SAFE_RELEASE(x) {if(x){(x)->Release();(x)=NULL;}}

#define FVF_VERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//Vertex3D Wall[WALL_MAX];


CWall::WALL CWall::g_wall[WALL_MAX];

void CWall::MeshWall_Init(void)
{
	for (int i = 0; i < WALL_MAX; i++)
	{
		g_wall[i].wuse = false;
		g_wall[i].tex = -1;
		g_wall[i].pVBuffer = NULL;
		g_wall[i].pIBuffer = NULL;
		g_wall[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wall[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_wall[i].widthmax = 0;
		g_wall[i].heightmax = 0;
		g_wall[i].widthcube = 0;
		g_wall[i].heightcube = 0;
		g_wall[i].type = 0;
		for (int c = 0; c < WALL_WIDTH_MAX; c++)
		{
			g_wall[i].col[c].position.x = 0.0f;
			g_wall[i].col[c].position.y = WALL_NOTCOLIISION;
			g_wall[i].col[c].position.z = 0.0f;
		}
	}
}
//						テクスチャ番号、横の長さ、縦の長さ、横四角数、縦四角数  ,ポジション		法線
void CWall::MeshWall_Create(int tex, int width, int height, int wcube, int hcube, D3DXVECTOR3 pos, D3DXVECTOR3 normal)
{
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (!g_wall[i].wuse)
		{
			g_wall[i].wuse = true;
			g_wall[i].tex = tex;
			g_wall[i].widthmax = width;
			g_wall[i].heightmax = height;
			g_wall[i].widthcube = wcube;
			g_wall[i].heightcube = hcube;
			g_wall[i].pos = pos;
			g_wall[i].nor = normal;

			// 誤差修正
			if (g_wall[i].widthmax % 2 == 1)
			{
				g_wall[i].widthmax--;
				g_wall[i].widthcube--;
			}

			// 当たり判定設定
			if (g_wall[i].nor.z == 0.0f)		// 縦壁
			{
				for (int c = 0; c <= width; c++)
				{
					g_wall[i].col[c].position.x = g_wall[i].pos.x;
					g_wall[i].col[c].position.y = 0.0f;
					g_wall[i].col[c].position.z = g_wall[i].pos.z - width / 2 + c * 1.0f;
				}
			}
			else                                // 横壁
			{
				for (int c = 0; c <= width; c++)
				{
					g_wall[i].col[c].position.x = g_wall[i].pos.x - width / 2 + c * 1.0f;
					g_wall[i].col[c].position.y = 0.0f;
					g_wall[i].col[c].position.z = g_wall[i].pos.z;
				}
			}


			
			m_pD3DDevice->CreateVertexBuffer(		//
				sizeof(Vertex3D) * ((g_wall[i].widthcube + 1) * (g_wall[i].heightcube + 1)),// 借りたい頂点の容量	//
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX3D,
				D3DPOOL_MANAGED,
				&g_wall[i].pVBuffer,
				NULL);	//

			m_pD3DDevice->CreateIndexBuffer(		//28
				sizeof(WORD) * ((2 + g_wall[i].widthcube * 2) * g_wall[i].heightcube + (g_wall[i].heightcube - 1) * 2),// 	//sizeof(WORD) * (2 + WIDTH_CUBE * 2) * HEIGHT_CUBE + (HEIGHT_CUBE - 1) * 2	←
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_wall[i].pIBuffer,
				NULL);

			Vertex3D *pV;
			g_wall[i].pVBuffer->Lock(0, 0,/* ここに0を入れると全部借りるという意味)*/(void**)&pV, 0); // ロックをかける  pVに仮想アドレスが入るアンロック掛けたらもう使えない
			for (int y = 0; y <= g_wall[i].heightcube; y++)
			{
				for (int x = 0; x <= g_wall[i].widthcube; x++)
				{
					// バーテックス
					if (g_wall[i].nor.z == 1.0f) // 上壁
						pV[y * g_wall[i].widthcube + x + y].position = D3DXVECTOR3((float)(x * (g_wall[i].widthmax / g_wall[i].widthcube) - g_wall[i].widthmax / 2 + g_wall[i].pos.x), (float)(g_wall[i].heightmax / 2 - (g_wall[i].heightmax / g_wall[i].heightcube) * y), (float)g_wall[i].pos.z);
				//	pV[y * g_wall[i].widthcube + x + y].position = D3DXVECTOR3(x * (g_wall[i].widthmax / g_wall[i].widthcube) - g_wall[i].widthmax / 2, g_wall[i].pos.y, g_wall[i].heightmax / 2 - (g_wall[i].heightmax / g_wall[i].heightcube) * y);
					if (g_wall[i].nor.z == -1.0f) // 下壁
						pV[y * g_wall[i].widthcube + x + y].position = D3DXVECTOR3((float)(g_wall[i].widthmax / 2 - x * (g_wall[i].widthmax / g_wall[i].widthcube) + g_wall[i].pos.x), (float)(g_wall[i].heightmax / 2 - (g_wall[i].heightmax / g_wall[i].heightcube) * y), (float)g_wall[i].pos.z);
						//	pV[y * WIDTH_CUBE + x + y].position = D3DXVECTOR3(WIDTH_MAX / 2 - x * (WIDTH_MAX / WIDTH_CUBE), HEIGHT_MAX / 2 - (HEIGHT_MAX / HEIGHT_CUBE) * y, -10.0f);
					if (g_wall[i].nor.x == 1.0f) // 左壁
						pV[y * g_wall[i].widthcube + x + y].position = D3DXVECTOR3((float)g_wall[i].pos.x, (float)(g_wall[i].heightmax / 2 - (g_wall[i].heightmax / g_wall[i].heightcube) * y), (float)(x * (g_wall[i].widthmax / g_wall[i].widthcube) - g_wall[i].widthmax / 2 + g_wall[i].pos.z));
					//pV[y * WIDTH_CUBE + x + y].position = D3DXVECTOR3(-10.0f, HEIGHT_MAX / 2 - (HEIGHT_MAX / HEIGHT_CUBE) * y, x * (WIDTH_MAX / WIDTH_CUBE) - WIDTH_MAX / 2);

					if (g_wall[i].nor.x == -1.0f) // 右壁
						pV[y * g_wall[i].widthcube + x + y].position = D3DXVECTOR3((float)g_wall[i].pos.x, (float)(g_wall[i].heightmax / 2 - (g_wall[i].heightmax / g_wall[i].heightcube) * y), (float)(g_wall[i].widthmax / 2 - x * (g_wall[i].widthmax / g_wall[i].widthcube) + g_wall[i].pos.z));
					//pV[y * WIDTH_CUBE + x + y].position = D3DXVECTOR3(10.0f, HEIGHT_MAX / 2 - (HEIGHT_MAX / HEIGHT_CUBE) * y, WIDTH_MAX / 2 - x * (WIDTH_MAX / WIDTH_CUBE));

					//pV[y * WIDTH_CUBE + x + y].position = D3DXVECTOR3(-10.0f, HEIGHT_MAX / 2 - (HEIGHT_MAX / HEIGHT_CUBE) * y, x * (WIDTH_MAX / WIDTH_CUBE) - WIDTH_MAX / 2);
					
					//pV[y * WIDTH_CUBE + x + y].position = D3DXVECTOR3(x * (WIDTH_MAX / WIDTH_CUBE) - WIDTH_MAX / 2, HEIGHT_MAX / 2 - (HEIGHT_MAX / HEIGHT_CUBE) * y, 10.0f);
					pV[y * g_wall[i].widthcube + x + y].normal = g_wall[i].nor;
					pV[y * g_wall[i].widthcube + x + y].color = D3DCOLOR_RGBA(255, 255, 255, 255);
					pV[y * g_wall[i].widthcube + x + y].texcoord = D3DXVECTOR2(1.0f * (x % 2), 1.0f * (y % 2));
				}
			}

			g_wall[i].pVBuffer->Unlock();

			LPWORD pIndex;
			g_wall[i].pIBuffer->Lock(0, 0, (void**)&pIndex, 0);

			for (int y = 0; y < g_wall[i].heightcube; y++)
			{
				if (y > 0)
				{
					// 右にどうち

					pIndex[y * (g_wall[i].widthcube * 2 + 2) + y + y - 2] = pIndex[y * (g_wall[i].widthcube * 2 + 2) + y + y - 3];
					// 左二度打ち
					pIndex[y * (g_wall[i].widthcube * 2 + 2) + y + y - 1] = (y + 1) * (g_wall[i].widthcube + 1);
				}

				for (int x = 0; x < (g_wall[i].widthcube + 1) * 2; x++)
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
							pIndex[y * (g_wall[i].widthcube * 2 + 2) + x + y + y] = y * g_wall[i].widthcube + (x + 1) / 2 + y - 1;
						}
					}
					else // 偶数の場合
					{
						if (y <= 0)
						{
							pIndex[x] = g_wall[i].widthcube + x / 2 + 1;
						}
						else
						{
							pIndex[y * (g_wall[i].widthcube * 2 + 2) + x + y + y] = (y + 1) * (g_wall[i].widthcube + 1) + x / 2;
						}
					}
				}
			}
			g_wall[i].pIBuffer->Unlock();
			break;
		}
	}
}

void CWall::MeshWall_Finalize(void)
{
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (g_wall[i].wuse)
		{
			SAFE_RELEASE(g_wall[i].pVBuffer);
			SAFE_RELEASE(g_wall[i].pIBuffer);
		}
	}
}

void CWall::MeshWall_Draw(void)
{

	//Set_Light();
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (g_wall[i].wuse)
		{

			// ワールド座標変換行列の作成と設定
			D3DXMATRIX mtxWorld;

			D3DXMatrixIdentity(&mtxWorld);

			// わーやーフレーム
			//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			m_pD3DDevice->SetFVF(FVF_VERTEX3D);

			m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			m_pD3DDevice->SetTexture(0, Texture_GetTexture(g_wall[i].tex));

			m_pD3DDevice->SetStreamSource(0, g_wall[i].pVBuffer, 0, sizeof(Vertex3D)); //第一引数パイプライン番号, 住所Buffer, 
			m_pD3DDevice->SetIndices(g_wall[i].pIBuffer);// テクスチャ使わない場合は値をNULLにする
			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (g_wall[i].widthcube + 1) * (g_wall[i].heightcube + 1), 0, (2 + g_wall[i].widthcube * 2) * g_wall[i].heightcube + (g_wall[i].heightcube - 1) * 2 - 2);
		}
	}
}
