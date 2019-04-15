//=============================================================================
//	インクルードファイル
//=============================================================================

#include <d3dx9.h>
#include "common.h"

#include "CGameObj.h"

//=============================================================================
//	グローバル変数宣言
//=============================================================================


typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
} FadeVertex;
#define FVF_FADE_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

//static D3DCOLOR g_FadeColor;
//	D3DXCOLORはここ以外使わない
static int g_r = 0;				//	赤
static int g_g = 0;				//	緑
static int g_b = 0;				//	青
static int g_Alpha = 0;			//	アルファ
static int g_AddAlpha = 1;		//	アルファ加算値

static bool g_bOut = false;		// true フェードアウト　false フェードイン
static bool g_bIsFade = false;	// フェードインアウトしてるか

//=============================================================================
//	初期化処理
//=============================================================================

void Fade_Initialize(void)
{
	g_bIsFade = false;
	g_r = 0;
	g_g = 0;
	g_b = 0;
	g_Alpha = 0;
}

//=============================================================================
//	終了処理
//=============================================================================

void Fade_Finalize(void)
{
	
}

//=============================================================================
//	更新処理
//=============================================================================

void Fade_Update(void)
{
	if (!g_bIsFade)
	{
		return;
	}

	
	g_Alpha += g_AddAlpha;
	
	//	フェードイン
	if (g_bOut)
	{
		if (g_Alpha >= 255)
		{
			g_bIsFade = false;
			g_Alpha = 255;
		}
	}
	//	フェードアウト
	else
	{
		if (g_Alpha <= 0)
		{
			g_bIsFade = false;
			g_Alpha = 0;
		}
	}

}

//=============================================================================
//	描画処理
//=============================================================================

void Fade_Draw(void)
{
	if (g_Alpha <= 0)
	{
		return;
		// 見えないなら描かない
	}

	
	FadeVertex v[] = {
		{ D3DXVECTOR4(0.0f		  ,0.0f		   ,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
		{ D3DXVECTOR4(WINDOW_WIDTH,0.0f		   ,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
		{ D3DXVECTOR4(0.0f		  ,WINDOW_HEIGHT,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
		{ D3DXVECTOR4(WINDOW_WIDTH,WINDOW_HEIGHT,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
	};

	//LPDIRECT3DDEVICE9  pDevicce = mochi_GetDevice();

	CGameObj::m_pD3DDevice->SetFVF(FVF_FADE_VERTEX);
	CGameObj::m_pD3DDevice->SetTexture(0, NULL);
	CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

//=============================================================================
//	関数定義
//=============================================================================

//	フェードスタート
void Fade_Start(bool bOut, int frame, int r, int g, int b)
{
	g_bOut = bOut;
	g_AddAlpha = frame;
	g_r = r;
	g_g = g;
	g_b = b;
	

	g_bIsFade = true;
	//	フェードイン
	if (g_bOut)
	{
		g_Alpha = 0;
	}
	//	フェードアウト
	else
	{
		g_Alpha = 255;
		g_AddAlpha = -g_AddAlpha;
	}
}

//	フェードチェック
bool Fade_IsFade(void)
{
	return g_bIsFade;
}