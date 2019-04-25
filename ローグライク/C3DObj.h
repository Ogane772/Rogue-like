//////////////////////////////////////////////////
////
////	3D�I�u�W�F�N�g�N���X
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
//���S�����^�[�����鐔�l
#define DEATH (true)//���񂾂�true
#define NORMAL (false)
#define MAX_NAME (30) //���O�����̍ő吔�@�S�p�Ȃ̂Ŏ���15�����܂�
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
		D3DXVECTOR3 vPosition;
		int type;						// ���f���ɂ���ē�����ς��邽�߂̃^�C�v
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
		TYPE_etc,		// ���̑�
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
		ANIME_MODEL_MAX,//�A�j�����f���ő吔
	}AnimeModelFileData;;
	typedef enum {
		PLAYER_FIRST,			//�K���ړ��������̍ŏ��̏���	
		PLAYER_STANDBY,			// HP�̉񕜂Ȃǂ��s��
		PLAYER_KEY_INPUT,		// ���͑҂�
		PLAYER_SERECT_UI,		// ��q,�A�C�e���Ȃǂ̑I��
		PLAYER_DESTROY,			// �v���C���[�������
		PLAYER_ACT,				// �s����
		PLAYER_ACT_END,			// �s���I��
		PLAYER_MOVE,			// �ړ���
		PLAYER_MOVE_END,		// �ړ��I��
		PLAYER_TURN_END,		// �^�[���I��
		PLAYER_NONE
	}PLAYERTURN;
	typedef enum {
		ENEMY_WAIT,			// �v���C���[���͑҂�
		ENEMY_ACTION,		// �s����
		ENEMY_ACT_END,		// �s���I��
		ENEMY_MOVE,			// �ړ���
		ENEMY_MOVE_END,		// �ړ��I��
		ENEMY_TURN_END,		// �^�[���I��
		ENEMY_NONE
	}ENEMYTURN;

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
	static C3DObj *Get(int nIdx);					// �C���X�^���X�擾
	int Get_3DObjIndex() { return m_3DObjIndex; }	// ���[�N�C���f�b�N�X�擾
	int Get_3DObjType() { return m_3DObjType; }		// ��ގ擾
	int Get_Angle(void) { return (int)m_Angle; }	//	�p�x�擾	
	float Get_Hp(void) { return m_Hp; }		//	HP�擾
	float Get_MaxHp(void) { return m_MaxHp; }		//	�ő�HP�擾
	int Get_Lv(void) { return m_Lv; }		//	���x���擾
	int Get_Gold(void) { return m_Gold; }		// �������擾
	char* Get_Name(void) { return name; };		// ���O�擾
	bool Get_RivalFlag(void) { return rival_flag; }
	bool Get_MapDrawFlag(void) { return map_drawflag; }
	bool Get_EnterFlag(void) { return enter_flag; }
	void Set_MapDrawFlag(bool type) { map_drawflag = type; }
	void Set_RivalFlag(bool type) { rival_flag = type; }
	void Set_EnterFlag(bool type) { enter_flag = type; }
	void Set_Attack_End(int end) { attack_endframe = end; }
	
	D3DXVECTOR3 Get_Position(void) { return m_Position; } //���W�擾
	static char* Get_AnimeFileName(int index) { return ANIME_MODEL_FILES[index].filename; }
	virtual bool Get_DrawCheck(void) = 0;
	virtual bool Damage(int str) = 0;
	static HRESULT InitModelLoad();  //	���f���ǂݍ���
	//���f�����擾
	THING* C3DObj::GetAnimeModel(void);
	//THING GetAnimeModel(int index);
	NormalModelData GetNormalModel(int index) {	return NormalModel[index];}
	//NormalModelFile GetNormal(int index);
	CUserinterface::CHARATYPE type;			// �G�̎��
	//	�I������
	static void Model_Finalize(void);
	static void NormalModel_Finalize(NormalModelData *DeleteNormalModel);

	static JUDGE m_Judge_player;
	static JUDGE Judgement_GetPlayer(void) { return m_Judge_player; }
	int m_ObjectType;
	// �}�b�v�񎟌��z��p
	int m_Mapx;
	int m_Mapz;
	bool alive;			// �g���Ă��邩

	static Sphere m_EnemyColision;	// �v���C���[�̃G�l�~�[�Ƃ̓����蔻���p

	Sphere m_EnemyMyColision;		// �G�l�~�[�̓����蔻��
	const Sphere* Enemy_GetCollision(int index) { return &m_EnemyMyColision; }

	JUDGE m_Judge_enemy;

	JUDGE Judgement_GetEnemy(void) { return m_Judge_enemy; }
	//�����蔻��
	void PlayerVsWall(JUDGE *player_judge, Sphere *m_PlayerColision);

	static void Collision_AnimeVSAnime(JUDGE *player_judge, Sphere *m_PlayerEnemyColision, JUDGE *enemy_judge, Sphere *m_EnemyMyColision);
	static 	PLAYERTURN turn;
	ENEMYTURN enemyturn;
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
	int m_Def;			    // �h���
	int m_Lv;				// ���x��
	int m_Gold;				// ������

	int attack_endframe; // �U���I���t���[��
	int walkf;
	int attackframe;
	int nanawalk;
	char name[MAX_NAME];			// ���O
	bool map_drawflag;		// �~�j�}�b�v�ɕ\�����邩 true=�\��
	bool rival_flag;		// �U�����肪����ł���true
	bool enter_flag;		// �G���^�[�������ꂽ�Ƃ�
	// �`�揈��
	void DrawDX_Anime(D3DXMATRIX mtxWorld, int type, THING* pNomalModel);
	void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, NormalModelData* pNomalModel);


	void Animation_Change(int index, float speed);
	LPD3DXANIMATIONSET pAnimSet[10];//�I���������f����10�܂ł̃A�j���[�V�������Z�b�g
	D3DXTRACK_DESC TrackDesc;
	SKIN_MESH SkinMesh;




	//static THING Thing[];//�ǂݍ��ރ��f���̍ő吔+1
	THING Thing;//�ǂݍ��ރ��f���̍ő吔+1
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
	
	static HRESULT InitNormalModelLoad(NormalModelData *pNomalModel, LPSTR szXFileName);//�m�[�}�����f���̓ǂݍ���
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
