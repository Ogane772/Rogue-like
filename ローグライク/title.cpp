#include "scene.h"
#include "input.h"
#include "fade.h"
#include "input.h"
#include "common.h"
#include "CTexture.h"
#include "bilboard.h"
#include "CCamera.h"
#include "stage.h"
/*======================================================================
グローバル変数
======================================================================*/
static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;
static int tFlag = 0;
void Title_Initialize(void)
{

	g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/title.png", WINDOW_WIDTH, WINDOW_HEIGHT);

	CBilboard::BilBoard_Initialize();

	if (CTexture::Texture_Load() > 0)
	{
		return;
	}

	if (tFlag > 0)
	{
		Fade_Start(false, 90, 0, 0, 0);
	}

	g_bIsFade = false;
	tFlag++;
}
void Title_Finalize(void)
{
	//Texture_ReleaseAll();
}
void Title_Draw(void)
{
	//Sprite_Draw(g_TextureIndex, 0, 0);
	CBilboard::BilBoard_ChangeSizeColor(12.8f, 7.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
	//CBilboard::BilBoard3_Draw(g_TextureIndex, D3DXVECTOR3(Camera_GetData().x - 6.5f, Camera_GetData().y - 6.6f, Camera_GetData().z - 1.0f));
	//BilBoard3_Draw(g_TextureIndex, D3DXVECTOR3(-6.4f, -3.6f, 0.0f));
}
void Title_Update(void)
{
	CBilboard::BilBoard_Update();


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
			Scene_Change(SCENE_INDEX_STAGE);

			//Scene_Check();
			// 階層の初期化
			CStage::Stage_SetLevel(0);
		}
	}
}