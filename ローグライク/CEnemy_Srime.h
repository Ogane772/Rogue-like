#ifndef CENEMYSRIME_H_
#define CENEMYSRIME_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// íËêî
//===================================
#define ENEMY_RADIUS	 (2.6f)				// ìñÇΩÇËîªíËîºåa

class CEnemy_Srime : public CEnemy
{
public:
	CEnemy_Srime(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Srime();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);

	void Enemy_AI(void);

	void Enemy_Act(void);
	void Enemy_TurboMove(void);
	void Enemy_Move(void);
	// éŒÇﬂà⁄ìÆ(óÕãZ)
	void Enemy_lefttopMove(void);
	void Enemy_righttopMove(void);
	void Enemy_leftbottomMove(void);
	void Enemy_rightbottomMove(void);
	void Enemy_TurbolefttopMove(void);
	void Enemy_TurborighttopMove(void);
	void Enemy_TurboleftbottomMove(void);
	void Enemy_TurborightbottomMove(void);
	bool Damage(int str, float angle);
	void Enemy_Destroy(void);
};
#endif 
