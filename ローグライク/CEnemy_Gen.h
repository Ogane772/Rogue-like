#ifndef CENEMYGEN_H_
#define CENEMYGEN_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"

class CEnemy_Gen : public CEnemy
{
public:
	CEnemy_Gen(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Gen();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;
	
};
#endif 
