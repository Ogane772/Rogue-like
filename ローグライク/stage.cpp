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
	switch (Level)
	{
	case 1:
		//g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/1F.png", WINDOW_WIDTH, WINDOW_HEIGHT);
		break;

	case 2:
		//g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/2F.png", WINDOW_WIDTH, WINDOW_HEIGHT);
		break;

	case 3:
		//g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/3F.png", WINDOW_WIDTH, WINDOW_HEIGHT);
		break;
	}
	
	Fade_Start(false, 10, 0, 0, 0);

}

void CStage::Stage_Finalize(void)
{
	//Texture_ReleaseAll();
}

void CStage::Stage_Draw(void)
{
	CBilboard::BilBoard_ChangeSizeColor(12.8f, 7.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
	C2DObj::Sprite_Draw(CTexture::TEX_1F,0.0f, 0.0f, 0, 0, CTexture::Texture_GetWidth(CTexture::TEX_1F), CTexture::Texture_GetHeight(CTexture::TEX_1F));
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