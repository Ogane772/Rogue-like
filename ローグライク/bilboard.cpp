#include "bilboard.h"
#include "CTexture.h"
#include "CCamera.h"
#include <d3dx9.h>
#include "C3DObj.h"
static D3DXMATRIX mtxImV;

typedef struct Vertex3D
{
	D3DXVECTOR3 position;	// x,y,z
	D3DCOLOR color;			// r,g,b,a
	D3DXVECTOR2 texcoord;	// u,v
}Vertex3D;
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;
static LPDIRECT3DVERTEXBUFFER9 g_ShadowVertexBuf = NULL;
static LPDIRECT3DINDEXBUFFER9 g_ShadowIndexBuf = NULL;
static D3DXMATRIX gWorld;
static int g_Shadow = -1;

Vertex3D board[] = {
	// �O
	{ D3DXVECTOR3(0.0f, 1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,128),D3DXVECTOR2(0.0f,0.0f) },	// 0
	{ D3DXVECTOR3(1.0f, 1.0f,0.0f),D3DCOLOR_RGBA(255,255,255,128),D3DXVECTOR2(1.0f,0.0f) },	// 1
	{ D3DXVECTOR3(1.0f, 0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,128),D3DXVECTOR2(1.0f,1.0f) },	// 2
	{ D3DXVECTOR3(0.0f, 0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,128),D3DXVECTOR2(0.0f,1.0f) }		// 3
};


Vertex3D shadow[] = {

	{ D3DXVECTOR3(0.0f, 3.5f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f) },	// 0
	{ D3DXVECTOR3(3.5f, 3.5f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f) },	// 1
	{ D3DXVECTOR3(3.5f, 0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f) },	// 2
	{ D3DXVECTOR3(0.0f, 0.0f,0.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f) }		// 3
};

void CBilboard::Shadow_Init(void)
{
	g_Shadow = CTexture::Texture_SetLoadFile("./asset/texture/shadow000.jpg", 128, 128);

	CGameObj::m_pD3DDevice->CreateVertexBuffer(sizeof(Vertex3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_ShadowVertexBuf, NULL);
	CGameObj::m_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_ShadowIndexBuf,  NULL);

	Vertex3D* pV;


	g_ShadowVertexBuf->Lock(0, 0, (void**)&pV, 0);
	memcpy(pV, shadow, sizeof(Vertex3D) * 4);
	g_ShadowVertexBuf->Unlock();



	WORD* pIndex;
	//pIndex = (WORD*)malloc(sizeof(WORD) * 6);
	g_ShadowIndexBuf->Lock(0, 0, (void**)&pIndex, 0);
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 0;
	pIndex[4] = 2;
	pIndex[5] = 3;

	g_ShadowIndexBuf->Unlock();
}

void CBilboard::BilBoard_Initialize(void)
{
	CGameObj::m_pD3DDevice->CreateVertexBuffer(sizeof(Vertex3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX3D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	CGameObj::m_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);

	Vertex3D* pV;


	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
	memcpy(pV, board, sizeof(Vertex3D) * 4);
	g_pVertexBuffer->Unlock();
	


	WORD* pIndex;
	//pIndex = (WORD*)malloc(sizeof(WORD) * 6);
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 0;
	pIndex[4] = 2;
	pIndex[5] = 3;

	g_pIndexBuffer->Unlock();

	//free(pIndex);
}

void CBilboard::BilBoard_Finalize(void)
{
	if (g_pIndexBuffer)
		g_pIndexBuffer->Release();
	g_pIndexBuffer = NULL;

	if (g_pVertexBuffer)
		g_pVertexBuffer->Release();
	g_pVertexBuffer = NULL;
	//Texture_Release(&g_TextureIndex, 1);
}

void CBilboard::BilBoard_Update(void)
{
	
	/*

	mtxImV._41 = 0.0f;
	mtxImV._42 = 0.0f;
	mtxImV._43 = 0.0f;
	*/
	D3DXMatrixTranspose(&mtxImV, &CCamera::Get_ViewMtx());
	mtxImV._14 = 0.0f;
	mtxImV._24 = 0.0f;
	mtxImV._34 = 0.0f;

	gWorld = mtxImV;
}


void CBilboard::BilBoard3_Draw(const int TexIndex, D3DXVECTOR3 pos)
{
	//CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ��΍��i
	//CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	// �A���t�@�e�X�g���g�p
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	// �A���t�@�e�X�g���i�l��ݒ�
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 10);
	// ��r(�������l < ���l)
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//CGameObj::m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMATRIX mtxT;
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	D3DXMatrixTranslation(&mtxT, pos.x, pos.y, pos.z);
	//gWorld = world  
	gWorld *= mtxT;
	
	CGameObj::m_pD3DDevice->SetFVF(FVF_VERTEX3D);
	CGameObj::m_pD3DDevice->SetTransform(D3DTS_WORLD, &gWorld);
	CGameObj::m_pD3DDevice->SetTexture(0, CTexture::Texture_GetTexture(TexIndex));
	CGameObj::m_pD3DDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(Vertex3D));
	CGameObj::m_pD3DDevice->SetIndices(g_pIndexBuffer);
	CGameObj::m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	//CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, g_pVertexBuffer, sizeof(Vertex3D));
	// �A���t�@�e�X�g���g�p
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// �A���t�@�e�X�g���g�p
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBilboard::BilBoard_ChangeSizeColor(float sizeX, float sizeY, D3DCOLOR color)
{
	/*board[] = {
		// �O
		{ D3DXVECTOR3(-0.5f, 0.5f,0.0f),color,D3DXVECTOR2(0.0f,0.0f) },	// 0
		{ D3DXVECTOR3(6.5f, 0.5f,0.0f), color,D3DXVECTOR2(1.0f,0.0f) },	// 1
		{ D3DXVECTOR3(6.5f,-0.5f,0.0f), color,D3DXVECTOR2(1.0f,1.0f) },	// 2
		{ D3DXVECTOR3(-0.5f,-0.5f,0.0f),color,D3DXVECTOR2(0.0f,1.0f) }		// 3
	};*/
	board[0] = { D3DXVECTOR3(0.0f, sizeY,0.0f),color,D3DXVECTOR2(0.0f,0.0f) };
	board[1] = { D3DXVECTOR3(sizeX, sizeY,0.0f),color,D3DXVECTOR2(1.0f,0.0f) };
	board[2] = { D3DXVECTOR3(sizeX, 0.0f,0.0f),color,D3DXVECTOR2(1.0f,1.0f) };
	board[3] = { D3DXVECTOR3(0.0f, 0.0f,0.0f),color,D3DXVECTOR2(0.0f,1.0f) };
	BilBoard_Finalize();
	BilBoard_Initialize();
	BilBoard_Update();
}

