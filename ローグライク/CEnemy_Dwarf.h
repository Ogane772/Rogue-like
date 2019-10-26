#ifndef CENEMYDWARF_H_
#define CENEMYDWARF_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// íËêî
//===================================

class CEnemy_Dwarf : public CEnemy
{
public:
	CEnemy_Dwarf(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Dwarf();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;

};
#endif 
