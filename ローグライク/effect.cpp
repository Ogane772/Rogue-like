/*======================================================================

シューティングゲーム [explosion.cpp]
Author : Yanagida yohta
Date   : 2018/ 6 / 29作成
------------------------------------------------------------------------*/

#include "common.h"
#include "CTexture.h"
#include "CPlayer.h"
#include <d3dx9.h>
#include "effect.h"
#include "sound.h"
#include "bilboard.h"

#define	EFFECT_WIDTH			(32)
#define EFFECT_HEIGHT			(32)
#define EFFECT_PATTERN_FRAME	(4)// 待ちフレーム
#define EFFECT_PATTERN_MAX		(4)
#define EFFECT_PATTERN_W_MAX	(4)

static CEffect::EFFECT g_effect[EFFECT_MAX];

static D3DXVECTOR3 shadow;
/*======================================================================
グローバル変数
======================================================================*/
//static bool g_Enable[EXPLOSION_MAX];

//static float g_ExplosionX[EXPLOSION_MAX];
//static float g_ExplosionY[EXPLOSION_MAX];

//static int g_Age[EXPLOSION_MAX];

// 誕生フレーム
static int g_EffectCreateFrame[EFFECT_MAX];
// 現在表示すべきフレーム
static int g_EffectPatternFrame[EFFECT_MAX];
static int g_EffectFramecount = 0;

//static int g_ExplosionTexture = TEXTURE_INVALID_INDEX;

static int g_EffectTexture1;
static int g_EffectTexture2;
static int g_EffectTexture3;
static int g_EffectTexture4;

static int g_ShadowTexture;

void CEffect::Effect_Initialize(void)
{
	// テクスチャの読み込み予約をしておく
	//g_ExplosionTexture = Texture_SetLoadFile("asset/texture/explosion000.png", 8, 1);

	//g_EffectTexture1 = Texture_SetLoadFile("asset/texture/effect_hit1.png", 5, 5);
	//g_EffectTexture2 = Texture_SetLoadFile("asset/texture/effect_hit2.png", 5, 5);
	//g_EffectTexture3 = Texture_SetLoadFile("asset/texture/effect_hit3.png", 5, 5);
	//g_EffectTexture4 = Texture_SetLoadFile("asset/texture/effect_hit4.png", 5, 5);

	for (int i = 0; i < EFFECT_MAX; i++)
	{
		g_effect[i].Age = 0;
		g_effect[i].alive = false;
		g_effect[i].EffectPatternFrame = 0;
		g_effect[i].EffectCreateFrame = 0;
	}

	for (int i = 0; i < SHADOW_MAX; i++)
	{
		g_effect[i].alive = false;
	}

}

void CEffect::Effect_Finalize(void)
{
	// テクスチャの解放
	//Texture_Release(&g_EffectTexture1, 1);
	//Texture_Release(&g_EffectTexture2, 1);
	//Texture_Release(&g_EffectTexture3, 1);
	//Texture_Release(&g_EffectTexture4, 1);
	//Texture_Release(&g_ShadowTexture, 1);

}

void CEffect::Effect_Update(void)
{

	g_EffectFramecount++;

	for (int i = 0; i < EFFECT_MAX; i++)
	{

		// 有効だったら現在のフレーム数から誕生フレームを引く
		if (g_effect[i].alive)
		{
			// 年齢
			g_effect[i].Age = g_EffectFramecount - g_effect[i].EffectCreateFrame;

			g_effect[i].EffectPatternFrame = g_effect[i].Age / EFFECT_PATTERN_FRAME;

			if (g_effect[i].EffectPatternFrame >= EFFECT_PATTERN_MAX)
			{
				g_effect[i].alive = false;
				g_effect[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				g_effect[i].EffectCreateFrame = 0;
				g_effect[i].EffectPatternFrame = 0;
				g_effect[i].Age = 0;
			}
		}

	}

}

void CEffect::Effect_Draw(void)
{

	for (int i = 0; i < EFFECT_MAX; i++)
	{

		// ワールド座標変換行列の作成と設定
		D3DXMATRIX mtxWorld;
		D3DXMATRIX mtxRotation;
		D3DXMATRIX mtxTranslotation;

		if (g_effect[i].alive)
		{
			D3DXMatrixIdentity(&mtxWorld);

			D3DXMatrixTranslation(&mtxTranslotation, g_effect[i].pos.x, g_effect[i].pos.y, g_effect[i].pos.z);
			switch (g_effect[i].EffectPatternFrame)
			{
			case 0:
				CBilboard::BilBoard_ChangeSizeColor(3.0f, 3.0f, D3DCOLOR_RGBA(255, 255, 255, 255));
				CBilboard::BilBoard3_Draw(g_EffectTexture1, g_effect[i].pos);
				break;
			case 1:
				CBilboard::BilBoard_ChangeSizeColor(3.0f, 3.0f, D3DCOLOR_RGBA(255, 255, 255, 255));
				CBilboard::BilBoard3_Draw(g_EffectTexture2, g_effect[i].pos);
				break;
			case 2:
				CBilboard::BilBoard_ChangeSizeColor(3.0f, 3.0f, D3DCOLOR_RGBA(255, 255, 255, 255));
				CBilboard::BilBoard3_Draw(g_EffectTexture3, g_effect[i].pos);
				break;
			case 3:
				CBilboard::BilBoard_ChangeSizeColor(3.0f, 3.0f, D3DCOLOR_RGBA(255, 255, 255, 255));
				CBilboard::BilBoard3_Draw(g_EffectTexture4, g_effect[i].pos);
				break;
			}
		}
	}
}


// 弾発射関数		発射座標X, Y
void CEffect::Effect_Create(D3DXVECTOR3 pos)
{

	for (int i = 0; i < EFFECT_MAX; i++)
	{

		if (!g_effect[i].alive)
		{
			g_effect[i].alive = true;

			g_effect[i].pos = pos;
			g_effect[i].pos.x -= 1.5f;
			g_effect[i].pos.y += 1.5f;
			g_effect[i].pos.z -= 2.5f;
			//g_effect[i].pos.y -= 1.0f;

			// 誕生日
			g_effect[i].EffectCreateFrame = g_EffectFramecount;
			g_effect[i].EffectPatternFrame = 0;

			break;

		}
	}

}
