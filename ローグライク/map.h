#ifndef MAP_H_
#define MAP_H_
#include <d3dx9.h>
#include "CTexture.h"
#include "C3DObj.h"
//===================================
// 定数
//===================================
#define MAX_MAPWIDTH (100)
#define MAX_MAPHEIGHT (100)
#define MAX_MAP (3)	//最上階
#define MAP_FLOOR_CENTER (25)	// フロアを全体の真ん中に持ってくための調整

#define MAP_AXIS_MAX (4)		// 各軸の最大フロア数

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
		HAVELADDER,
		HAVEITEM,
		HAVEWEPON,
		HAVETRAP,
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
		int passageNum;	// 通路番号
		bool passagecheck; // そのフロアに通路があるか

	}MAP;

	static void Map_Initialize(void);
	// テクスチャ番号、横の長さ、縦の長さ、横四角数、縦四角数  ,ポジション
	static void Map_Create(void);
	static void Map_Create_B(void);			// グループ規則関係なく繋がってるグループ同士で通路を結ぶ
	static void Map_CreateOneFloor(void);	// 一部屋デバッグ用
	static void Map_Finalize(void);
	static void Map_Draw(void);
	static void MapPlayerSet(void);//マップにプレイヤー配置処理
	static void WorpPlayerSet(int z, int x);//ワープ時のプレイヤー配置処理
	static MAP Map_GetData(int z, int x) { return g_map[z][x]; }
	static void Map_SetHaveData(int z, int x, HAVEMAP have) { g_map[z][x].have = have; }
	static void Map_SetData(int z, int x, int alpha) { g_map[z][x].alpha = alpha; }
	static void WorpEnemySet(C3DObj *enemy, int z, int x);//ワープエネミー配置処理
	static void MapItemPosSet(int item_type, int x, int z);//指定場所にアイテム配置
	static void MapPlayerPosSet(int mapz, int mapx, int oldz, int oldx);

	static void MapEnemySingleSet(void);
	static void MapEnemyPosSet(int mapz, int mapx, int oldz, int oldx);
private:
	//static int g_TexWood2;
	static void MapWallSet(void);//マップに壁配置処理
	static void MapItemSet(void);//マップにアイテム配置処理
	static void MapWeponSet(void);//マップに装備配置処理
	static void MapTrapSet(void);//マップにワナ配置処理
	static void MapEnemySet(void);//マップにエネミー配置処理
	
	
	static void MapLadderSet(void);//マップに階段配置処理

	static void MapdeletePassage(int passagenum);// 選んだ通路ナンバーを削除

	static MAP **g_map, *base_g_map;

	static int GroupWidth;
	static int GroupHeight;
	static int DeletePassageNum;					// 削除する通路数

	typedef struct
	{
		bool use;
		int number;
	}DELETEPASSAGE;

	static DELETEPASSAGE *g_deletepassage;

};

#endif
