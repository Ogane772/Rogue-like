#include <d3dx9.h>
#include <stdio.h>
#include "common.h"
#include "CTexture.h"
#include "CWepon.h"
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
int CUserinterface::g_Textbackground;
int CUserinterface::g_TextFramecount;
D3DXVECTOR2 time_pos;//���Ԃŕς��ʒu
int add_time; //�e�L�X�g�\�����Ԃ�+����Ƃ�����
bool text_draw;

void CUserinterface::UI_Initialize(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	g_pD3DXFont = NULL;
	g_TextFramecount = 0;

	D3DXCreateFont(m_pD3DDevice, 40, 0, 0, 0, false,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "HGP�n�p�p�߯�ߑ�", &g_pD3DXFont);

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
	g_text.type = 0;
	g_text.damage = 0;
	strcpy_s(g_text.player_name, MAX_NAME, getplayer->Get_Name());
}

void CUserinterface::UI_Finalize(void)
{
	SAFE_RELEASE(g_pD3DXFont);

}

void CUserinterface::UI_Update(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	//if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END && !getplayer->Get_EnterFlag())
	//{
		//�G���^�[�҂��̎��͉������Ȃ�
		
	//}
	//else

	if (g_text.alive)
	{
		g_text.Age = g_TextFramecount - g_text.TextCreateFrame;

		if (g_text.Age == TEXT_FRAMEMAX + ENEMY_DESTROY_TEXT)
		{
			//if (getplayer->Get_EnterFlag())
			{
				getplayer->Set_RivalFlag(false);
				getplayer->Set_EnterFlag(false);
				getplayer->Set_PlayerTurn(CPlayer::PLAYER_TURN_END);
			}
		}
		if (g_text.Age > TEXT_FRAMEMAX + add_time)
		{
			UI_Delete();
		}
	}
	if (getplayer->Get_PlayerTurn() != CPlayer::PLAYER_WINDOW)
	{
		if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_STATUS)
		{
			return;
		}
		if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_ITEM)
		{
			return;
		}
		else
		{
			g_TextFramecount++;
		}
	}
}

void CUserinterface::UI_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (getplayer->Get_DarkFlag())
	{
		Sprite_Draw(TEX_WINDOW_MARU, 0.0f, 0.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WINDOW_MARU), (float)Texture_GetHeight(TEX_WINDOW_MARU), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	}
	CommandWindow_Draw();
	ItemWindow_Draw();
	StatusWindow_Draw();
	UI_UpDraw();
	Map_Draw();
	UI_TextDraw();
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
	vsprintf_s(Tbuffer, TEXT_MAX, text, ap);
	va_end(ap);

	// ����								�������炵����-1�Ȃ�S���ɂȂ�
	g_pD3DXFont->DrawTextA(NULL, Tbuffer, -1, &rect, DT_LEFT, color);

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
	vsprintf_s(Tbuffer, TEXT_MAX, text, ap);
	va_end(ap);



	// ����								�������炵����-1�Ȃ�S���ɂȂ�
	g_pD3DXFont->DrawTextA(NULL, Tbuffer, -1, &rect, DT_LEFT, color);
#endif // _DEBUG || DEBUG
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act)
{
	g_text.Age = 0;
	time_pos.y = 0;
	text_draw = false;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
	
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	strcpy_s(g_text.name, MAX_NAME, CObject::Get_ObjectName(type));
	strcpy_s(g_text.item_log, MAX_NAME, CObject::Get_ObjectEffect(type));
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type, int purasu_number)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	strcpy_s(g_text.name, MAX_NAME, CWepon::Get_Wepon_Data(type)->Wepon_name);
	strcpy_s(g_text.item_log, MAX_NAME, CWepon::Get_Wepon_Data(type)->Wepon_effect);
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	g_text.purasu_number = purasu_number;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.damage = damage;
	g_text.act = act;
	if (type > 0)
	{
		g_text.type = type;
	}
	//�G�l�~�[�f�[�^���疼�O�ƌo���l�����炤�̂Ű1����
	if (hitchara > 1)
	{
		g_text.exp = CEnemy::Get_EnemyExp(g_text.type - 1);
		g_text.gold = CEnemy::Get_EnemyExp(g_text.type - 1);
		strcpy_s(g_text.name, MAX_NAME, CEnemy::Get_EnemyName(g_text.type - 1));
	}
	if (chara > 1)
	{
		strcpy_s(g_text.name, MAX_NAME, CEnemy::Get_EnemyName(g_text.type - 1));
	}
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
}

