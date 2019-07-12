#define _CRT_SECURE_NO_WARNINGS
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
#include "Attack.h"
#include "map.h"
#include "stage.h"
#include "debug_font.h"
#include "CTrap.h"
/*----------------------------------------------------------------------
　　定数
 ----------------------------------------------------------------------*/
#define TEXT_MAX (100)
//ミニマップ表示位置
#define MAP_POS_X (250)
#define MAP_POS_Y (250)
//プレイヤー位置からの未公開マップ開放範囲
#define MAP_ON_X (25)
#define MAP_ON_Y (25)
//エネミーアイコン表示範囲
#define ENEMY_ON_X (30)
#define ENEMY_ON_Y (30)
//オブジェクトアイコン表示範囲
#define OBJECT_ON_X (30)
#define OBJECT_ON_Y (30)
 /*----------------------------------------------------------------------
 　　グローバル変数
   ----------------------------------------------------------------------*/
LPD3DXFONT CUserinterface::g_pD3DXFont;

CUserinterface::TEXT CUserinterface::g_text;
int CUserinterface::g_Textbackground;
int CUserinterface::g_TextFramecount;
D3DXVECTOR2 time_pos;//時間で変わる位置
int add_time; //テキスト表示時間を+するとき足す
bool text_draw;

void CUserinterface::UI_Initialize(void)
{
	g_pD3DXFont = NULL;
	g_TextFramecount = 0;

	D3DXCreateFont(m_pD3DDevice, 40, 0, 0, 0, false,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "HGP創英角ﾎﾟｯﾌﾟ体", &g_pD3DXFont);

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
}

void CUserinterface::UI_NameInitialize(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
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
		//エンター待ちの時は何もしない
		
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
				//レベルアップしたら&複数攻撃中以外ならターンエンドにする
				if(getplayer->Get_PlayerTurn() != CPlayer::PLAYER_RANGEHIT_WAIT)
				{
					getplayer->Set_PlayerTurn(CPlayer::PLAYER_TURN_END);
				}
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
	UI_UpDraw();
	Map_Draw();
	UI_TextDraw();
	NanameYajirusi_Draw();
	ConditionIconDraw();
	CommandWindow_Draw();
	ItemWindow_Draw();
	StatusWindow_Draw();
}

// 後にデバッグだけでなくゲームない文章用も作成
void CUserinterface::UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...)
{
#if defined(_DEBUG) || defined(DEBUG)

	RECT rect = { x, y, WINDOW_WIDTH, WINDOW_HEIGHT };

	char Tbuffer[TEXT_MAX];
	va_list ap;
	// 可変長引数を１個の変数にまとめる
	va_start(ap, text);
	// まとめられた変数で処理する
	vsprintf_s(Tbuffer, TEXT_MAX, text, ap);
	va_end(ap);

	// 実験								文字数らしいが-1なら全部になる
	g_pD3DXFont->DrawTextA(NULL, Tbuffer, -1, &rect, DT_LEFT, color);

#endif // _DEBUG || DEBUG
}

void CUserinterface::UI_TextDraw2(int x, int y, D3DCOLOR color, const char* text, ...)
{
#if defined(_DEBUG) || defined(DEBUG)

	RECT rect = { x, y, WINDOW_WIDTH, WINDOW_HEIGHT };

	char Tbuffer[TEXT_MAX];  
	va_list ap;
	// 可変長引数を１個の変数にまとめる
	va_start(ap, text);
	// まとめられた変数で処理する
	vsprintf_s(Tbuffer, TEXT_MAX, text, ap);
	va_end(ap);



	// 実験								文字数らしいが-1なら全部になる
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
	
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, char* message)
{
	g_text.Age = 0;
	time_pos.y = 0;
	text_draw = false;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
	strcpy_s(g_text.free_message, MAX_NAME, message);
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type, char* rog_message)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	strcpy_s(g_text.name, MAX_NAME, CObject::Get_ObjectName(type));
	strcpy_s(g_text.item_log, MAX_NAME, rog_message);
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextCreateCondition(CHARATYPE chara, ACTTYPE act, int condition)
{
	g_text.Age = 0;
	time_pos.y = 0;
	text_draw = false;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
	switch (condition)
	{
	case C3DObj::POIZUN_CONDITION:
		strcpy_s(g_text.heal_log, MAX_NAME, "の毒が抜けた！");
		break;
	case C3DObj::SLEEP_CONDITION:
		strcpy_s(g_text.heal_log, MAX_NAME, "の目が覚めた！");
		break;
	case C3DObj::KURAYAMI_CONDITION:
		strcpy_s(g_text.heal_log, MAX_NAME, "の目が回復した！");
		break;
	case C3DObj::BAISOKU_CONDITION:
	case C3DObj::DONSOKU_CONDITION:
		strcpy_s(g_text.heal_log, MAX_NAME, "の速度が元に戻った！");
		break;
	}
}

