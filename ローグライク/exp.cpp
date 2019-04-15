/*
#include <d3dx9.h>
#include "CTexture.h"
#include "common.h"
#include "billboard.h"
#include "exp.h"
#define MAX_EFFECT (99)

typedef struct {
	D3DXVECTOR3 exp_position;
	float puttern;
	float size;//ビルボードの大きさ
	float rotation;//回転角度
	int anime_texture;//画像番号格納
	int effect_counter;//アニメ経過時間
	int anime_pattan;//アニメパターン数
	int anime_time;//アニメを何フレームごとに再生するか
	int anime_tx;//1アニメの横幅
	int anime_ty;//1アニメの縦幅
	int width_patan;//横が何パターンあるか
	bool eflg;
}EFFECT;

EFFECT g_effect[MAX_EFFECT];

void Exp_Init(void)
{
	int i;
	for (i = 0; i < MAX_EFFECT; i++)
	{
		g_effect[i].exp_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_effect[i].puttern = 0;
		g_effect[i].anime_texture = 0;
		g_effect[i].effect_counter = 0;
		g_effect[i].anime_pattan = 0;
		g_effect[i].anime_time = 0;
		g_effect[i].eflg = false;
	}
}

void Exp_Update(void)
{
	int i;
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_effect[i].eflg == true)
		{
			//アニメの終わりまで行ったら消去
			if (g_effect[i].effect_counter >= g_effect[i].anime_time * g_effect[i].anime_pattan)
			{
				g_effect[i].eflg = false;
				g_effect[i].effect_counter = 0;
			}
		}
	}
}

void Exp_Draw(void)
{
	int i;
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_effect[i].eflg == true)
		{
			BillBoard_Exp_Draw(g_effect[i].exp_position.x, g_effect[i].exp_position.y, g_effect[i].exp_position.z - 1.0f, g_effect[i].anime_texture, g_effect[i].anime_pattan, g_effect[i].size, g_effect[i].rotation, g_effect[i].anime_tx, g_effect[i].anime_ty, g_effect[i].width_patan, g_effect[i].effect_counter, g_effect[i].anime_time);
			g_effect[i].effect_counter++;
		}
	}
}

void Exp_Set(int exp_number, float x, float y, float z, float size, float rotation)
{
	switch (exp_number)
	{
	case ENEMY_HIKKAI:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_HIT2, 10, 2, 1200 / 5, 480 / 2, 5);
		break;
	case STAR:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_STAR, 15, 2, 1200 / 5, 720 / 3, 5);
		break;
	case SHINE:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_SHINE, 10, 4, 2400 / 10, 240, 10);
		break;
	case SMALLSTAR:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_HEAL_17, 10, 4, 960 / 5, 384 / 2, 5);
		break;
	case HIT:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_HIT, 8, 4, 4000 / 4, 2000 / 2, 4);
		break;
	case IMPACT_13:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_IMPACT_13, 10, 4, 960 / 5, 384 / 2, 5);
		break;
	case SURASYU:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_EFFECT_P020, 8, 4, 1600 / 8, 200 / 1, 8);
		break;
	case HEAL:
		Exp_Create(x, y, z, size, rotation, CTexture::TEX_EFFECT_HEAL, 10, 4, 960 / 5, 384 / 2, 5);
		break;
	}
}

void Exp_Create(float x, float y, float z, float size, float rotation, int texture_index, int anime_pattan, int anime_time, int one_width, int one_height, int width_patan)
{
	int i;
	for (i = 0; i < MAX_EFFECT; i++)
	{
		if (g_effect[i].eflg == false)
		{
			g_effect[i].exp_position.x = x;
			g_effect[i].exp_position.y = y;
			g_effect[i].exp_position.z = z;
			g_effect[i].size = size;
			g_effect[i].rotation = rotation;
			g_effect[i].effect_counter = 0;
			g_effect[i].anime_texture = texture_index;
			g_effect[i].anime_pattan = anime_pattan;
			g_effect[i].anime_time = anime_time;
			g_effect[i].anime_tx = one_width;
			g_effect[i].anime_ty = one_height;
			g_effect[i].width_patan = width_patan;
			//爆発を有効にする
			g_effect[i].eflg = true;
			return;
		}
	}
}

void Effect_Destroy(int i)
{
	//爆発を無効にする
	g_effect[i].eflg = false;
}

bool Effect_IsEnable(int i)
{
	return g_effect[i].eflg;
}*/