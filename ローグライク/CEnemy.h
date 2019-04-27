//////////////////////////////////////////////////
////
////	エネミークラス
////
//////////////////////////////////////////////////

#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "C3DObj.h"
#include "userinterface.h"
#define ENEMY_MAX (100)

class CEnemy :virtual public C3DObj
{
public:
	// 敵の種類
	enum
	{
		TYPE_ALL = 0,

		TYPE_SRIME,	

		TYPE_MAX
	};


	CEnemy();
	CEnemy(int nType);
	~CEnemy();

	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual bool Damage(int str) = 0;

	void Enemy_Finalize(int Index);
	static void EnemyTurnEnd(void);

	static void Create(int enemy_type, int x, int z);
	static int Get_EnemyNum(int EnemyType) { return m_EnemyNum[EnemyType]; }
	static int Get_EnemyIndex(int EnemyType) { return m_EnemyNum[EnemyType] - 1; }

	int Get_Type(void) { return m_Type; }
	static int Get_EnemyMaxNum(void) { return m_ENEMY_MAX; }
	static C3DObj *Get_Enemy(int index);
	bool Get_DrawCheck(void) { return alive; }
	static int Get_EnemyExp(int index) { return m_EnemyData[index].exp; }
	static char* Get_EnemyName(int index) { return m_EnemyData[index].enemy_name; }
	static void Reset_EnemyEnableNum(void) {  m_EnemyEnableNum = 0; }
	int m_Type;			// 種類
	static void DeleteAllEnemy(void);
	static void EnemyVsWall(JUDGE *enemy_judge, Sphere *m_EnemyMyColision);
protected:
	typedef struct {
		char enemy_name[MAX_NAME];
		float Hp;				
		int str,
			def,		
			exp,	
			gold;
		CUserinterface::CHARATYPE type;
	}ENEMY_Data;
	// 目的地
	int m_Goalz;
	int m_Goalx;
	// 通路通過中か
	bool m_Goway;

	float velocity;
	
	static bool attackflag; //エネミーが攻撃中ならtrue
	
	int add_time;//ターン終了時間+

	D3DXVECTOR3 vecenemy;
	D3DXVECTOR3 vec2enemy;

	static int TurnCount;

	int m_EnemyIndex;
	
	bool m_playercheck;

	int m_CreateCount;
	int m_InitCreateCount;
	int m_InitDirection;
	bool m_Death;	//	死んだら1回だけ通る用
	static int m_EnemyEnableNum;
	static int m_EnemyNum[TYPE_MAX];//各エネミーの総数
private:
	static int m_EnemyMaxNum;//エネミーの総数
	static int m_ENEMY_MAX;
	static ENEMY_Data m_EnemyData[];
};

#endif // !1