#ifndef EFFECT_H_
#define EFFECT_H_

#include "d3dx9.h"
#include "C2DObj.h"
#define CUT_TX (128)				// 切り取り幅
#define CUT_TY (128)				// 切り取り高さ
#define EFFECT_MAX (10)			// 爆発最大数
#define SHADOW_MAX (10)			// 影最大数

class CEffect : CTexture
{
public:
	typedef struct {
		D3DXVECTOR3 pos;

		int Age;

		int EffectCreateFrame;		// 誕生フレーム
		int EffectPatternFrame;		// 現在表示すべきフレーム

		bool alive;
	}EFFECT;


	static void Effect_Initialize(void);
	static void Effect_Finalize(void);
	static void Effect_Update(void);
	static void Effect_Draw(void);

	// 弾発射関数		発射座標
	static void Effect_Create(D3DXVECTOR3 pos);
};
#endif

