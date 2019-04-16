#include <d3dx9.h>
#include <stdio.h>
#include "common.h"
#include "CTexture.h"
#include "userinterface.h"
#include "bilboard.h"
#include "Cplayer.h"
#include "CCamera.h"
#include "map.h"

/*----------------------------------------------------------------------
�@�@�萔
 ----------------------------------------------------------------------*/
#define TEXT_MAX (100)

 /*----------------------------------------------------------------------
 �@�@�O���[�o���ϐ�
   ----------------------------------------------------------------------*/
LPD3DXFONT CUserinterface::g_pD3DXFont;

CUserinterface::TEXT CUserinterface::g_text;
D3DXVECTOR3 CUserinterface::MapPos;
D3DXVECTOR3 CUserinterface::DMapPos;
int CUserinterface::g_Textbackground;
int CUserinterface::g_TextFramecount;

void CUserinterface::UI_Initialize(void)
{
	g_pD3DXFont = NULL;
	//g_Textbackground = TEXTURE_INVALID_INDEX;
	g_TextFramecount = 0;
	// �e�N�X�`���̓ǂݍ��ݗ\������Ă���
	//g_Textbackground = CTexture::Texture_SetLoadFile("asset/texture/white.png", 64, 64);

	D3DXCreateFont(m_pD3DDevice, 40, 0, 0, 0, false,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "HGP�n�p�p�߯�ߑ�", &g_pD3DXFont);

	MapPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	DMapPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_text.alive = false;
	g_text.Age = 0;
	g_text.TextCountFrame = 0;
	g_text.TextCreateFrame = 0;
	g_text.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_text.act = ACTTYPENONE;
	g_text.chara = CHARATYPENONE;
	g_text.hitchara = CHARATYPENONE;
	g_text.damage = 0;
}

void CUserinterface::UI_Finalize(void)
{
	SAFE_RELEASE(g_pD3DXFont);

}

void CUserinterface::UI_Update(void)
{
	g_TextFramecount++;

	MapPos = CCamera::Camera_GetData();
	MapPos.x -= 8.0f;
	MapPos.y -= 8.0f;
	MapPos.z += 3.0f;

	DMapPos = CCamera::Camera_GetData();
	DMapPos.x -= 8.0f;
	DMapPos.y -= 8.1f;
	DMapPos.z += 3.0f;

	g_text.pos = CCamera::Camera_GetData();
	g_text.pos.x -= 8.0f;
	g_text.pos.y -= 10.0f;
	g_text.pos.z += 0.0f;

	if (g_text.alive)
	{
		g_text.Age = g_TextFramecount - g_text.TextCreateFrame;

		if (g_text.Age > TEXT_FRAMEMAX)
		{
			UI_Delete();
		}
	}
}

void CUserinterface::UI_Draw(void)
{
	// ���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	if (g_text.alive)
	{

		switch (g_text.chara)
		{
		case PLAYERCHARA:

			switch (g_text.act)
			{
			case REGULARATTACK:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�̍U���I�X���C����%d�_���[�W�^����!", g_text.damage);
				break;

			case GOLADDER:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "��ɐi�݂܂����H");
				UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "�� �i��\n\n   ���̂܂�");
				break;

			case NOTGOLADDER:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "��ɐi�݂܂����H");
				UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "    �i��\n\n�˂��̂܂�");
				break;

			case DESTROY:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�͂���Ă��܂���");
				break;
			}

			break;

		case SRAIM:
			if (g_text.act == REGULARATTACK)
			{
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "�X���C���̍U���I�v���C���[��%d�_���[�W�^����!", g_text.damage);
				//Sprite_Draw(TEX_MESSAGE_WINDOW, g_text.pos.x, g_text.pos.y, 0, 0, Texture_GetWidth(TEX_MESSAGE_WINDOW), Texture_GetHeight(TEX_MESSAGE_WINDOW));
				//Sprite_Draw(TEX_WHITE, g_text.pos.x, g_text.pos.y, 0, 0, Texture_GetWidth(TEX_WHITE), Texture_GetHeight(TEX_WHITE));
				break;
			}

		case SMALEMAP:
			// �~�j�}�b�v�`��
			int x, y;
			for (x = 0; x < MAX_MAPWIDTH; x++)
			{
				for (y = 0; y < MAX_MAPHEIGHT; y++)
				{
					if (CMap::Map_GetData(y, x).type != 0)
					{
						Sprite_Draw(TEX_WHITE, (CMap::Map_GetData(y, x).pos.x + 250) * 1, (CMap::Map_GetData(y, x).pos.z + 250) * 1, 0, 0, 5, 5);
					}
				}
			}
			break;
		}
	}

	// �~�j�}�b�v�`��
	/*for (int x = 0; x < MAX_MAPWIDTH; x++)
	{
		for (int y = 0; y < MAX_MAPHEIGHT; y++)
		{
			if (Map_GetData(x, y).type != 0)
			{
				BilBoard_ChangeMapPos(0.05f + x * 0.05f, 5.00f - y * 0.05f, D3DCOLOR_RGBA(0, 0, 255, 128)); //1�}�X
				BilBoard3_Draw(g_Textbackground, MapPos);
			}
		}
	}*/

}

// ��Ƀf�o�b�O�����łȂ��Q�[���Ȃ����͗p���쐬
void CUserinterface::UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...)
{
#if defined(_DEBUG) || defined(DEBUG)

	RECT rect = { x, y, WINDOW_WIDTH, WINDOW_HEIGHT };

	char Tbuffer[TEXT_MAX];
	va_list ap;
	// �ϒ��������P�̕ϐ��ɂ܂Ƃ߂�
	va_start(ap, text);
	// �܂Ƃ߂�ꂽ�ϐ��ŏ�������
	//vprintf(text, ap);
	vsprintf_s(Tbuffer, TEXT_MAX, text, ap);
	va_end(ap);



	// ����								�������炵����-1�Ȃ�S���ɂȂ�
	g_pD3DXFont->DrawTextA(NULL, Tbuffer, -1, &rect, DT_LEFT, color);
	//g_pD3DXFont->DrawTextA(NULL, Ppos.x, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
#endif // _DEBUG || DEBUG
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act)
{
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
	
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage)
{
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.damage = damage;
	g_text.act = act;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
}

void CUserinterface::UI_Delete(void)
{
	g_text.alive = false;
	g_text.TextCreateFrame = 0;
	g_text.Age = 0;
	g_text.act = ACTTYPENONE;
	g_text.chara = CHARATYPENONE;
}