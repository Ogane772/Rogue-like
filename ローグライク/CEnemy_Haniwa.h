#ifndef CENEMYHANIWA_H_
#define CENEMYHANIWA_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// íËêî
//===================================

class CEnemy_Haniwa : public CEnemy
{
public:
	CEnemy_Haniwa(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Haniwa();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;

};
#endif 
