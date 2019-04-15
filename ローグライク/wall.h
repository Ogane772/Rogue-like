#ifndef WALL_H_
#define WALL_H_
#include <d3dx9.h>
#include "collision.h"
#include "C2DObj.h"
//===================================================
// 定数
//===================================================
#define WALL_NOTCOLIISION (-100.0f)	// 当たり判定じゃない配列
#define WALL_MAX (250)		// メッシュ作成可能数
#define WALL_WIDTH_MAX (200)

//===================================================
// メッシュウォール構造体
//===================================================
class CWall : public C2DObj
{
public:
	// 3D用頂点構造体
	typedef struct
	{
		D3DXVECTOR3 position;		// 座標
		D3DXVECTOR3 normal;			// 法線
		D3DCOLOR color;				// カラー

		D3DXVECTOR2 texcoord;
	}Vertex3D;
	typedef struct
	{
		D3DXVECTOR3 pos;					// 座標
		D3DXVECTOR3 nor;					// 法線
		int tex;							// テクスチャー
		bool wuse;							// 使用確認
		LPDIRECT3DVERTEXBUFFER9 pVBuffer;	// バーテックスバッファ
		LPDIRECT3DINDEXBUFFER9 pIBuffer;	// インデックスバッファ
		int widthmax;						// 横の長さ 
		int heightmax;						// 縦の長さ
		int widthcube;						// 横の四画数
		int heightcube;						// 縦の四角数

		int type;							// 方向タイプ
		Vertex col[WALL_WIDTH_MAX];			// 当たり判定
	}WALL;

	static void MeshWall_Init(void);
	// テクスチャ番号、横の長さ、縦の長さ、横四角数、縦四角数  ,ポジション		法線				
	static void MeshWall_Create(int tex, int width, int height, int wcube, int hcube, D3DXVECTOR3 pos, D3DXVECTOR3 normal);
	static void MeshWall_Finalize(void);
	static void MeshWall_Draw(void);

	static WALL Wall_GetData(int i) { return g_wall[i]; }

	static const Vertex* Wall_GetCollision(int i, int c) { return &g_wall[i].col[c];}
	static WALL g_wall[WALL_MAX];
};
#endif
