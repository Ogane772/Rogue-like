#include "scene.h"
#include "input.h"
#include "CTexture.h"
#include "common.h"
#include "fade.h"
#include "bilboard.h"
#include "CCamera.h"
#include "C3DObj.h"
#include "rezult.h"
#include "gamepad.h"
/*======================================================================
グローバル変数
======================================================================*/
//static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;
C2DObj *pResult;

void CRezult::Rezult_Initialize(void)
{
	C3DObj::DeleteAll();			//	3Dオブジェクト全消去
	CGameObj::DeleteAll2D();			//	2Dオブジェクト全消去

	//g_TextureIndex = CTexture::Texture_SetLoadFile("asset/texture/rezult.png", WINDOW_WIDTH, WINDOW_HEIGHT);

	//Camera_Initialize();
	CBilboard::BilBoard_Initialize();

	if (CTexture::Texture_Load() > 0)
	{
		return;
	}
	pResult = new C2DObj;
	Fade_Start(false, 90, 0, 0, 0);
	GamePadInit();

}

void CRezult::Rezult_Finalize(void)
{
	delete pResult;;
}

void CRezult::Rezult_Draw(void)
{
	pResult->m_Sprite_Draw(CTexture::TEX_RESULT, 0, 0, 0, 0, pResult->Texture_GetWidth(CTexture::TEX_RESULT), pResult->Texture_GetHeight(CTexture::TEX_RESULT));
}

void CRezult::Rezult_Update(void)
{
	//XBOXコントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)))
	{
		trigger = false;
	}
	if (!g_bIsFade)
	{
		if (Keyboard_IsTrigger(DIK_SPACE) || Keyboard_IsTrigger(DIK_RETURN) || JoyDevice_IsPress(CONTROLLER::A_BUTTON) && !trigger)
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