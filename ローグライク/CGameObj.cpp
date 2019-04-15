//////////////////////////////////////////////////
////
////	ゲームオブジェクトクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CGameObj.h"
#include "common.h"
#include "debug_font.h"

//=============================================================================
//	定数定義
//=============================================================================


//=============================================================================
//	静的変数
//=============================================================================

int CGameObj::m_FrameCount = 0;
int CGameObj::m_GameObjNum = 0;
LPDIRECT3D9 CGameObj::m_pD3D = NULL;
LPDIRECT3DDEVICE9 CGameObj::m_pD3DDevice = NULL;

CGameObj *CGameObj::pGameObj[MAX_GAMEOBJ];
//=============================================================================
//	生成
//=============================================================================

CGameObj::CGameObj()
{

}
CGameObj::CGameObj(int type)
{
	// ワークに自分自身(this)を格納
	for (m_GameObjIndex = 0; m_GameObjIndex < MAX_GAMEOBJ; m_GameObjIndex++)
	{
		if (pGameObj[m_GameObjIndex] == NULL)
		{
			pGameObj[m_GameObjIndex] = this;
			m_GameObjType = type;
			m_GameObjNum++;
			break;
		}
	}
	// ワーク格納失敗
	if (m_GameObjIndex >= MAX_GAMEOBJ)
	{
		m_GameObjIndex = -1;
	}
}
void CGameObj::FrameCountReset(void)
{
	m_FrameCount = 0;
}



//=============================================================================
//	破棄
//=============================================================================

CGameObj::~CGameObj()
{

	m_TimeKeep = 0;
	if (m_GameObjIndex >= 0)
	{
		m_GameObjNum--;
		pGameObj[m_GameObjIndex] = NULL;
	}

}


//=============================================================================
//	デバイス生成
//=============================================================================

bool CGameObj::Device_Initialize(HWND hWnd)
{
	// Direct3Dインターフェースの取得
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL) {
		// Direct3Dインターフェースの取得に失敗
		MessageBox(hWnd, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// デバイスの生成に利用する情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = (UINT)(WINDOW_WIDTH);                       // バックバッファの横幅				
	d3dpp.BackBufferHeight = (UINT)(WINDOW_HEIGHT);					    // バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;                    // バックバッファのフォーマット指定
	d3dpp.BackBufferCount = 1;                                  // バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   // スワップエフェクト（スワップ方法）の設定		
	d3dpp.Windowed = TRUE;                                      // ウィンドウモード or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE;                        // 深度バッファ・ステンシルバッファの使用				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                  // 深度バッファ・ステンシルバッファのフォーマット指定
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // フルスクリーン時のリフレッシュレートの指定
																// d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;       // リフレッシュレートとPresent処理の関係
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;       // リフレッシュレートとPresent処理の関係

																	  // Direct3Dデバイスの取得
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice))) {
		// デバイスの作成に失敗
		MessageBox(hWnd, "Direct3Dデバイスの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// サンプラーの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// アルファブレンドの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 頂点カラーとテクスチャのブレンド設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// Zバッファ有効
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// ライトを有効 
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	return true;
}

//=============================================================================
//	デバイス破棄
//=============================================================================

void CGameObj::Device_Finalize(void)
{
	// Direct3Dデバイスの開放
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dインターフェースの開放
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

void CGameObj::DebugDraw(void)
{
	DebugFont_Draw(10, 500, "FrameCount\n%d\nTIME\n%d.%02d", m_FrameCount, m_FrameCount / 60, m_FrameCount % 100);

}


//=============================================================================
// 全オブジェクト更新
//=============================================================================
void CGameObj::UpdateAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのUpdate()が呼ばれる
		if (pGameObj[i])
		{
			pGameObj[i]->Update();
		}
	}
}

//=============================================================================
// 全オブジェクト描画
//=============================================================================
void CGameObj::DrawAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのDraw()が呼ばれる
		if (pGameObj[i])
		{
			pGameObj[i]->Draw();
		}
	}
}



//=============================================================================
// 全オブジェクト削除
//=============================================================================
void CGameObj::DeleteAll2D()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (pGameObj[i])
		{
			delete pGameObj[i];
			//pGameObj[i] = NULL;
		}
	}
}


CGameObj *CGameObj::Get(int type)
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (pGameObj[i])
		{
			if (pGameObj[i]->m_GameObjType == type)
			{
				return pGameObj[i];
			}
		}
	}
	return 0;
}