void CUserinterface::UI_TextCreateDeleteItem(CHARATYPE chara, ACTTYPE act, int type, char* rog_message,char* action_message)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	strcpy_s(g_text.name, MAX_NAME, CObject::Get_ObjectName(type));
	strcpy_s(g_text.item_log, MAX_NAME, rog_message);
	strcpy_s(g_text.free_message, MAX_NAME, action_message);
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextCreateTrap(CHARATYPE chara, ACTTYPE act, int type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	strcpy_s(g_text.name, MAX_NAME, CTrap::Get_TrapName(type));
	strcpy_s(g_text.item_log, MAX_NAME, CTrap::Get_TrapEffect(type));
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextCreateTrapRog(CHARATYPE chara, ACTTYPE act, int type,char * rog)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	strcpy_s(g_text.name, MAX_NAME, CTrap::Get_TrapName(type));
	strcpy_s(g_text.item_log, MAX_NAME, rog);
	g_text.pos.x = TEXT_POSX;
	g_text.pos.y = TEXT_POSY;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
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
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextCreateBack(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type)
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
	strcpy_s(g_text.skill_log, MAX_NAME, "は壁に当たった！");
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
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
	g_text.act2 = 0;
	g_text.attribute_type = week_type;
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextCreate(char *skill_effect, int week_type, CHARATYPE chara, int act, CHARATYPE hitchara, int damage, int type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.damage = damage;
	g_text.act2 = act;
	g_text.attribute_type = week_type;
	strcpy_s(g_text.skill_log, MAX_NAME, skill_effect);
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextCreateSnatch(int week_type, CHARATYPE chara, int act, CHARATYPE hitchara, int damage, int type, int item_type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.damage = damage;
	g_text.act2 = act;
	g_text.attribute_type = week_type;
	strcpy_s(g_text.skill_log, MAX_NAME, "の盗む！");
	strcpy_s(g_text.snatch_log, MAX_NAME, CObject::Get_ObjectName(item_type));
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextPlayerSkill(char *skill_effect, int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type)
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
	strcpy_s(g_text.skill_log, MAX_NAME, skill_effect);
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextPlayerSkillCondition(char *skill_effect, char *skill_log,int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type)
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
	strcpy_s(g_text.skill_log, MAX_NAME, skill_effect);
	strcpy_s(g_text.free_message, MAX_NAME, skill_log);
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
}

void CUserinterface::UI_TextPlayerNodamageSkill(char *skill_effect, char *skill_log, int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int type)
{
	g_text.lv_up = false;
	g_text.Age = 0;
	text_draw = false;
	time_pos.y = 0;
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.act = act;
	g_text.attribute_type = week_type;
	strcpy_s(g_text.skill_log, MAX_NAME, skill_effect);
	strcpy_s(g_text.free_message, MAX_NAME, skill_log);
	if (type > 0)
	{
		g_text.type = type;
	}
	//エネミーデータから名前と経験値をもらうのでｰ1する
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
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	add_time = 0;
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
{
	//武器が両手から片手（逆も）になったらテキストで出すこと
	C3DObj *getplayer = CPlayer::Get_Player();
	//アイテム名描画
	//武器は１ページ目のみ描画
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW)
	{
		RightWindow_Draw();
		//上のウィンドウ
		Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.38f, 1.1f, 0.0f);
		//カーソル位置
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
		UI_TextDraw(145, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "道具");
		UI_TextDraw(275, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "能力");
		UI_TextDraw(145, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "足元");
		UI_TextDraw(275, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "操作");
		//下のウィンドウ
		Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 510.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.2f, 0.0f);
		//UI_TextDraw(145, 530, D3DCOLOR_RGBA(255, 255, 255, 255), "到達度");
		//UI_TextDraw(475, 530, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", ((CStage::Stage_GetLevel() * 10) / MAX_MAP) * 10);
		UI_TextDraw(145, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃力");
		if (getplayer->Get_WMode())
		{//上から攻撃力、防御力
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
		//盾アイコンは両手持ちの時は表示しない	
		if (!getplayer->Get_WMode() && CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_str > 0)
		{
			Sprite_Draw(TEX_SHELD_ICON, 320.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SHELD_ICON), (float)Texture_GetHeight(TEX_SHELD_ICON), 0.0f, 0.0f, 0.35f, 0.35f, 0.0f);
		}
		if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str > 0)
		{
			Sprite_Draw(TEX_RING_ICON, 380.0f, 525.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_RING_ICON), (float)Texture_GetHeight(TEX_RING_ICON), 0.0f, 0.0f, 0.35f, 0.35f, 0.0f);
		}
		UI_TextDraw(145, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "防御力");
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
		UI_TextDraw(595, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "満腹度");
		UI_TextDraw(925, 540, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d/%3d%%", getplayer->Get_PlayerOnaka(),MAX_ONAKA);
		UI_TextDraw(595, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "次のレベルまで");
		UI_TextDraw(1025, 595, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", getplayer->Get_PlayerNextExp(getplayer->Get_Lv()) - getplayer->Get_Exp() );
	}
}

void CUserinterface::ItemWindow_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_ITEM)
	{
		//右のウィンドウ
		RightWindow_Draw();
		//左のウィンドウ
		Sprite_Draw(TEX_WIDTH_WINDOW, 60.0f, 115.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.1f, 1.53f, D3DXToRadian(0.0f));
		//所持アイテムセット
		SetItemWindow();
		if (getplayer->Get_ItemOn())
		{//アイテム選択時のウィンドウ
			SetItemOnWindow();
		}
	}
}

void CUserinterface::RightWindow_Draw(void)
{//右のウィンドウ
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
		UI_TextDraw(875, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number);
	}
	else if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number == 0)
	{
		UI_TextDraw(875, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name);
	}
	//両手持ちの時は盾の場所に武器名65
	if (getplayer->Get_WMode())
	{
		if (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number > 0)
		{
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::WEPON_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_purasu_number);
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
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::SHELD_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_purasu_number);
		}
		else if (CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_purasu_number == 0)
		{
			UI_TextDraw(875, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::SHELD_NUMBER])->Wepon_name);
		}
	}
	if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number > 0)
	{
		UI_TextDraw(875, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%d", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::RING_NUMBER])->Wepon_name, CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number);
	}
	else if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number == 0)
	{
		UI_TextDraw(875, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[CPlayer::RING_NUMBER])->Wepon_name);
	}

}

void CUserinterface::NanameYajirusi_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	//斜め移動モードの時4方の斜めに矢印表示
	if (getplayer->Get_NanameFlag())
	{
		Sprite_Draw(TEX_PLAYER_YAJIRUSI3, 445, 125, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI3), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI3), 0.0f, 0.0f, 0.8f, 0.8f, D3DXToRadian(0.0f));
	}
}

void CUserinterface::ConditionIconDraw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (getplayer->Get_Condition() == C3DObj::KURAYAMI_CONDITION)
	{
		Sprite_Draw(TEX_PLAYER_MEKAKUSI_ICON, 595, 235, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_MEKAKUSI_ICON), (float)Texture_GetHeight(TEX_PLAYER_MEKAKUSI_ICON), 0.0f, 0.0f, 0.4f, 0.4f, D3DXToRadian(0.0f));
	}
	if (getplayer->Get_Condition() == C3DObj::POIZUN_CONDITION)
	{
		Sprite_Draw(TEX_PLAYER_POIZUN_ICON, 595, 235, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_POIZUN_ICON), (float)Texture_GetHeight(TEX_PLAYER_POIZUN_ICON), 0.0f, 0.0f, 0.4f, 0.4f, D3DXToRadian(0.0f));
	}
	if (getplayer->Get_Condition() == C3DObj::SLEEP_CONDITION)
	{
		Sprite_Draw(TEX_PLAYER_SLEEP_ICON, 595, 235, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_SLEEP_ICON), (float)Texture_GetHeight(TEX_PLAYER_SLEEP_ICON), 0.0f, 0.0f, 0.4f, 0.4f, D3DXToRadian(0.0f));
	}
	if (getplayer->Get_Str2() >= CHARGE_BUFF)
	{
		Sprite_Draw(TEX_PLAYER_CHARGE_ICON, 595, 235, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_CHARGE_ICON), (float)Texture_GetHeight(TEX_PLAYER_CHARGE_ICON), 0.0f, 0.0f, 0.4f, 0.4f, D3DXToRadian(0.0f));
	}
}


