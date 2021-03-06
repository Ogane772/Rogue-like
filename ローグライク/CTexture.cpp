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

CTexture::TextureFileData CTexture::TEXTURE_FILES[] = {
	{ "asset/texture/title.png",1280,720 },
	{ "asset/texture/blue.png",5,5 },
	{ "asset/texture/wood.png",225,225 },
	{ "asset/texture/Renga.jpg",1300,1300 },
	{ "asset/texture/blackup.png",200,200 },
	{ "asset/texture/block.png",128,128 },
	{ "asset/texture/ladder.png",640,640 },
	{ "asset/texture/1F.png",1284,720 },
	{ "asset/texture/2F.png",1284,720 },
	{ "asset/texture/3F.png",1284,720 },
	{ "asset/texture/4F.png",1284,720 },
	{ "asset/texture/5F.png",1284,720 },
	{ "asset/texture/shadow000.png",128,128 },
	{ "asset/texture/effect_hit.png",960,240 },
	{ "asset/texture/effect_hit_week.png",960,240 },
	{ "asset/texture/effect_hit_resist.png",960,240 },
	{ "asset/texture/rezult.png",1920,1080 },
	{ "asset/texture/message_window.png",845,154 },
	{ "asset/texture/player_icon.png",8,8 },
	{ "asset/texture/enemy_icon.png",8,8 },
	{ "asset/texture/ladder_icon.png",8,8 },
	{ "asset/texture/item_icon.png",8,8 },
	{ "asset/texture/Gage.png",250,40 },
	{ "asset/texture/HP_Gage.png",250,40 },
	{ "asset/texture/HP_Gage_2.png",250,40 },
	{ "asset/texture/HP_Gage_poizun.png",250,40 },
	{ "asset/texture/cursor.png",139,192 },
	{ "asset/texture/cursor_gry.png",139,192 },
	{ "asset/texture/swordicon.png",200,200 },
	{ "asset/texture/bigswordicon.png",200,200 },
	{ "asset/texture/reipiaicon.png",200,200 },
	{ "asset/texture/yariicon.png",200,200 },
	{ "asset/texture/konbouicon.png",200,200 },
	{ "asset/texture/onoicon.png",200,200 },
	{ "asset/texture/sheldicon.png",200,200 },
	{ "asset/texture/ringicon.png",200,200 },
	{ "asset/texture/largewindow.png",175,334 },
	{ "asset/texture/yakusou.png",200,200 },
	{ "asset/texture/maru.png",1284,720 },
	{ "asset/texture/yajirusi.png",800,800 },
	{ "asset/texture/yajirusi2.png",800,800 },
	{ "asset/texture/yajirusi3.png",500,500 },
	{ "asset/texture/kurayami_icon.png",250,200 },
	{ "asset/texture/poizun_icon.png",250,200 },
	{ "asset/texture/sleep_icon.png",250,200 },
	{ "asset/texture/charge_icon.png",250,200 },
	{ "asset/texture/mahuji_icon.png",250,200 },
	{ "asset/texture/black.png",1920,1080 },
	{ "asset/texture/middlewindow.png",845,154 },
	{ "asset/texture/HP_Gage_onaka.png",250,40 },
	{ "asset/texture/kakiicon.png",200,200 },
	{ "asset/texture/aisyou.png",884,582 },
	{ "asset/texture/buki_maru.png",458,278 },
	{ "asset/texture/trap.png",200,200 },
	{ "asset/texture/trap_icon.png",8,8 },
	{ "asset/texture/trap_damage.png",200,200 },
	{ "asset/texture/trap_warp.png",200,200 },
	{ "asset/texture/trap_kuhuku.png",200,200 },
	{ "asset/texture/trap_sleep.png",200,200 },
	{ "asset/texture/trap_poizun.png",200,200 },
	{ "asset/texture/trap_kuyarami.png",200,200 },
	{ "asset/texture/trap_donsoku.png",200,200 },
	{ "asset/texture/trap_jyosou.png",200,200 },
	{ "asset/texture/trap_sabi.png",200,200 },
};

int CTexture::TEXTURE_MAX = sizeof(CTexture::TEXTURE_FILES) / sizeof(TEXTURE_FILES[0]);

LPDIRECT3DTEXTURE9 CTexture::m_pTextures[sizeof(CTexture::TEXTURE_FILES) / sizeof(TEXTURE_FILES[0])] = {};




//=============================================================================
//	生成
//=============================================================================

CTexture::CTexture()
{

}

//=============================================================================
//	破棄
//=============================================================================

CTexture::~CTexture()
{
	//CTexture::Texture_Release();

}


//	テクスチャの読み込み
int CTexture::Texture_Load(void)
{

	HRESULT hr;
	int i;
	int failed_count = 0;
	for (i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_pTextures[i] == NULL)
		{
			hr = D3DXCreateTextureFromFile(m_pD3DDevice, TEXTURE_FILES[i].filename, &m_pTextures[i]);
			if (FAILED(hr))
			{
				failed_count++;
				MessageBox(NULL, "テクスチャデータを読み込めませんでした", "確認", MB_OK);
			}
		}
	}

	return failed_count;
}


// テクスチャの開放
void CTexture::Texture_Release(void)
{
	int i;
	for (i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_pTextures[i] != NULL)
		{
			m_pTextures[i]->Release();
			m_pTextures[i] = NULL;
		}
	}
}

void CTexture::Texture_Load(int index)
{
	HRESULT hr;
	if (m_pTextures[index] == NULL)
	{
		hr = D3DXCreateTextureFromFile(m_pD3DDevice, TEXTURE_FILES[index].filename, &m_pTextures[index]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "テクスチャデータを読み込めませんでした", "確認", MB_OK);
		}
	}
}

void CTexture::Texture_Release(int index)
{
	if (m_pTextures[index] != NULL)
	{
		m_pTextures[index]->Release();
		m_pTextures[index] = NULL;
	}
}













