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
�O���[�o���ϐ�
======================================================================*/
static bool g_bIsFade;
static int tFlag = 0;
C2DObj *pTitleMenu;
static int cursor = 0;
static int save_delete_cursor = 0;
static bool save_delete = false;
static bool save_delete_on = false;//�Z�[�u�f�[�^������true

enum menu_command
{
	GAME_START,
	GAME_GALLERY,
	SAVE_DELETE,
	GAME_END,
};
void TitleMenu_Initialize(void)
{
	pTitleMenu = new C2DObj;
	g_bIsFade = false;
	tFlag++;
	Fade_Start(false, 10, 0, 0, 0);
	cursor = 0;
	save_delete = false;
	save_delete_cursor = 0;
	save_delete_on = false;
}

void TitleMenu_Finalize(void)
{
	delete pTitleMenu;
}

void TitleMenu_Update(void)
{
	 
	if (!g_bIsFade)
	{
		if (Keyboard_IsTrigger(DIK_RETURN))
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
			else if(save_delete && !save_delete_on)
			{
				if (save_delete_cursor == 0)
				{
					PlaySound(CURSOR_OK_SE);
					save_delete_on = true;
					//�����ŃZ�[�u�f�[�^����
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
		if (Keyboard_IsTrigger(DIK_W))
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
		if (Keyboard_IsTrigger(DIK_S))
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
	}
	else
	{
		// �t�F�[�h���I�������
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
				// �K�w�̏�����
				CStage::Stage_SetLevel(0);
				break;
			case GAME_GALLERY:
				//�Z�[�u�f�[�^�̃��[�h
				Load();
				Scene_Change(SCENE_INDEX_GALLERY);
				break;
			case SAVE_DELETE:
				//�����ɗ���̂͂��肦�Ȃ��̂ł���������Q�[���I��
				exit(1);
				break;
			case GAME_END:
				exit(1);
				break;
			}
		}
	}
}

void TitleMenu_Draw(void)
{
	pTitleMenu->m_Sprite_Draw(CTexture::TEX_BLACK, 0, 0, 0, 0, pTitleMenu->Texture_GetWidth(CTexture::TEX_BLACK), pTitleMenu->Texture_GetHeight(CTexture::TEX_BLACK));
	pTitleMenu->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 50.0f, 50.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 1.7f, 0.72f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(105, 75, D3DCOLOR_RGBA(255, 255, 255, 255), "���ɒ���");
	CUserinterface::UI_TextDraw(105, 125, D3DCOLOR_RGBA(255, 255, 255, 255), "�M�������[");
	CUserinterface::UI_TextDraw(105, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "�f�[�^����");
	CUserinterface::UI_TextDraw(105, 225, D3DCOLOR_RGBA(255, 255, 255, 255), "�Q�[���I��");

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
			CUserinterface::UI_TextDraw(350, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "�Z�|�u�f�[�^���������܂����H");
			CUserinterface::UI_TextDraw(350, 225, D3DCOLOR_RGBA(255, 255, 255, 255), "   �͂�");
			CUserinterface::UI_TextDraw(350, 275, D3DCOLOR_RGBA(255, 255, 255, 255), "   ������");
			pTitleMenu->Sprite_Draw(CTexture::TEX_CURSOL, 350.0f, 225.0f + (save_delete_cursor * 50.0f), 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
		}
		else if (save_delete_on)
		{
			pTitleMenu->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 300.0f, 162.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.7f, 0.23f, D3DXToRadian(0.0f));
			CUserinterface::UI_TextDraw(350, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "�Z�|�u�f�[�^���������܂���");
		}
	}
}