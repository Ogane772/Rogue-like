#include "scene.h"
#include "input.h"
#include "CTexture.h"
#include "common.h"
#include "fade.h"
#include "bilboard.h"
#include "CCamera.h"

/*======================================================================
グローバル変数
======================================================================*/
static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;

void Rezult_Initialize(void)
{

	g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/rezult.png", WINDOW_WIDTH, WINDOW_HEIGHT);

	//Camera_Initialize();
	CBilboard::BilBoard_Initialize();

	if (CTexture::Texture_Load() > 0)
	{
		return;
	}

	Fade_Start(false, 90, 0, 0, 0);

}
void Rezult_Finalize(void)
{
	//Texture_ReleaseAll();
}
void Rezult_Draw(void)
{
	CBilboard::BilBoard_ChangeSizeColor(12.8f, 7.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
	//CBilboard::BilBoard3_Draw(g_TextureIndex, D3DXVECTOR3(Camera_GetData().x - 6.5f, Camera_GetData().y - 6.6f, Camera_GetData().z - 1.0f));
}
void Rezult_Update(void)
{
	CBilboard::BilBoard_Update();
	//Camera_TitleUpdate();

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
			Scene_Change(SCENE_INDEX_TITLE);
			//Scene_Check();
		}
	}
}