void CUserinterface::SetItemWindow(void)
{
	//アイテム名表示
	int i;
	int stock = 0;//アイテムが何個あるか数える
	C3DObj *getplayer = CPlayer::Get_Player();
	int m_WeponStock[MAX_WEPON] = { 0 };
	int m_ItemStock[MAX_ITEM] = { 0 };
	//アイテム名描画
	//武器は１ページ目のみ描画
	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = getplayer->Get_PlayerWeponStock(i);
		if (!getplayer->Get_NextItemPage())
		{
			if (CPlayer::GetPlayerWeponData(i)->wepon_purasu_number > 0)
			{
				UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%d", CWepon::Get_Wepon_Data(m_WeponStock[i])->Wepon_name, CPlayer::GetPlayerWeponData(i)->wepon_purasu_number);
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
	//アイテムが１ページ埋まってたら２ページ目があることを示すカーソルを出す
	if (m_WeponStock[2] != 0 && m_ItemStock[5] != 0)
	{
		Sprite_Draw(TEX_CURSOL, 510.0f, 125.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.23f, 0.23f, 0.0f);
		Sprite_Draw(TEX_CURSOL, 505.0f, 167.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.23f, 0.23f, D3DXToRadian(180.0f));
	}
	//カーソル位置
	//アイテム選択中はグレイにする
	if (getplayer->Get_ItemOn())
	{
		Sprite_Draw(TEX_CURSOL_GRY, 110.0f, 157.0f + (TEXT_Y * (getplayer->Get_CursorNumber())), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL_GRY), (float)Texture_GetHeight(TEX_CURSOL_GRY), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	else if (!getplayer->Get_ItemOn())
	{
		Sprite_Draw(TEX_CURSOL, 110.0f, 157.0f + (TEXT_Y * (getplayer->Get_CursorNumber())), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	//UI_TextDraw(145, 155 + (TEXT_Y * stock), D3DCOLOR_RGBA(255, 255, 255, 255), "戻る");
}

void CUserinterface::SetItemOnWindow(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	int add_cursor = 0;//もし2ページ目を開いてたら足す
	//アイテムタイプにより表示ウィンドウ、メッセージを変える
	//装備の時
	if (getplayer->Get_NextItemPage())
	{
		add_cursor += ((MAX_ITEM + MAX_WEPON) / 2);
	}
	if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::WEPON)
	{
		Sprite_Draw(TEX_MESSAGE_WINDOW, 555.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.25f, 0.7f, D3DXToRadian(0.0f));
		UI_TextDraw(620, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "説明");
	}
	else if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::ITEM)
	{
		Sprite_Draw(TEX_MESSAGE_WINDOW, 555.0f, 130.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.25f, 1.46f, D3DXToRadian(0.0f));
		UI_TextDraw(620, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "飲む");
		UI_TextDraw(620, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "置く");
		UI_TextDraw(620, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "説明");
	}
	//カーソル位置
	Sprite_Draw(TEX_CURSOL, 585.0f, 157.0f + (TEXT_Y * (getplayer->Get_TimeCursorNumber())), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	//フラグがONならアイテム説明文を表示
	if (getplayer->Get_ItemTips())
	{
		SetItemTips();
	}
}

void CUserinterface::SetItemTips(void)
{
	int i;
	int add_cursor = 0;//もし2ページ目を開いてたら足す
	C3DObj *getplayer = CPlayer::Get_Player();
	int m_WeponStock[MAX_WEPON] = { 0 };
	int m_ItemStock[MAX_ITEM] = { 0 };
	//アイテム名描画
	//武器は１ページ目のみ描画
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
	Sprite_Draw(TEX_MESSAGE_WINDOW, 555.0f, 329.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 0.755f, 2.1f, D3DXToRadian(0.0f));
	//アイテムタイプにより説明文を変える
	//装備の時
	if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::WEPON)
	{//後でenum定義しておくこと
		//武器の能力値を見て1以上あれば表示する
		switch (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_type)
		{
		case CWepon::TYPE_SWORD:
		case CWepon::TYPE_REIPIA:
		case CWepon::TYPE_MEISU:
			UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "片手武器");
			break;
		case CWepon::TYPE_BIGSWORD:
		case CWepon::TYPE_RANCE:
		case CWepon::TYPE_AX:
			UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "両手武器");
			break;
		case CWepon::TYPE_SHELD:
			if (getplayer->Get_WMode())
			{
				UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "非装備中");
			}
			else
			{
				UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "装備中");
			}
			break;
		case CWepon::TYPE_RING:
			UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "装飾品");
			break;
		}
		if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str > 0 && CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def > 0)
		{
			UI_TextDraw(765, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃力 %d  防御力 %d", CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str, CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def);
		}
		else if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str > 0)
		{
			UI_TextDraw(765, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃力 %d", CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_str);
		}
		else if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def > 0)
		{
			UI_TextDraw(765, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "防御力 %d", CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_def);
		}
		strchg(CWepon::Get_Wepon_Data(m_WeponStock[getplayer->Get_CursorNumber()])->Wepon_effect, "kk", "\n");
		//装備説明文
		UI_TextDraw(640, 410, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Data(m_WeponStock[getplayer->Get_CursorNumber()])->Wepon_effect);
		//3行目以降に武器特技名表示
		if (CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_type <= CWepon::TYPE_AX)
		{
			//スキル名を取得しその文字数+1文字分先に消費HPを書く
			int len = strlen(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_sikibetu_number)->Skill_name);
			UI_TextDraw(585, 500, D3DCOLOR_RGBA(255, 255, 255, 255), "スキル:%s", CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_sikibetu_number)->Skill_name);
			//スキルの消費HP表示 
			//↓は使用HP表示せずテキストのみ
			//UI_TextDraw(640 + ((len + 4)* 22), 500, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_sikibetu_number)->Skill_Hp);
			//使用HP表示
			UI_TextDraw(640 + ((len + 4) * 22), 500, D3DCOLOR_RGBA(255, 255, 255, 255), "消費HP:%3d", getplayer->Player_SkillHpGet());
			UI_TextDraw(640, 550, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(getplayer->Get_CursorNumber())->wepon_sikibetu_number)->Skill_effect);
		}
	}
	else if (getplayer->Get_PlayerItemStockType(getplayer->Get_CursorNumber() + add_cursor) == CPlayer::ITEM)
	{//スイッチじゃなくてアイテムストックのタイプ参照しろ！！
		UI_TextDraw(585, 355, D3DCOLOR_RGBA(255, 255, 255, 255), "使用アイテム");
		//↓改行文字があった場合改行を行う
		strchg(CObject::Get_Item_Data(m_ItemStock[getplayer->Get_CursorNumber() + add_cursor - MAX_WEPON])->object_tips, "kk", "\n");
		UI_TextDraw(640, 410, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", CObject::Get_Item_Data(m_ItemStock[getplayer->Get_CursorNumber() + add_cursor - MAX_WEPON])->object_tips);
	}
}

