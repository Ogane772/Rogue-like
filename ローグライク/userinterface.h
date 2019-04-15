#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_
#include <d3dx9.h>
#include "CGameObj.h"
#include "C2DObj.h"
//===================================
// 定数
//===================================
#define TEXT_FRAMEMAX (100)	// テキスト表示フレーム数
#define TEXT_POSX (100)		// テキスト表示X座標
#define TEXT_POSY (530)		// テキスト表示Y座標
//===================================
// テキスト列挙
//===================================
class CUserinterface : public C2DObj
{
public:

	typedef enum {
		SMALEMAP,		// ミニマップ(仮)
		PLAYERCHARA,	// プレイヤー

		// 敵キャラ
		SRAIM,			// スライム
		CHARATYPENONE
	}CHARATYPE;

	typedef enum {
		REGULARATTACK,	// 通常攻撃
		GOLADDER,		// 梯子を進む(次の階へ)
		NOTGOLADDER,	// 梯子を進まない(次の階へ)
		DESTROY,		// 倒れた
		ACTTYPENONE
	}ACTTYPE;
	//===================================
	// 構造体
	//===================================
	typedef struct {
		CHARATYPE chara;
		CHARATYPE hitchara;
		ACTTYPE act;
		int damage;
		int Age;

		int TextCreateFrame;	// 誕生フレーム
		int TextCountFrame;		// 表示したフレーム
		D3DXVECTOR3 pos;

		bool alive;
	}TEXT;

	//後で改良しないと使い物にならない
	static void UI_Initialize(void);
	static void UI_Finalize(void);
	static void UI_Update(void);
	static void UI_Draw(void);

	static void UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...);

	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage);

	static void UI_Delete(void);
private:
	// LPD3DXFONT = フォントの型
	static LPD3DXFONT g_pD3DXFont;

	static TEXT g_text;
	static D3DXVECTOR3 MapPos;
	static D3DXVECTOR3 DMapPos;
	static int g_Textbackground;
	static int g_TextFramecount;
};
#endif
