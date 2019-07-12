//////////////////////////////////////////////////
////
////	3Dオブジェクトクラス
////
//////////////////////////////////////////////////

#ifndef _3DOBJ_H_
#define _3DOBJ_H_
#include <crtdbg.h>

#include "gamepad.h"
#include "CGameObj.h"
#include <d3dx9.h>
#include "CSkinAnimation.h"
#include "collision.h"
#include "userinterface.h"
#include <iostream>
#include <random>
//死亡時リターンする数値
#define DEATH (true)//死んだらtrue
#define NORMAL (false)

//ダメージ計算式 後で武器タイプ補正も入れる
#define PLAYER_W_DAMAGEKEISAN ((str * str2) / 2) - ((getplayer->Get_Def() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def) / 4)
#define PLAYER_NORMAL_DAMAGEKEISAN ((str * str2) / 2) - ((getplayer->Get_Def() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def) / 4)
#define ENEMY_DAMAGEKEISAN ((str * str2) / 2) - (enemy->Get_Def() / 4)
#define ENEMY_NODEF_DAMAGEKEISAN ((str * str2) / 2)
#define HITSTOP (30)
//XBOXコントローラーの十字キー
#define LEFT_BUTTON (27000)
#define RIGHT_BUTTON (9000)
#define UP_BUTTON (0)
#define DOWN_BUTTON (18000)
#define RIGHT_DOWN_BUTTON (13500)
#define RIGHT_TOP_BUTTON (4500)
#define LEFT_DOWN_BUTTON (22500)
#define LEFT_TOP_BUTTON (31500)

class C3DObj :virtual public CGameObj
{
public:
	C3DObj();
	~C3DObj();
	typedef struct
	{
		LPD3DXMESH pMesh;	// メッシュを受け取る変数
		DWORD dwNumMaterials;	// マテリアル総数
		D3DMATERIAL9* pMaterials;
		LPDIRECT3DTEXTURE9* pTextures;
	}NormalModelData;
	typedef enum {
		NORMAL_CONDITION,
		SLEEP_CONDITION,//睡眠状態
		POIZUN_CONDITION,//毒状態
		KURAYAMI_CONDITION,
		MAHUJI_CONDITION,//特技封印
		BAISOKU_CONDITION,
		DONSOKU_CONDITION,
	}PLAYER_JYOUTAIIJYOU;
	typedef struct {
		bool HitTop;
		bool HitBottom;
		bool HitRight;
		bool HitLeft;
		bool HitTopLeft;
		bool HitBottomRight;
		bool HitTopRight;
		bool HitBottomLeft;

		// 攻撃用
		bool HitEnemy;
		bool HitEnemyTop;
		bool HitEnemyBottom;
		bool HitEnemyRight;
		bool HitEnemyLeft;
		bool HitEnemyTopLeft;
		bool HitEnemyBottomRight;
		bool HitEnemyTopRight;
		bool HitEnemyBottomLeft;

		bool Hitnaname;

		// オブジェクト用
		bool HitLadder;
		bool HitItem;
	}JUDGE;

	C3DObj(int type);
	typedef enum
	{
		A_BUTTON,
		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,
		L_BUTTON,
		R_BUTTON,
		BACK_BUTTON,
		START_BUTTON,
	} CONTROLLER;
	typedef enum
	{
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// エネミー

		TYPE_OBJECT,	// 障害物
		TYPE_WEPON,	// 装備
		TYPE_TRAP,	// ワナ
		TYPE_etc,		// その他
		TYPE_MAX

	} OBJTYPE;

	typedef enum {
		MODELL_PLAYER,
		MODELL_ENEMY_1,
		MODELL_KISHI,
	}NorMalModelFileData;
	enum ANIME_MODEL
	{
		MODELL_ANIME_PLAYER,
		MODELL_ANIME_GENSAN,
		MODELL_ANIME_PIEL,
		ANIME_MODEL_MAX,//アニメモデル最大数
	}AnimeModelFileData;;


	virtual void Finalize(void);
	virtual void Update(void) = 0;	//	更新
	virtual void Draw(void) = 0;	//	描画

	D3DXMATRIX Get_mtxWorld(void) { return m_mtxWorld; }				//	ワールド変換用行列取得
	D3DXMATRIX Get_mtxTranslation(void) { return m_mtxTranslation; }	//	移動用行列取得

