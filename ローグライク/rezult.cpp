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
//static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;
C2DObj *pResult;

void Rezult_Initialize(void)
{

	//g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/rezult.png", WINDOW_WIDTH, WINDOW_HEIGHT);

	//Camera_Initialize();
	CBilboard::BilBoard_Initialize();

	if (CTexture::Texture_Load() > 0)
	{
		return;
	}
	pResult = new C2DObj;
	Fade_Start(false, 90, 0, 0, 0);

}
void Rezult_Finalize(void)
{
	delete pResult;;
}
void Rezult_Draw(void)
{
	CBilboard::BilBoard_ChangeSizeColor(12.8f, 7.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
	pResult->m_Sprite_Draw(CTexture::TEX_RESULT, 0, 0, 0, 0, pResult->Texture_GetWidth(CTexture::TEX_RESULT), pResult->Texture_GetHeight(CTexture::TEX_RESULT));
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