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
/*----------------------------------------------------------------------
　　定数
 ----------------------------------------------------------------------*/
#define TEXT_MAX (100)

 /*----------------------------------------------------------------------
 　　グローバル変数
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
	// テクスチャの読み込み予約をしておく
	//g_Textbackground = CTexture::Texture_SetLoadFile("asset/texture/white.png", 64, 64);

	D3DXCreateFont(m_pD3DDevice, 40, 0, 0, 0, false,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH, "HGP創英角ﾎﾟｯﾌﾟ体", &g_pD3DXFont);

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
	// ワールド座標変換行列の作成と設定
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	C3DObj *getplayer = CPlayer::Get_Player();
	//UI_TextCreate(CUserinterface::SMALEMAP, CUserinterface::ACTTYPENONE);
	//上のUI表示///////////////////
	UI_TextDraw(570, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_Hp());
	UI_TextDraw(710, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%.0f", getplayer->Get_MaxHp());

	UI_TextDraw(500, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "HP        /");

	//HPバー
	Sprite_Draw(TEX_HPGAGE_REDBAR, 500,50, 0, 0, Texture_GetWidth(TEX_HPGAGE_REDBAR), Texture_GetHeight(TEX_HPGAGE_REDBAR), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_WAKU, 500, 50, 0, 0, Texture_GetWidth(TEX_HPGAGE_WAKU), Texture_GetHeight(TEX_HPGAGE_WAKU), 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	Sprite_Draw(TEX_HPGAGE_GREENBAR, 512, 60, 0, 0, 250 * getplayer->Get_Hp() / getplayer->Get_MaxHp(), Texture_GetHeight(TEX_HPGAGE_GREENBAR), 0.0f, 0.0f, 0.91f, 0.5f, 0.0f);

	UI_TextDraw(80, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", CStage::Stage_GetLevel());
	UI_TextDraw(100, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "  F");

	UI_TextDraw(300, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%2d", getplayer->Get_Lv());
	UI_TextDraw(200, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "  Lv");

	UI_TextDraw(910, 10, D3DCOLOR_RGBA(255, 255, 255, 255), "%4d", getplayer->Get_Gold());
	UI_TextDraw(980, 10, D3DCOLOR_RGBA(0, 255, 200, 255), "  G");
	///////////////////

	//ミニマップ描画///////////////////
	int x = 0, y = 0, i = 0;
	for (x = 0; x < MAX_MAPWIDTH; x++)
	{
		for (y = 0; y < MAX_MAPHEIGHT; y++)
		{
			if (CMap::Map_GetData(x, y).type != 0)
			{
				Sprite_Draw(TEX_BLUE, (CMap::Map_GetData(x, y).pos.x + 250) * 1, (CMap::Map_GetData(x, y).pos.z * -1) + 250, 0, 0, 5, 5);
			}
		}
	}
	//プレイヤーアイコン表示
	Sprite_Draw(TEX_PLAYER_ICON, getplayer->Get_Position().x + 250, (getplayer->Get_Position().z*-1) + 250, 0, 0, 8, 8);
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		C3DObj *enemy = CEnemy::Get_Enemy(i);
		if (enemy)
		{
			Sprite_Draw(CTexture::TEX_ENEMY_ICON, enemy->Get_Position().x + 250, (enemy->Get_Position().z*-1) + 250, 0, 0, 8, 8);
		}
		C3DObj *object = CObject::Get_Object(i);
		//オブジェクトの判定
		if (object)
		{
			Sprite_Draw(CTexture::TEX_LADDER_ICON, object->Get_Position().x + 250, (object->Get_Position().z*-1) + 250, 0, 0, 8, 8);
		}
	}
	
	///////////////////
	//下のテキスト描画
	if (g_text.alive)
	{

		switch (g_text.chara)
		{
		case PLAYERCHARA:

			switch (g_text.act)
			{
			case REGULARATTACK:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "プレイヤーの攻撃！スライムに%dダメージ与えた!", g_text.damage);
				break;

			case GOLADDER:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "先に進みますか？");
				UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "⇒ 進む\n\n   そのまま");
				break;

			case NOTGOLADDER:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "先に進みますか？");
				UI_TextDraw(TEXT_POSX + 800, TEXT_POSY + 20, D3DCOLOR_RGBA(255, 255, 255, 255), "    進む\n\n⇒そのまま");
				break;

			case DESTROY:
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "プレイヤーはやられてしまった");
				break;
			}

			break;

		case SRAIM:
			if (g_text.act == REGULARATTACK)
			{
				Sprite_Draw(TEX_MESSAGE_WINDOW, 90.0f, 500.0f, 0.0f, 0.0f, (float)Texture_GetWidth(TEX_MESSAGE_WINDOW), (float)Texture_GetHeight(TEX_MESSAGE_WINDOW), 0.0f, 0.0f, 1.3f, 1.3f, 0.0f);
				UI_TextDraw(TEXT_POSX, TEXT_POSY, D3DCOLOR_RGBA(255, 255, 255, 255), "スライムの攻撃！プレイヤーに%dダメージ与えた!", g_text.damage);
				break;
			}

		/*case SMALEMAP:
			// ミニマップ描画
			int x = 0, y = 0;
			for (x = 0; x < MAX_MAPWIDTH; x++)
			{
				for (y = 0; y < MAX_MAPHEIGHT; y++)
				{
					if (CMap::Map_GetData(y, x).type != 0)
					{
						Sprite_Draw(TEX_BLUE, (CMap::Map_GetData(y, x).pos.x + 250) * 1, (CMap::Map_GetData(y, x).pos.z + 250) * 1, 0, 0, 5, 5);
					}
				}
			}
			break;*/
		}
	}
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
	//vprintf(text, ap);
	vsprintf_s(Tbuffer, TEXT_MAX, text, ap);
	va_end(ap);



	// 実験								文字数らしいが-1なら全部になる
	g_pD3DXFont->DrawTextA(NULL, Tbuffer, -1, &rect, DT_LEFT, color);
	//g_pD3DXFont->DrawTextA(NULL, Ppos.x, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 0, 255));
#endif // _DEBUG || DEBUG
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act)
{
	g_text.alive = true;
	g_text.chara = chara;
	g_text.act = act;
	// 誕生日
	g_text.TextCreateFrame = g_TextFramecount;
	
}

void CUserinterface::UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage)
{
	g_text.alive = true;
	g_text.chara = chara;
	g_text.hitchara = hitchara;
	g_text.damage = damage;
	g_text.act = act;
	// 誕生日
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