	static void FinalizeAll();	// 全オブジェクト初期化
	static void UpdateAll();	// 全オブジェクト更新
	static void DrawAll();		// 全オブジェクト描画
	static void DeleteAll();	// 全オブジェクト削除
	void C3DObj_delete(void);	// オブジェクト削除	
	//ゲッター
	static C3DObj *Get(int nIdx);	// インスタンス取得
	virtual int Get_PlayerWeponStock(int index) { return 0; }//所持武器取得
	virtual int Get_PlayerItemStock(int index) { return 0; }//所持アイテム取得
	virtual int Get_CursorNumber(void) { return 0; }//ウィンドウカーソル位置取得
	virtual int Get_TimeCursorNumber(void) { return 0; }
	virtual int Get_PlayerNextExp(int index) { return 0; } //次のレベルまでの必要経験値を取得
	virtual void Player_SkillHpDown(void) { };//プレイヤーのHPを下げる
	virtual void Player_OnakaDown(void) {}; //プレイヤーのおなかを減らす
	virtual int Get_PlayerOnaka(void) { return 0; }//プレイヤーのおなか取得
	virtual int Get_OnakaTurn(void) { return 0; }//プレイヤーのお腹が減るターン数を取得
	virtual bool Get_DarkFlag(void) { return 0; };
	virtual bool Get_WMode(void) { return 0; }//プレイヤーが両手持ちか取得
	virtual bool ExpGoldCheck(int exp, int gold) { return 0; }//プレイヤーの経験とお金チェック
	virtual bool Get_PlayerHealFlag(void) { return 0; };
	virtual bool Get_TurboMode(void) { return 0; }//ターボモード取得
	virtual bool Get_ItemOn(void) { return 0; }//アイテム選択を取得
	virtual bool Get_ItemTips(void) { return 0; }//アイテムを選択から効果を選んだとき
	virtual bool Get_NextItemPage(void) { return 0; }//所持アイテムウィンドウが2ページ目かどうか返す
	virtual bool Get_EnemyONFlag(void) { return 0; }//マップ上のエネミーアイコン表示フラグ
	virtual int Get_PlayerItemStockType(int index) { return 0; }//取得アイテムタイプを取得
	virtual int Get_PlayerAllItemStock(int index) { return 0; }//全ての取得アイテムを格納
	virtual int Get_PlayerTurn(void) { return 0; }//プレイヤーのターンモード取得
	virtual int Player_SkillHpGet(void) { return 0; }//スキルの消費HP取得
	virtual void Set_PlayerTurn(int turn) {}//プレイヤーのターンモードセット
	virtual int Get_EnemyTurn(void) { return 0; }//エネミーのターンモード取得
	virtual void Set_EnemyTurn(int turn) {}//エネミーのターンモードセット
	virtual bool Get_Hold(void) { return 0; }//キーおしっぱを取得
	virtual bool Get_NanameFlag(void) { return 0; }//斜め移動状態を取得
	virtual bool Get_GekikaFlag(void) { return 0; }//相性激化状態を取得
	virtual bool Get_EnemyPoizunDeath(void) { return 0; }//敵が毒で死んでいるか返す
	virtual bool GetWanaCheck(void) { return 0; }//罠が見えてるか取得
	virtual bool GetWanaEffectOn(void) { return 0; }//罠の効果が発動してるか取得
	virtual void SetWanaCheck(bool type) { };//罠が見えてるかセット
	virtual void SetWanaEffectOn(bool type) { };//罠の効果が発動してるかセット
	virtual void SetEnemyBack(bool type) { };//敵が吹っ飛び中かセット
	virtual void Enemy_SetWorpPos(int pposZ, int pposX) {};//エネミーのワープ位置セット
	virtual void Set_Warp(bool type) {};
	virtual void SetEnemyDeath(bool type) { };
	virtual bool Get_EnemyDeath(void) { return 0; }
	virtual bool Get_RangeHit(void) { return 0; }
	virtual void Set_RangeHit(bool type) { };


