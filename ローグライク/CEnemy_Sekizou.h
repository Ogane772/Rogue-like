#ifndef CENEMYSEKIZOU_H_
#define CENEMYSEKIZOU_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// íËêî
//===================================

class CEnemy_Sekizou : public CEnemy
{
public:
	CEnemy_Sekizou(int x, int z, ENEMY_Data enemy_data);
	~CEnemy_Sekizou();

	void Initialize(int x, int z, ENEMY_Data enemy_data);
	void Update(void);
	void Draw(void);
	void EnemySkill(void)override;

};
#endif 
