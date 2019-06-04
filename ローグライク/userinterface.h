#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_
#include <d3dx9.h>
#include "CGameObj.h"
#include "C2DObj.h"
//===================================
// 定数
//===================================
#define TEXT_FRAMEMAX (100)	// テキスト表示フレーム数
#define TEXT_POSX (150)		// テキスト表示X座標
#define TEXT_POSY (520)		// テキスト表示Y座標
#define TEXT_ADD_POSY (50)		// テキスト表示Y座標
#define MAX_NAME (30)
#define TEXT_Y (55)//テキストのY軸の幅
#define ENEMY_DESTROY_TEXT (20) //　敵死亡時の動けないテキスト表示時間
#define UI_Y (30)	//上のUI表示位置
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
		ENEMY,		
		CHARATYPENONE
	}CHARATYPE;

	typedef enum {
		REGULARATTACK,	// 通常攻撃
		GOLADDER,		// 梯子を進む(次の階へ)
		NOTGOLADDER,	// 梯子を進まない(次の階へ)
		DESTROY,		// 倒れた
		ACTTYPENONE,
		ITEM,			// アイテムに触れたとき
		NOITEM,			// アイテムを持てないとき
		ITEM_EFFECT,	// アイテム使用時
		ITEM_ASIMOTO,	// アイテム置いたとき
		NOITEM_ASIMOTO,	// アイテムが置けないとき
		WEPON_GET,	// 装備を手に入れた時
		WEPON_W_GETCHENGE,	// 装備を手に入れた時(片手→両手）
		WEPON_NORMAL_GETCHENGE,	// 装備を手に入れた時（両手→片手）
		SHELD_W_GET,	// 装備を手に入れた時（両手の時盾ゲット）
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
		int exp;				// 獲得経験値
		int gold;				// 獲得金
		int type;				// 種類
		int purasu_number;		// 装備の+値
		int attribute_type;			// 耐性タイプ
		D3DXVECTOR2 pos;
		
		char player_name[MAX_NAME];
		char name[MAX_NAME];				// 対象のエネミーの名前
		char item_log[MAX_NAME];			// アイテム効果の説明
		bool alive;
		bool lv_up;							//レベルアップしたらtrue
	}TEXT;
	//耐性の結果
	enum ATTRIBUTE_TYPE{
		WEEK_TYPE,
		NORMAL_TYPE,
		RESIST_TYPE,
	};
	//後で改良しないと使い物にならない
	static void UI_Initialize(void);
	static void UI_Finalize(void);
	static void UI_Update(void);
	static void UI_Draw(void);

	static void UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...);
	static void UI_TextDraw2(int x, int y, D3DCOLOR color, const char* text, ...);

	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type);//アイテム用
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type, int purasu_number);//装備用
	static void UI_TextCreate(int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);

	static void UI_Delete(void);
private:
	static void CommandWindow_Draw(void);
	static void ItemWindow_Draw(void);
	static void RightWindow_Draw(void);//右の装備欄表示
	static void SetItemWindow(void);//所持アイテム表示
	static void SetItemOnWindow(void);//所持アイテム選択した時のウィンドウ
	static void SetItemTips(void);//所持アイテム説明文
	static void StatusWindow_Draw(void);
	static void UI_UpDraw(void);
	static void Map_Draw(void);
	static void UI_TextDraw(void);
	// LPD3DXFONT = フォントの型
	static LPD3DXFONT g_pD3DXFont;

	static TEXT g_text;
	static int g_Textbackground;
	static int g_TextFramecount;
};
#endif