	int Get_3DObjIndex() { return m_3DObjIndex; }	// ワークインデックス取得
	int Get_3DObjType() { return m_3DObjType; }		// 種類取得
	float Get_Angle(void) { return m_Angle; }	//	角度取得	
	float Get_Hp(void) { return m_Hp; }		//	HP取得
	float Get_MaxHp(void) { return m_MaxHp; }		//	最大HP取得
	int Get_Str(void) { return m_Str; }		//	攻撃力取得
	float Get_Str2(void) { return m_Str2; }		//	変化攻撃力取得
	int Get_Def(void) { return m_Def; }		//	防御力取得
	int Get_Lv(void) { return m_Lv; }		//	レベル取得
	int Get_Gold(void) { return m_Gold; }		// 所持金取得
	int Get_Exp(void) { return m_Exp; }		// 経験値取得
	int Get_Type(void) { return m_Type; }		// エネミータイプ取得
	int Get_Condition(void) { return m_Condition; }	// 状態取得
	char* Get_Name(void) { return name; };		// 名前取得
	bool Get_RivalFlag(void) { return rival_flag; }
	bool Get_MapDrawFlag(void) { return map_drawflag; }
	bool Get_EnterFlag(void) { return enter_flag; }
	bool Get_BackMove(void) { return m_Back; }
	//セッター
	void SetCondition(int condition_type) { m_Baisoku_Flag = false; m_Donsoku_Flag = false; m_Condition = condition_type; }
	void Set_MapDrawFlag(bool type) { map_drawflag = type; }
	void Set_RivalFlag(bool type) { rival_flag = type; }
	void Set_EnterFlag(bool type) { enter_flag = type; }
	void SetBackMove(bool type) { m_Back = type; }//吹っ飛び状態をセット
	void Set_Attack_End(int end) { attack_endframe = end; }//攻撃終了時間をセット
	void SetTurnCount(int turn) { m_TurnCount = turn; }
	

	D3DXVECTOR3 Get_Position(void) { return m_Position; } //座標取得
	static char* Get_AnimeFileName(int index) { return ANIME_MODEL_FILES[index].filename; }
	virtual bool Get_DrawCheck(void) = 0;
	virtual bool Damage(int str, float angle, int week_type) { return 0; }
	virtual int ItemSnatch(void) { return 0; }//アイテムを盗まれたときの処理　戻り値　盗まれたアイテム番号
	virtual void Enemy_PoizunDamageStart(void) {};//エネミーの毒ダメージ計算
	virtual void Enemy_ConditionCount(void) {};//エネミーの睡眠・暗闇・特技封じ処理
	virtual void Set_EnemyItem(int type) { };//エネミーの所持アイテムセット
	virtual int Get_EnemyItem(void) { return 0; }//エネミーの所持アイテムゲット
	static HRESULT InitModelLoad();  //	モデル読み込み
	//モデル情報取得
	THING* C3DObj::GetAnimeModel(void);
	//THING GetAnimeModel(int index);
	NormalModelData GetNormalModel(int index) {	return NormalModel[index];}
	//NormalModelFile GetNormal(int index);
	int m_Type;			// 敵の種類
	//	終了処理
	static void Model_Finalize(void);
	static void NormalModel_Finalize(NormalModelData *DeleteNormalModel);
	
	static void AnimeModel_Finalize(THING *DeleteAnimeModel);

	static JUDGE m_Judge_player;
	static JUDGE Judgement_GetPlayer(void) { return m_Judge_player; }
	int m_ObjectType;
	int m_TrapType;
	int m_WeponType;
	// マップ二次元配列用
	int m_Mapx;
	int m_Mapz;
	// 1ターン前座標
	int m_Oldmx;
	int m_Oldmz;
	bool alive;			// 使われているか

	static Sphere m_EnemyColision;	// プレイヤーのエネミーとの当たり判定専用

	Sphere m_EnemyMyColision;		// エネミーの当たり判定
	const Sphere* Enemy_GetCollision(int index) { return &m_EnemyMyColision; }

	JUDGE m_Judge_enemy;

	JUDGE Judgement_GetEnemy(void) { return m_Judge_enemy; }
	//当たり判定
	void PlayerVsWall(JUDGE *player_judge, Sphere *m_PlayerColision);

