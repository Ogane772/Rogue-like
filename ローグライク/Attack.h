#ifndef ATTACK_H_
#define ATTACK_H_
#define ESCAPE_NUMBER (12)//‰ñ”ğ’l
#define ESCAPE_OK (true)  //‰ñ”ğ¬Œ÷
#define ESCAPE_NO (false) //‰ñ”ğ¸”s
class CAttack :virtual public C3DObj
{
public:
	static void Attack_PlayerUpdate(int charatype, float vec, int str);
	static void Attack_EnemyUpdate(int charatype, int e, float angle);
	static bool Attack_Check(void);//UŒ‚“–‚½‚é‚©”ğ‚¯‚é‚©
};
#endif