//////////////////////////////////////////////////
////
////	2Dオブジェクトクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================


#include "C2DObj.h"
#include "CCamera.h"
#include "common.h"
#include "CObject.h"
//=============================================================================
//	定数定義
//=============================================================================


//=============================================================================
//	静的変数
//=============================================================================

LPDIRECT3DVERTEXBUFFER9 C2DObj::m_p2DVertexBuffer = NULL;	
LPDIRECT3DINDEXBUFFER9 C2DObj::m_p2DIndexBuffer = NULL;

//=============================================================================
//	生成
//=============================================================================

C2DObj::C2DObj()
{
	
}

//=============================================================================
//	破棄
//=============================================================================

C2DObj::~C2DObj()
{
	
}

//=============================================================================
//	スプライト初期化
//=============================================================================

void C2DObj::Sprite_Initialize(void)
{
	C2DObj::m_pD3DDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &m_p2DVertexBuffer, NULL);
	/*
	1 頂点のサイズ
	2 用途
	3 FVF
	4 VRAM上のメモリの管理
	5 メモ帳の場所
	6 NULL(分かんないときはNULLを入れる)
	*/

	// インデックスバッファの確保
	C2DObj::m_pD3DDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_p2DIndexBuffer, NULL);
	/*
	1 インデックスのサイズ
	2 用途
	3 インデックスのフォーマット(16ビットか32ビット)
	4 VRAM上のメモリの管理
	5 メモ帳の場所
	6 NULL(分かんないときはNULLを入れる)
	*/
}

//=============================================================================
//	スプライト描画
//=============================================================================

void C2DObj::Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th)
{
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	float u0 = (float)tx / w;
	float v0 = (float)ty / h;
	float u1 = (float)tw / w;
	float v1 = (float)th / h;


	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f,dy - 0.5f,0.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0	  ,v0) },	//	第4引数は絶対1.0ｆ
		{ D3DXVECTOR4(dx + tw - 0.5f,dy - 0.5f,0.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0 + u1,v0) },
		{ D3DXVECTOR4(dx - 0.5f,dy + th - 0.5f,0.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0	  ,v0 + v1) },
		{ D3DXVECTOR4(dx + tw - 0.5f,dy + th - 0.5f,0.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0 + u1,v0 + v1) },

	};

	// 頂点インデックス
	WORD index[] = { 0, 1, 2, 1, 3, 2 };
	// window.h　必用　unsigned short

	// 仮想アドレス　
	Vertex2D *pv;
	// 頂点バッファのロック
	m_p2DVertexBuffer->Lock(0, 0, (void**)&pv, 0);
	/*
	1 どこから
	2 0でいい
	3 仮想アドレスを入れる
	4 0でいい
	*/
	memcpy(pv, v, sizeof(v));
	m_p2DVertexBuffer->Unlock();

	// 仮想アドレス　
	WORD *Ipv;
	// インデックスバッファのロック
	m_p2DIndexBuffer->Lock(0, 0, (void**)&Ipv, 0);
	/*
	1 どこから
	2 0でいい
	3 仮想アドレスを入れる
	4 0でいい
	*/
	Ipv[0] = 0;
	Ipv[1] = 1;
	Ipv[2] = 2;
	Ipv[3] = 1;
	Ipv[4] = 3;
	Ipv[5] = 2;
	m_p2DIndexBuffer->Unlock();

	m_pD3DDevice->SetFVF(FVF_VERTEX2D);		//	FVFをデバイスに設定

	m_pD3DDevice->SetTexture(0, Texture_GetTexture(texture_index));

	m_pD3DDevice->SetStreamSource(0, m_p2DVertexBuffer, 0, sizeof(Vertex2D));
	m_pD3DDevice->SetIndices(m_p2DIndexBuffer);

	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

