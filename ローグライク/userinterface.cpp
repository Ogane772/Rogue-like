#include <d3dx9.h>
#include <stdio.h>
#include "common.h"
#include "CTexture.h"
#include "userinterface.h"
#include "bilboard.h"
#include "CEnemy.h"
#include "CObject.h"
#include "Cplayer.h"
#include "CCamera.h"
#include "map.h"
#include "stage.h"
#include "debug_font.h"
/*----------------------------------------------------------------------
�@�@�萔
 ----------------------------------------------------------------------*/
#define TEXT_MAX (100)
//�~�j�}�b�v�\���ʒu
#define MAP_POS_X (250)
#define MAP_POS_Y (250)
//�v���C���[�ʒu����̖����J�}�b�v�J���͈�
#define MAP_ON_X (25)
#define MAP_ON_Y (25)
//�G�l�~�[�A�C�R���\���͈�
#define ENEMY_ON_X (30)
#define ENEMY_ON_Y (30)
//�I�u�W�F�N�g�A�C�R���\���͈�
#define OBJECT_ON_X (30)
#define OBJECT_ON_Y (30)
 /*----------------------------------------------------------------------
 �@�@�O���[�o���ϐ�
   ----------------------------------------------------------------------*/
LPD3DXFONT CUserinterface::g_pD3DXFont;

CUserinterface::TEXT CUserinterface::g_text;
D3DXVECTOR3 CUserinterface::MapPos;
D3DXVECTOR3 CUserinterface::DMapPos;
int CUserinterface::g_Textbackground;
int CUserinterface::g_TextFramecount;
D3DXVECTOR2 time_pos;//���Ԃŕς��ʒu
int add_time; //�e�L�X�g�\�����Ԃ�+����Ƃ�����
bool text_draw;

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
	time_pos = D3DXVECTOR2(0.0f, 0.0f);
	add_time = 0;
	text_draw = false;

	g_text.alive = false;
	g_text.Age = 0;
	g_text.TextCountFrame = 0;
	g_text.TextCreateFrame = 0;
	g_text.pos = D3DXVECTOR2(0.0f, 0.0f);
	g_text.act = ACTTYPENONE;
	g_text.chara = CHARATYPENONE;
	g_text.hitchara = CHARATYPENONE;
	g_text.damage = 0;
	g_text.text_number = 0;

}

void CUserinterface::UI_Finalize(void)
{
	SAFE_RELEASE(g_pD3DXFont);

}

void CUserinterface::UI_Update(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END && !getplayer->Get_EnterFlag())
	{
		//�G���^�[�҂��̎��͉������Ȃ�
		
	}
	else
	{
		g_TextFramecount++;
	}
	MapPos = CCamera::Camera_GetData();
	MapPos.x -= 8.0f;
	MapPos.y -= 8.0f;
	MapPos.z += 3.0f;

	DMapPos = CCamera::Camera_GetData();
	DMapPos.x -= 8.0f;
	DMapPos.y -= 8.1f;
	DMapPos.z += 3.0f;

	/*
	g_text.pos = CCamera::Camera_GetData();
	g_text.pos.x -= 8.0f;
	g_text.pos.y -= 10.0f;
	g_text.pos.z += 0.0f;
	*/
	if (g_text.alive)
	{
		g_text.Age = g_TextFramecount - g_text.TextCreateFrame;

		if (g_text.Age > TEXT_FRAMEMAX - 20)
		{
			if (getplayer->Get_EnterFlag())
			{
				getplayer->Set_RivalFlag(false);
				getplayer->Set_EnterFlag(false);
				getplayer->turn = C3DObj::PLAYER_TURN_END;
			}
		}
		if (g_text.Age > TEXT_FRAMEMAX + add_time)
		{
			UI_Delete();
		}
	}
}

