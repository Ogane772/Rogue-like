//////////////////////////////////////////////////
////
////	ライトクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CLight.h"
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//=============================================================================
//	定数定義
//=============================================================================


//=============================================================================
//	静的変数
//=============================================================================

CLight *CLight::m_pLight;


//=============================================================================
//	生成
//=============================================================================
CLight::CLight():CGameObj(CGameObj::TYPE_LIGHT)
{
	Light_Initialize();
}

//=============================================================================
//	破棄
//=============================================================================
CLight::~CLight()
{
	//Light_Finalize();
}



void CLight::Update(void)
{
	
}

void CLight::Draw(void)
{
	Light_Set();
}

//	ライト初期化
void CLight::Light_Initialize(void)
{
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// 法線ベクトルを1にする
}

CLight* CLight::Light_Create(void)
{
	m_pLight = new CLight;
	return m_pLight;
}

void CLight::Finalize(void)
{
	if (m_pLight != NULL)
	{
		delete m_pLight;
	}
}

//	ライトセット
void CLight::Light_Set(void)
{

	// 平行光源(ディレクショナルディフェーズ)
	D3DLIGHT9 dirLight;

	ZeroMemory(&dirLight, sizeof(dirLight));

	dirLight.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの向き
	D3DXVECTOR3 vecDir(0.0f, 0.0f, 1.0f);
	// 正規化
	D3DXVec3Normalize((D3DXVECTOR3*)&dirLight.Direction, &vecDir);//(1にしたものを格納するためのベクトル, 長さ1にしたいベクトル)

																  // カラー
	dirLight.Diffuse.r = 1.0f;
	dirLight.Diffuse.g = 1.0f;
	dirLight.Diffuse.b = 1.0f;
	dirLight.Diffuse.a = 1.0f;

	// アンビエントライト(環境光)					// 暗めがいい
	//pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(8, 0, 24, 255));
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(128, 128, 128, 255));
	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);	// D3DMCS_COLOR1 キューブ用
																			// D3DMCS_MATERIAL マテリアル用
																			// ライトをつけてくれ～ 4まである
	m_pD3DDevice->SetLight(0, &dirLight);

	// 0番ライトを有効にする
	m_pD3DDevice->LightEnable(0, true);
}