//	タイトルリザルト用
void C2DObj::m_Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th)
{
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	float u0 = (float)tx / w;
	float v0 = (float)ty / h;
	float u1 = (float)tw / w;
	float v1 = (float)th / h;


	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f,dy - 0.5f,1.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0	  ,v0) },	//	第4引数は絶対1.0ｆ
		{ D3DXVECTOR4(dx + tw - 0.5f,dy - 0.5f,1.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0 + u1,v0) },
		{ D3DXVECTOR4(dx - 0.5f,dy + th - 0.5f,1.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0	  ,v0 + v1) },
		{ D3DXVECTOR4(dx + tw - 0.5f,dy + th - 0.5f,1.0f,1.0f),D3DCOLOR(0xffffffff),D3DXVECTOR2(u0 + u1,v0 + v1) },

	};

	// 頂点インデックス
	WORD index[] = { 0, 1, 2, 1, 3, 2 };
	// window.h　必用　unsigned short

	// 仮想アドレス　
	Vertex2D *pv;
	// 頂点バッファのロック
	m_p2DVertexBuffer->Lock(0, 0, (void**)&pv, 0);
	/*
	1 どこから
	2 0でいい
	3 仮想アドレスを入れる
	4 0でいい
	*/
	memcpy(pv, v, sizeof(v));
	m_p2DVertexBuffer->Unlock();

	// 仮想アドレス　
	WORD *Ipv;
	// インデックスバッファのロック
	m_p2DIndexBuffer->Lock(0, 0, (void**)&Ipv, 0);
	/*
	1 どこから
	2 0でいい
	3 仮想アドレスを入れる
	4 0でいい
	*/
	Ipv[0] = 0;
	Ipv[1] = 1;
	Ipv[2] = 2;
	Ipv[3] = 1;
	Ipv[4] = 3;
	Ipv[5] = 2;
	m_p2DIndexBuffer->Unlock();

	m_pD3DDevice->SetFVF(FVF_VERTEX2D);		//	FVFをデバイスに設定

	m_pD3DDevice->SetTexture(0, Texture_GetTexture(texture_index));

	m_pD3DDevice->SetStreamSource(0, m_p2DVertexBuffer, 0, sizeof(Vertex2D));
	m_pD3DDevice->SetIndices(m_p2DIndexBuffer);


	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void C2DObj::SpriteDraw(int texture_index, float dx, float dy)
{
	int w = Texture_GetWidth(texture_index);
	int h = Texture_GetHeight(texture_index);
	Vertex2D v[] = {
		{ D3DXVECTOR4(dx , dy,        1.0f, 1) , D3DCOLOR_ARGB(255,255,255,255) | D3DFVF_DIFFUSE , D3DXVECTOR2(0, 0) },
		{ D3DXVECTOR4(dx + w ,dy,     1.0f, 1) , D3DCOLOR_ARGB(255,255,255,255) | D3DFVF_DIFFUSE , D3DXVECTOR2(1 ,0) },
		{ D3DXVECTOR4(dx , dy + h,    1.0f, 1) , D3DCOLOR_ARGB(255,255,255,255) | D3DFVF_DIFFUSE , D3DXVECTOR2(0 ,1) },
		{ D3DXVECTOR4(dx + w, dy + h, 1.0f, 1) , D3DCOLOR_ARGB(255,255,255,255) | D3DFVF_DIFFUSE , D3DXVECTOR2(1 ,1) }
	};

	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//Invisible anable
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//Line dot type and filter
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetTexture(0, Texture_GetTexture(texture_index));
	m_pD3DDevice->SetFVF(FVF_VERTEX2D);
	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));		//drawing are betwwen beginscene and end scene
}

