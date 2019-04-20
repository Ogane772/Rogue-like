#ifndef MAP_H_
#define MAP_H_
#include <d3dx9.h>
#include "CTexture.h"
//===================================
// 定数
//===================================
#define MAX_MAPWIDTH (100)
#define MAX_MAPHEIGHT (100)
//#define GroupWidth  (3)		// 横の短径グループ数
//#define GroupHeight (2)		// 縦の短径グループ数


//===================================
// 列挙
//===================================
class CMap : public CTexture
{
public:
	// 足場に乗ってる種類
	typedef enum {
		NOTHAVE,
		HAVEPLAYER,
		HAVEENEMY,
		HAVEITEM,
		HAVEMAPNONE
	}HAVEMAP;

	// やること マップを作る プレイヤーの初期位置を決める 階段(目的地)の座標を決める
	//===================================
	// マップ構造体
	//===================================
	typedef struct
	{
		D3DXVECTOR3 pos;	// 使うかも
		bool use;	// 足場になっているか 0 = 足場無し ,1 = フロア, 2 = 通路, 3 = フロアの通路出入口
		int type;	// 足場の種類
		HAVEMAP have;	// 足場に何かあるか(いるか)
		bool Czwall;	// 壁が作られたか
		bool Cxwall;	// 壁が作られたか
		int Group;		// 短径グループ
		int alpha; // ミニマップ用のアルファ値操作用
		bool Cxtopwall;	// 通路上壁が作られたか
		bool Cxbotwall;	// 通路下壁が作られたか
		bool CzRwall;	// 通路右壁が作られたか
		bool CzLwall;	// 通路左壁が作られたか

	}MAP;

	static void Map_Initialize(void);
	// テクスチャ番号、横の長さ、縦の長さ、横四角数、縦四角数  ,ポジション
	static void Map_Create(void);
	static void Map_Finalize(void);
	static void Map_Draw(void);

	static MAP Map_GetData(int z, int x) { return g_map[z][x]; }
	static void Map_SetData(int z, int x,int alpha) { g_map[z][x].alpha = alpha; }
private:
	//static int g_TexWood2;
	static MAP **g_map, *base_g_map;

	static int GroupWidth;
	static int GroupHeight;
};

#endif
