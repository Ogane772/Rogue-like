//フロア切り替え画面
#include "scene.h"
#include "input.h"
#include "CTexture.h"
#include "common.h"
#include "fade.h"
#include "bilboard.h"
#include "CCamera.h"
#include "stage.h"
/*======================================================================
グローバル変数
======================================================================*/
//int CStage::g_TextureIndex;
int CStage::displayframe;
int CStage::Level;

void CStage::Stage_Initialize(void)
{
	g_bIsFade = false;
	//g_TextureIndex = TEXTURE_INVALID_INDEX;;
	displayframe = 0;
	Level++;

	
	Fade_Start(false, 10, 0, 0, 0);

}

void CStage::Stage_Finalize(void)
{
	//Texture_ReleaseAll();
}

void CStage::Stage_Draw(void)
{
	switch (Level)
	{
	case 1:
		C2DObj::Sprite_Draw(CTexture::TEX_1F, 0.0f, 0.0f, 0, 0, CTexture::Texture_GetWidth(CTexture::TEX_1F), CTexture::Texture_GetHeight(CTexture::TEX_1F));
		break;
	case 2:
		C2DObj::Sprite_Draw(CTexture::TEX_2F, 0.0f, 0.0f, 0, 0, CTexture::Texture_GetWidth(CTexture::TEX_2F), CTexture::Texture_GetHeight(CTexture::TEX_2F));
		break;
	case 3:
		C2DObj::Sprite_Draw(CTexture::TEX_3F, 0.0f, 0.0f, 0, 0, CTexture::Texture_GetWidth(CTexture::TEX_3F), CTexture::Texture_GetHeight(CTexture::TEX_3F));
		break;
	}
}

void CStage::Stage_Update(void)
{
	//Camera_TitleUpdate();
	displayframe++;


	if (!g_bIsFade)
	{
		// フレームで動かす
		if (displayframe > FRAME_MAX)
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
			Scene_Change(SCENE_INDEX_GAME);
			//Scene_Check();
		}
	}
}