void CUserinterface::UI_TextCreate(int week_type,CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.damage = damage;
	g_text.act = act;
	g_text.attribute_type = week_type;
	if (type > 0)
	{
		g_text.type = type;
	}
	//�G�l�~�[�f�[�^���疼�O�ƌo���l�����炤�̂Ű1����
	if (hitchara > 1)
	{
		g_text.exp = CEnemy::Get_EnemyExp(g_text.type - 1);
		g_text.gold = CEnemy::Get_EnemyExp(g_text.type - 1);
		strcpy_s(g_text.name, MAX_NAME, CEnemy::Get_EnemyName(g_text.type - 1));
	}
	if (chara > 1)
	{
		strcpy_s(g_text.name, MAX_NAME, CEnemy::Get_EnemyName(g_text.type - 1));
	}
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// �a����
	g_text.TextCreateFrame = g_TextFramecount;
}

void CUserinterface::UI_Delete(void)
{
	g_text.lv_up = false;
	time_pos.y = 0;
	text_draw = false;
	g_text.alive = false;
	g_text.TextCreateFrame = 0;
	g_text.Age = 0;
	g_text.act = ACTTYPENONE;
	g_text.chara = CHARATYPENONE;
	g_text.pos = D3DXVECTOR2(0.0f, 0.0f);
	add_time = 0;
}

