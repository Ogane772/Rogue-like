#ifndef EFFECT_H_
#define EFFECT_H_

#include "d3dx9.h"
#include "C2DObj.h"
#define CUT_TX (128)				// �؂��蕝
#define CUT_TY (128)				// �؂��荂��
#define EFFECT_MAX (10)			// �����ő吔
#define SHADOW_MAX (10)			// �e�ő吔

class CEffect : CTexture
{
public:
	typedef struct {
		D3DXVECTOR3 pos;

		int Age;

		int EffectCreateFrame;		// �a���t���[��
		int EffectPatternFrame;		// ���ݕ\�����ׂ��t���[��

		bool alive;
	}EFFECT;


	static void Effect_Initialize(void);
	static void Effect_Finalize(void);
	static void Effect_Update(void);
	static void Effect_Draw(void);

	// �e���ˊ֐�		���ˍ��W
	static void Effect_Create(D3DXVECTOR3 pos);
};
#endif

