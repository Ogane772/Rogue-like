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
	CEnemy_Srime(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Srime();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;
	
};
#endif 
