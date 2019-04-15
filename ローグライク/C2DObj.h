//////////////////////////////////////////////////
////
////	2Dオブジェクトクラス
////
//////////////////////////////////////////////////

#ifndef _2DOBJ_H_
#define _2DOBJ_H_

#include "CTexture.h"
#include <d3dx9.h>

class C2DObj : public CTexture
{
public:

	C2DObj();
	~C2DObj();

	void Update(void);						//	更新
	void Draw(void);						//	描画
	void Finalize(void);					//	終了処理
	static void Sprite_Initialize(void);	//	スプライト初期化
	static void Sprite_Finalize(void);		//	スプライト破棄

	void m_Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th);	//	スプライト描画
	static void Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th);	//	スプライト描画タイトルリザルト用
	void Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, float cx, float cy, float sx, float sy, float rotation);
	static void SpriteDraw(int texture_index, float dx, float dy);
	/*	
		texture_index:テクスチャ番号
		dx:表示したいスプライトの左上座標X
		dy:表示したいスプライトの左上座標Y
		tx:表示したいテクスチャの始点X
		ty:表示したいテクスチャの始点Y
		tw:表示したいテクスチャの幅
		th:表示したいテクスチャの高さ
	*/
protected:

	//2Dポリゴン頂点　構造体
	typedef struct Vertex2D_tag
	{
		D3DXVECTOR4 pos;	//	flootにして4つ目1.0f(RHW)　座標変換済み頂点
		D3DCOLOR color;		//  色
		D3DXVECTOR2 texcoord;	//　テクスチャ
	}Vertex2D;

	#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//	構造体のすぐ下に書く
    //#define FVF_VERTEX2DUI (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//	構造体のすぐ下に書く
	static LPDIRECT3DVERTEXBUFFER9 m_p2DVertexBuffer;		//	頂点バッファ
	static LPDIRECT3DINDEXBUFFER9 m_p2DIndexBuffer;		//	インデックスバッファ

};

#endif // !1