void C2DObj::Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, float cx, float cy, float sx, float sy, float rotation)
{
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h;

	// 座標変換
	float px[4], py[4];
	px[0] = -cx        * sx * cosf(rotation) - -cy        * sy * sinf(rotation);
	py[0] = -cx        * sx * sinf(rotation) + -cy        * sy * cosf(rotation);
	px[1] = (-cx + tw) * sx * cosf(rotation) - -cy        * sy * sinf(rotation);
	py[1] = (-cx + tw) * sx * sinf(rotation) + -cy        * sy * cosf(rotation);
	px[2] = -cx        * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[2] = -cx        * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);
	px[3] = (-cx + tw) * sx * cosf(rotation) - (-cy + th) * sy * sinf(rotation);
	py[3] = (-cx + tw) * sx * sinf(rotation) + (-cy + th) * sy * cosf(rotation);

	Vertex2D* pV;
	m_p2DVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].pos = D3DXVECTOR4(px[0] + dx + cx - 0.5f, py[0] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(px[1] + dx + cx - 0.5f, py[1] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(px[2] + dx + cx - 0.5f, py[2] + dy + cy - 0.5f, 0.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(px[3] + dx + cx - 0.5f, py[3] + dy + cy - 0.5f, 0.0f, 1.0f);

	pV[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pV[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pV[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pV[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	m_p2DVertexBuffer->Unlock();

	m_pD3DDevice->SetFVF(FVF_VERTEX2D);
	m_pD3DDevice->SetTexture(0, Texture_GetTexture(texture_index));
	m_pD3DDevice->SetStreamSource(0, m_p2DVertexBuffer, 0, sizeof(Vertex2D));
	m_pD3DDevice->SetIndices(m_p2DIndexBuffer);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void C2DObj::Sprite_Draw(int texture_index, float dx, float dy, int alpha)
{
	// テクスチャの幅、高さ取得
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// 頂点バッファへの頂点情報の書き込み

	Vertex2D* pV; // 仮想アドレス
	m_p2DVertexBuffer->Lock(0, 0, (void**)&pV, 0);

	pV[0].pos = D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[1].pos = D3DXVECTOR4(dx + w - 0.5f, dy - 0.5f, 0.0f, 1.0f);
	pV[2].pos = D3DXVECTOR4(dx - 0.5f, dy + h - 0.5f, 0.0f, 1.0f);
	pV[3].pos = D3DXVECTOR4(dx + w - 0.5f, dy + h - 0.5f, 0.0f, 1.0f);

	pV[0].color = D3DCOLOR_RGBA(255, 255, 255, alpha);
	pV[1].color = D3DCOLOR_RGBA(255, 255, 255, alpha);
	pV[2].color = D3DCOLOR_RGBA(255, 255, 255, alpha);
	pV[3].color = D3DCOLOR_RGBA(255, 255, 255, alpha);

	pV[0].texcoord = D3DXVECTOR2(0.0f, 0.0f);
	pV[1].texcoord = D3DXVECTOR2(1.0f, 0.0f);
	pV[2].texcoord = D3DXVECTOR2(0.0f, 1.0f);
	pV[3].texcoord = D3DXVECTOR2(1.0f, 1.0f);

	m_p2DVertexBuffer->Unlock();

	m_pD3DDevice->SetFVF(FVF_VERTEX2D);                                     // デバイスに頂点の構成要素を設定
	m_pD3DDevice->SetTexture(0, Texture_GetTexture(texture_index));         // デバイスにテクスチャを設定
	m_pD3DDevice->SetStreamSource(0, m_p2DVertexBuffer, 0, sizeof(Vertex2D)); // 頂点バッファの設定
	m_pD3DDevice->SetIndices(m_p2DIndexBuffer);                               // インデックスバッファの設定
																	   // プリミティブの描画
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

//=============================================================================
//	スプライト破棄
//=============================================================================

void C2DObj::Sprite_Finalize(void)
{
	// インデックスバッファの解放
	if (m_p2DIndexBuffer != NULL)
	{
		m_p2DIndexBuffer->Release();
		m_p2DIndexBuffer = NULL;
	}

	// 頂点バッファの解放
	if (m_p2DVertexBuffer != NULL)
	{
		m_p2DVertexBuffer->Release();
		m_p2DVertexBuffer = NULL;
	}
}




void C2DObj::Update(void)
{

}
void C2DObj::Draw(void)
{

}
void C2DObj::Finalize(void)
{

}