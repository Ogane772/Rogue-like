//////////////////////////////////////////////////
////
////	テクスチャクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CTexture.h"

//=============================================================================
//	静的変数
//=============================================================================

CTexture::TextureFile CTexture::g_Files[TEXTURE_MANAGE_MAX];
LPDIRECT3DTEXTURE9 CTexture::g_pTextures[TEXTURE_MANAGE_MAX];

// 読み込むテクスチャファイルの事前予約をする
// 
// 引数:pFileName = ファイル名
//          width = テクスチャの
//		   height = 幅テクスチャの高さ
// 戻り値:予約(テクスチャ管理)番号
// 

int CTexture::Texture_SetLoadFile(const char* pFileName, int width, int height)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		// ネズミ返しコード
		if (g_Files[i].filename[0]) continue;


		// 使われてないコード
		strcpy(g_Files[i].filename, pFileName);
		g_Files[i].width = width;
		g_Files[i].height = height;



		return i;
	}

	return TEXTURE_INVALID_INDEX;
}


// テクスチャの読み込み
// 
// 戻り値:読み込めなかった数
// 

int CTexture::Texture_Load(void)
{
	int failed_count = 0;
	HRESULT hr;

	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		// ネズミ返しコード
		if (!g_Files[i].filename[0]) continue;

		// データベースに記載があった

		if (g_pTextures[i]) continue;

		if (!m_pD3DDevice)
		{
			failed_count++;
			continue;
		}



		hr = D3DXCreateTextureFromFile(m_pD3DDevice, g_Files[i].filename, &g_pTextures[i]);

		if (FAILED(hr))
		{
			failed_count++;
			continue;
		}

	}
	//	安全に読み込める
	return failed_count;
}

// テクスチャの取得
LPDIRECT3DTEXTURE9 CTexture::Texture_GetTexture(int index)
{
	return g_pTextures[index];
}
// テクスチャの幅の取得
int CTexture::Texture_GetWidth(int index)
{
	return g_Files[index].width;
}

// テクスチャの高さの取得
int CTexture::Texture_GetHeight(int index)
{
	return g_Files[index].height;
}

// テクスチャの解放
// 
//	引数: indices = 解放したいテクスチャ番号
//					が入った配列の先頭アドレス
//		  count   = 解放したいテクスチャ番号
//                  の個数
//
int CTexture::Texture_Release(int indices[], int count)
{
	int unrelease_count = 0;

	for (int i = 0; i < count; i++)
	{
		// SAFERELEASEでも可能
		if (g_pTextures[indices[i]])
		{
			g_pTextures[indices[i]]->Release();
			g_pTextures[indices[i]] = NULL;

		}
		else
		{
			unrelease_count++;
		}
		g_Files[indices[i]].filename[0] = 0; // \0と同義
	}

	return unrelease_count;
}

// テクスチャの全解放
void CTexture::Texture_ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{// SAFERELEASE
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
		g_Files[i].filename[0] = 0; // \0と同義
	}

}