void CUserinterface::CommandWindow_Draw(void)
{//���莝����Ԃ̎��������Ă�
	//���킪���肩��Ў�i�t���j�ɂȂ�����e�L�X�g�ŏo������
	C3DObj *getplayer = CPlayer::Get_Player();
	//�A�C�e�����`��
	//����͂P�y�[�W�ڂ̂ݕ`��
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW)
	{
		RightWindow_Draw();
		//��̃E�B���h�E
		Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.38f, 1.1f, 0.0f);
		//�J�[�\���ʒu
		switch (getplayer->Get_CursorNumber())
		{
		case CPlayer::CURSOL_LEFT_TOP:
			Sprite_Draw(TEX_CURSOL, 110.0f, 157.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
			break;
		case CPlayer::CURSOL_RIGHT_TOP:
			Sprite_Draw(TEX_CURSOL, 240.0f, 157.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
			break;
		case CPlayer::CURSOL_LEFT_DOWN:
			Sprite_Draw(TEX_CURSOL, 110.0f, 212.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
			break;
		case CPlayer::CURSOL_RIGHT_DOWN:
			Sprite_Draw(TEX_CURSOL, 240.0f, 212.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
			break;				
		}
		UI_TextDraw(145, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
		UI_TextDraw(275, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "�\��");
		UI_TextDraw(145, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
		UI_TextDraw(275, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
		//���̃E�B���h�E
		Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 510.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.2f, 0.0f);
		//UI_TextDraw(145, 530, D3DCOLOR_RGBA(255, 255, 255, 255), "���B�x");
		//UI_TextDraw(475, 530, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", ((CStage::Stage_GetLevel() * 10) / MAX_MAP) * 10);
		UI_TextDraw(145, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "�U����");
		if (getplayer->Get_WMode())
		{//�ォ��U���́A�h���
			UI_TextDraw(475, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", getplayer->Get_Str() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str);
			UI_TextDraw(475, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", getplayer->Get_Def() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def);
		}
		else
		{
			UI_TextDraw(475, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", getplayer->Get_Str() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str);
			UI_TextDraw(475, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", getplayer->Get_Def() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def);
		}
		switch (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type)
		{
		case CWepon::TYPE_SWORD:
			Sprite_Draw(TEX_SWORD_ICON, 260.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SWORD_ICON), (float)Texture_GetHeight(TEX_SWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
			break;
		case CWepon::TYPE_BIGSWORD:
			Sprite_Draw(TEX_BIGSWORD_ICON, 260.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
			break;
		case CWepon::TYPE_REIPIA:
			Sprite_Draw(TEX_REIPIA_ICON, 260.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
			break;
		case CWepon::TYPE_RANCE:
			Sprite_Draw(TEX_RANCE_ICON, 260.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
			break;
		case CWepon::TYPE_MEISU:
			Sprite_Draw(TEX_MEISU_ICON, 260.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
			break;
		case CWepon::TYPE_AX:
			Sprite_Draw(TEX_AX_ICON, 260.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
			break;
		}
		//���A�C�R���͗��莝���̎��͕\�����Ȃ�	
		if (!getplayer->Get_WMode() && CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_str > 0)
		{
			Sprite_Draw(TEX_SHELD_ICON, 320.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SHELD_ICON), (float)Texture_GetHeight(TEX_SHELD_ICON), 0.0f, 0.0f, 0.35f, 0.35f, 0.0f);
		}
		if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str > 0)
		{
			Sprite_Draw(TEX_RING_ICON, 380.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_RING_ICON), (float)Texture_GetHeight(TEX_RING_ICON), 0.0f, 0.0f, 0.35f, 0.35f, 0.0f);
		}
		UI_TextDraw(145, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "�h���");
		if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def > 0)
		{
			switch (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type)
			{
			case CWepon::TYPE_SWORD:
				Sprite_Draw(TEX_SWORD_ICON, 260.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SWORD_ICON), (float)Texture_GetHeight(TEX_SWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
				break;
			case CWepon::TYPE_BIGSWORD:
				Sprite_Draw(TEX_BIGSWORD_ICON, 260.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
				break;
			case CWepon::TYPE_REIPIA:
				Sprite_Draw(TEX_REIPIA_ICON, 260.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
				break;
			case CWepon::TYPE_RANCE:
				Sprite_Draw(TEX_RANCE_ICON, 260.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
				break;
			case CWepon::TYPE_MEISU:
				Sprite_Draw(TEX_MEISU_ICON, 260.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
				break;
			case CWepon::TYPE_AX:
				Sprite_Draw(TEX_AX_ICON, 260.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.45f, 0.45f, 0.0f);
				break;
			}
		}
		if (!getplayer->Get_WMode() && CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_def > 0)
		{
			Sprite_Draw(TEX_SHELD_ICON, 320.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SHELD_ICON), (float)Texture_GetHeight(TEX_SHELD_ICON), 0.0f, 0.0f, 0.35f, 0.35f, 0.0f);
		}
		if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def > 0)
		{
			Sprite_Draw(TEX_RING_ICON, 380.0f, 585.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_RING_ICON), (float)Texture_GetHeight(TEX_RING_ICON), 0.0f, 0.0f, 0.35f, 0.35f, 0.0f);
		}
		UI_TextDraw(595, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "�����x");
		UI_TextDraw(925, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d/%3d%%", getplayer->Get_PlayerOnaka(),MAX_ONAKA);
		UI_TextDraw(595, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "���̃��x���܂�");
		UI_TextDraw(1025, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", getplayer->Get_PlayerNextExp(getplayer->Get_Lv()) - getplayer->Get_Exp() );
	}
}

void CUserinterface::ItemWindow_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_ITEM)
	{
		//�E�̃E�B���h�E
		RightWindow_Draw();
		//���̃E�B���h�E
		Sprite_Draw(TEX_WIDTH_WINDOW, 60.0f, 115.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.1f, 1.53f, D3DXToRadian(0.0f));
		//�����A�C�e���Z�b�g
		SetItemWindow();
		if (getplayer->Get_ItemOn())
		{//�A�C�e���I�����̃E�B���h�E
			SetItemOnWindow();
		}
	}
}

void CUserinterface::RightWindow_Draw(void)
{//�E�̃E�B���h�E
	C3DObj *getplayer = CPlayer::Get_Player();
	int i;
	int m_WeponStock[MAX_WEPON] = { 0 };
	int m_ItemStock[MAX_ITEM] = { 0 };
	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = getplayer->Get_PlayerWeponStock(i);
	}
	Sprite_Draw(TEX_MESSAGE_WINDOW, 800.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.46f, 1.5f, 0.0f);
	for (i = 0; i < MAX_WEPON; i++)
	{
		switch (CPlayer::GetPlayerWeponData(i)->wepon_type)
		{
		case CWepon::TYPE_SWORD:
			Sprite_Draw(TEX_SWORD_ICON, 805.0f, 140.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SWORD_ICON), (float)Texture_GetHeight(TEX_SWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
			break;
		case CWepon::TYPE_BIGSWORD:
			Sprite_Draw(TEX_BIGSWORD_ICON, 805.0f, 140.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
			break;
		case CWepon::TYPE_REIPIA:
			Sprite_Draw(TEX_REIPIA_ICON, 805.0f, 140.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
			break;
		case CWepon::TYPE_RANCE:
			Sprite_Draw(TEX_RANCE_ICON, 805.0f, 140.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
			break;
		case CWepon::TYPE_MEISU:
			Sprite_Draw(TEX_MEISU_ICON, 805.0f, 140.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
			break;
		case CWepon::TYPE_AX:
			Sprite_Draw(TEX_AX_ICON, 805.0f, 140.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
			break;
		case CWepon::TYPE_SHELD:
			if (!getplayer->Get_WMode())
			{
				Sprite_Draw(TEX_SHELD_ICON, 810.0f, 145.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SHELD_ICON), (float)Texture_GetHeight(TEX_SHELD_ICON), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
			}
			break;
		case CWepon::TYPE_RING:
			Sprite_Draw(TEX_RING_ICON, 810.0f, 145.0f + (i * TEXT_Y), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_RING_ICON), (float)Texture_GetHeight(TEX_RING_ICON), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
			break;
		}
	}
	if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number > 0)
	{
		UI_TextDraw(875, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number);
	}
	else if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number == 0)
	{
		UI_TextDraw(875, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name);
	}
	//���莝���̎��͏��̏ꏊ�ɕ��햼
	if (getplayer->Get_WMode())
	{
		if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number > 0)
		{
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number);
		}
		else if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number == 0)
		{
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name);
		}
	}
	else if (!getplayer->Get_WMode())
	{
		if (CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_purasu_number > 0)
		{
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::SHELD_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_purasu_number);
		}
		else if (CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_purasu_number == 0)
		{
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::SHELD_NUMBER])->Wepon_name);
		}
	}
	if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number > 0)
	{
		UI_TextDraw(875, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::RING_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number);
	}
	else if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number == 0)
	{
		UI_TextDraw(875, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::RING_NUMBER])->Wepon_name);
	}
}

void CUserinterface::SetItemWindow(void)
{
	//�A�C�e�����\��
	int i;
	int stock = 0;//�A�C�e���������邩������
	C3DObj *getplayer = CPlayer::Get_Player();
	int m_WeponStock[MAX_WEPON] = { 0 };
	int m_ItemStock[MAX_ITEM] = { 0 };
	//�A�C�e�����`��
	//����͂P�y�[�W�ڂ̂ݕ`��
	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = getplayer->Get_PlayerWeponStock(i);
		if (!getplayer->Get_NextItemPage())
		{
			if (CPlayer::GetPlayerWeponData(i)->wepon_purasu_number > 0)
			{
				UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d", CWepon::Get_Wepon_Data(m_WeponStock[i])->Wepon_name, CPlayer::GetPlayerWeponData(i)->wepon_purasu_number);
			}
			else
			{
				UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[i])->Wepon_name);
			}
			
			stock++;
		}
	}
	
	for (i = 0; i < (MAX_ITEM / 2) - 1; i++)
	{
		m_ItemStock[i] = getplayer->Get_PlayerItemStock(i);
		if (!getplayer->Get_NextItemPage())
		{
			if (m_ItemStock[i] > 0)
			{
				UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CObject::Get_ObjectName(m_ItemStock[i]));
				stock++;
			}
		}
	}

	for (i = (MAX_ITEM / 2) - 1; i < MAX_ITEM; i++)
	{
		m_ItemStock[i] = getplayer->Get_PlayerItemStock(i);
		if (getplayer->Get_NextItemPage())
		{
			if (m_ItemStock[i] > 0)
			{
				UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CObject::Get_ObjectName(m_ItemStock[i]));
				stock++;
			}
		}
	}
	//�A�C�e�����P�y�[�W���܂��Ă���Q�y�[�W�ڂ����邱�Ƃ������J�[�\�����o��
	if (m_WeponStock[2] != 0 && m_ItemStock[5] != 0)
	{
		Sprite_Draw(TEX_CURSOL, 515.0f, 125.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.23f, 0.23f, 0.0f);
		Sprite_Draw(TEX_CURSOL, 510.0f, 167.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.23f, 0.23f, D3DXToRadian(180.0f));
	}
	//�J�[�\���ʒu
	//�A�C�e���I�𒆂̓O���C�ɂ���
	if (getplayer->Get_ItemOn())
	{
		Sprite_Draw(TEX_CURSOL_GRY, 110.0f, 157.0f + (TEXT_Y * (getplayer->Get_CursorNumber())), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL_GRY), (float)Texture_GetHeight(TEX_CURSOL_GRY), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	else if (!getplayer->Get_ItemOn())
	{
		Sprite_Draw(TEX_CURSOL, 110.0f, 157.0f + (TEXT_Y * (getplayer->Get_CursorNumber())), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	//UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "�߂�");
}

void CUserinterface::SetItemOnWindow(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	int add_cursor = 0;//����2�y�[�W�ڂ��J���Ă��瑫��
	//�A�C�e���^�C�v�ɂ��\���E�B���h�E�A���b�Z�[�W��ς���
	//�����̎�
	if (getplayer->Get_NextItemPage())
	{
		add_cursor += ((MAX_ITEM + MAX_WEPON) / 2);
	}
	if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::WEPON)
	{
		Sprite_Draw(TEX_MESSAGE_WINDOW, 555.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.25f, 0.7f, D3DXToRadian(0.0f));
		UI_TextDraw(620, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
	}
	else if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::ITEM)
	{
		Sprite_Draw(TEX_MESSAGE_WINDOW, 555.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.25f, 1.46f, D3DXToRadian(0.0f));
		UI_TextDraw(620, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
		UI_TextDraw(620, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "�u��");
		UI_TextDraw(620, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
	}
	//�J�[�\���ʒu
	Sprite_Draw(TEX_CURSOL, 585.0f, 157.0f + (TEXT_Y * (getplayer->Get_TimeCursorNumber())), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	//�t���O��ON�Ȃ�A�C�e����������\��
	if (getplayer->Get_ItemTips())
	{
		SetItemTips();
	}
}

void CUserinterface::SetItemTips(void)
{
	int i;
	int add_cursor = 0;//����2�y�[�W�ڂ��J���Ă��瑫��
	C3DObj *getplayer = CPlayer::Get_Player();
	int m_WeponStock[MAX_WEPON] = { 0 };
	int m_ItemStock[MAX_ITEM] = { 0 };
	//�A�C�e�����`��
	//����͂P�y�[�W�ڂ̂ݕ`��
	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = getplayer->Get_PlayerWeponStock(i);
	}
	for (i = 0; i < MAX_ITEM; i++)
	{
		m_ItemStock[i] = getplayer->Get_PlayerItemStock(i);
	}
	if (getplayer->Get_NextItemPage())
	{
		add_cursor += ((MAX_ITEM + MAX_WEPON) / 2);
	}
	Sprite_Draw(TEX_MESSAGE_WINDOW, 555.0f, 329.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.755f, 1.1f, D3DXToRadian(0.0f));
	//�A�C�e���^�C�v�ɂ���������ς���
	//�����̎�
	if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::WEPON)
	{//���enum��`���Ă�������
		//����̔\�͒l������1�ȏ゠��Ε\������
		switch (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_type)
		{
		case CWepon::TYPE_SWORD:
		case CWepon::TYPE_REIPIA:
		case CWepon::TYPE_MEISU:
			UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�Ў蕐��");
			break;
		case CWepon::TYPE_BIGSWORD:
		case CWepon::TYPE_RANCE:
		case CWepon::TYPE_AX:
			UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "���蕐��");
			break;
		case CWepon::TYPE_SHELD:
			if (getplayer->Get_WMode())
			{
				UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�񑕔���");
			}
			else
			{
				UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "������");
			}
			break;
		case CWepon::TYPE_RING:
			UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�����i");
			break;
		}
		if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str > 0 && CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def > 0)
		{
			UI_TextDraw(765, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�U���� %d  �h��� %d", CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str, CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def);
		}
		else if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str > 0)
		{
			UI_TextDraw(765, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�U���� %d", CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str);
		}
		else if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def > 0)
		{
			UI_TextDraw(765, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�h��� %d", CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def);
		}
		UI_TextDraw(640, 410, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[getplayer->Get_CursorNumber()])->Wepon_effect);
	}
	else if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::ITEM)
	{//�X�C�b�`����Ȃ��ăA�C�e���X�g�b�N�̃^�C�v�Q�Ƃ���I�I
		UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "�g�p�A�C�e��");
		UI_TextDraw(640, 410, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CObject::Get_Item_Data(m_ItemStock[getplayer->Get_CursorNumber() + add_cursor - MAX_WEPON])->object_tips);
	}
}

