//////////////////////////////////////////////////
////
////	3D�I�u�W�F�N�g�N���X
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
//���S�����^�[�����鐔�l
#define DEATH (true)//���񂾂�true
#define NORMAL (false)

//�_���[�W�v�Z�� ��ŕ���^�C�v�␳�������
#define PLAYER_W_DAMAGEKEISAN ((str * str2) / 2) - ((getplayer->Get_Def() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def) / 4)
#define PLAYER_NORMAL_DAMAGEKEISAN ((str * str2) / 2) - ((getplayer->Get_Def() + CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::SHELD_NUMBER)->wepon_def + CPlayer::GetPlayerWeponData(CPlayer::RING_NUMBER)->wepon_def) / 4)
#define ENEMY_DAMAGEKEISAN ((str * str2) / 2) - (enemy->Get_Def() / 4)
#define ENEMY_NODEF_DAMAGEKEISAN ((str * str2) / 2)
#define HITSTOP (30)
//XBOX�R���g���[���[�̏\���L�[
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
		LPD3DXMESH pMesh;	// ���b�V�����󂯎��ϐ�
		DWORD dwNumMaterials;	// �}�e���A������
		D3DMATERIAL9* pMaterials;
		LPDIRECT3DTEXTURE9* pTextures;
	}NormalModelData;
	typedef enum {
		NORMAL_CONDITION,
		SLEEP_CONDITION,//�������
		POIZUN_CONDITION,//�ŏ��
		KURAYAMI_CONDITION,
		MAHUJI_CONDITION,//���Z����
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

		// �U���p
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

		// �I�u�W�F�N�g�p
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
		TYPE_PLAYER,	// �v���C���[
		TYPE_ENEMY,		// �G�l�~�[

		TYPE_OBJECT,	// ��Q��
		TYPE_WEPON,	// ����
		TYPE_TRAP,	// ���i
		TYPE_etc,		// ���̑�
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
		ANIME_MODEL_MAX,//�A�j�����f���ő吔
	}AnimeModelFileData;;


	virtual void Finalize(void);
	virtual void Update(void) = 0;	//	�X�V
	virtual void Draw(void) = 0;	//	�`��

	D3DXMATRIX Get_mtxWorld(void) { return m_mtxWorld; }				//	���[���h�ϊ��p�s��擾
	D3DXMATRIX Get_mtxTranslation(void) { return m_mtxTranslation; }	//	�ړ��p�s��擾

	static void FinalizeAll();	// �S�I�u�W�F�N�g������
	static void UpdateAll();	// �S�I�u�W�F�N�g�X�V
	static void DrawAll();		// �S�I�u�W�F�N�g�`��
	static void DeleteAll();	// �S�I�u�W�F�N�g�폜
	void C3DObj_delete(void);	// �I�u�W�F�N�g�폜	
	//�Q�b�^�[
	static C3DObj *Get(int nIdx);	// �C���X�^���X�擾
	virtual int Get_PlayerWeponStock(int index) { return 0; }//��������擾
	virtual int Get_PlayerItemStock(int index) { return 0; }//�����A�C�e���擾
	virtual int Get_CursorNumber(void) { return 0; }//�E�B���h�E�J�[�\���ʒu�擾
	virtual int Get_TimeCursorNumber(void) { return 0; }
	virtual int Get_PlayerNextExp(int index) { return 0; } //���̃��x���܂ł̕K�v�o���l���擾
	virtual void Player_SkillHpDown(void) { };//�v���C���[��HP��������
	virtual void Player_OnakaDown(void) {}; //�v���C���[�̂��Ȃ������炷
	virtual int Get_PlayerOnaka(void) { return 0; }//�v���C���[�̂��Ȃ��擾
	virtual int Get_OnakaTurn(void) { return 0; }//�v���C���[�̂���������^�[�������擾
	virtual bool Get_DarkFlag(void) { return 0; };
	virtual bool Get_WMode(void) { return 0; }//�v���C���[�����莝�����擾
	virtual bool ExpGoldCheck(int exp, int gold) { return 0; }//�v���C���[�̌o���Ƃ����`�F�b�N
	virtual bool Get_PlayerHealFlag(void) { return 0; };
	virtual bool Get_TurboMode(void) { return 0; }//�^�[�{���[�h�擾
	virtual bool Get_ItemOn(void) { return 0; }//�A�C�e���I�����擾
	virtual bool Get_ItemTips(void) { return 0; }//�A�C�e����I��������ʂ�I�񂾂Ƃ�
	virtual bool Get_NextItemPage(void) { return 0; }//�����A�C�e���E�B���h�E��2�y�[�W�ڂ��ǂ����Ԃ�
	virtual bool Get_EnemyONFlag(void) { return 0; }//�}�b�v��̃G�l�~�[�A�C�R���\���t���O
	virtual int Get_PlayerItemStockType(int index) { return 0; }//�擾�A�C�e���^�C�v���擾
	virtual int Get_PlayerAllItemStock(int index) { return 0; }//�S�Ă̎擾�A�C�e�����i�[
	virtual int Get_PlayerTurn(void) { return 0; }//�v���C���[�̃^�[�����[�h�擾
	virtual int Player_SkillHpGet(void) { return 0; }//�X�L���̏���HP�擾
	virtual void Set_PlayerTurn(int turn) {}//�v���C���[�̃^�[�����[�h�Z�b�g
	virtual int Get_EnemyTurn(void) { return 0; }//�G�l�~�[�̃^�[�����[�h�擾
	virtual void Set_EnemyTurn(int turn) {}//�G�l�~�[�̃^�[�����[�h�Z�b�g
	virtual bool Get_Hold(void) { return 0; }//�L�[�������ς��擾
	virtual bool Get_NanameFlag(void) { return 0; }//�΂߈ړ���Ԃ��擾
	virtual bool Get_GekikaFlag(void) { return 0; }//����������Ԃ��擾
	virtual bool Get_EnemyPoizunDeath(void) { return 0; }//�G���łŎ���ł��邩�Ԃ�
	virtual bool GetWanaCheck(void) { return 0; }//㩂������Ă邩�擾
	virtual bool GetWanaEffectOn(void) { return 0; }//㩂̌��ʂ��������Ă邩�擾
	virtual void SetWanaCheck(bool type) { };//㩂������Ă邩�Z�b�g
	virtual void SetWanaEffectOn(bool type) { };//㩂̌��ʂ��������Ă邩�Z�b�g
	virtual void SetEnemyBack(bool type) { };//�G��������ђ����Z�b�g
	virtual void Enemy_SetWorpPos(int pposZ, int pposX) {};//�G�l�~�[�̃��[�v�ʒu�Z�b�g
	virtual void Set_Warp(bool type) {};
	virtual void SetEnemyDeath(bool type) { };
	virtual bool Get_EnemyDeath(void) { return 0; }
	virtual bool Get_RangeHit(void) { return 0; }
	virtual void Set_RangeHit(bool type) { };


	int Get_3DObjIndex() { return m_3DObjIndex; }	// ���[�N�C���f�b�N�X�擾
	int Get_3DObjType() { return m_3DObjType; }		// ��ގ擾
	float Get_Angle(void) { return m_Angle; }	//	�p�x�擾	
	float Get_Hp(void) { return m_Hp; }		//	HP�擾
	float Get_MaxHp(void) { return m_MaxHp; }		//	�ő�HP�擾
	int Get_Str(void) { return m_Str; }		//	�U���͎擾
	float Get_Str2(void) { return m_Str2; }		//	�ω��U���͎擾
	int Get_Def(void) { return m_Def; }		//	�h��͎擾
	int Get_Lv(void) { return m_Lv; }		//	���x���擾
	int Get_Gold(void) { return m_Gold; }		// �������擾
	int Get_Exp(void) { return m_Exp; }		// �o���l�擾
	int Get_Type(void) { return m_Type; }		// �G�l�~�[�^�C�v�擾
	int Get_Condition(void) { return m_Condition; }	// ��Ԏ擾
	char* Get_Name(void) { return name; };		// ���O�擾
	bool Get_RivalFlag(void) { return rival_flag; }
	bool Get_MapDrawFlag(void) { return map_drawflag; }
	bool Get_EnterFlag(void) { return enter_flag; }
	bool Get_BackMove(void) { return m_Back; }
	//�Z�b�^�[
	void SetCondition(int condition_type) { m_Baisoku_Flag = false; m_Donsoku_Flag = false; m_Condition = condition_type; }
	void Set_MapDrawFlag(bool type) { map_drawflag = type; }
	void Set_RivalFlag(bool type) { rival_flag = type; }
	void Set_EnterFlag(bool type) { enter_flag = type; }
	void SetBackMove(bool type) { m_Back = type; }//������я�Ԃ��Z�b�g
	void Set_Attack_End(int end) { attack_endframe = end; }//�U���I�����Ԃ��Z�b�g
	void SetTurnCount(int turn) { m_TurnCount = turn; }
	

	D3DXVECTOR3 Get_Position(void) { return m_Position; } //���W�擾
	static char* Get_AnimeFileName(int index) { return ANIME_MODEL_FILES[index].filename; }
	virtual bool Get_DrawCheck(void) = 0;
	virtual bool Damage(int str, float angle, int week_type) { return 0; }
	virtual int ItemSnatch(void) { return 0; }//�A�C�e���𓐂܂ꂽ�Ƃ��̏����@�߂�l�@���܂ꂽ�A�C�e���ԍ�
	virtual void Enemy_PoizunDamageStart(void) {};//�G�l�~�[�̓Ń_���[�W�v�Z
	virtual void Enemy_ConditionCount(void) {};//�G�l�~�[�̐����E�ÈŁE���Z��������
	virtual void Set_EnemyItem(int type) { };//�G�l�~�[�̏����A�C�e���Z�b�g
	virtual int Get_EnemyItem(void) { return 0; }//�G�l�~�[�̏����A�C�e���Q�b�g
	static HRESULT InitModelLoad();  //	���f���ǂݍ���
	//���f�����擾
	THING* C3DObj::GetAnimeModel(void);
	//THING GetAnimeModel(int index);
	NormalModelData GetNormalModel(int index) {	return NormalModel[index];}
	//NormalModelFile GetNormal(int index);
	int m_Type;			// �G�̎��
	//	�I������
	static void Model_Finalize(void);
	static void NormalModel_Finalize(NormalModelData *DeleteNormalModel);
	
	static void AnimeModel_Finalize(THING *DeleteAnimeModel);

	static JUDGE m_Judge_player;
	static JUDGE Judgement_GetPlayer(void) { return m_Judge_player; }
	int m_ObjectType;
	int m_TrapType;
	int m_WeponType;
	// �}�b�v�񎟌��z��p
	int m_Mapx;
	int m_Mapz;
	// 1�^�[���O���W
	int m_Oldmx;
	int m_Oldmz;
	bool alive;			// �g���Ă��邩

	static Sphere m_EnemyColision;	// �v���C���[�̃G�l�~�[�Ƃ̓����蔻���p

	Sphere m_EnemyMyColision;		// �G�l�~�[�̓����蔻��
	const Sphere* Enemy_GetCollision(int index) { return &m_EnemyMyColision; }

	JUDGE m_Judge_enemy;

	JUDGE Judgement_GetEnemy(void) { return m_Judge_enemy; }
	//�����蔻��
	void PlayerVsWall(JUDGE *player_judge, Sphere *m_PlayerColision);

	static void Collision_AnimeVSAnime(JUDGE *player_judge, Sphere *m_PlayerEnemyColision, JUDGE *enemy_judge, Sphere *m_EnemyMyColision);
	static void Collision_EnemyVSEnemy(JUDGE *enemy_judge, Sphere *m_EnemyMyColision, JUDGE *enemy2_judge, Sphere *m_Enemy2MyColision);
