#include "scene.h"
#include "fade.h"
#include "input.h"
#include "CTexture.h"
#include "bilboard.h"

/*======================================================================
グローバル変数
======================================================================*/
//static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;
static int tFlag = 0;
C2DObj *pTitle;

void Title_Initialize(void)
{

	//g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/title.png", WINDOW_WIDTH, WINDOW_HEIGHT);

	if (tFlag > 0)
	{
		Fade_Start(false, 90, 0, 0, 0);
	}
	pTitle = new C2DObj;
	g_bIsFade = false;
	tFlag++;
}

void Title_Finalize(void)
{
	delete pTitle;
}

void Title_Update(void)
{
	if (!g_bIsFade)
	{
		if (Keyboard_IsTrigger(DIK_SPACE) || Keyboard_IsTrigger(DIK_RETURN))
		{
			
			Fade_Start(true, 90, 0, 0, 0);
			g_bIsFade = true;

		}
	}
	else
	{
		// フェードが終わったら
		if (!Fade_IsFade())  
		{
			g_bIsFade = false;
			Scene_Change(SCENE_INDEX_TITLEMENU);
		}
	}
}

void Title_Draw(void)
{
	//Sprite_Draw(g_TextureIndex, 0, 0);
	pTitle->m_Sprite_Draw(CTexture::TEX_TITLE, 0, 0, 0, 0, pTitle->Texture_GetWidth(CTexture::TEX_TITLE), pTitle->Texture_GetHeight(CTexture::TEX_TITLE));
	//CBilboard::BilBoard3_Draw(g_TextureIndex, D3DXVECTOR3(Camera_GetData().x - 6.5f, Camera_GetData().y - 6.6f, Camera_GetData().z - 1.0f));
	//BilBoard3_Draw(g_TextureIndex, D3DXVECTOR3(-6.4f, -3.6f, 0.0f));
}