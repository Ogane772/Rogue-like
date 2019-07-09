#include "scene.h"
#include "fade.h"
#include "input.h"
#include "CTexture.h"
#include "bilboard.h"
#include "title.h"
#include "debug_font.h"
/*======================================================================
グローバル変数
======================================================================*/
bool CTitle::g_bIsFade;
int CTitle::tFlag;
C2DObj *CTitle::pTitle;

void CTitle::Title_Initialize(void)
{
	tFlag = 0;
	if (tFlag > 0)
	{
		Fade_Start(false, 90, 0, 0, 0);
	}
	pTitle = new C2DObj;
	g_bIsFade = false;
	tFlag++;
	GamePadInit();
}

void CTitle::Title_Finalize(void)
{
	delete pTitle;
}

void CTitle::Title_Update(void)
{
	//XBOXコントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}

	if (!g_bIsFade)
	{
		if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::B_BUTTON)))
		{
			trigger = false;
		}
		if (Keyboard_IsTrigger(DIK_SPACE) || Keyboard_IsTrigger(DIK_RETURN) || JoyDevice_IsTrigger(CONTROLLER::A_BUTTON) && !trigger || JoyDevice_IsTrigger(CONTROLLER::B_BUTTON) && !trigger)
		{
			Fade_Start(true, 90, 0, 0, 0);
			g_bIsFade = true;
			trigger = true;
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

void CTitle::Title_Draw(void)
{
	pTitle->m_Sprite_Draw(CTexture::TEX_TITLE, 0, 0, 0, 0, pTitle->Texture_GetWidth(CTexture::TEX_TITLE), pTitle->Texture_GetHeight(CTexture::TEX_TITLE));
	//DebugFont_Draw(0, 350, "js.rgdwPOV[0]= %d", js.rgdwPOV[0]);
}