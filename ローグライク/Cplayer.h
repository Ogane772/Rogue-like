#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "C3DObj.h"
#include <d3dx9.h>
#include "collision.h"
#define ATTACK_END (55)//攻撃の待機時間
#define NOATTACK_END (15) //空振り攻撃の待機時間

#define PLAYER_RADIUS	 (2.6f)				// 当たり判定半径
#define PLAYER_ENERADIUS (5.0f)				// エネミー専用当たり判定
#define PLAYER_RECOVERY (100)				// 自然回復 = MAXHP / PLAYER_RECOVERY
#define PLAYER_MAXHP (500)
#define PLAYER_STR (6)
#define PLAYER_HEAL (300)	//歩いたときの回復量
#define PLAYER_HIGH_HEAL (100)	//ヒールフラグONの時の歩いたときの回復量
#define PLAYER_RING_HEAL (15) //癒しの指輪の+値増加値
#define WALK_COUNT (9) //ウォークカウント基礎値
#define MAX_GOLD (9999)//最大のお金	
#define MAX_ONAKA (100) //おなかの最大値
#define WAIT_FRAME (50) //アイテム取得時の待機時間
#define WAIT_ITEM_FRAME (90) //アイテム使用時の待機時間
#define ITEM_EFFECT_FRAME (45)//アイテムの効果発動フレーム
#define MAX_ITEM (13)	//所持アイテム最大数
#define MAX_WEPON (3)	//所持装備数
#define DEFFAULT_ONAKA_TURN (10)//初期のお腹が減るターン数
#define SIAWASE_RING (1.5f)//しあわせの指輪のデフォルト値
//プレイヤーの向き
#define UP_ANGLE (3.2f)
#define LEFT_ANGLE (1.6f)
#define DOWN_ANGLE (0.0f)
#define RIGHT_ANGLE (4.8f)
#define LEFT_TOP_ANGLE (2.4f)
#define LEFT_DOWN_ANGLE (0.8f)
#define RIGHT_TOP_ANGLE (4.0f)
#define RIGHT_DOWN_ANGLE (5.6f)
//スキル使用HP量
#define SMOLL_SKILL_HP (m_MaxHp / 8)
#define MIDDLE_SKILL_HP (m_MaxHp / 4)
#define BIG_SKILL_HP (m_MaxHp / 2)
//罠成功率
#define TRAP_OK (70)
//0.25 19 
//0.5 9
class CPlayer :public C3DObj
{

public:
	typedef enum {
		PLAYER_FIRST,			// 階を移動した時の最初の処理
		PLAYER_WORPFIRST,			// ワープした時の最初の処理	
		PLAYER_STANDBY,			// HPの回復などを行う
		PLAYER_KEY_INPUT,		// 入力待ち
		PLAYER_WINDOW,			// コマンドウィンドウを開いてるとき
		PLAYER_WINDOW_STATUS,	// コマンドウィンドウの能力確認時
		PLAYER_WINDOW_ASIMOTO,	// コマンドウィンドウの足元確認
		PLAYER_WINDOW_ITEM,	    // コマンドウィンドウの道具確認
		PLAYER_SERECT_UI,		// 梯子,アイテムなどの選択
		PLAYER_DESTROY,			// プレイヤーがやられる
		PLAYER_ITEM_EFFECT,		// アイテム使用時
		PLAYER_ITEM_WAIT,		// アイテム取得時の待機時間
		PLAYER_ENEMYTURN_WAIT,  // 敵ターン中の待機時間
		PLAYER_TIME_WAIT,      // 入力不可のプレイヤー待機時間
		PLAYER_RANGEHIT_WAIT,      // 複数攻撃処理ターン
		PLAYER_CONDITION_HEAL_WAIT,// 状態異常回復の待機時間
		PLAYER_SLEEP_WAIT,      // 睡眠状態の待機時間
		PLAYER_WARP_WAIT,      // ワープ状態の待機時間
		PLAYER_ITEM_ASIMOTO,	// アイテムを置いたとき
		PLAYER_TRAP_EFFECT,		// ワナを踏んだ時の効果
		PLAYER_ENEMY_BACK_WAIT, // 敵吹っ飛び中のの待機時間
		PLAYER_ACT,				// 行動中
		PLAYER_ACT_END,			// 行動終了
		PLAYER_MOVE,			// 移動中
		PLAYER_MOVE_END,		// 移動終了
		PLAYER_TURN_END,		// ターン終了
		PLAYER_TURN_CONPLETE,		// 倍速鈍足時のターン終了
		PLAYER_NONE
	}PLAYERTURN;
	typedef enum {
		CURSOL_LEFT_TOP,
		CURSOL_RIGHT_TOP,
		CURSOL_LEFT_DOWN,
		CURSOL_RIGHT_DOWN,
	}CURSOR_NUMBER;
	typedef enum {
		WEPON,//装備
		ITEM,//使用アイテム
	}ITEMTYPE;
	//アイテム選択時の選択肢
	typedef enum {
		EFFECT_ITEM,//アイテム使用
		ASIMOTO_ITEM,//その場に置く
		TIPS_ITEM,//説明
	}ITEMWINDOW;
	//装備番号
	typedef enum {
		WEPON_NUMBER,//武器格納番号
		SHELD_NUMBER,//盾格納番号
		RING_NUMBER,//指輪格納番号
	}WEPON_SOUBINUMBER;
	typedef enum {
		TIPS_WEPON,
	}WEPONWINDOW;

