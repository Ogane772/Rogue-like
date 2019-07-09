//////////////////////////////////////////////////
////
////	�G�l�~�[�N���X
////
//////////////////////////////////////////////////

#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "C3DObj.h"
#include "userinterface.h"
#define ENEMY_POINZU_DAMAGE (m_MaxHp / 8)
#define ENEMY_POIZUN_HEALTURN (10)
#define ENEMY_SLEEP_HEALTURN (2 + 1)//2�^�[���ŉ�
#define ENEMY_KURAYAMI_HEALTURN (10 + 1)//10�^�[���ŉ�
#define ENEMY_TOKUGI_HEALTURN (10 + 1)//10�^�[���ŉ�
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
	enum EnemyAnimation
	{
		IDLE,
		WALK,
		ATTACK,
	};

	// �G�̎��
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

	static void EnemyDataInit(void);//CSV�̓ǂݍ���
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
		int first_floor;//�o���J�n�t���A
		int end_floor;//�o���I���t���A
		int enemychance;//�o����
	}ENEMY_Data;
	int enemyturn;

	//�����A�C�e���i�[
	int set_item;
	// �ړI�n
	int m_Goalz;
	int m_Goalx;
	// �ʘH�ʉߒ���
	bool m_Goway;
	bool get_turbo;
	
	//���[�v�t���O
	bool m_Warp;
	//���[�v�\��t���O
	bool m_TimeWarp;
	//�����U�����󂯂��Ƃ��̃t���O
	bool m_RangeHit;
	float velocity;
	
	static bool attackflag; //�G�l�~�[���U�����Ȃ�true
	
	int add_time;//�^�[���I������+

	D3DXVECTOR3 vecenemy;
	D3DXVECTOR3 vec2enemy;

	static int TurnCount;

	int m_EnemyIndex;	
	
	static int m_EnemyEnableNum;
	static int m_EnemyNum[TYPE_MAXENEMY];//�e�G�l�~�[�̑���

	//�֐�

	void Enemy_Update(void);
	void Enemy_Draw(void);

	virtual void EnemySkill(void) = 0;

	void Enemy_AI(void);

	void Enemy_Act(void);
	void Enemy_TurboMove(void);
	void Enemy_Move(void);
	// �΂߈ړ�(�͋Z)
	void Enemy_lefttopMove(void);
	void Enemy_righttopMove(void);
	void Enemy_leftbottomMove(void);
	void Enemy_rightbottomMove(void);
	void Enemy_TurbolefttopMove(void);
	void Enemy_TurborighttopMove(void);
	void Enemy_TurboleftbottomMove(void);
	void Enemy_TurborightbottomMove(void);
	// �ړ������m��֐�
	void Enemy_LeftMoveCheck(void);
	void Enemy_RightMoveCheck(void);
	void Enemy_BottomMoveCheck(void);
	void Enemy_TopMoveCheck(void);
	void Enemy_TopLeftMoveCheck(void);
	void Enemy_TopRightMoveCheck(void);
	void Enemy_BottomLeftMoveCheck(void);
	void Enemy_BottomRightMoveCheck(void);
	//������шړ��֐�
	void Enemy_BackMove(void);
	//���[�v�ړ��֐�
	void Enemy_WarpMove(void);
	bool Damage(int str, float angle, int week_type);
	bool PoizunDamage(int str);
	void Enemy_Destroy(void);
	void Enemy_SetWorpPos(int pposZ, int pposX);
	void Enemy_PoizunDamageStart(void);
	void Enemy_PoizunDamage(void);
	void Enemy_ConditionCount(void);
	//�A�C�e���h���b�v�֐�
	void Enemy_ItemDrop(void);
private:
	static int m_EnemyMaxNum;//�G�l�~�[�̑���
	static int m_ENEMY_MAX;
	static ENEMY_Data m_EnemyData[];

	static int CEnemy::CSV_EnemyLoad(ENEMY_Data* enemydata, const int num);
};

#endif // !1