void CUserinterface::StatusWindow_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	int i;
	int m_WeponStock[MAX_WEPON] = { 0 };	//�A�C�e�����`��
	//����͂P�y�[�W�ڂ̂ݕ`��
	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = getplayer->Get_PlayerWeponStock(i);
	}
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_STATUS)
	{
		//�E�̃E�B���h�E
		RightWindow_Draw();
		//��̃E�B���h�E
		Sprite_Draw(TEX_WIDTH_WINDOW, 60.0f, 115.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.1f, 1.53f, D3DXToRadian(0.0f));
		//�J�[�\���ʒu
		Sprite_Draw(TEX_CURSOL, 110.0f, 547.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
		UI_TextDraw(105, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "%s�̔\��",getplayer->Get_Name());
		UI_TextDraw(125, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "������    %d", getplayer->Get_Str());
		UI_TextDraw(125, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "�܂���    %d", getplayer->Get_Def());
		UI_TextDraw(125, 320, D3DCOLOR_RGBA(255, 255, 255, 255), "�o���l    %d", getplayer->Get_Exp());
		switch (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type)
		{
		case CWepon::TYPE_SWORD:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "����^�C�v   �Ў茕");
			break;
		case CWepon::TYPE_BIGSWORD:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "����^�C�v   ���茕");
			break;
		case CWepon::TYPE_REIPIA:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "����^�C�v   ���C�s�A");
			break;
		case CWepon::TYPE_RANCE:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "����^�C�v   �����X");
			break;
		case CWepon::TYPE_MEISU:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "����^�C�v   ���C�X");
			break;
		case CWepon::TYPE_AX:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "����^�C�v   �I�m");
			break;
		}
		
		if (getplayer->Get_WMode())
		{
			UI_TextDraw(125, 430, D3DCOLOR_RGBA(255, 255, 255, 255), "����U����   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str);
			UI_TextDraw(125, 485, D3DCOLOR_RGBA(255, 255, 255, 255), "����h���   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def);
		}
		else
		{
			UI_TextDraw(125, 430, D3DCOLOR_RGBA(255, 255, 255, 255), "����U����   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str);
			UI_TextDraw(125, 485, D3DCOLOR_RGBA(255, 255, 255, 255), "����h���   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def);
		}
		UI_TextDraw(145, 545, D3DCOLOR_RGBA(255, 255, 255, 255), "�߂�");
	}
}

void CUserinterface::UI_UpDraw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	//���UI�\��///////////////////
	UI_TextDraw(570, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_Hp());
	UI_TextDraw(710, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_MaxHp());

	UI_TextDraw(500, UI_Y, D3DCOLOR_RGBA(0, 255, 200, 255), "HP        /");

	//HP�o�[
	Sprite_Draw(TEX_HPGAGE_REDBAR, 500, 50 + UI_Y, 0, 0, (float)Texture_GetWidth(TEX_HPGAGE_REDBAR), (float)Texture_GetHeight(TEX_HPGAGE_REDBAR), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_WAKU, 500, 50 + UI_Y, 0, 0, (float)Texture_GetWidth(TEX_HPGAGE_WAKU), (float)Texture_GetHeight(TEX_HPGAGE_WAKU), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_GREENBAR, 512, 60 + UI_Y, 0, 0, 250 * getplayer->Get_Hp() / getplayer->Get_MaxHp(), (float)Texture_GetHeight(TEX_HPGAGE_GREENBAR), 0.0f, 0.0f, 0.91f, 0.5f, 0.0f);

	UI_TextDraw(80, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", CStage::Stage_GetLevel());
	UI_TextDraw(100, UI_Y, D3DCOLOR_RGBA(0, 255, 200, 255), "  F");

	UI_TextDraw(300, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", getplayer->Get_Lv());
	UI_TextDraw(200, UI_Y, D3DCOLOR_RGBA(0, 255, 200, 255), "  Lv");

	UI_TextDraw(910, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%4d", getplayer->Get_Gold());
	UI_TextDraw(980, UI_Y, D3DCOLOR_RGBA(0, 255, 200, 255), "  G");
}

void CUserinterface::Map_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	C3DObj *enemy;
	C3DObj *object;
	C3DObj *wepon;
	//�E�B���h�E���J���Ă�Ƃ��ȊO�Ƀ~�j�}�b�v�`��///////////////////
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_STATUS)
	{
		return;
	}
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_ITEM)
	{
		return;
	}
	if (getplayer->Get_PlayerTurn() != CPlayer::PLAYER_WINDOW)
	{
		int x = 0, y = 0, i = 0;
		for (x = 0; x < MAX_MAPWIDTH; x++)
		{
			for (y = 0; y < MAX_MAPHEIGHT; y++)
			{
				if (CMap::Map_GetData(x, y).type != 0)
				{//�����̃R�����g���o���ƃ}�b�v���S�J�ɂȂ�
					CMap::Map_SetData(x, y, D3DCOLOR_RGBA(255, 255, 255, 255));
					if (CMap::Map_GetData(x, y).alpha == 0 && (getplayer->Get_Position().x + MAP_POS_X - MAP_ON_X) < (CMap::Map_GetData(x, y).pos.x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + MAP_ON_X > (CMap::Map_GetData(x, y).pos.x + MAP_POS_X)
						&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - MAP_ON_X < (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + MAP_ON_Y >(CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y)
					{//�}�b�v�𓥂񂾂��������
						CMap::Map_SetData(x, y, 255);
					}
					if (CMap::Map_GetData(x, y).alpha != 0)
					{
						Sprite_Draw(TEX_BLUE, (CMap::Map_GetData(x, y).pos.x + MAP_POS_X) * 1, (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y, CMap::Map_GetData(x, y).alpha);
					}
				}
			}
		}
		//�v���C���[�A�C�R���\��
		Sprite_Draw(TEX_PLAYER_ICON, getplayer->Get_Position().x + MAP_POS_X, (getplayer->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
		for (i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{//�͈͓��ɂ�����G�l�~�[�A�C�R���\�� if���R�����g�A�E�g�őS�\��
			//	if ((getplayer->Get_Position().x + MAP_POS_X - ENEMY_ON_X) < (enemy->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + ENEMY_ON_X > (enemy->Get_Position().x + MAP_POS_X)
			//		&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - ENEMY_ON_Y < (enemy->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + ENEMY_ON_Y >(enemy->Get_Position().z*-1) + MAP_POS_Y)
				{
					Sprite_Draw(CTexture::TEX_ENEMY_ICON, enemy->Get_Position().x + MAP_POS_X, (enemy->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
				}
			}
			object = CObject::Get_Object(i);
			//�I�u�W�F�N�g�̔���
			if (object)
			{//�I�u�W�F�N�g�͈�x�}�b�s���O�������������܂ŕ\��������@if���R�����g�A�E�g�őS�\��
				//if ((getplayer->Get_Position().x + MAP_POS_X - OBJECT_ON_X) < (object->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + OBJECT_ON_X > (object->Get_Position().x + MAP_POS_X)
				//	&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - OBJECT_ON_Y < (object->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + OBJECT_ON_Y >(object->Get_Position().z*-1) + MAP_POS_Y)
				{
					object->Set_MapDrawFlag(true);
				}
				if (object->Get_MapDrawFlag())
				{
					if (object->m_ObjectType == CObject::TYPE_LADDER)
					{
						Sprite_Draw(CTexture::TEX_LADDER_ICON, object->Get_Position().x + MAP_POS_X, (object->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
					}
					if (object->m_ObjectType != CObject::TYPE_LADDER)
					{
						Sprite_Draw(CTexture::TEX_ITEM_ICON, object->Get_Position().x + MAP_POS_X, (object->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
					}
				}
			}
			wepon = CWepon::Get_Wepon(i);
			//����̔���
			if (wepon)
			{//����͈�x�}�b�s���O�������������܂ŕ\��������@if���R�����g�A�E�g�őS�\��
			// if ((getplayer->Get_Position().x + MAP_POS_X - OBJECT_ON_X) < (wepon->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + OBJECT_ON_X > (wepon->Get_Position().x + MAP_POS_X)
			// 	&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - OBJECT_ON_Y < (wepon->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + OBJECT_ON_Y >(wepon->Get_Position().z*-1) + MAP_POS_Y)
				{
					wepon->Set_MapDrawFlag(true);
				}
				if (wepon->Get_MapDrawFlag())
				{
					Sprite_Draw(CTexture::TEX_ITEM_ICON, wepon->Get_Position().x + MAP_POS_X, (wepon->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 8, 8);
				}
			}
		}
	}
}

void CUserinterface::UI_TextDraw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_STATUS)
	{
		return;
	}
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_ITEM)
	{
		return;
	}
	if (getplayer->Get_PlayerTurn() != CPlayer::PLAYER_WINDOW)
	{
		//���̃e�L�X�g�`��
		if (g_text.alive)
		{
			Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
			switch (g_text.chara)
			{
			case PLAYERCHARA:
				switch (g_text.act)
				{
				case REGULARATTACK://�L�[���͂�����Ȃ�R�����g�A�E�g����
					if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END /*&& getplayer->Get_EnterFlag() */ || text_draw)
					{//���x���A�b�v���̓��x���A�b�v�\���܂Ń^�[�����~�߂邱��
						text_draw = true;
						time_pos.y -= 2;
						//�E�B���h�E�㕔�܂ŕ������X�N���[�����A�㕔�܂ŗ��������
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�̍U���I", g_text.player_name);
						}
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y + 50)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�U���͊O��Ă��܂����I");
							}
							else
							{
								//������̒����𒲂ׂ��̒��������̕���X�𑫂�
								int len = strlen(g_text.name);
								int add_x = 0;
								if (g_text.damage < 10)
								{
									add_x = 1;
								}
								else if (g_text.damage >= 10 && g_text.damage <= 99)
								{
									add_x = 2;
								}
								else if (g_text.damage >= 100)
								{
									add_x = 3;
								}
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s��", g_text.name);
								//�����ɂ���ă_���[�W�����F�ύX
								if (g_text.attribute_type == WEEK_TYPE)
								{
									UI_TextDraw((int)g_text.pos.x + ((len * 22)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 0, 0, 255), "%d", g_text.damage);
								}
								if (g_text.attribute_type == NORMAL_TYPE)
								{
									UI_TextDraw((int)g_text.pos.x + ((len * 22)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%d", g_text.damage);
								}
								if (g_text.attribute_type == RESIST_TYPE)
								{
									UI_TextDraw((int)g_text.pos.x + ((len * 22)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(0, 0, 255, 255), "%d", g_text.damage);
								}
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�_���[�W�^����!");
							}
						}
						//��������E�B���h�E�ɕ\�����镶���A�����̏ꏊ�܂ŃX�N���[������
						if (g_text.pos.y + time_pos.y + 100 < g_text.pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s��|�����I", g_text.name);
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%s��|�����I", g_text.name);
						}
						if (g_text.Age >= ATTACK_END + 30)//1�s�ڂ��o�Ă��班���҂��Ă���o��
						{
							if (g_text.Age == ATTACK_END + 30)
							{
								g_text.lv_up = getplayer->ExpGoldCheck(g_text.exp, g_text.gold);
								add_time += 40;//�\�����ԉ���
							}
							if (g_text.pos.y + time_pos.y + 150 < g_text.pos.y + 50)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "�o���l%d�l��", g_text.exp);
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 150 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�o���l%d�l��", g_text.exp);
							}

						}
						if (g_text.Age >= ATTACK_END + 50 && g_text.lv_up)
						{//���x���A�b�v������
							if (g_text.Age == ATTACK_END + 50)
							{
								add_time += 80;//�\�����ԉ���
							}
							if (g_text.pos.y + time_pos.y + 200 < g_text.pos.y + 100)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�̓��x��%d�ɂȂ����I", g_text.player_name, getplayer->Get_Lv());
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 200 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�̓��x��%d�ɂȂ����I", g_text.player_name, getplayer->Get_Lv());
							}
						}
					}
					else if (g_text.Age <= ATTACK_END + 1)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�̍U���I", g_text.player_name);
						if (g_text.Age > 30)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�U���͊O��Ă��܂����I");
							}
							else if (g_text.damage > 0)
							{
								//������̒����𒲂ׂ��̒��������̕���X�𑫂�
								int len = strlen(g_text.name);
								int add_x = 0;
								if (g_text.damage < 10)
								{
									add_x = 1;
								}
								else if (g_text.damage >= 10 && g_text.damage <= 99)
								{
									add_x = 2;
								}
								else if (g_text.damage >= 100)
								{
									add_x = 3;
								}
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s��", g_text.name);
								//�����ɂ���ă_���[�W�����F�ύX
								if (g_text.attribute_type == WEEK_TYPE)
								{
									UI_TextDraw((int)g_text.pos.x + ((len * 22)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 0, 0, 255), "%d", g_text.damage);
								}
								if (g_text.attribute_type == NORMAL_TYPE)
								{
									UI_TextDraw((int)g_text.pos.x + ((len * 22)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%d", g_text.damage);
								}
								if (g_text.attribute_type == RESIST_TYPE)
								{
									UI_TextDraw((int)g_text.pos.x + ((len * 22)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(0, 0, 255, 255), "%d", g_text.damage);
								}
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�_���[�W�^����!");
							}
							if (getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
							{
								UI_TextDraw((int)(g_text.pos.x + 470), (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "��");
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
					UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "%s�͂���Ă��܂���", g_text.player_name);
					UI_TextDraw((int)(g_text.pos.x + 470), (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "��");
					break;

				case ITEM:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s����ɓ��ꂽ", g_text.name);
					break;

				case NOITEM:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�̏�ɏ����", g_text.name);
					break;

				case ITEM_EFFECT:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s������", g_text.name);
					if (g_text.Age > ITEM_EFFECT_FRAME)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s", g_text.item_log);
					}
					break;

				case ITEM_ASIMOTO:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�𑫌��ɒu����", g_text.name);
					break;

				case NOITEM_ASIMOTO:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "���̏ꏊ�ɂ͒u���Ȃ��I");
					break;

				case WEPON_GET:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d�𑕔�����", g_text.name, g_text.purasu_number);
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�𑕔�����", g_text.name);
					}
					break;
				case WEPON_W_GETCHENGE:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d�𑕔�����", g_text.name, g_text.purasu_number);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�������܂���");
						}
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�𑕔�����", g_text.name);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�������܂���");
						}
					}
					break;
				case WEPON_NORMAL_GETCHENGE:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d�𑕔�����", g_text.name, g_text.purasu_number);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "���܂��Ă������𑕔�����");
						}
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�𑕔�����", g_text.name);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "���܂��Ă������𑕔�����");
						}
					}
					break;
				case SHELD_W_GET:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�{%d�����܂���", g_text.name, g_text.purasu_number);
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s�����܂���", g_text.name);
					}
					break;
				}
				break;

			case ENEMY:
				if (g_text.act == REGULARATTACK)
				{
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s����̍U���I", g_text.name);
					if (g_text.Age > 30)
					{
						if (g_text.damage == 0)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�U���͊O��Ă��܂����I");
						}
						else
						{
							//������̒����𒲂ׂ��̒��������̕���X�𑫂�
							int len = strlen(g_text.player_name);
							int add_x = 0;
							if (g_text.damage < 10)
							{
								add_x = 1;
							}
							else if (g_text.damage >= 10 && g_text.damage <= 99)
							{
								add_x = 2;
							}
							else if (g_text.damage >= 100)
							{
								add_x = 3;
							}
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s��", g_text.player_name);
							//�����ɂ���ă_���[�W�����F�ύX
							if (g_text.attribute_type == WEEK_TYPE)
							{
								UI_TextDraw((int)g_text.pos.x + ((len * 25)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 0, 0, 255), "%d", g_text.damage);
							}
							if (g_text.attribute_type == NORMAL_TYPE)
							{
								UI_TextDraw((int)g_text.pos.x + ((len * 25)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%d", g_text.damage);
							}
							if (g_text.attribute_type == RESIST_TYPE)
							{
								UI_TextDraw((int)g_text.pos.x + ((len * 25)), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(0, 0, 255, 255), "%d", g_text.damage);
							}
							UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 42), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "�_���[�W�^����!");
						}
					}
					break;
				}
			}
		}
	}
}