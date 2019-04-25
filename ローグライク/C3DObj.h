//////////////////////////////////////////////////
////
////	3Dオブジェクトクラス
////
//////////////////////////////////////////////////

#ifndef _3DOBJ_H_
#define _3DOBJ_H_
#include <crtdbg.h>

#include "CGameObj.h"
#include <d3dx9.h>
#include "CSkinAnimation.h"
#include "collision.h"
#include "userinterface.h"
//死亡時リターンする数値
#define DEATH (true)//死んだらtrue
#define NORMAL (false)
#define MAX_NAME (30) //名前文字の最大数　全角なので実質15文字まで
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
		D3DXVECTOR3 vPosition;
		int type;						// モデルによって動きを変えるためのタイプ
	}NormalModelData;
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
		TYPE_etc,		// その他
		TYPE_MAX

	} OBJTYPE;

	typedef enum {
		MODELL_PLAYER,
		MODELL_ENEMY_1,
	}NorMalModelFileData;
	enum ANIME_MODEL
	{
		MODELL_ANIME_PLAYER,
		MODELL_ANIME_SMALL,
		MODELL_ANIME_MIDDLE,
		MODELL_ANIME_SPECIAL,
		MODELL_ANIME_BIG,
		ANIME_MODEL_MAX,//アニメモデル最大数
	}AnimeModelFileData;;
	typedef enum {
		PLAYER_FIRST,			//階を移動した時の最初の処理	
		PLAYER_STANDBY,			// HPの回復などを行う
		PLAYER_KEY_INPUT,		// 入力待ち
		PLAYER_SERECT_UI,		// 梯子,アイテムなどの選択
		PLAYER_DESTROY,			// プレイヤーがやられる
		PLAYER_ACT,				// 行動中
		PLAYER_ACT_END,			// 行動終了
		PLAYER_MOVE,			// 移動中
		PLAYER_MOVE_END,		// 移動終了
		PLAYER_TURN_END,		// ターン終了
		PLAYER_NONE
	}PLAYERTURN;
	typedef enum {
		ENEMY_WAIT,			// プレイヤー入力待ち
		ENEMY_ACTION,		// 行動中
		ENEMY_ACT_END,		// 行動終了
		ENEMY_MOVE,			// 移動中
		ENEMY_MOVE_END,		// 移動終了
		ENEMY_TURN_END,		// ターン終了
		ENEMY_NONE
	}ENEMYTURN;

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
	static C3DObj *Get(int nIdx);					// インスタンス取得
	int Get_3DObjIndex() { return m_3DObjIndex; }	// ワークインデックス取得
	int Get_3DObjType() { return m_3DObjType; }		// 種類取得
	int Get_Angle(void) { return (int)m_Angle; }	//	角度取得	
	float Get_Hp(void) { return m_Hp; }		//	HP取得
	float Get_MaxHp(void) { return m_MaxHp; }		//	最大HP取得
	int Get_Lv(void) { return m_Lv; }		//	レベル取得
	int Get_Gold(void) { return m_Gold; }		// 所持金取得
	char* Get_Name(void) { return name; };		// 名前取得
	bool Get_RivalFlag(void) { return rival_flag; }
	bool Get_MapDrawFlag(void) { return map_drawflag; }
	bool Get_EnterFlag(void) { return enter_flag; }
	void Set_MapDrawFlag(bool type) { map_drawflag = type; }
	void Set_RivalFlag(bool type) { rival_flag = type; }
	void Set_EnterFlag(bool type) { enter_flag = type; }
	void Set_Attack_End(int end) { attack_endframe = end; }
	
	D3DXVECTOR3 Get_Position(void) { return m_Position; } //座標取得
	static char* Get_AnimeFileName(int index) { return ANIME_MODEL_FILES[index].filename; }
	virtual bool Get_DrawCheck(void) = 0;
	virtual bool Damage(int str) = 0;
	static HRESULT InitModelLoad();  //	モデル読み込み
	//モデル情報取得
	THING* C3DObj::GetAnimeModel(void);
	//THING GetAnimeModel(int index);
	NormalModelData GetNormalModel(int index) {	return NormalModel[index];}
	//NormalModelFile GetNormal(int index);
	CUserinterface::CHARATYPE type;			// 敵の種類
	//	終了処理
	static void Model_Finalize(void);
	static void NormalModel_Finalize(NormalModelData *DeleteNormalModel);

	static JUDGE m_Judge_player;
	static JUDGE Judgement_GetPlayer(void) { return m_Judge_player; }
	int m_ObjectType;
	// マップ二次元配列用
	int m_Mapx;
	int m_Mapz;
	bool alive;			// 使われているか

	static Sphere m_EnemyColision;	// プレイヤーのエネミーとの当たり判定専用

	Sphere m_EnemyMyColision;		// エネミーの当たり判定
	const Sphere* Enemy_GetCollision(int index) { return &m_EnemyMyColision; }

	JUDGE m_Judge_enemy;

	JUDGE Judgement_GetEnemy(void) { return m_Judge_enemy; }
	//当たり判定
	void PlayerVsWall(JUDGE *player_judge, Sphere *m_PlayerColision);

	static void Collision_AnimeVSAnime(JUDGE *player_judge, Sphere *m_PlayerEnemyColision, JUDGE *enemy_judge, Sphere *m_EnemyMyColision);
	static 	PLAYERTURN turn;
	ENEMYTURN enemyturn;
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
	int m_Def;			    // 防御力
	int m_Lv;				// レベル
	int m_Gold;				// 所持金

	int attack_endframe; // 攻撃終了フレーム
	int walkf;
	int attackframe;
	int nanawalk;
	char name[MAX_NAME];			// 名前
	bool map_drawflag;		// ミニマップに表示するか true=表示
	bool rival_flag;		// 攻撃相手が死んでたらtrue
	bool enter_flag;		// エンターが押されたとき
	// 描画処理
	void DrawDX_Anime(D3DXMATRIX mtxWorld, int type, THING* pNomalModel);
	void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, NormalModelData* pNomalModel);


	void Animation_Change(int index, float speed);
	LPD3DXANIMATIONSET pAnimSet[10];//選択したモデルに10個までのアニメーションをセット
	D3DXTRACK_DESC TrackDesc;
	SKIN_MESH SkinMesh;




	//static THING Thing[];//読み込むモデルの最大数+1
	THING Thing;//読み込むモデルの最大数+1
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
	
	static HRESULT InitNormalModelLoad(NormalModelData *pNomalModel, LPSTR szXFileName);//ノーマルモデルの読み込み
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