void CUserinterface::StatusWindow_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	int i;
	int m_WeponStock[MAX_WEPON] = { 0 };	//アイテム名描画
	//武器は１ページ目のみ描画
	for (i = 0; i < MAX_WEPON; i++)
	{
		m_WeponStock[i] = getplayer->Get_PlayerWeponStock(i);
	}

	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WINDOW_STATUS)
	{
		//右のウィンドウ
		RightWindow_Draw();
		//上のウィンドウ
		Sprite_Draw(TEX_WIDTH_WINDOW, 60.0f, 115.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 3.1f, 1.53f, D3DXToRadian(0.0f));
		//カーソル位置
		Sprite_Draw(TEX_CURSOL, 110.0f, 547.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_CURSOL), (float)Texture_GetHeight(TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
		UI_TextDraw(105, 155, D3DCOLOR_RGBA(255, 255, 255, 255), "%sの能力",getplayer->Get_Name());
		UI_TextDraw(125, 210, D3DCOLOR_RGBA(255, 255, 255, 255), "ちから    %d", getplayer->Get_Str());
		UI_TextDraw(125, 265, D3DCOLOR_RGBA(255, 255, 255, 255), "まもり    %d", getplayer->Get_Def());
		UI_TextDraw(125, 320, D3DCOLOR_RGBA(255, 255, 255, 255), "経験値    %d", getplayer->Get_Exp());
		switch (CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type)
		{
		case CWepon::TYPE_SWORD:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "武器タイプ   片手剣");
			break;
		case CWepon::TYPE_BIGSWORD:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "武器タイプ   両手剣");
			break;
		case CWepon::TYPE_REIPIA:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "武器タイプ   レイピア");
			break;
		case CWepon::TYPE_RANCE:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "武器タイプ   ランス");
			break;
		case CWepon::TYPE_MEISU:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "武器タイプ   メイス");
			break;
		case CWepon::TYPE_AX:
			UI_TextDraw(125, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "武器タイプ   オノ");
			break;
		}
		
		if (getplayer->Get_WMode())
		{
			UI_TextDraw(125, 430, D3DCOLOR_RGBA(255, 255, 255, 255), "武器攻撃力   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str);
			UI_TextDraw(125, 485, D3DCOLOR_RGBA(255, 255, 255, 255), "武器防御力   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def);
		}
		else
		{
			UI_TextDraw(125, 430, D3DCOLOR_RGBA(255, 255, 255, 255), "武器攻撃力   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_str + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_str);
			UI_TextDraw(125, 485, D3DCOLOR_RGBA(255, 255, 255, 255), "武器防御力   %d", CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def);
		}
		UI_TextDraw(145, 545, D3DCOLOR_RGBA(255, 255, 255, 255), "戻る");
		//相性表
		Sprite_Draw(TEX_WIDTH_WINDOW, 790.0f, 145.0f + (TEXT_Y + 130), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 2.43f, 0.553f, D3DXToRadian(0.0f));
		Sprite_Draw(TEX_AISYOU_ICON, 840.0f, 150.5f + (TEXT_Y + 130), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_AISYOU_ICON), (float)Texture_GetHeight(TEX_AISYOU_ICON), 0.0f, 0.0f, 0.33f, 0.31f, 0.0f);
		if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_SWORD || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_BIGSWORD)
		{
			Sprite_Draw(TEX_BUKI_MARU, 930.0f, 140.0f + (TEXT_Y + 130), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BUKI_MARU), (float)Texture_GetHeight(TEX_BUKI_MARU), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
		}
		else if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_REIPIA || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_RANCE)
		{
			Sprite_Draw(TEX_BUKI_MARU, 1020.0f, 260.0f + (TEXT_Y + 130), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BUKI_MARU), (float)Texture_GetHeight(TEX_BUKI_MARU), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
		}
		else if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_MEISU || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_AX)
		{
			Sprite_Draw(TEX_BUKI_MARU, 830.0f, 255.0f + (TEXT_Y + 130), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BUKI_MARU), (float)Texture_GetHeight(TEX_BUKI_MARU), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
		}
	}
}

