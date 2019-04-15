#ifndef BILBOARD_H_
#define BILBOARD_H_
#include <d3dx9.h>
#include "C2DObj.h"
#define FVF_VERTEX3D (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
class CBilboard : virtual public C2DObj
{
public:
	static void Shadow_Init(void);
	static void BilBoard_Initialize(void);
	static void BilBoard_Finalize(void);
	static void BilBoard3_Draw(const int TexIndex, D3DXVECTOR3 pos);
	static void BilBoard_Update(void);

	static void BilBoard_ChangeSizeColor(float sizeX, float sizeY, D3DCOLOR color);
	static void BilBoard_ChangeMapPos(float sizeX, float sizeY, D3DCOLOR color);

	static void Shadow_Draw(D3DXMATRIX mtxS, D3DXVECTOR3 pos);
	static void Shadow_Finalize(void);
	//エフェクト用ビルボード
	static void BillBoard_Exp_Draw(float x, float y, float z, int texture_index, int anime_pattan, float size, float rotation, int tx, int ty, int width_patan, int counter, int animetime);
};
//void BilBoard_NextDraw(void);

#endif