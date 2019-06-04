//////////////////////////////////////////////////
////
////	テクスチャクラス
////
//////////////////////////////////////////////////

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include "CGameObj.h"

#define TEXTURE_FILENAME_MAX (64)

class CTexture : virtual public CGameObj
{
public:
	CTexture();
	~CTexture();

	enum
	{
		TEX_TITLE,
		TEX_BLUE,
		TEX_WOOD,
		TEX_KUSA,
		TEX_BLOCK,
		TEX_LADDER,
		TEX_1F,
		TEX_2F,
		TEX_3F,
		TEX_4F,
		TEX_5F,
		TEX_SHADOW,
		TEX_HIT,
		TEX_HIT_WEEK,
		TEX_HIT_RESIST,
		TEX_RESULT,
		TEX_MESSAGE_WINDOW,
		TEX_PLAYER_ICON,
		TEX_ENEMY_ICON,
		TEX_LADDER_ICON,
		TEX_ITEM_ICON,
		TEX_HPGAGE_WAKU,
		TEX_HPGAGE_GREENBAR,
		TEX_HPGAGE_REDBAR,
		TEX_CURSOL,
		TEX_CURSOL_GRY,
		TEX_SWORD_ICON,
		TEX_BIGSWORD_ICON,
		TEX_REIPIA_ICON,
		TEX_RANCE_ICON,
		TEX_MEISU_ICON,
		TEX_AX_ICON,
		TEX_SHELD_ICON,
		TEX_RING_ICON,
		TEX_WIDTH_WINDOW,
		TEX_ITEM_YAKUSOU,
		TEX_WINDOW_MARU,
		TEX_MAX,
	};


	static int Texture_Load(void);	//	テクスチャ読み込み
	static void Texture_Release(void);	//	テクスチャ破棄
	static void Texture_Load(int index);	//	指定したテクスチャ読み込み
	static void Texture_Release(int index);	//	指定したテクスチャ破棄
	static LPDIRECT3DTEXTURE9 Texture_GetTexture(int index) { return m_pTextures[index]; }	//	テクスチャ取得
	static char* Texture_GetFailName(int index) { return TEXTURE_FILES[index].filename; }
	static int Texture_GetWidth(int index) { return TEXTURE_FILES[index].width; }	// テクスチャ幅
	static int Texture_GetHeight(int index) { return TEXTURE_FILES[index].height; }	//	テクスチャ高さ
protected:




private:
	//	テクスチャデータの構造体
	typedef struct TextureFile_tag
	{
		char filename[TEXTURE_FILENAME_MAX];	//	ファイル名
		int width;	//	幅
		int height;	//	高さ
	}TextureFileData;
	static TextureFileData TEXTURE_FILES[];	//	テクスチャ構造体宣言
	static int TEXTURE_MAX;	//	テクスチャ構造体総数
	static LPDIRECT3DTEXTURE9 m_pTextures[];	//	テクスチャポインタ
};
#endif // !1
