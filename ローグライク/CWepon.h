#ifndef CWEPON_H_
#define CWEPON_H_

#include "C3DObj.h"
//===================================
// �萔
//===================================
#define WEPON_MAXTIPS (99)//�������̍ő咷��
#define MAX_WEPON_NUMBER (30)//����ԍ��i�[�ő吔
class CWepon :virtual public C3DObj
{
public:
	typedef struct {
		int type;//����^�C�v
		char Wepon_name[MAX_NAME];
		char Wepon_effect[WEPON_MAXTIPS];//�I�u�W�F�N�g������
		int str;
		int	def;
		int first_floor;//�o���J�n�t���A
		int end_floor;//�o���I���t���A
		int weponchance;//�����o����
		int rare;//����̃��A�l
		int kantei_type;//�ǂ̑��������ʂ���
	}Wepon_Data;
	//����X�L���̖��O�Ɛ�����
	typedef struct {
		char Skill_name[MAX_NAME];
		char Skill_Hp[MAX_NAME];//�X�L���̏���HP��
		char Skill_effect[WEPON_MAXTIPS];
		char Skill_uieffect[WEPON_MAXTIPS];//UI���O�ł̕\��
	}Wepon_Skill;
	typedef enum {
		TYPE_ALL = 0,

		TYPE_SWORD,//�Ў茕
		TYPE_BIGSWORD,//�匕
		TYPE_REIPIA,//���C�s�A
		TYPE_RANCE,//�����X
		TYPE_MEISU,//���C�X
		TYPE_AX,//��	
		TYPE_SHELD,//��
		TYPE_RING,//�w��
		WEPONTYPE_NONE
	}WEPONTTYPE;

	typedef enum {
		TYPE_NONE = 0,

		TYPE_IYASI_RING,
		TYPE_WANAYOKE_RING,
		TYPE_HARAMOTI_RING,
		TYPE_SIAWASE_RING,
		TYPE_HIGHATTACK_SKILL,//���U��
		TYPE_BACKATTACK_SKILL,//������΂��U��
		TYPE_WARP_SKILL,//���[�v�U��	
		TYPE_POIZUN_ATTACK,//�ōU��	
		TYPE_SLEEP_ATTACK,//����U��
		TYPE_KURAYAMI_ATTACK,//�ÈōU��	
		TYPE_MAHUJI_ATTACK,//�������U��	
		TYPE_THREE_ATTACK,//�ڂ̑O3�}�X�U��	
		WEPON_NAME_NONE
	}WEPON_NAME_TYPE;

	CWepon();
	CWepon::CWepon(int WeponType);
	~CWepon();

	void Wepon_Finalize(int Index);
	static void WeponDataInit(void);
	static void Create(int wepon_type, int x, int z);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int Get_WEPONDATAMAX(void) { return m_WEPONDATA_MAX; }
	static int Get_WeponNum(int OrnamType) { return m_WeponNum[OrnamType]; }
	static int Get_WeponIndex(int EnemyType) { return m_WeponNum[EnemyType] - 1; }
	int Get_Type(void) { return m_Type; }
	static Wepon_Skill* Get_Wepon_Skill(int index) { return &m_WeponSkill[index]; }
	static Wepon_Data* Get_Wepon_Data(int index) { return &m_WeponData[index]; }
	static char* Get_WeponName(int index) { return m_WeponData[index].Wepon_name; }
	static char* Get_WeponEffect(int index) { return m_WeponData[index].Wepon_effect; }
	static C3DObj *Get_Wepon(int index);
	bool Get_DrawCheck(void) { return alive; }
	static void CWepon::DeleteAllWepon(void);
	static C3DObj *Get_Map_Wepon(int Index);
protected:

	int m_WeponIndex;

	static C3DObj *Get_AllWepon(int index);

private:
	static int m_WEPONDATA_MAX;//�E�F�|���f�[�^�̍ő吔
	static int m_WeponNum[WEPONTYPE_NONE];

	static Wepon_Data m_WeponData[];
	static Wepon_Skill m_WeponSkill[];
	static int CSV_Wepon_Load(Wepon_Data* wepondata, const int num);//�����e�[�u���ǂݍ���
};
#endif
