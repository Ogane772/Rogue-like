#ifndef CENEMYPIEL_H_
#define CENEMYPIEL_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// íËêî
//===================================

class CEnemy_Piel : public CEnemy
{
public:
	CEnemy_Piel(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Piel();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;

};
#endif 
