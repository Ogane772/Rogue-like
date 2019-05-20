//////////////////////////////////////////////////
////
////	�G�l�~�[�N���X
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
	typedef enum {
		ENEMY_WAIT,			// �v���C���[���͑҂�
		ENEMY_ACTION,		// �s����
		ENEMY_ACT_END,		// �s���I��
		ENEMY_MOVE,			// �ړ���
		ENEMY_MOVE_END,		// �ړ��I��
		ENEMY_TURN_END,		// �^�[���I��
		ENEMY_NONE
	}ENEMYTURN;
	// �G�̎��
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
	virtual bool Damage(int str, float angle) = 0;

	void Enemy_Finalize(int Index);
	static void EnemyTurnEnd(void);

	static void Create(int enemy_type, int x, int z);
	static int Get_EnemyNum(int EnemyType) { return m_EnemyNum[EnemyType]; }
	static int Get_EnemyIndex(int EnemyType) { return m_EnemyNum[EnemyType] - 1; }

	static int Get_EnemyMaxNum(void) { return m_ENEMY_MAX; }
	static C3DObj *Get_Enemy(int index);
	bool Get_DrawCheck(void) { return alive; }
	int Get_EnemyTurn(void){ return enemyturn; }
	void Set_EnemyTurn(int turn) { enemyturn = turn;	}
	static int Get_EnemyExp(int index) { return m_EnemyData[index].exp; }
	static char* Get_EnemyName(int index) { return m_EnemyData[index].enemy_name; }
	static void Reset_EnemyEnableNum(void) {  m_EnemyEnableNum = 0; }
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
		int first_floor;//�o���J�n�t���A
		int end_floor;//�o���I���t���A
	}ENEMY_Data;
	int enemyturn;
	// �ړI�n
	int m_Goalz;
	int m_Goalx;
	// �ʘH�ʉߒ���
	bool m_Goway;

	float velocity;
	
	static bool attackflag; //�G�l�~�[���U�����Ȃ�true
	
	int add_time;//�^�[���I������+

	D3DXVECTOR3 vecenemy;
	D3DXVECTOR3 vec2enemy;

	static int TurnCount;

	int m_EnemyIndex;	

	static int m_EnemyEnableNum;
	static int m_EnemyNum[TYPE_MAX];//�e�G�l�~�[�̑���
private:
	static int m_EnemyMaxNum;//�G�l�~�[�̑���
	static int m_ENEMY_MAX;
	static ENEMY_Data m_EnemyData[];
};

#endif // !1