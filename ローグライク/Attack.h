#ifndef ATTACK_H_
#define ATTACK_H_
#define ESCAPE_NUMBER (12)//���l
#define ESCAPE_OK (true)  //��𐬌�
#define ESCAPE_NO (false) //������s
class CAttack :virtual public C3DObj
{
public:
	static void Attack_PlayerUpdate(int charatype, float vec, int str);
	static void Attack_EnemyUpdate(int enemy_wepon_type, int charatype, int e, float angle);
	static bool Attack_Check(void);//�U�������邩�����邩
	static int Attack_WeekCheck(int attack_wepontype, int damage_wepontype);//�������r
};
#endif