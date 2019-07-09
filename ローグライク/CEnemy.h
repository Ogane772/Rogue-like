//////////////////////////////////////////////////
////
////	エネミークラス
////
//////////////////////////////////////////////////

#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "C3DObj.h"
#include "userinterface.h"
#define ENEMY_POINZU_DAMAGE (m_MaxHp / 8)
#define ENEMY_POIZUN_HEALTURN (10)
#define ENEMY_SLEEP_HEALTURN (2 + 1)//2ターンで回復
#define ENEMY_KURAYAMI_HEALTURN (10 + 1)//10ターンで回復
#define ENEMY_TOKUGI_HEALTURN (10 + 1)//10ターンで回復
class CEnemy :virtual public C3DObj
{
public:
	typedef enum {
		ENEMY_WAIT,			// プレイヤー入力待ち
		ENEMY_ACTION,		// 行動中
		ENEMY_ACT_END,		// 行動終了
		ENEMY_MOVE,			// 移動中
		ENEMY_MOVE_END,		// 移動終了
		ENEMY_TURN_END,		// ターン終了
		ENEMY_NONE
	}ENEMYTURN;
	enum EnemyAnimation
	{
		IDLE,
		WALK,
		ATTACK,
	};

	// 敵の種類
	enum
	{
		TYPE_ALL = 0,

		TYPE_SRIME,	

		TYPE_MAXENEMY
	};


	CEnemy();
	CEnemy(int nType);
	~CEnemy();

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	

	void Enemy_Finalize(int Index);
	static void EnemyTurnEnd(void);

	static void EnemyDataInit(void);//CSVの読み込み
	static void Create(int enemy_type, int x, int z);
	static int Get_EnemyNum(int EnemyType) { return m_EnemyNum[EnemyType]; }
	static int Get_EnemyIndex(int EnemyType) { return m_EnemyNum[EnemyType] - 1; }

	static int Get_EnemyMaxNum(void) { return m_ENEMY_MAX; }
	static C3DObj *Get_Enemy(int index);
	bool Get_DrawCheck(void) { return alive; }
	bool Get_RangeHit(void) { return m_RangeHit; }
	void Set_RangeHit(bool type) { m_RangeHit = type; }
	int Get_EnemyItem(void) { return set_item; }
	int Get_EnemyTurn(void){ return enemyturn; }
	void Set_EnemyTurn(int turn) { enemyturn = turn;}
	void Set_Warp(bool type) { m_Warp = type; }
	void Set_EnemyItem(int type) { set_item = type; }
	static int Get_EnemyExp(int index) { return m_EnemyData[index].exp; }
	static char* Get_EnemyName(int index) { return m_EnemyData[index].enemy_name; }
	static void Reset_EnemyEnableNum(void) {  m_EnemyEnableNum = 0; }
	static void DeleteAllEnemy(void);
	static void EnemyVsWall(JUDGE *enemy_judge, Sphere *m_EnemyMyColision);
protected:
	typedef struct {
		int wepon_type;
		char enemy_name[MAX_NAME];
		float Hp;				
		int str,
			def,		
			exp,	
			gold;
		int first_floor;//出現開始フロア
		int end_floor;//出現終わりフロア
		int enemychance;//出現率
	}ENEMY_Data;
	int enemyturn;

	//所持アイテム格納
	int set_item;
	// 目的地
	int m_Goalz;
	int m_Goalx;
	// 通路通過中か
	bool m_Goway;
	bool get_turbo;
	
	//ワープフラグ
	bool m_Warp;
	//ワープ予約フラグ
	bool m_TimeWarp;
	//複数攻撃を受けたときのフラグ
	bool m_RangeHit;
	float velocity;
	
	static bool attackflag; //エネミーが攻撃中ならtrue
	
	int add_time;//ターン終了時間+

	D3DXVECTOR3 vecenemy;
	D3DXVECTOR3 vec2enemy;

	static int TurnCount;

	int m_EnemyIndex;	
	
	static int m_EnemyEnableNum;
	static int m_EnemyNum[TYPE_MAXENEMY];//各エネミーの総数

	//関数

	void Enemy_Update(void);
	void Enemy_Draw(void);

	virtual void EnemySkill(void) = 0;

	void Enemy_AI(void);

	void Enemy_Act(void);
	void Enemy_TurboMove(void);
	void Enemy_Move(void);
	// 斜め移動(力技)
	void Enemy_lefttopMove(void);
	void Enemy_righttopMove(void);
	void Enemy_leftbottomMove(void);
	void Enemy_rightbottomMove(void);
	void Enemy_TurbolefttopMove(void);
	void Enemy_TurborighttopMove(void);
	void Enemy_TurboleftbottomMove(void);
	void Enemy_TurborightbottomMove(void);
	// 移動方向確定関数
	void Enemy_LeftMoveCheck(void);
	void Enemy_RightMoveCheck(void);
	void Enemy_BottomMoveCheck(void);
	void Enemy_TopMoveCheck(void);
	void Enemy_TopLeftMoveCheck(void);
	void Enemy_TopRightMoveCheck(void);
	void Enemy_BottomLeftMoveCheck(void);
	void Enemy_BottomRightMoveCheck(void);
	//吹っ飛び移動関数
	void Enemy_BackMove(void);
	//ワープ移動関数
	void Enemy_WarpMove(void);
	bool Damage(int str, float angle, int week_type);
	bool PoizunDamage(int str);
	void Enemy_Destroy(void);
	void Enemy_SetWorpPos(int pposZ, int pposX);
	void Enemy_PoizunDamageStart(void);
	void Enemy_PoizunDamage(void);
	void Enemy_ConditionCount(void);
	//アイテムドロップ関数
	void Enemy_ItemDrop(void);
private:
	static int m_EnemyMaxNum;//エネミーの総数
	static int m_ENEMY_MAX;
	static ENEMY_Data m_EnemyData[];

	static int CEnemy::CSV_EnemyLoad(ENEMY_Data* enemydata, const int num);
};

#endif // !1