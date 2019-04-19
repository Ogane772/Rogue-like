#ifndef CENEMYSRIME_H_
#define CENEMYSRIME_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// �萔
//===================================
#define ENEMY_RADIUS	 (2.6f)				// �����蔻�蔼�a

class CEnemy_Srime : public CEnemy
{
public:
	CEnemy_Srime(int x, int z);
	~CEnemy_Srime();

	void Initialize(int x, int z);
	void Update(void);
	void Draw(void);

	void Enemy_AI(void);

	void Enemy_Act(void);
	void Enemy_Move(void);
	// �΂߈ړ�(�͋Z)
	void Enemy_lefttopMove(void);
	void Enemy_righttopMove(void);
	void Enemy_leftbottomMove(void);
	void Enemy_rightbottomMove(void);

	int Damage(int str);
	void Enemy_Destroy(void);
};
#endif 
