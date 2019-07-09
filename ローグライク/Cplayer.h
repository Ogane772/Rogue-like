#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "C3DObj.h"
#include <d3dx9.h>
#include "collision.h"
#define ATTACK_END (55)//�U���̑ҋ@����
#define NOATTACK_END (15) //��U��U���̑ҋ@����

#define PLAYER_RADIUS	 (2.6f)				// �����蔻�蔼�a
#define PLAYER_ENERADIUS (5.0f)				// �G�l�~�[��p�����蔻��
#define PLAYER_RECOVERY (100)				// ���R�� = MAXHP / PLAYER_RECOVERY
#define PLAYER_MAXHP (500)
#define PLAYER_STR (6)
#define PLAYER_HEAL (300)	//�������Ƃ��̉񕜗�
#define PLAYER_HIGH_HEAL (100)	//�q�[���t���OON�̎��̕������Ƃ��̉񕜗�
#define PLAYER_RING_HEAL (15) //�����̎w�ւ�+�l�����l
#define WALK_COUNT (9) //�E�H�[�N�J�E���g��b�l
#define MAX_GOLD (9999)//�ő�̂���	
#define MAX_ONAKA (100) //���Ȃ��̍ő�l
#define WAIT_FRAME (50) //�A�C�e���擾���̑ҋ@����
#define WAIT_ITEM_FRAME (90) //�A�C�e���g�p���̑ҋ@����
#define ITEM_EFFECT_FRAME (45)//�A�C�e���̌��ʔ����t���[��
#define MAX_ITEM (13)	//�����A�C�e���ő吔
#define MAX_WEPON (3)	//����������
#define DEFFAULT_ONAKA_TURN (10)//�����̂���������^�[����
#define SIAWASE_RING (1.5f)//�����킹�̎w�ւ̃f�t�H���g�l
//�v���C���[�̌���
#define UP_ANGLE (3.2f)
#define LEFT_ANGLE (1.6f)
#define DOWN_ANGLE (0.0f)
#define RIGHT_ANGLE (4.8f)
#define LEFT_TOP_ANGLE (2.4f)
#define LEFT_DOWN_ANGLE (0.8f)
#define RIGHT_TOP_ANGLE (4.0f)
#define RIGHT_DOWN_ANGLE (5.6f)
//�X�L���g�pHP��
#define SMOLL_SKILL_HP (m_MaxHp / 8)
#define MIDDLE_SKILL_HP (m_MaxHp / 4)
#define BIG_SKILL_HP (m_MaxHp / 2)
//㩐�����
#define TRAP_OK (70)
//0.25 19 
//0.5 9
class CPlayer :public C3DObj
{

public:
	typedef enum {
		PLAYER_FIRST,			// �K���ړ��������̍ŏ��̏���
		PLAYER_WORPFIRST,			// ���[�v�������̍ŏ��̏���	
		PLAYER_STANDBY,			// HP�̉񕜂Ȃǂ��s��
		PLAYER_KEY_INPUT,		// ���͑҂�
		PLAYER_WINDOW,			// �R�}���h�E�B���h�E���J���Ă�Ƃ�
		PLAYER_WINDOW_STATUS,	// �R�}���h�E�B���h�E�̔\�͊m�F��
		PLAYER_WINDOW_ASIMOTO,	// �R�}���h�E�B���h�E�̑����m�F
		PLAYER_WINDOW_ITEM,	    // �R�}���h�E�B���h�E�̓���m�F
		PLAYER_SERECT_UI,		// ��q,�A�C�e���Ȃǂ̑I��
		PLAYER_DESTROY,			// �v���C���[�������
		PLAYER_ITEM_EFFECT,		// �A�C�e���g�p��
		PLAYER_ITEM_WAIT,		// �A�C�e���擾���̑ҋ@����
		PLAYER_ENEMYTURN_WAIT,  // �G�^�[�����̑ҋ@����
		PLAYER_TIME_WAIT,      // ���͕s�̃v���C���[�ҋ@����
		PLAYER_RANGEHIT_WAIT,      // �����U�������^�[��
		PLAYER_CONDITION_HEAL_WAIT,// ��Ԉُ�񕜂̑ҋ@����
		PLAYER_SLEEP_WAIT,      // ������Ԃ̑ҋ@����
		PLAYER_WARP_WAIT,      // ���[�v��Ԃ̑ҋ@����
		PLAYER_ITEM_ASIMOTO,	// �A�C�e����u�����Ƃ�
		PLAYER_TRAP_EFFECT,		// ���i�𓥂񂾎��̌���
		PLAYER_ENEMY_BACK_WAIT, // �G������ђ��̂̑ҋ@����
		PLAYER_ACT,				// �s����
		PLAYER_ACT_END,			// �s���I��
		PLAYER_MOVE,			// �ړ���
		PLAYER_MOVE_END,		// �ړ��I��
		PLAYER_TURN_END,		// �^�[���I��
		PLAYER_TURN_CONPLETE,		// �{���ݑ����̃^�[���I��
		PLAYER_NONE
	}PLAYERTURN;
	typedef enum {
		CURSOL_LEFT_TOP,
		CURSOL_RIGHT_TOP,
		CURSOL_LEFT_DOWN,
		CURSOL_RIGHT_DOWN,
	}CURSOR_NUMBER;
	typedef enum {
		WEPON,//����
		ITEM,//�g�p�A�C�e��
	}ITEMTYPE;
	//�A�C�e���I�����̑I����
	typedef enum {
		EFFECT_ITEM,//�A�C�e���g�p
		ASIMOTO_ITEM,//���̏�ɒu��
		TIPS_ITEM,//����
	}ITEMWINDOW;
	//�����ԍ�
	typedef enum {
		WEPON_NUMBER,//����i�[�ԍ�
		SHELD_NUMBER,//���i�[�ԍ�
		RING_NUMBER,//�w�֊i�[�ԍ�
	}WEPON_SOUBINUMBER;
	typedef enum {
		TIPS_WEPON,
	}WEPONWINDOW;

