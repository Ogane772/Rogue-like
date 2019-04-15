#ifndef MESHFIELD_H_
#define MESHFIELD_H_
#include <d3dx9.h>
#define MESH_MAX (20)		// メッシュ作成可能数
#include "C2DObj.h"

class CMeshField : public C2DObj
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

	// メッシュ構造体
	typedef struct
	{
		D3DXVECTOR3 pos;					// 座標
		D3DXVECTOR3 nor;					// 法線
		int tex;							// テクスチャー
		bool muse;							// 使用確認
		LPDIRECT3DVERTEXBUFFER9 pVBuffer;	// バーテックスバッファ
		LPDIRECT3DINDEXBUFFER9 pIBuffer;	// インデックスバッファ
		int widthmax;						// 横の長さ 
		int heightmax;						// 縦の長さ
		int widthcube;						// 横の四画数
		int heightcube;						// 縦の四角数
	}MESH;

	static void MeshField_Init(void);
	// テクスチャ番号、横の長さ、縦の長さ、横四角数、縦四角数  ,ポジション
	static void MeshField_Create(int tex, int width, int height, int wcube, int hcube, D3DXVECTOR3 pos);
	static void MeshField_Finalize(void);
	static void MeshField_Draw(void);
private:
	static MESH g_mesh[MESH_MAX];
	static D3DXVECTOR3 Rpos;

	static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer;

	static int gTexture;
};

#endif