	//武器変数
	typedef struct {
		int wepon_str;
		int wepon_def;
		int wepon_type;
		int wepon_purasu_number;
		int wepon_sikibetu_number;//装備の特殊効果を判定するための数字
	}PLAYER_WEPON;


	CPlayer();
	~CPlayer();


	static void Player_SetPos(int z, int x);		// 階層ごとにプレイヤーの初期位置を決める
	static void Player_SetWorpPos(int z, int x);	// ワープ時のプレイヤー位置を決める
	void Update(void);
	void Draw(void);
	void Finalize(void);	//	終了処理

	static CPlayer *PlayerCreate(void);
	static const Sphere* Player_GetCollision(void) { return &m_Colision; }
	static const Sphere* Player_GetEnemyCollision(void) { return &m_EnemyColision; }
	static PLAYER_WEPON* GetPlayerWeponData(int index) { return &player_wepon[index]; }
	static void Player_NextTurn(void);
	bool Damage(int str, float angle, int week_type);
	void Player_OnakaDown(void); //おなかを減らす
	void Player_SkillHpDown(void);
	int Get_PlayerOnaka(void) { return m_Onaka; }
	bool Get_PlayerHealFlag(void) { return m_HealFlag; }
	int Get_CursorNumber(void) { return m_CursorNumber; }
	int Get_TimeCursorNumber(void) { return m_TimeCursorNumber; }
	int Get_PlayerTurn(void) { return m_Turn; }
	int Get_PlayerNextExp(int index) { return m_PlayerLvData[index].lvup_exp; }
	int Get_PlayerWeponStock(int index) { return m_WeponStock[index]; }
	int Get_PlayerItemStock(int index) { return m_ItemStock[index]; }
	int Get_PlayerItemStockType(int index) { return m_ItemStockType[index]; }
	int Get_PlayerAllItemStock(int index) { return m_AllItemStock[index]; }
	int Get_OnakaTurn(void) { return m_OnakaTurn; }
	int Player_SkillHpGet(void);
	void Set_PlayerTurn(int turn) { m_Turn = turn; }
	void SetEnemyBack(bool type) { m_EnemyBack = type; }
	void SetEnemyDeath(bool type) { m_EnemyDeath = type; }
	bool Get_DarkFlag(void) { return m_DarkFlag; }
	bool Get_WMode(void) { return m_Wmode; }
	bool Get_TurboMode(void) { return turbo; }
	bool Get_ItemOn(void) { return m_ItemOn; }
	bool Get_ItemTips(void) { return m_ItemTips; }
	bool Get_NanameFlag(void) { return m_NanameFlag; }
	bool Get_NextItemPage(void) { return m_NextItemPage; }
	bool Get_DrawCheck(void) { return alive; }
	bool Get_GekikaFlag(void) { return m_Gekika; }
	bool Get_EnemyONFlag(void) { return m_EnemyOn; }
	bool Get_EnemyDeath(void) { return m_EnemyDeath; }
	bool ExpGoldCheck(int exp, int gold);		// 獲得経験値とお金処理
	

	static C3DObj *Get_Player(void);
	
	// マップ二次元配列用

	static bool goladder;	// 梯子
protected:

