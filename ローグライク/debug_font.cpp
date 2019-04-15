
//=============================================================================
//	インクルードファイル
//=============================================================================
#include <d3dx9.h>
#include"common.h"
//#include"mochi_d3d.h"
#include <stdio.h>
#include "CGameObj.h"
//=============================================================================
//	グローバル変数宣言
//=============================================================================
static LPD3DXFONT g_pD3DXFont;


//=============================================================================
//	初期化処理
//=============================================================================

void DebugFont_Initialize(void)
{
#if defined(_DEBUG) || defined(DEBUG)

	//	LPDIRECT3DDEVICE9 pDevice = mochi_GetDevice();
	D3DXCreateFont(CGameObj::m_pD3DDevice, 35, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

#endif //(_DEBUG) || defined(DEBUG)
}



//=============================================================================
//	終了処理
//=============================================================================

void DebugFont_Finalize(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	if (g_pD3DXFont != NULL)
	{
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}

#endif	//(_DEBUG) || defined(DEBUG)
}

//=============================================================================
//	描画処理
//=============================================================================

void DebugFont_Draw(void)
{
#if defined(_DEBUG) || defined(DEBUG)
	RECT rect = { 0,0,(LONG)WINDOW_WIDTH,(LONG)WINDOW_HEIGHT };
	g_pD3DXFont->DrawText(NULL, "文字列", -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));

#endif	(_DEBUG) || defined(DEBUG)
}

void DebugFont_Draw(int x, int y, const char* pFormat, ...)
{
#if defined(_DEBUG) || defined(DEBUG)
	RECT rect = { x, y, (LONG)WINDOW_WIDTH, (LONG)WINDOW_HEIGHT
	};

	va_list argp;
	va_start(argp, pFormat);
	// 文字列用バッファ
	char buf[256];
	vsprintf_s(buf, 256, pFormat, argp);
	va_end(argp);

	g_pD3DXFont->DrawText(NULL,
		buf,
		-1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(255, 0, 0, 255));
#else
	UNREFERENCED_PARAMETER(pFormat);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
#endif // _DEBUG || DEBUG
}