protected:
	D3DXMATRIX m_mtxWorld;			//	���[���h�ϊ��p�s��
	D3DXMATRIX m_mtxTranslation;	//	�ړ��s��
	D3DXMATRIX m_mtxRotation;		//	�ړ��s��
	D3DXMATRIX m_mtxScaling;		//	�ړ��s��
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;			//  �p�x
	D3DXVECTOR3 m_Front;			//	�t�����g�x�N�g��
	D3DXVECTOR3 m_Up;				//	���C�g�x�N�g��
	D3DXVECTOR3 m_Right;			//	�A�b�v�x�N�g��

	float m_Angle;			//	�p�x

	float m_MaxHp;
	float m_Hp;				//	HP
	int m_Str;			    // �U����
	float m_Str2;				// �{���ω����̍U����
	int m_Def;			    // �h���
	int m_Lv;				// ���x��
	int m_Gold;				// ������
	int m_Exp;				// �o���l
	int m_Condition;			// ��Ԉُ��Ԋi�[
	int m_BackFrame;//������т̃t���[���J�E���g
	int attack_endframe; // �U���I���t���[��
	int walkf;
	int attackframe;
	int nanawalk;
	int m_TurnCount;	//�@�o�߃^�[������ɏ�Ԉُ�̎��g��
	char name[MAX_NAME];			// ���O
	bool map_drawflag;		// �~�j�}�b�v�ɕ\�����邩 true=�\��
	bool rival_flag;		// �U�����肪����ł���true
	bool enter_flag;		// �G���^�[�������ꂽ�Ƃ�
	bool m_Baisoku_Flag;		// �{������1�^�[���ڂ�false true�̎��̓^�[���G���h
	bool m_Donsoku_Flag;		// �ݑ�����1�^�[���ڂ�false true�̎��͑���\
	bool m_Back;//������я�ԂȂ�true
	// �`�揈��
	void DrawDX_Anime(D3DXMATRIX mtxWorld, THING* pNomalModel);
	void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, NormalModelData* pNomalModel);

	static HRESULT InitNormalModelLoad(NormalModelData *pNomalModel, LPSTR szXFileName);//�m�[�}�����f���̓ǂݍ���
	void Animation_Change(int index, float speed);
	LPD3DXANIMATIONSET pAnimSet[10];//�I���������f����10�܂ł̃A�j���[�V�������Z�b�g
	D3DXTRACK_DESC TrackDesc;
	SKIN_MESH SkinMesh;



	//XBOX�Q�[���p�b�h�ϐ�
	static DIJOYSTATE2 js;
	static LPDIRECTINPUTDEVICE8 pJoyDevice;
	static HRESULT hr;
	static bool trigger;//�{�^�����g���K�[���͂�����p

	//�Q�[���p�b�h�̃g���K�[����
	static void GamePadInit(void);
	static bool JoyDevice_IsTrigger(int nKey);
	//�Q�[���p�b�h�\���L�[�̃g���K�[����
	static bool JoyDevice_IsCrossTrigger(int nKey);
	//�Q�[���p�b�h�v���X����
	static bool JoyDevice_IsPress(int nKey);
	static bool JoyDevice_IsCrossPress(int nKey);
	//static THING Thing[];//�ǂݍ��ރ��f���̍ő吔+1
	THING anime_model;//�ǂݍ��ރ��f���̍ő吔+1
	NormalModelData Normal_model;//�ǂݍ��ރ��f���̍ő吔+1
	
	int m_AnimationType;
	
private:

	//	���f���f�[�^�̍\����
	typedef struct
	{
		char filename[256];

	}MaterialFileData;
	//	���f���f�[�^�̍\����
	typedef struct
	{
		char filename[256];

	}MaterialFileData2;
	//	�g���������f���̐���������
	static MaterialFileData NORMAL_MODEL_FILES[];
	static MaterialFileData2 ANIME_MODEL_FILES[];
	static int MODEL_FILES_MAX;	//	�e�N�X�`���\���̑���
	static int ANIME_MODEL_FILES_MAX;	//	�e�N�X�`���\���̑���
	static NormalModelData NormalModel[];//�ǂݍ��ރ��f���̍ő吔+1
	
	
	int m_3DObjIndex;		//	3D�I�u�W�F�N�g�C���f�b�N�X
	int m_3DObjType;		//	3D�I�u�W�F�N�g�^�C�v
	static int m_3DObjNum;	//	�Q�[���I�u�W�F�N�g����

	static C3DObj *p3DObj[MAX_GAMEOBJ];


	
	static bool VFCulling(D3DXVECTOR3* pPosition);

	FLOAT fAnimTime = 0.0f;
	BOOL boPlayAnim = true;
	//float fAnimTimeHold;


};

#endif // !1