	static void Collision_AnimeVSAnime(JUDGE *player_judge, Sphere *m_PlayerEnemyColision, JUDGE *enemy_judge, Sphere *m_EnemyMyColision);
	static void Collision_EnemyVSEnemy(JUDGE *enemy_judge, Sphere *m_EnemyMyColision, JUDGE *enemy2_judge, Sphere *m_Enemy2MyColision);
protected:
	D3DXMATRIX m_mtxWorld;			//	ワールド変換用行列
	D3DXMATRIX m_mtxTranslation;	//	移動行列
	D3DXMATRIX m_mtxRotation;		//	移動行列
	D3DXMATRIX m_mtxScaling;		//	移動行列
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;			//  角度
	D3DXVECTOR3 m_Front;			//	フロントベクトル
	D3DXVECTOR3 m_Up;				//	ライトベクトル
	D3DXVECTOR3 m_Right;			//	アップベクトル

	float m_Angle;			//	角度

	float m_MaxHp;
	float m_Hp;				//	HP
	int m_Str;			    // 攻撃力
	float m_Str2;				// 倍率変化率の攻撃力
	int m_Def;			    // 防御力
	int m_Lv;				// レベル
	int m_Gold;				// 所持金
	int m_Exp;				// 経験値
	int m_Condition;			// 状態異常状態格納
	int m_BackFrame;//吹っ飛びのフレームカウント
	int attack_endframe; // 攻撃終了フレーム
	int walkf;
	int attackframe;
	int nanawalk;
	int m_TurnCount;	//　経過ターン数主に状態異常の時使う
	char name[MAX_NAME];			// 名前
	bool map_drawflag;		// ミニマップに表示するか true=表示
	bool rival_flag;		// 攻撃相手が死んでたらtrue
	bool enter_flag;		// エンターが押されたとき
	bool m_Baisoku_Flag;		// 倍速中の1ターン目はfalse trueの時はターンエンド
	bool m_Donsoku_Flag;		// 鈍足中の1ターン目はfalse trueの時は操作可能
	bool m_Back;//吹っ飛び状態ならtrue
	// 描画処理
	void DrawDX_Anime(D3DXMATRIX mtxWorld, THING* pNomalModel);
	void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, NormalModelData* pNomalModel);

	static HRESULT InitNormalModelLoad(NormalModelData *pNomalModel, LPSTR szXFileName);//ノーマルモデルの読み込み
	void Animation_Change(int index, float speed);
	LPD3DXANIMATIONSET pAnimSet[10];//選択したモデルに10個までのアニメーションをセット
	D3DXTRACK_DESC TrackDesc;
	SKIN_MESH SkinMesh;



	//XBOXゲームパッド変数
	static DIJOYSTATE2 js;
	static LPDIRECTINPUTDEVICE8 pJoyDevice;
	static HRESULT hr;
	static bool trigger;//ボタンをトリガー入力させる用

	//ゲームパッドのトリガー処理
	static void GamePadInit(void);
	static bool JoyDevice_IsTrigger(int nKey);
	//ゲームパッド十字キーのトリガー処理
	static bool JoyDevice_IsCrossTrigger(int nKey);
	//ゲームパッドプレス処理
	static bool JoyDevice_IsPress(int nKey);
	static bool JoyDevice_IsCrossPress(int nKey);
	//static THING Thing[];//読み込むモデルの最大数+1
	THING anime_model;//読み込むモデルの最大数+1
	NormalModelData Normal_model;//読み込むモデルの最大数+1
	
	int m_AnimationType;
	
private:

	//	モデルデータの構造体
	typedef struct
	{
		char filename[256];

	}MaterialFileData;
	//	モデルデータの構造体
	typedef struct
	{
		char filename[256];

	}MaterialFileData2;
	//	使いたいモデルの数だけ書く
	static MaterialFileData NORMAL_MODEL_FILES[];
	static MaterialFileData2 ANIME_MODEL_FILES[];
	static int MODEL_FILES_MAX;	//	テクスチャ構造体総数
	static int ANIME_MODEL_FILES_MAX;	//	テクスチャ構造体総数
	static NormalModelData NormalModel[];//読み込むモデルの最大数+1
	
	
	int m_3DObjIndex;		//	3Dオブジェクトインデックス
	int m_3DObjType;		//	3Dオブジェクトタイプ
	static int m_3DObjNum;	//	ゲームオブジェクト総数

	static C3DObj *p3DObj[MAX_GAMEOBJ];


	
	static bool VFCulling(D3DXVECTOR3* pPosition);

	FLOAT fAnimTime = 0.0f;
	BOOL boPlayAnim = true;
	//float fAnimTimeHold;


};

#endif // !1