void CUserinterface::UI_UpDraw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	//上のUI表示///////////////////
	//HPウィンドウ
	Sprite_Draw(TEX_WIDTH_WINDOW, 470, UI_Y - 7, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 2.1f, 0.16f, D3DXToRadian(0.0f));
	UI_TextDraw(570, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_Hp());
	UI_TextDraw(710, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_MaxHp());

	UI_TextDraw(500, UI_Y, D3DCOLOR_RGBA(255, 255, 20, 255), "HP        /");
	
	//HPバー
	Sprite_Draw(TEX_HPGAGE_REDBAR, 500, 50 + UI_Y, 0, 0, (float)Texture_GetWidth(TEX_HPGAGE_REDBAR), (float)Texture_GetHeight(TEX_HPGAGE_REDBAR), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_WAKU, 500, 50 + UI_Y, 0, 0, (float)Texture_GetWidth(TEX_HPGAGE_WAKU), (float)Texture_GetHeight(TEX_HPGAGE_WAKU), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	if (getplayer->Get_Condition() != C3DObj::POIZUN_CONDITION && getplayer->Get_PlayerOnaka() > 0)
	{
		Sprite_Draw(TEX_HPGAGE_GREENBAR, 512, 60 + UI_Y, 0, 0, 250 * getplayer->Get_Hp() / getplayer->Get_MaxHp(), (float)Texture_GetHeight(TEX_HPGAGE_GREENBAR), 0.0f, 0.0f, 0.91f, 0.5f, 0.0f);
	}
	else if(getplayer->Get_Condition() == C3DObj::POIZUN_CONDITION)
	{
		Sprite_Draw(TEX_HPGAGE_POIZUNBAR, 512, 60 + UI_Y, 0, 0, 250 * getplayer->Get_Hp() / getplayer->Get_MaxHp(), (float)Texture_GetHeight(TEX_HPGAGE_GREENBAR), 0.0f, 0.0f, 0.91f, 0.5f, 0.0f);
	}
	else if (getplayer->Get_PlayerOnaka() == 0)
	{
		Sprite_Draw(TEX_HPGAGE_ONAKA, 512, 60 + UI_Y, 0, 0, 250 * getplayer->Get_Hp() / getplayer->Get_MaxHp(), (float)Texture_GetHeight(TEX_HPGAGE_GREENBAR), 0.0f, 0.0f, 0.91f, 0.5f, 0.0f);
	}
	//レベルとフロアウィンドウ
	Sprite_Draw(TEX_WIDTH_WINDOW, 240, UI_Y - 7, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 1.2f, 0.16f, D3DXToRadian(0.0f));
	UI_TextDraw(100+140, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", CStage::Stage_GetLevel());
	UI_TextDraw(120+150, UI_Y, D3DCOLOR_RGBA(255, 255, 20, 255), "  F");

	UI_TextDraw(290+100, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", getplayer->Get_Lv());
	UI_TextDraw(220+100, UI_Y, D3DCOLOR_RGBA(255, 255, 20, 255), "  Lv");

	//UI_TextDraw(910, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%4d", getplayer->Get_Gold());
	//UI_TextDraw(980, UI_Y, D3DCOLOR_RGBA(0, 255, 200, 255), "  G");
	//武器アイコンウィンドウ
	Sprite_Draw(TEX_WIDTH_WINDOW, 835, UI_Y - 7, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 1.6f, 0.16f, D3DXToRadian(0.0f));
	//武器アイコン表示
	switch (CPlayer::GetPlayerWeponData(0)->wepon_type)
	{
	case CWepon::TYPE_SWORD:
		Sprite_Draw(TEX_SWORD_ICON, 810 + 30, UI_Y -10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SWORD_ICON), (float)Texture_GetHeight(TEX_SWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		break;
	case CWepon::TYPE_BIGSWORD:
		Sprite_Draw(TEX_BIGSWORD_ICON, 800 + 30, UI_Y - 15, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		break;
	case CWepon::TYPE_REIPIA:
		Sprite_Draw(TEX_REIPIA_ICON, 800 + 30, UI_Y - 10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		break;
	case CWepon::TYPE_RANCE:
		Sprite_Draw(TEX_RANCE_ICON, 800 + 30, UI_Y - 15, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		break;
	case CWepon::TYPE_MEISU:
		Sprite_Draw(TEX_MEISU_ICON, 800 + 30, UI_Y - 10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		break;
	case CWepon::TYPE_AX:
		Sprite_Draw(TEX_AX_ICON, 790 + 30, UI_Y - 15, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		break;
	}
	//攻撃力表示
	
	//UI_TextDraw(850, UI_Y + 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d",CPlayer::GetPlayerWeponData(0)->wepon_str + CPlayer::GetPlayerWeponData(1)->wepon_str + CPlayer::GetPlayerWeponData(2)->wepon_str + getplayer->Get_Str());
	UI_TextDraw(850 + 30, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", CPlayer::GetPlayerWeponData(0)->wepon_str + CPlayer::GetPlayerWeponData(1)->wepon_str + CPlayer::GetPlayerWeponData(2)->wepon_str);

	Sprite_Draw(TEX_SHELD_ICON, 960, UI_Y - 10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SHELD_ICON), (float)Texture_GetHeight(TEX_SHELD_ICON), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
	UI_TextDraw(1000 + 30, UI_Y, D3DCOLOR_RGBA(255, 255, 255, 255), "%3d", CPlayer::GetPlayerWeponData(0)->wepon_def + CPlayer::GetPlayerWeponData(1)->wepon_def + CPlayer::GetPlayerWeponData(2)->wepon_def + getplayer->Get_Def());

	/*
	//相性武器表示
	if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_SWORD || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_BIGSWORD)
	{
		Sprite_Draw(TEX_MEISU_ICON, 1070, UI_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_AX_ICON, 1140, UI_Y - 5, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_PLAYER_YAJIRUSI, 1195, UI_Y - 10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI), 0.0f, 0.0f, 0.1f, 0.08f, 0.0f);

		Sprite_Draw(TEX_REIPIA_ICON, 1070, UI_Y + 60, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.37f, 0.37f, 0.0f);
		Sprite_Draw(TEX_RANCE_ICON, 1140, UI_Y - 5 + 60, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.42f, 0.42f, 0.0f);
		Sprite_Draw(TEX_PLAYER_YAJIRUSI2, 1195, UI_Y + 68, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI), 0.0f, 0.0f, 0.1f, 0.08f, 0.0f);
	}
	if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_REIPIA || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_RANCE)
	{
		Sprite_Draw(TEX_SWORD_ICON, 1060, UI_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SWORD_ICON), (float)Texture_GetHeight(TEX_SWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_BIGSWORD_ICON, 1135, UI_Y - 5, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_PLAYER_YAJIRUSI, 1195, UI_Y - 10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI), 0.0f, 0.0f, 0.1f, 0.08f, 0.0f);

		Sprite_Draw(TEX_MEISU_ICON, 1070, UI_Y + 60, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_AX_ICON, 1140, UI_Y - 5 + 60, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_PLAYER_YAJIRUSI2, 1195, UI_Y + 68, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI), 0.0f, 0.0f, 0.1f, 0.08f, 0.0f);
	}
	if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_MEISU || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_AX)
	{
		Sprite_Draw(TEX_REIPIA_ICON, 1070, UI_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.37f, 0.37f, 0.0f);
		Sprite_Draw(TEX_RANCE_ICON, 1140, UI_Y - 5, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.42f, 0.42f, 0.0f);
		Sprite_Draw(TEX_PLAYER_YAJIRUSI, 1195, UI_Y - 10, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI), 0.0f, 0.0f, 0.1f, 0.08f, 0.0f);

		Sprite_Draw(TEX_SWORD_ICON, 1060, UI_Y + 60, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_SWORD_ICON), (float)Texture_GetHeight(TEX_SWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_BIGSWORD_ICON, 1135, UI_Y - 5 + 60, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BIGSWORD_ICON), (float)Texture_GetHeight(TEX_BIGSWORD_ICON), 0.0f, 0.0f, 0.4f, 0.4f, 0.0f);
		Sprite_Draw(TEX_PLAYER_YAJIRUSI2, 1195, UI_Y + 68, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_YAJIRUSI), (float)Texture_GetHeight(TEX_PLAYER_YAJIRUSI), 0.0f, 0.0f, 0.1f, 0.08f, 0.0f);
	}*/
	if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_KEY_INPUT && !g_text.alive && !getplayer->Get_TurboMode() && !getplayer->Get_Hold())
	{
		//相性表
		Sprite_Draw(TEX_WIDTH_WINDOW, 790.0f + 65, 145.0f + (TEXT_Y + 290), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_WIDTH_WINDOW), (float)Texture_GetHeight(TEX_WIDTH_WINDOW), 0.0f, 0.0f, 2.43f, 0.553f, D3DXToRadian(0.0f));
		Sprite_Draw(TEX_AISYOU_ICON, 840.0f + 65, 150.5f + (TEXT_Y + 290), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_AISYOU_ICON), (float)Texture_GetHeight(TEX_AISYOU_ICON), 0.0f, 0.0f, 0.33f, 0.31f, 0.0f);
		if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_SWORD || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_BIGSWORD)
		{
			Sprite_Draw(TEX_BUKI_MARU, 930.0f + 65, 140.0f + (TEXT_Y + 290), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BUKI_MARU), (float)Texture_GetHeight(TEX_BUKI_MARU), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
		}
		else if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_REIPIA || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_RANCE)
		{
			Sprite_Draw(TEX_BUKI_MARU, 1020.0f + 65, 260.0f + (TEXT_Y + 290), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BUKI_MARU), (float)Texture_GetHeight(TEX_BUKI_MARU), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
		}
		else if (CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_MEISU || CPlayer::GetPlayerWeponData(0)->wepon_type == CWepon::TYPE_AX)
		{
			Sprite_Draw(TEX_BUKI_MARU, 830.0f + 65, 255.0f + (TEXT_Y + 290), 0.0f, 0.0f, (float)Texture_GetWidth(TEX_BUKI_MARU), (float)Texture_GetHeight(TEX_BUKI_MARU), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
		}
	}
}

void CUserinterface::Map_Draw(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	C3DObj *enemy;
	C3DObj *object;
	C3DObj *wepon;
	C3DObj *trap;
	//ウィンドウを開いてるとき以外にミニマップ描画///////////////////
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
				{//↓このコメントを出すとマップが全開になる
					CMap::Map_SetData(x, y, D3DCOLOR_RGBA(255, 255, 255, 255));
					if (CMap::Map_GetData(x, y).alpha == 0 && (getplayer->Get_Position().x + MAP_POS_X - MAP_ON_X) < (CMap::Map_GetData(x, y).pos.x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + MAP_ON_X > (CMap::Map_GetData(x, y).pos.x + MAP_POS_X)
						&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - MAP_ON_X < (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + MAP_ON_Y >(CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y)
					{//マップを踏んだら可視化する
						if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
						{
							CMap::Map_SetData(x, y, 255);
						}
					}
					if (CMap::Map_GetData(x, y).alpha != 0)
					{
						Sprite_Draw(TEX_BLUE, (CMap::Map_GetData(x, y).pos.x + MAP_POS_X) * 1, (CMap::Map_GetData(x, y).pos.z * -1) + MAP_POS_Y, CMap::Map_GetData(x, y).alpha);
					}
				}
			}
		}
		//プレイヤーアイコン表示
		Sprite_Draw(TEX_PLAYER_ICON, getplayer->Get_Position().x + MAP_POS_X, (getplayer->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
		//Sprite_Draw(TEX_PLAYER_ICON, getplayer->Get_Position().x + MAP_POS_X, (getplayer->Get_Position().z*-1) + MAP_POS_Y, 0, 0, 4, 4);
		for (i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{//範囲内にいたらエネミーアイコン表示 if文コメントアウトで全表示
		//		if ((getplayer->Get_Position().x + MAP_POS_X - ENEMY_ON_X) < (enemy->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + ENEMY_ON_X > (enemy->Get_Position().x + MAP_POS_X)
		//			&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - ENEMY_ON_Y < (enemy->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + ENEMY_ON_Y >(enemy->Get_Position().z*-1) + MAP_POS_Y)
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						Sprite_Draw(CTexture::TEX_ENEMY_ICON, enemy->Get_Position().x + MAP_POS_X, (enemy->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
					}
				}
				if (getplayer->Get_EnemyONFlag())
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						Sprite_Draw(CTexture::TEX_ENEMY_ICON, enemy->Get_Position().x + MAP_POS_X, (enemy->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
					}
				}
			}
			object = CObject::Get_Object(i);
			//オブジェクトの判定
			if (object)
			{//オブジェクトは一度マッピングさせたら消えるまで表示させる　if文コメントアウトで全表示
			//	if ((getplayer->Get_Position().x + MAP_POS_X - OBJECT_ON_X) < (object->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + OBJECT_ON_X > (object->Get_Position().x + MAP_POS_X)
			//		&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - OBJECT_ON_Y < (object->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + OBJECT_ON_Y >(object->Get_Position().z*-1) + MAP_POS_Y)
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						object->Set_MapDrawFlag(true);
					}
				}
				if (object->Get_MapDrawFlag())
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						if (object->m_ObjectType == CObject::TYPE_LADDER)
						{
							Sprite_Draw(CTexture::TEX_LADDER_ICON, object->Get_Position().x + MAP_POS_X, (object->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
						}
						if (object->m_ObjectType != CObject::TYPE_LADDER)
						{
							Sprite_Draw(CTexture::TEX_ITEM_ICON, object->Get_Position().x + MAP_POS_X, (object->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
						}
					}
				}
			}
			wepon = CWepon::Get_Wepon(i);
			//武器の判定
			if (wepon)
			{//武器は一度マッピングさせたら消えるまで表示させる　if文コメントアウトで全表示
		//	if ((getplayer->Get_Position().x + MAP_POS_X - OBJECT_ON_X) < (wepon->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + OBJECT_ON_X > (wepon->Get_Position().x + MAP_POS_X)
		//	 	&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - OBJECT_ON_Y < (wepon->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + OBJECT_ON_Y >(wepon->Get_Position().z*-1) + MAP_POS_Y)
				{
				 if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
				 {
					 wepon->Set_MapDrawFlag(true);
				 }
				}
				if (wepon->Get_MapDrawFlag())
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						Sprite_Draw(CTexture::TEX_ITEM_ICON, wepon->Get_Position().x + MAP_POS_X, (wepon->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
					}
				}
			}
			trap = CTrap::Get_Trap(i);
			//武器の判定
			if (trap)
			{//武器は一度マッピングさせたら消えるまで表示させる　if文コメントアウトで全表示
			 //	if ((getplayer->Get_Position().x + MAP_POS_X - OBJECT_ON_X) < (trap->Get_Position().x + MAP_POS_X) && (getplayer->Get_Position().x + MAP_POS_X) + OBJECT_ON_X > (trap->Get_Position().x + MAP_POS_X)
			 //	 	&& (getplayer->Get_Position().z*-1) + MAP_POS_Y - OBJECT_ON_Y < (trap->Get_Position().z*-1) + MAP_POS_Y && (getplayer->Get_Position().z*-1) + MAP_POS_Y + OBJECT_ON_Y >(trap->Get_Position().z*-1) + MAP_POS_Y)
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						trap->Set_MapDrawFlag(true);
					}
				}
				if (trap->Get_MapDrawFlag())
				{
					if (getplayer->Get_Condition() != C3DObj::KURAYAMI_CONDITION)
					{
						Sprite_Draw(CTexture::TEX_TRAP_ICON, trap->Get_Position().x + MAP_POS_X, (trap->Get_Position().z*-1) + MAP_POS_Y, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_PLAYER_ICON), (float)Texture_GetHeight(TEX_PLAYER_ICON), 0.0f, 0.0f, 0.78f, 0.78f, 0.0f);
					}
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
		//下のテキスト描画
		if (g_text.alive)
		{
			switch (g_text.chara)
			{
			case PLAYERCHARA:
				/*
				if (g_text.act == REGULARATTACK || g_text.act == SKILLATTACK || g_text.act == BACKENEMY_ATTACK)
				{
					if (g_text.Age <= ATTACK_END + 30 && !g_text.lv_up)
					{
						Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
					}
					else if(getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END)
					{
						Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
					}
					else if (g_text.lv_up && g_text.Age > ATTACK_END)
					{
						Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
					}
				}
				else
				{
					Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				}
				*/
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				switch (g_text.act)
				{
				case REGULARATTACK://キー入力させるならコメントアウト解除
					//このifはレベルアップ時のテキスト
					if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END /*&& getplayer->Get_EnterFlag() */ || text_draw)
					{//レベルアップ時はレベルアップ表示までターンを止めること
						text_draw = true;
						time_pos.y -= 2;
						//ウィンドウ上部まで文字をスクロールし、上部まで来たら消す
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの攻撃！", g_text.player_name);
						}
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y + 50)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.name);
								//相性によってダメージ文字色変更
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
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
							}
						}
						//ここからウィンドウに表示する文字、自分の場所までスクロールする
						if (g_text.pos.y + time_pos.y + 100 < g_text.pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
						}
						if (g_text.Age >= ATTACK_END + 30)//1行目が出てから少し待ってから出す
						{
							if (g_text.Age == ATTACK_END + 30)
							{
								if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_sikibetu_number == CWepon::TYPE_SIAWASE_RING)
								{
									g_text.exp = (int)((g_text.exp * (SIAWASE_RING + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number * 0.1f)));
								}
								g_text.lv_up = getplayer->ExpGoldCheck(g_text.exp, g_text.gold);
								add_time += 40;//表示時間延長
							}
							if (g_text.pos.y + time_pos.y + 150 < g_text.pos.y + 50)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 150 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
							}

						}
						if (g_text.Age >= ATTACK_END + 50 && g_text.lv_up)
						{//レベルアップしたら
							if (g_text.Age == ATTACK_END + 50)
							{
								add_time += 80;//表示時間延長
							}
							if (g_text.pos.y + time_pos.y + 200 < g_text.pos.y + 100)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 200 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
							}
						}
					}
					else if (g_text.Age <= ATTACK_END + 1)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの攻撃！", g_text.player_name);
						if (g_text.Age > 30)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else if (g_text.damage > 0)
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.name);
								//相性によってダメージ文字色変更
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
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
							}
							if (getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
							{
								UI_TextDraw((int)(g_text.pos.x + 470), (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "▼");
							}
						}
					}
					break;
				case ENEMY_POIZUN_DEATH://キー入力させるならコメントアウト解除
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは毒で倒れた！", g_text.name);
					if (g_text.Age == ITEM_EFFECT_FRAME)
					{
						if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_sikibetu_number == CWepon::TYPE_SIAWASE_RING)
						{
							g_text.exp = (int)((g_text.exp * (SIAWASE_RING + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number * 0.1f)));
						}
						g_text.lv_up = getplayer->ExpGoldCheck(g_text.exp, g_text.gold);
					}
					if (g_text.Age > ITEM_EFFECT_FRAME)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
					}
					//レベルアップしたら
					if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END /*&& getplayer->Get_EnterFlag() */ || text_draw)
					{//レベルアップ時はレベルアップ表示までターンを止めること
						if (g_text.Age == ATTACK_END + 1)
						{
							add_time += 40;//表示時間延長
						}
						text_draw = true;
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
					}
					break;
				case SKILLATTACK://キー入力させるならコメントアウト解除
					if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END /*&& getplayer->Get_EnterFlag() */ || text_draw)
				{//レベルアップ時はレベルアップ表示までターンを止めること
					text_draw = true;
					time_pos.y -= 2;
					//ウィンドウ上部まで文字をスクロールし、上部まで来たら消す
					if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの%s！", g_text.player_name, g_text.skill_log);
					}
					if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y + 50)
					{
						if (g_text.damage == 0)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
						}
						else
						{
							//文字列の長さを調べその長さ分次の文のXを足す
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
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.name);
							//相性によってダメージ文字色変更
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
							UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
						}
					}
					//ここからウィンドウに表示する文字、自分の場所までスクロールする
					if (g_text.pos.y + time_pos.y + 100 < g_text.pos.y)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
					}
					if (g_text.Age >= ATTACK_END + 30)//1行目が出てから少し待ってから出す
					{
						if (g_text.Age == ATTACK_END + 30)
						{
							if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_sikibetu_number == CWepon::TYPE_SIAWASE_RING)
							{//しあわせの指輪は+値1につき0.1+
								g_text.exp = (int)((g_text.exp * (SIAWASE_RING + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number * 0.1f)));
							}
							g_text.lv_up = getplayer->ExpGoldCheck(g_text.exp, g_text.gold);
							add_time += 40;//表示時間延長
						}
						if (g_text.pos.y + time_pos.y + 150 < g_text.pos.y + 50)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 150 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
						}
					}
					if (g_text.Age >= ATTACK_END + 50 && g_text.lv_up)
					{//レベルアップしたら
						if (g_text.Age == ATTACK_END + 50)
						{
							add_time += 80;//表示時間延長
						}
						if (g_text.pos.y + time_pos.y + 200 < g_text.pos.y + 100)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 200 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
						}
					}
				}
					else if (g_text.Age <= ATTACK_END + 1)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの%s！", g_text.player_name, g_text.skill_log);
						if (g_text.Age > 30)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else if (g_text.damage > 0)
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.name);
								//相性によってダメージ文字色変更
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
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
							}
							if (getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
							{
								UI_TextDraw((int)(g_text.pos.x + 470), (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "▼");
							}
						}
					}
					break;
				case SKILL_CONDITIONATTACK://キー入力させるならコメントアウト解除
					if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END /*&& getplayer->Get_EnterFlag() */ || text_draw)
					{//レベルアップ時はレベルアップ表示までターンを止めること
						text_draw = true;
						time_pos.y -= 2;
						//ウィンドウ上部まで文字をスクロールし、上部まで来たら消す
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの%s！", g_text.player_name, g_text.skill_log);
						}
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y + 50)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.name);
								//相性によってダメージ文字色変更
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
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
							}
						}
						//ここからウィンドウに表示する文字、自分の場所までスクロールする
						if (g_text.pos.y + time_pos.y + 100 < g_text.pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
						}
						if (g_text.Age >= ATTACK_END + 30)//1行目が出てから少し待ってから出す
						{
							if (g_text.Age == ATTACK_END + 30)
							{
								if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_sikibetu_number == CWepon::TYPE_SIAWASE_RING)
								{//しあわせの指輪は+値1につき0.1+
									g_text.exp = (int)((g_text.exp * (SIAWASE_RING + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number * 0.1f)));
								}
								g_text.lv_up = getplayer->ExpGoldCheck(g_text.exp, g_text.gold);
								add_time += 40;//表示時間延長
							}
							if (g_text.pos.y + time_pos.y + 150 < g_text.pos.y + 50)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 150 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
							}

						}
						if (g_text.Age >= ATTACK_END + 50 && g_text.lv_up)
						{//レベルアップしたら
							if (g_text.Age == ATTACK_END + 50)
							{
								add_time += 80;//表示時間延長
							}
							if (g_text.pos.y + time_pos.y + 200 < g_text.pos.y + 100)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 200 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
							}
						}
					}
					else if (g_text.Age <= ATTACK_END + 1)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの%s！", g_text.player_name, g_text.skill_log);
						if (g_text.Age > 30)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else if (g_text.damage > 0)
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.name);
								//相性によってダメージ文字色変更
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
								UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 26), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
							}
							if (getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
							{
								UI_TextDraw((int)(g_text.pos.x + 470), (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "▼");
							}
						}
						//敵が死んでなければ状態異常メッセージ
						if (g_text.Age > 40 && !getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.free_message);
						}
					}
					break;
				case BACKENEMY_ATTACK://キー入力させるならコメントアウト解除
					if (getplayer->Get_RivalFlag() && g_text.Age > ATTACK_END /*&& getplayer->Get_EnterFlag() */ || text_draw)
					{//レベルアップ時はレベルアップ表示までターンを止めること
						text_draw = true;
						time_pos.y -= 2;
						//ウィンドウ上部まで文字をスクロールし、上部まで来たら消す
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name,g_text.skill_log);
						}
						if (g_text.pos.y - 19 < g_text.pos.y + time_pos.y + 50)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに%dダメージ与えた", g_text.name, g_text.damage);
							}
						}
						//ここからウィンドウに表示する文字、自分の場所までスクロールする
						if (g_text.pos.y + time_pos.y + 100 < g_text.pos.y)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを倒した！", g_text.name);
						}
						if (g_text.Age >= ATTACK_END + 30)//1行目が出てから少し待ってから出す
						{
							if (g_text.Age == ATTACK_END + 30)
							{
								if (CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_sikibetu_number == CWepon::TYPE_SIAWASE_RING)
								{
									g_text.exp = (int)((g_text.exp * (SIAWASE_RING + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_purasu_number * 0.1f)));
								}
								g_text.lv_up = getplayer->ExpGoldCheck(g_text.exp, g_text.gold);
								add_time += 40;//表示時間延長
							}
							if (g_text.pos.y + time_pos.y + 150 < g_text.pos.y + 50)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 150 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "経験値%d獲得", g_text.exp);
							}
		
						}
						if (g_text.Age >= ATTACK_END + 50 && g_text.lv_up)
						{//レベルアップしたら
							if (g_text.Age == ATTACK_END + 50)
							{
								add_time += 80;//表示時間延長
							}
							if (g_text.pos.y + time_pos.y + 200 < g_text.pos.y + 100)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
							}
							else
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 200 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sはレベル%dになった！", g_text.player_name, getplayer->Get_Lv());
							}
						}
					}
					else if (g_text.Age <= ATTACK_END + 1)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
						if (g_text.Age > 30)
						{
							if (g_text.damage == 0)
							{
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
							}
							else if (g_text.damage > 0)
							{
								//文字列の長さを調べその長さ分次の文のXを足す
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
								UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに%dダメージ与えた", g_text.name, g_text.damage);
							}
							if (getplayer->Get_RivalFlag() && !getplayer->Get_EnterFlag())
							{
								UI_TextDraw((int)(g_text.pos.x + 470), (int)(g_text.pos.y + 100), D3DCOLOR_RGBA(255, 255, 255, 255), "▼");
							}
						}
					}
					break;
				case NODAMAGE_SKILL:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの%s！", g_text.player_name, g_text.skill_log);
					if (g_text.Age > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.free_message);
					}
					break;
				case GOLADDER:
					UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "先に進みますか？");
					UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "⇒ 進む\n\n   そのまま");
					break;

				case NOTGOLADDER:
					UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "先に進みますか？");
					UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "    進む\n\n⇒そのまま");
					break;

				case DESTROY:
					UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "%sはやられてしまった▼", g_text.player_name);
					break;
				case FREE_MESSAGE:
					UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "%s", g_text.free_message);
					break;

				case ITEM:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを手に入れた", g_text.name);
					break;

				case NOITEM:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの上に乗った", g_text.name);
					break;

				case ITEM_EFFECT:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを飲んだ", g_text.name);
					if (g_text.Age > ITEM_EFFECT_FRAME)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s", g_text.item_log);
					}
					break;
				case ITEM_DELETE:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.item_log,g_text.free_message);
					if (g_text.Age > ITEM_EFFECT_FRAME)
					{//アイテムが無かったら効果がないと表示
						if (getplayer->Get_PlayerItemStock(0) == 0)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは道具を持っていなかった！", g_text.player_name, g_text.name);
						}
						else
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sが消えてしまった！", g_text.name);
						}
					}
					break;
				case TRAP_EFFECT:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを踏んでしまった！", g_text.name);
					if (g_text.Age > ITEM_EFFECT_FRAME)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s", g_text.item_log);
					}
					break;
				case TRAP_NOEFFECT:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを踏んでしまった！", g_text.name);
					if (g_text.Age > ITEM_EFFECT_FRAME)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "しかし罠は発動しなかった！");
					}
					break;
				case ITEM_ASIMOTO:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを足元に置いた", g_text.name);
					break;

				case NOITEM_ASIMOTO:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "この場所には置けない！");
					break;

				case WEPON_GET:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%dを装備した", g_text.name, g_text.purasu_number);
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを装備した", g_text.name);
					}
					break;
				case WEPON_W_GETCHENGE:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%dを装備した", g_text.name, g_text.purasu_number);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "盾をしまった");
						}
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを装備した", g_text.name);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "盾をしまった");
						}
					}
					break;
				case WEPON_NORMAL_GETCHENGE:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%dを装備した", g_text.name, g_text.purasu_number);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "しまっていた盾を装備した");
						}
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sを装備した", g_text.name);
						if (g_text.Age > ITEM_EFFECT_FRAME)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "しまっていた盾を装備した");
						}
					}
					break;
				case SHELD_W_GET:
					if (g_text.purasu_number > 0)
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s＋%dをしまった", g_text.name, g_text.purasu_number);
					}
					else
					{
						UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sをしまった", g_text.name);
					}
					break;
				case CONDITION_HEAL:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.player_name,g_text.heal_log);
					break;
				case SLEEP_WAIT:
					UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは眠っている…", g_text.player_name);
					break;
				}
				break;

			case ENEMY:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				switch (g_text.act2)
				{
				case CAttack::NORMAL_ATTACK_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sからの攻撃！", g_text.name);
					if (g_text.Age > 30)
					{
						if (g_text.damage == 0)
						{
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
						}
						else
						{
							//文字列の長さを調べその長さ分次の文のXを足す
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
							UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.player_name);
							//相性によってダメージ文字色変更
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
							UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 42), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
						}
					}
					break;
				case CAttack::HIGH_ATTACK_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name,g_text.skill_log);
					SkillAttackLog();
					break;
				case CAttack::HITYU_ATTACK_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					SkillAttackLog();
					break;
				case CAttack::HANI_FURASHU_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					SkillAttackLog();
					break;
				case CAttack::KIAIDAME_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					if (g_text.Age > 30)
					{
						UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "%sの次の攻撃力が上がった！", g_text.name);
					}
					break;
				case CAttack::SUNAKEMURI_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					if (g_text.Age > 30)
					{
						UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは目が見えなくなってしまった！", g_text.player_name);
					}
					break;
				case CAttack::KOMORIUTA_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					if (g_text.Age > 30)
					{
						UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは眠ってしまった", g_text.player_name);
					}
					break;
				case CAttack::POIZUN_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					if (g_text.Age > 30)
					{
						UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは毒に侵された！", g_text.player_name);
					}
					break;
				case CAttack::TOUZOKU_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					if (g_text.Age > 30)
					{
						UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "%sは%sを盗まれた！", g_text.player_name, g_text.snatch_log);
					}
					break;
				case CAttack::NOTOUZOKU_SKILL:
					UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%s%s", g_text.name, g_text.skill_log);
					if (g_text.Age > 30)
					{
						UI_TextDraw((int)(g_text.pos.x), (int)(g_text.pos.y + 50), D3DCOLOR_RGBA(255, 255, 255, 255), "しかし%sはアイテムを持っていなかった！", g_text.player_name);
					}
					break;
				}
			}
		}
	}
}

