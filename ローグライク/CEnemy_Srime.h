#ifndef CENEMYSRIME_H_
#define CENEMYSRIME_H_

#include <d3dx9.h>
#include "collision.h"
#include "userinterface.h"
//===================================
// 定数
//===================================
#define ENEMY_RADIUS	 (2.6f)				// 当たり判定半径

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
	// 斜め移動(力技)
	void Enemy_lefttopMove(void);
	void Enemy_righttopMove(void);
	void Enemy_leftbottomMove(void);
	void Enemy_rightbottomMove(void);
	void Enemy_TurbolefttopMove(void);
	void Enemy_TurborighttopMove(void);
	void Enemy_TurboleftbottomMove(void);
	void Enemy_TurborightbottomMove(void);
	// 移動方向確定関数
	void Enemy_LeftMoveCheck(void);
	void Enemy_RightMoveCheck(void);
	void Enemy_BottomMoveCheck(void);
	void Enemy_TopMoveCheck(void);
	void Enemy_TopLeftMoveCheck(void);
	void Enemy_TopRightMoveCheck(void);
	void Enemy_BottomLeftMoveCheck(void);
	void Enemy_BottomRightMoveCheck(void);
	bool Damage(int str, float angle, int week_type);
	void Enemy_Destroy(void);
};
#endif 
