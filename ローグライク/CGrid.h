#ifndef GRID_H_
#define GRID_H_
#include <d3dx9.h>
#include "CGameObj.h"

#define GRID_MAX (12)

class CGrid : public CGameObj
{
public:
	

	// 3D用頂点構造体
	typedef struct
	{
		D3DXVECTOR3 position;		// 座標
		D3DXVECTOR3 normal;			// 法線
		D3DCOLOR color;				// カラー
	}Vertex3d;

	

	static void Init();
	// テクスチャ番号、横の長さ、縦の長さ、横四角数、縦四角数  ,ポジション
	static void Finalize();
	static void Update(int z, int x);
	static void Draw();
	static void Grid_Draw();
private:
	static Vertex3d gLine[GRID_MAX + GRID_MAX];

	};

#endif
