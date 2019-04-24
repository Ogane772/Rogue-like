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
#define PLAYER_HEAL (m_MaxHp / 5000)	//�������Ƃ��̉񕜗�
#define WALK_COUNT (9) //�E�H�[�N�J�E���g��b�l
//0.25 19 
//0.5 9
class CPlayer :public C3DObj
{

public:

	CPlayer();
	~CPlayer();


	static void Player_SetPos(int z, int x);		// �K�w���ƂɃv���C���[�̏����ʒu�����߂�
	void Update(void);
	void Draw(void);
	void Finalize(void);	//	�I������

	static CPlayer *PlayerCreate(void);
	static const Sphere* Player_GetCollision(void) { return &m_Colision; }
	static const Sphere* Player_GetEnemyCollision(void) { return &m_EnemyColision; }
	static void Player_NextTurn(void);
	bool Damage(int str);
	bool Get_DrawCheck(void) { return alive; }

	static C3DObj *Get_Player(void);
	// �}�b�v�񎟌��z��p

	static bool goladder;	// ��q
protected:

	bool m_DrawCheck;			//	�`��t���O
private:
	void Initialize(void);	//	������
	void Player_Move(void);
	void Player_MoveChenge(int MoveType);			//	�ړ������̏���
	void Player_lefttopMove(void);
	void Player_righttopMove(void);
	void Player_leftbottomMove(void);
	void Player_rightbottomMove(void);
	void Player_GamepadMove(void);			//	�Q�[���p�b�h�̈ړ�����
	void AngleChange(float Angle);	//	�����ϊ�
	void ControllerAngleChange(int index);	//	�R���g���[���[�̕����ϊ�
	void Player_Act(void);
	void Player_Destroy(void);
	void Player_Check(void);//�v���C���[�̓����蔻��
	bool JoyDevice_IsTrigger(int nKey);//�R���g���[���[�̃g���K�[����
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


	static CPlayer *m_pPlayer;

	int m_DrawCount;
	bool right_trigger = false;
	bool left_trigger = false;
	bool right_on;//�E�X�e�B�b�N���͊m�F

	static Sphere m_Colision;		// �����蔻��
	
	int charatype;		// �L�����N�^�[�̎��

	
	D3DXVECTOR3 walkpos;	// �ړ�����ꍇ�̖ړI�n�Z�b�g

	D3DXVECTOR3 vecplayer;
	D3DXVECTOR3 vec2player;	// �΂ߗp

	static D3DXVECTOR3 start_m_Position;//�J�n���̈ʒu
	static int start_m_Mapz;
	static int start_m_Mapx;
};



#endif // !