void CUserinterface::UI_Draw(void)
{
	DebugFont_Draw(200, 180, "g_text.Age   = %d", g_text.Age);
	// ���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	C3DObj *getplayer = CPlayer::Get_Player();
	//UI_TextCreate(CUserinterface::SMALEMAP, CUserinterface::ACTTYPENONE);
	//���UI�\��///////////////////
	UI_TextDraw(570, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_Hp());
	UI_TextDraw(710, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_MaxHp());

	UI_TextDraw(500, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "HP        /");

	//HP�o�[
	Sprite_Draw(TEX_HPGAGE_REDBAR, 500,50, 0, 0, (float)Texture_GetWidth(TEX_HPGAGE_REDBAR), (float)Texture_GetHeight(TEX_HPGAGE_REDBAR), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_WAKU, 500, 50, 0, 0, (float)Texture_GetWidth(TEX_HPGAGE_WAKU), (float)Texture_GetHeight(TEX_HPGAGE_WAKU), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_GREENBAR, 512, 60, 0, 0, 250 * getplayer->Get_Hp() / getplayer->Get_MaxHp(), (float)Texture_GetHeight(TEX_HPGAGE_GREENBAR), 0.0f, 0.0f, 0.91f, 0.5f, 0.0f);

	UI_TextDraw(80, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", CStage::Stage_GetLevel());
	UI_TextDraw(100, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "  F");

	UI_TextDraw(300, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", getplayer->Get_Lv());
	UI_TextDraw(200, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "  Lv");

	UI_TextDraw(910, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%4d", getplayer->Get_Gold());
	UI_TextDraw(980, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "  G");
	///////////////////

	//�~�j�}�b�v�`��///////////////////
	int x = 0, y = 0, i = 0;
	for (x = 0; x < MAX_MAPWIDTH; x++)
	{
		for (y = 0; y < MAX_MAPHEIGHT; y++)
		{
			if (CMap::Map_GetData(x, y).type != 0)
			{//�����̃R�����g���o���ƃ}�b�v���S�J�ɂȂ�
				//CMap::Map_SetData(x, y, D3DCOLOR_RGBA(255, 255, 255, 255));
				if (CMap::Map_GetData(x, y).alpha == 0 && (getplayer->Get_Position().x + MAP_POS_X - MAP_ON_X) < (CMap::Map_GetData(x, y).pos.x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + MAP_ON_X > (CMap::Map_GetData(x, y).pos.x + MAP_POS_X)
					&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - MAP_ON_X < (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + MAP_ON_Y > (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y)
				{//�}�b�v�𓥂񂾂��������
					CMap::Map_SetData(x, y, 255);
				}
				if (CMap::Map_GetData(x, y).alpha != 0)
				{
					Sprite_Draw(TEX_BLUE, (CMap::Map_GetData(x, y).pos.x + MAP_POS_X) * 1, (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y, CMap::Map_GetData(x, y).alpha);
				}
				//Sprite_Draw(TEX_BLUE, (CMap::Map_GetData(x, y).pos.x + 250) * 1, (CMap::Map_GetData(x, y).pos.z * -1) + 250, 0, 0, 5, 5);
			}
		}
	}
	//�v���C���[�A�C�R���\��
	Sprite_Draw(TEX_PLAYER_ICON, getplayer->Get_Position().x + MAP_POS_X, (getplayer->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
	C3DObj *enemy;
	C3DObj *object;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);
		if (enemy)
		{//�͈͓��ɂ�����G�l�~�[�A�C�R���\��
			if ((getplayer->Get_Position().x + MAP_POS_X - ENEMY_ON_X) < (enemy->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + ENEMY_ON_X > (enemy->Get_Position().x + MAP_POS_X)
				&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - ENEMY_ON_Y < (enemy->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + ENEMY_ON_Y > (enemy->Get_Position().z*-1) + MAP_POS_Y)
			{
				Sprite_Draw(CTexture::TEX_ENEMY_ICON, enemy->Get_Position().x + MAP_POS_X, (enemy->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
			}
		}
		object = CObject::Get_Object(i);
		//�I�u�W�F�N�g�̔���
		if (object)
		{//�I�u�W�F�N�g�͈�x�}�b�s���O�������������܂ŕ\��������
			if ((getplayer->Get_Position().x + MAP_POS_X - OBJECT_ON_X) < (object->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + OBJECT_ON_X > (object->Get_Position().x + MAP_POS_X)
				&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - OBJECT_ON_Y < (object->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + OBJECT_ON_Y > (object->Get_Position().z*-1) + MAP_POS_Y)
			{
				object->Set_MapDrawFlag(true);
			}
			if (object->Get_MapDrawFlag())
			{
				if (object->m_ObjectType == CObject::TYPE_LADDER)
				{
					Sprite_Draw(CTexture::TEX_LADDER_ICON, object->Get_Position().x + MAP_POS_X, (object->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
				}
			}
		}
	}
	
	///////////////////
	//���̃e�L�X�g�`��
	if (g_text.alive)
	{
		Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
		switch (g_text.chara)
		{
		case PLAYERCHARA:
			switch (g_text.act)
			{
			case REGULARATTACK:
				if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END && getplayer->Get_EnterFlag() || text_draw)
				{//���x���A�b�v���̓��x���A�b�v�\���܂Ń^�[�����~�߂邱��
					text_draw = true;
					time_pos.y-=2;
					//�E�B���h�E�㕔�܂ŕ������X�N���[�����A�㕔�܂ŗ��������
					if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y)
					{
						UI_TextDraw(g_text.pos.x, g_text.pos.y + time_pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�̍U���I");
					}
					if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y + 50)
					{
						UI_TextDraw(g_text.pos.x, g_text.pos.y + 50 + time_pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�X���C����%d�_���[�W�^����!", g_text.damage);
					}
					//��������E�B���h�E�ɕ\�����镶���A�����̏ꏊ�܂ŃX�N���[������
					if (g_text.pos.y + time_pos.y + 100 < g_text.pos.y)
					{
						UI_TextDraw(g_text.pos.x, g_text.pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�X���C����|�����I");
					}
					else
					{
						UI_TextDraw(g_text.pos.x, g_text.pos.y + time_pos.y + 100, D3DCOLOR_RGBA(255, 255, 255, 255), "�X���C����|�����I");
					}
					if (g_text.Age >= ATTACK_END + 30)//1�s�ڂ��o�Ă��班���҂��Ă���o��
					{
						if (g_text.Age == ATTACK_END + 30)
						{
							add_time += 40;//�\�����ԉ���
						}
						if (g_text.pos.y + time_pos.y + 150 < g_text.pos.y + 50)
						{
							UI_TextDraw(g_text.pos.x, g_text.pos.y + 50, D3DCOLOR_RGBA(255, 255, 255, 255), "�o���l0�l��");
						}
						else
						{
							UI_TextDraw(g_text.pos.x, g_text.pos.y + 150 + time_pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�o���l0�l��");
						}
						
					}
					if (g_text.Age >= ATTACK_END + 50)
					{//���x���A�b�v������
						if (g_text.Age == ATTACK_END + 50)
						{
							add_time += 20;//�\�����ԉ���
						}
						if (g_text.pos.y + time_pos.y + 200 < g_text.pos.y + 100)
						{
							UI_TextDraw(g_text.pos.x, g_text.pos.y + 100, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�̓��x��2�ɂȂ����I");
						}
						else
						{
							UI_TextDraw(g_text.pos.x, g_text.pos.y + 200 + time_pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�̓��x��2�ɂȂ����I");
						}
					}
				}
				else if(g_text.Age <= ATTACK_END + 1)
				{
					UI_TextDraw(g_text.pos.x, g_text.pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�̍U���I");
					if (g_text.Age > 30)
					{
						UI_TextDraw(g_text.pos.x, g_text.pos.y + 50, D3DCOLOR_RGBA(255, 255, 255, 255), "�X���C����%d�_���[�W�^����!", g_text.damage);
						if (getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
						{
							UI_TextDraw(g_text.pos.x + 470, g_text.pos.y + 100, D3DCOLOR_RGBA(255, 255, 255, 255), "��");
						}
					}
				}
				break;
			case GOLADDER:
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "��ɐi�݂܂����H");
				UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "�� �i��\n\n   ���̂܂�");
				break;

			case NOTGOLADDER:
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "��ɐi�݂܂����H");
				UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "    �i��\n\n�˂��̂܂�");
				break;

			case DESTROY:
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[�͂���Ă��܂���");
				UI_TextDraw(g_text.pos.x + 470, g_text.pos.y + 100, D3DCOLOR_RGBA(255, 255, 255, 255), "��");
				break;
			}

			break;

		case SRAIM:
			if (g_text.act == REGULARATTACK)
			{
				UI_TextDraw(g_text.pos.x, g_text.pos.y, D3DCOLOR_RGBA(255, 255, 255, 255), "�X���C������̍U���I");
				if (g_text.Age > 30)
				{
					UI_TextDraw(g_text.pos.x, g_text.pos.y + 50, D3DCOLOR_RGBA(255, 255, 255, 255), "�v���C���[��%d�_���[�W�^����!", g_text.damage);
				}			
				break;
			}
		}
	}
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

void CUserinterface::UI_TextDraw2(int x, int y, D3DCOLOR color, const char* text, ...)
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
	//g_text.text_number++;
	g_text.text_number = 1;
	if (g_text.text_number == 4)
	{
		g_text.text_number = 1;
	}
	//text_number�ɍ��킹�ďo���ꏊ��ς���
	switch (g_text.text_number)
	{
	case 1:
		g_text.pos = D3DXVECTOR2(TEXT_POSX, TEXT_POSY);
		break;
	case 2:
		g_text.pos = D3DXVECTOR2(TEXT_POSX, TEXT_POSY + TEXT_ADD_POSY);
		break;
	case 3:
		g_text.pos = D3DXVECTOR2(TEXT_POSX, TEXT_POSY + (TEXT_ADD_POSY * 2));
		break;
	}
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
}

void CUserinterface::UI_Delete(void)
{
	time_pos.y = 0;
	text_draw = false;
	g_text.alive = false;
	g_text.TextCreateFrame = 0;
	g_text.Age = 0;
	g_text.act = ACTTYPENONE;
	g_text.chara = CHARATYPENONE;
	g_text.text_number = 0;
	add_time = 0;
}