void CUserinterface::SkillAttackLog(void)
{
	if (g_text.Age > 30)
	{
		if (g_text.damage == 0)
		{
			UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "攻撃は外れてしまった！");
		}
		else
		{
			//文字列の長さを調べその長さ分次の文のXを足す
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
			UI_TextDraw((int)g_text.pos.x, (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "%sに", g_text.player_name);
			//相性によってダメージ文字色変更
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
			UI_TextDraw((int)g_text.pos.x + ((len * 22)) + (add_x * 42), (int)(g_text.pos.y + 50 + time_pos.y), D3DCOLOR_RGBA(255, 255, 255, 255), "ダメージ与えた!");
		}
	}
}

void CUserinterface::strchg(char *buf, const char *str1, const char *str2)
{
	char tmp[1024 + 1];
	char *p;

	while ((p = strstr(buf, str1)) != NULL) {
		/* 見つからなくなるまで繰り返す
		pは旧文字列の先頭を指している */
		*p = '\0'; /* 元の文字列を旧文字列の直前で区切って */
		p += strlen(str1);  /* ポインタを旧文字列の次の文字へ */
		strcpy(tmp, p);             /* 旧文字列から後を保存 */
		strcat(buf, str2);  /* 新文字列をその後につなぎ */
		strcat(buf, tmp);   /* さらに残りをつなぐ */
	}
}