	//����ϐ�
	typedef struct {
		int wepon_str;
		int wepon_def;
		int wepon_type;
		int wepon_purasu_number;
		int wepon_sikibetu_number;//�����̓�����ʂ𔻒肷�邽�߂̐���
	}PLAYER_WEPON;


	CPlayer();
	~CPlayer();


	static void Player_SetPos(int z, int x);		// �K�w���ƂɃv���C���[�̏����ʒu�����߂�
	static void Player_SetWorpPos(int z, int x);	// ���[�v���̃v���C���[�ʒu�����߂�
	void Update(void);
	void Draw(void);
	void Finalize(void);	//	�I������

	static CPlayer *PlayerCreate(void);
	static const Sphere* Player_GetCollision(void) { return &m_Colision; }
	static const Sphere* Player_GetEnemyCollision(void) { return &m_EnemyColision; }
	static PLAYER_WEPON* GetPlayerWeponData(int index) { return &player_wepon[index]; }
	static void Player_NextTurn(void);
	bool Damage(int str, float angle, int week_type);
	void Player_OnakaDown(void); //���Ȃ������炷
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
	bool ExpGoldCheck(int exp, int gold);		// �l���o���l�Ƃ�������
	

	static C3DObj *Get_Player(void);
	
	// �}�b�v�񎟌��z��p

	static bool goladder;	// ��q
protected:

	bool m_DrawCheck;			//	�`��t���O
private:
	typedef struct {
		int lv,
			lvup_exp,//���x���A�b�v�ɕK�v�Ȍo���l
			maxhp,
			str,
			def;
	}Player_LvData;
	void Initialize(void);	//	������
	int CSV_PlayerLv_Load(Player_LvData* lvdata, const int num);//�v���C���[�̃��x���e�[�u���ǂݍ���
	void Player_First(void);
	void Player_WorpIn(void);//���[�v���̑��
	void Player_Standby(void);
	void Player_KeyInput(void);
	void Player_WindowMode(void);
	void Player_SelectUi(void);
	void Player_SelectItem(void);
	void Player_ItemEffect(void);//�A�C�e���̎g�p�A�g�p����
	void Player_TrapEffect(void);//㩂̌���
	void Player_ItemAsimoto(void);//�A�C�e���𑫌��ɒu������
	void Player_CollisionUpdate(void);
	void Player_TurboMove(void);
	void Player_Move(void);
	void Player_MoveChenge(int MoveType);			//	�ړ������̏���
	void Player_TurboMoveChenge(int MoveType);			//	�ړ������̏���
	void Player_lefttopMove(void);
	void Player_righttopMove(void);
	void Player_leftbottomMove(void);
	void Player_rightbottomMove(void);
	void Player_TurbolefttopMove(void);
	void Player_TurborighttopMove(void);
	void Player_TurboleftbottomMove(void);
	void Player_TurborightbottomMove(void);
	void AngleChange(float Angle);	//	�����ϊ�
	void Player_Act(void);
	void Player_Destroy(void);
	void Player_BackCheck(void);//�A�C�e���擾���Ƀ����X�^�[���U�����Ă��Ȃ��悤���O�m�F
	void Player_Check(void);//�v���C���[�̓����蔻��
	void Player_TrapCheck(void);//㩂ɍU��������������悤�ɂ���
	void Player_WeponChenge(int wepon_type, int wepondata_number);//����̑��� �����@����̃^�C�v�A��������ꏊ
	void Player_W_WeponChenge(int wepon_type);//���蕐��̑��� �����@����̃^�C�v�A��������ꏊ
	void Player_ItemSort(void);//�g�p�A�C�e���폜���A�A�C�e����O�ɋl�߂�
	void Player_ItemSort2(void);//�r���̃A�C�e���폜���A�g�p�A�C�e���폜���A�A�C�e����O�ɋl�߂�
	bool JoyDevice_IsTrigger(int nKey);//�R���g���[���[�̃g���K�[����
	int WeponPurasuSet(void);//+�l�����߂ĕԂ�
	bool SkillHpCheck(void);//�X�L���g�pHP�����邩�Ԃ��@true=�g�p�ł���
	int ItemSnatch(void);//�A�C�e���𓐂܂ꂽ�Ƃ��̏����@�߂�l�@���܂ꂽ�A�C�e���ԍ�
	int m_PadDirection;
	enum
	{
		DIRE_TOP,		//�@��
		DIRE_RIGHT,		//	�E
		DIRE_BOTTOM,		//�@��
		DIRE_LEFT,		//�@��
		DIRE_TOP_RIGHT,	//	�E��
		DIRE_TOP_LEFT,	//	����
		DIRE_BOTTOM_RIGHT,//	�E��
		DIRE_BOTTOM_LEFT,	//	����

		DIRE_N,

	};//�@����
	int m_WeponStock[MAX_WEPON];//�擾�������i�[
	int m_ItemStock[MAX_ITEM];//�擾�A�C�e�����i�[
	int m_AllItemStock[MAX_WEPON + MAX_ITEM];//�S�擾�A�C�e�����i�[
	int m_ItemStockType[MAX_WEPON + MAX_ITEM];//�擾�A�C�e���̃^�C�v�i�[
	int m_Wait_frame;//�t���[���ҋ@����
	int m_Onaka;
	int m_MaxOnaka;
	int m_item_turncount;//��������A�C�e���̃^�[���J�E���g
	int m_CursorNumber;//�E�B���h�E�J�[�\���ʒu
	int m_TimeCursorNumber;//�A�C�e���I�����̃E�B���h�E�J�[�\���ʒu
	int m_AllWeponStock;//���������������Ă邩�i�[
	int m_Add_cursor;//����2�y�[�W�ڂ��J���Ă��瑫��
	int m_AutoHell;//�����񕜗�
	int m_TrapType;//���񂾃g���b�v�ԍ��i�[
	int m_TrapCheck;//㩔����`�F�b�N
	int m_OnakaTurn;//�����̌���^�[����
	int m_EnemyAttackCount;//�����̃G�l�~�[���U���������̐�
	static 	int m_Turn;
	int m_DrawCount;
	bool m_Wmode;//���蕐��������Ă�Ƃ���true
	bool m_ItemOn;//�A�C�e����I��������true
	bool m_ItemTips;//�A�C�e�������t���O
	bool m_NextItemPage;//�����A�C�e���E�B���h�E��2�y�[�W�ڂȂ�true
	bool right_trigger = false;
	bool left_trigger = false;
	bool right_on;//�E�X�e�B�b�N���͊m�F
	bool m_DarkFlag;//�Èł̒ʘH�ɂ�����true
	bool m_NanameFlag;//�΂߈ړ����[�h�̎�true
	bool m_HealFlag;//�����񕜍������t���O
	bool turbo;//�������t���O
	bool m_Gekika;//���������t���O
	bool m_EnemyOn;//�G�l�~�[�\���t���O
	bool m_SkillAtaack;//�X�L���U���Ȃ�true
	bool m_EnemyBack;//�G�l�~�[��������ђ���������true
	bool m_EnemyWarp;//�G�l�~�[��������ђ���������true
	bool m_EnemyDeath;//�G���U���ȊO�Ŏ��񂾂Ƃ��҂�
	static Sphere m_Colision;		// �����蔻��
	
	int charatype;		// �L�����N�^�[�̎��

	D3DXVECTOR3 walkpos;	// �ړ�����ꍇ�̖ړI�n�Z�b�g

	D3DXVECTOR3 vecplayer;
	D3DXVECTOR3 vec2player;	// �΂ߗp

	static D3DXVECTOR3 start_m_Position;//�J�n���̈ʒu
	static int start_m_Mapz;
	static int start_m_Mapx;

	static Player_LvData m_PlayerLvData[];
	static PLAYER_WEPON player_wepon[3];
};



#endif

