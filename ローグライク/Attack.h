#ifndef ATTACK_H_
#define ATTACK_H_
#define CHARGE_BUFF (1.25f)//�`���[�W�{��
#define ESCAPE_NUMBER (12)//���l
#define ESCAPE_OK (true)  //��𐬌�
#define ESCAPE_NO (false) //������s
//����`�F�b�N���邩�ǂ���
#define ESCAPE_CHECK_OK (true)//�`�F�b�N����
#define ESCAPE_CHECK_NO (false)//�`�F�b�N���Ȃ�
class CAttack :virtual public C3DObj
{
public:
	enum EnemySkillNumber{
		NORMAL_ATTACK_SKILL,
		HIGH_ATTACK_SKILL,
		HITYU_ATTACK_SKILL,//�K���a��
		HANI_FURASHU_SKILL,//�n�j�[�t���b�V��
		KIAIDAME_SKILL,//�C������
		SUNAKEMURI_SKILL,
		KOMORIUTA_SKILL,
		POIZUN_SKILL,
	};
	static void Attack_PlayerUpdate(int charatype, float vec, int str, float str2);
	static void Attack_PlayerSkill(int skill_number, float vec, int str, float str2);
	//�ڂ̑O��3�}�X�U��
	static int Attack_PlayerThreeRenge(int skill_number, float vec, int str, float str2);
	static void Attack_EnemyUpdate(int enemy_wepon_type, int charatype, int str, float str2 ,float angle);
	static void Attack_EnemySkill(int skill_number, int enemy_wepon_type, int charatype, int str, float str2, float angle, bool escape_check);
	static void Skill_PlayerEffect(C3DObj *enemy, int skill_number, int damage, int week_check);//�v���C���[�̑Ή�����X�L������
	static void Skill_Effect(int skill_number, int damage, int week_check, float angle, int charatype);//�G�̑Ή�����X�L������
	static bool Attack_Check(void);//�U�������邩�����邩
	static int Attack_WeekCheck(int attack_wepontype, int damage_wepontype);//�������r
	static int Attack_DamegeKeisan(int week_check, C3DObj *enemy, int str, float str2);//�_���[�W���v�Z���ĕԂ�
};
#endif