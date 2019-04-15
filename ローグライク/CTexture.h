//////////////////////////////////////////////////
////
////	テクスチャクラス
////
//////////////////////////////////////////////////

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include "CGameObj.h"

#define TEXTURE_MANAGE_MAX   (512)
#define TEXTURE_INVALID_INDEX (-1)

class CTexture : virtual public CGameObj
{
public:
	static int Texture_SetLoadFile(const char* pFileName, int width, int height);

	// テクスチャの読み込み
	// 
	// 戻り値:読み込めなかった数
	// 

	static int Texture_Load(void);

	// テクスチャの取得
	static LPDIRECT3DTEXTURE9 Texture_GetTexture(int index);

	// テクスチャの幅の取得
	static int Texture_GetWidth(int index);

	// テクスチャの高さの取得
	static int Texture_GetHeight(int index);

	// テクスチャの解放
	// 
	//	引数: indices = 解放したいテクスチャ番号
	//					が入った配列の先頭アドレス
	//		  count   = 解放したいテクスチャ番号
	//                  の個数
	//
	static int Texture_Release(int indices[], int count);

	// テクスチャの全解放
	static void Texture_ReleaseAll(void);
private:
	//	テクスチャファイル情報管理用レコード構造体
	typedef struct TextureFile_tag
	{
		char filename[TEXTURE_FILENAME_MAX];
		int width;
		int height;
	}TextureFile;

	static TextureFile g_Files[TEXTURE_MANAGE_MAX];

	static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_MANAGE_MAX];
};
#endif // !1
