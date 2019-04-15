#ifndef ATTACK_H_
#define ATTACK_H_

class CAttack :virtual public C3DObj
{
public:
	static void Attack_PlayerUpdate(int charatype, float vec, int str);
	static void Attack_EnemyUpdate(int charatype, int e);
};
#endif