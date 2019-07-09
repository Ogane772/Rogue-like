#include "scene.h"
#include "fade.h"
#include "input.h"
#include "CTexture.h"
#include "bilboard.h"
#include "stage.h"
#include "title_menu.h"
#include "userinterface.h"
#include "sound.h"
#include "save_load.h"
/*======================================================================
グローバル変数
======================================================================*/
bool CTitleMenu::g_bIsFade;
C2DObj *CTitleMenu::pTitleMenu;
int CTitleMenu::cursor;
int CTitleMenu::save_delete_cursor;
bool CTitleMenu::save_delete;
bool CTitleMenu::save_delete_on;//セーブデータ消去時true

enum menu_command
{
	GAME_START,
	GAME_GALLERY,
	SAVE_DELETE,
	GAME_END,
};

void CTitleMenu::TitleMenu_Initialize(void)
{
	pTitleMenu = new C2DObj;
	g_bIsFade = false;
	Fade_Start(false, 10, 0, 0, 0);
	cursor = 0;
	save_delete = false;
	save_delete_cursor = 0;
	save_delete_on = false;
	GamePadInit();
}

void CTitleMenu::TitleMenu_Finalize(void)
{
	delete pTitleMenu;
}

void CTitleMenu::TitleMenu_Update(void)
{
	//XBOXコントローラー情報があるときのみ取得
	if (pJoyDevice)
	{
		pJoyDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
	}
	//何もキーが押されてなければボタンを押していい状態にする
	if (!(JoyDevice_IsTrigger(CONTROLLER::A_BUTTON)) && !(JoyDevice_IsTrigger(CONTROLLER::B_BUTTON)) && !(JoyDevice_IsCrossTrigger(UP_BUTTON)) && !(JoyDevice_IsCrossTrigger(DOWN_BUTTON)))
	{
		trigger = false;
	}
	if (!g_bIsFade)
	{
		if (Keyboard_IsTrigger(DIK_RETURN) || JoyDevice_IsTrigger(CONTROLLER::A_BUTTON) && !trigger || JoyDevice_IsTrigger(CONTROLLER::B_BUTTON) && !trigger)
		{
			OK_BUTTON_SELECT();
			trigger = true;
		}
		if (Keyboard_IsTrigger(DIK_W) || JoyDevice_IsCrossTrigger(UP_BUTTON) && !trigger)
		{
			UP_BUTTON_SELECT();
			trigger = true;
		}
		if (Keyboard_IsTrigger(DIK_S) || JoyDevice_IsCrossTrigger(DOWN_BUTTON) && !trigger)
		{
			DOWN_BUTTON_SELECT();
			trigger = true;
		}
	}
	else
	{
		// フェードが終わったら
		if (!Fade_IsFade())
		{
			g_bIsFade = false;
			switch (cursor)
			{
			case GAME_START:
				SAVE_COUNT *save;
				save = Get_SaveCount();
				save->danjyon_in += 1;
				Scene_Change(SCENE_INDEX_STAGE);
				// 階層の初期化
				CStage::Stage_SetLevel(0);
				break;
			case GAME_GALLERY:
				//セーブデータのロード
				Load();
				Scene_Change(SCENE_INDEX_GALLERY);
				break;
			case SAVE_DELETE:
				//ここに来るのはありえないのでもし来たらゲーム終了
				exit(1);
				break;
			case GAME_END:
				exit(1);
				break;
			}
		}
	}
}

void CTitleMenu::TitleMenu_Draw(void)
{
	pTitleMenu->m_Sprite_Draw(CTexture::TEX_BLACK, 0, 0, 0, 0, pTitleMenu->Texture_GetWidth(CTexture::TEX_BLACK), pTitleMenu->Texture_GetHeight(CTexture::TEX_BLACK));
	pTitleMenu->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 50.0f, 50.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 1.7f, 0.72f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(105, 75, D3DCOLOR_RGBA(255, 255, 255, 255), "塔に挑む");
	CUserinterface::UI_TextDraw(105, 125, D3DCOLOR_RGBA(255, 255, 255, 255), "ギャラリー");
	CUserinterface::UI_TextDraw(105, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "データ消去");
	CUserinterface::UI_TextDraw(105, 225, D3DCOLOR_RGBA(255, 255, 255, 255), "ゲーム終了");

	if (!save_delete)
	{
		pTitleMenu->Sprite_Draw(CTexture::TEX_CURSOL, 75.0f, 75.0f + (cursor * 50.0f), 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	else
	{
		pTitleMenu->Sprite_Draw(CTexture::TEX_CURSOL_GRY, 75.0f, 75.0f + (cursor * 50.0f), 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	if (save_delete)
	{
		if (!save_delete_on)
		{
			pTitleMenu->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 300.0f, 162.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.7f, 0.5f, D3DXToRadian(0.0f));
			CUserinterface::UI_TextDraw(350, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "セ－ブデータを消去しますか？");
			CUserinterface::UI_TextDraw(350, 225, D3DCOLOR_RGBA(255, 255, 255, 255), "   はい");
			CUserinterface::UI_TextDraw(350, 275, D3DCOLOR_RGBA(255, 255, 255, 255), "   いいえ");
			pTitleMenu->Sprite_Draw(CTexture::TEX_CURSOL, 350.0f, 225.0f + (save_delete_cursor * 50.0f), 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
		}
		else if (save_delete_on)
		{
			pTitleMenu->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 300.0f, 162.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.7f, 0.23f, D3DXToRadian(0.0f));
			CUserinterface::UI_TextDraw(350, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "セ－ブデータを消去しました");
		}
	}
}

void CTitleMenu::OK_BUTTON_SELECT(void)
{
	if (!save_delete)
	{
		switch (cursor)
		{
		case GAME_START:
		case GAME_GALLERY:
		case GAME_END:
			PlaySound(CURSOR_OK_SE);
			Fade_Start(true, 90, 0, 0, 0);
			g_bIsFade = true;
			break;
		case SAVE_DELETE:
			PlaySound(CURSOR_OK_SE);
			save_delete = true;
			save_delete_cursor = 1;
			break;
		}
	}
	else if (save_delete && !save_delete_on)
	{
		if (save_delete_cursor == 0)
		{
			PlaySound(CURSOR_OK_SE);
			save_delete_on = true;
			//ここでセーブデータ消去
			SaveDelete();
		}
		if (save_delete_cursor == 1)
		{
			PlaySound(CURSOR_OK_SE);
			save_delete = false;
		}
	}
	else if (save_delete && save_delete_on)
	{
		PlaySound(CURSOR_OK_SE);
		save_delete = false;
		save_delete_on = false;
	}
}

void CTitleMenu::UP_BUTTON_SELECT(void)
{
	if (!save_delete)
	{
		PlaySound(CURSOR_SE);
		cursor--;
		if (cursor < GAME_START)
		{
			cursor = GAME_END;
		}
	}
	else if (save_delete && !save_delete_on)
	{
		if (save_delete_cursor > 0)
		{
			PlaySound(CURSOR_SE);
			save_delete_cursor--;
		}
	}
}

void CTitleMenu::DOWN_BUTTON_SELECT(void)
{
	if (!save_delete)
	{
		cursor++;
		PlaySound(CURSOR_SE);
		if (cursor > GAME_END)
		{
			cursor = GAME_START;
		}
	}
	else if (save_delete && !save_delete_on)
	{
		if (save_delete_cursor < 1)
		{
			PlaySound(CURSOR_SE);
			save_delete_cursor++;
		}
	}
}