void CBilboard::BilBoard_ChangeMapPos(float sizeX, float sizeY, D3DCOLOR color)
{
	/*board[] = {
		// �O
		{ D3DXVECTOR3(-0.5f, 0.5f,0.0f),color,D3DXVECTOR2(0.0f,0.0f) },	// 0
		{ D3DXVECTOR3(6.5f, 0.5f,0.0f), color,D3DXVECTOR2(1.0f,0.0f) },	// 1
		{ D3DXVECTOR3(6.5f,-0.5f,0.0f), color,D3DXVECTOR2(1.0f,1.0f) },	// 2
		{ D3DXVECTOR3(-0.5f,-0.5f,0.0f),color,D3DXVECTOR2(0.0f,1.0f) }		// 3
	};*/
	board[0] = { D3DXVECTOR3(sizeX,         sizeY + 0.05f,0.0f),color,D3DXVECTOR2(0.0f,0.0f) };
	board[1] = { D3DXVECTOR3(sizeX + 0.05f, sizeY + 0.05f,0.0f),color,D3DXVECTOR2(1.0f,0.0f) };
	board[2] = { D3DXVECTOR3(sizeX + 0.05f, sizeY,0.0f),color,D3DXVECTOR2(1.0f,1.0f) };
	board[3] = { D3DXVECTOR3(sizeX,         sizeY,0.0f),color,D3DXVECTOR2(0.0f,1.0f) };
	BilBoard_Finalize();
	BilBoard_Initialize();
	BilBoard_Update();
}

void CBilboard::Shadow_Draw(D3DXMATRIX mtxS, D3DXVECTOR3 pos)
{
	// ���Y����
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	D3DXMATRIX mtxTranslation;
	D3DXMATRIX mtxRotation;
	// 
	D3DXMatrixTranslation(&mtxTranslation, pos.x - 3.5f * 0.5f, 0.01f, pos.z - 3.5f * 0.5f);
	D3DXMatrixRotationYawPitchRoll(&mtxRotation, 0.0f, D3DXToRadian(90), 0.0f);
	mtxS = mtxRotation * mtxTranslation;

	CGameObj::m_pD3DDevice->SetFVF(FVF_VERTEX3D);

	CGameObj::m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxS);
	CGameObj::m_pD3DDevice->SetTexture(0, CTexture::Texture_GetTexture(g_Shadow));
	CGameObj::m_pD3DDevice->SetStreamSource(0, g_ShadowVertexBuf, 0, sizeof(Vertex3D));
	CGameObj::m_pD3DDevice->SetIndices(g_ShadowIndexBuf);
	CGameObj::m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	//CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, g_pVertexBuffer, sizeof(Vertex3D));

	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CGameObj::m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBilboard::Shadow_Finalize(void)
{
	if (g_ShadowIndexBuf)
		g_ShadowIndexBuf->Release();
	g_ShadowIndexBuf = NULL;

	if (g_ShadowVertexBuf)
		g_ShadowVertexBuf->Release();
	g_ShadowVertexBuf = NULL;
}