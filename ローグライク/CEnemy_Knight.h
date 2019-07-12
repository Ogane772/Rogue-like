#ifndef CENEMYKNIGHT_H_
#define CENEMYKNIGHT_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// íËêî
//===================================

class CEnemy_Knight : public CEnemy
{
public:
	CEnemy_Knight(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Knight();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;

};
#endif 
