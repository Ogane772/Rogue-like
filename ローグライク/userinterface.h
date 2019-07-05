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
#define MAX_NAME (90)
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
		ENEMY_POIZUN_DEATH,	// 敵が毒で死んだとき
		SKILLATTACK,	// スキル攻撃
		SKILL_CONDITIONATTACK,	// 状態異常スキル攻撃
		BACKENEMY_ATTACK,// 敵が壁に当たった時
		NODAMAGE_SKILL,	// ダメージのないスキル攻撃
		GOLADDER,		// 梯子を進む(次の階へ)
		NOTGOLADDER,	// 梯子を進まない(次の階へ)
		DESTROY,		// 倒れた
		FREE_MESSAGE,	// 自由記入文字
		ACTTYPENONE,
		ITEM,			// アイテムに触れたとき
		NOITEM,			// アイテムを持てないとき
		ITEM_EFFECT,	// アイテム使用時
		ITEM_ASIMOTO,	// アイテム置いたとき
		ITEM_DELETE,	// アイテム削除時
		NOITEM_ASIMOTO,	// アイテムが置けないとき
		TRAP_EFFECT,	// 罠を踏んだ時
		TRAP_NOEFFECT,	// 罠を踏んだ時(効果非発動時）
		WEPON_GET,	// 装備を手に入れた時
		WEPON_W_GETCHENGE,	// 装備を手に入れた時(片手→両手）
		WEPON_NORMAL_GETCHENGE,	// 装備を手に入れた時（両手→片手）
		SHELD_W_GET,	// 装備を手に入れた時（両手の時盾ゲット）
		CONDITION_HEAL,//状態異常回復時
		SLEEP_WAIT,		   //眠っている時のログ
	}ACTTYPE;
	//===================================
	// 構造体
	//===================================
	typedef struct {
		CHARATYPE chara;
		CHARATYPE hitchara;
		ACTTYPE act;
		int act2;
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
		char skill_log[MAX_NAME];//スキル特技名
		char heal_log[MAX_NAME];//状態異常回復ログ
		char free_message[MAX_NAME];//自由に記入ログ
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
	static void UI_NameInitialize(void);//プレイヤー名を登録
	static void UI_Finalize(void);
	static void UI_Update(void);
	static void UI_Draw(void);

	static void UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...);
	static void UI_TextDraw2(int x, int y, D3DCOLOR color, const char* text, ...);

	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, char* message);
	static void UI_TextCreateDeleteItem(CHARATYPE chara, ACTTYPE act, int type, char* rog_message, char* action_message);//アイテム削除用
	static void UI_TextCreateTrap(CHARATYPE chara, ACTTYPE act, int type);//罠用
	static void UI_TextCreateTrapRog(CHARATYPE chara, ACTTYPE act, int type, char * rog);//罠の効果発動テキスト自由
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int condition);//状態異常回復
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type , char* rog_message);//+値変化用
	static void UI_TextCreateCondition(CHARATYPE chara, ACTTYPE act, int type);//アイテム用
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type, int purasu_number);//装備用
	static void UI_TextCreate(int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextCreate(char *skill_effect, int week_type, CHARATYPE chara, int act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextPlayerSkill(char *skill_effect, int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextPlayerSkillCondition(char *skill_effect, char *skill_log, int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextPlayerNodamageSkill(char *skill_effect, char *skill_log, int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int type);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextCreateBack(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);//吹き飛ばしダメージログ

	static void UI_Delete(void);
private:
	static void CommandWindow_Draw(void);
	static void ItemWindow_Draw(void);
	static void RightWindow_Draw(void);//右の装備欄表示
	static void NanameYajirusi_Draw(void);//斜め移動モードの時の矢印表示
	static void ConditionIconDraw(void);//状態異常アイコン表示
	static void SetItemWindow(void);//所持アイテム表示
	static void SetItemOnWindow(void);//所持アイテム選択した時のウィンドウ
	static void SetItemTips(void);//所持アイテム説明文
	static void StatusWindow_Draw(void);
	static void UI_UpDraw(void);
	static void Map_Draw(void);
	static void UI_TextDraw(void);
	static void SkillAttackLog(void);
	//文字列置き換え関数
	//第一引数　渡す行　第二引数　変えたい文字　第三引数　置き換える文字
	static void strchg(char *buf, const char *str1, const char *str2);
	// LPD3DXFONT = フォントの型
	static LPD3DXFONT g_pD3DXFont;

	static TEXT g_text;
	static int g_Textbackground;
	static int g_TextFramecount;
};
#endif