	bool m_DrawCheck;			//	描画フラグ
private:
	typedef struct {
		int lv,
			lvup_exp,//レベルアップに必要な経験値
			maxhp,
			str,
			def;
	}Player_LvData;
	void Initialize(void);	//	初期化
	int CSV_PlayerLv_Load(Player_LvData* lvdata, const int num);//プレイヤーのレベルテーブル読み込み
	void Player_First(void);
	void Player_WorpIn(void);//ワープ時の代入
	void Player_Standby(void);
	void Player_KeyInput(void);
	void Player_WindowMode(void);
	void Player_SelectUi(void);
	void Player_SelectItem(void);
	void Player_ItemEffect(void);//アイテムの使用、使用効果
	void Player_TrapEffect(void);//罠の効果
	void Player_ItemAsimoto(void);//アイテムを足元に置く処理
	void Player_CollisionUpdate(void);
	void Player_TurboMove(void);
	void Player_Move(void);
	void Player_MoveChenge(int MoveType);			//	移動処理の準備
	void Player_TurboMoveChenge(int MoveType);			//	移動処理の準備
	void Player_lefttopMove(void);
	void Player_righttopMove(void);
	void Player_leftbottomMove(void);
	void Player_rightbottomMove(void);
	void Player_TurbolefttopMove(void);
	void Player_TurborighttopMove(void);
	void Player_TurboleftbottomMove(void);
	void Player_TurborightbottomMove(void);
	void AngleChange(float Angle);	//	方向変換
	void Player_Act(void);
	void Player_Destroy(void);
	void Player_BackCheck(void);//アイテム取得中にモンスターが攻撃してこないよう事前確認
	void Player_Check(void);//プレイヤーの当たり判定
	void Player_TrapCheck(void);//罠に攻撃した時見えるようにする
	void Player_WeponChenge(int wepon_type, int wepondata_number);//武器の装備 引数　武器のタイプ、装備する場所
	void Player_W_WeponChenge(int wepon_type);//両手武器の装備 引数　武器のタイプ、装備する場所
	void Player_ItemSort(void);//使用アイテム削除自、アイテムを前に詰める
	void Player_ItemSort2(void);//途中のアイテム削除時、使用アイテム削除自、アイテムを前に詰める
	bool JoyDevice_IsTrigger(int nKey);//コントローラーのトリガー処理
	int WeponPurasuSet(void);//+値を決めて返す
	bool SkillHpCheck(void);//スキル使用HPがあるか返す　true=使用できる
	int ItemSnatch(void);//アイテムを盗まれたときの処理　戻り値　盗まれたアイテム番号
	int m_PadDirection;
	enum
	{
		DIRE_TOP,		//　上
		DIRE_RIGHT,		//	右
		DIRE_BOTTOM,		//　下
		DIRE_LEFT,		//　左
		DIRE_TOP_RIGHT,	//	右上
		DIRE_TOP_LEFT,	//	左上
		DIRE_BOTTOM_RIGHT,//	右下
		DIRE_BOTTOM_LEFT,	//	左下

		DIRE_N,

	};//　方向
	int m_WeponStock[MAX_WEPON];//取得装備を格納
	int m_ItemStock[MAX_ITEM];//取得アイテムを格納
	int m_AllItemStock[MAX_WEPON + MAX_ITEM];//全取得アイテムを格納
	int m_ItemStockType[MAX_WEPON + MAX_ITEM];//取得アイテムのタイプ格納
	int m_Wait_frame;//フレーム待機時間
	int m_Onaka;
	int m_MaxOnaka;
	int m_item_turncount;//持続するアイテムのターンカウント
	int m_CursorNumber;//ウィンドウカーソル位置
	int m_TimeCursorNumber;//アイテム選択時のウィンドウカーソル位置
	int m_AllWeponStock;//装備をいくつ持ってるか格納
	int m_Add_cursor;//もし2ページ目を開いてたら足す
	int m_AutoHell;//自動回復力
	int m_TrapType;//踏んだトラップ番号格納
	int m_TrapCheck;//罠発動チェック
	int m_OnakaTurn;//お腹の減るターン数
	int m_EnemyAttackCount;//複数のエネミーを攻撃した時の数
	static 	int m_Turn;
	int m_DrawCount;
	bool m_Wmode;//両手武器を持ってるときはtrue
	bool m_ItemOn;//アイテムを選択したらtrue
	bool m_ItemTips;//アイテム説明フラグ
	bool m_NextItemPage;//所持アイテムウィンドウが2ページ目ならtrue
	bool right_trigger = false;
	bool left_trigger = false;
	bool right_on;//右スティック入力確認
	bool m_DarkFlag;//暗闇の通路にいたらtrue
	bool m_NanameFlag;//斜め移動モードの時true
	bool m_HealFlag;//自動回復高速化フラグ
	bool turbo;//高速化フラグ
	bool m_Gekika;//相性激化フラグ
	bool m_EnemyOn;//エネミー表示フラグ
	bool m_SkillAtaack;//スキル攻撃ならtrue
	bool m_EnemyBack;//エネミーが吹っ飛び中だったらtrue
	bool m_EnemyWarp;//エネミーが吹っ飛び中だったらtrue
	bool m_EnemyDeath;//敵が攻撃以外で死んだとき待つ
	static Sphere m_Colision;		// 当たり判定
	
	int charatype;		// キャラクターの種類

	D3DXVECTOR3 walkpos;	// 移動する場合の目的地セット

	D3DXVECTOR3 vecplayer;
	D3DXVECTOR3 vec2player;	// 斜め用

	static D3DXVECTOR3 start_m_Position;//開始時の位置
	static int start_m_Mapz;
	static int start_m_Mapx;

	static Player_LvData m_PlayerLvData[];
	static PLAYER_WEPON player_wepon[3];
};



#endif

