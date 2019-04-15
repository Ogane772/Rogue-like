#ifndef COLLISION_H_
#define COLLISION_H_

#include<d3dx9.h>

typedef struct
{
	D3DXVECTOR3 position;
	float radius;
} Sphere;

typedef struct
{
	D3DXVECTOR3 position;
	
} Vertex;

// 球と球
bool Collision_IsHitStoS(const Sphere* pA, const Sphere* pB);

// 球と点
bool Collision_IsHitVtoS(const Vertex* pA, const Sphere* pB);

// 球と点 どの方向にぶつかっているか

// 上判定
bool Collision_IsHitTopVtoS(const Vertex* pA, const Sphere* pB);

// 下判定
bool Collision_IsHitBottomVtoS(const Vertex* pA, const Sphere* pB);

// 右判定
bool Collision_IsHitRightVtoS(const Vertex* pA, const Sphere* pB);

// 左判定
bool Collision_IsHitLeftVtoS(const Vertex* pA, const Sphere* pB);

// 球と球 どの方向にぶつかっているか

// 上判定
bool Collision_IsHitTopStoS(const Sphere* pA, const Sphere* pB);
// 下判定
bool Collision_IsHitBottomStoS(const Sphere* pA, const Sphere* pB);
// 右判定
bool Collision_IsHitRightStoS(const Sphere* pA, const Sphere* pB);
// 左判定
bool Collision_IsHitLeftStoS(const Sphere* pA, const Sphere* pB);

// 左上判定
bool Collision_IsHitLeftTopStoS(const Sphere* pA, const Sphere* pB);
// 左下判定
bool Collision_IsHitLeftBottomStoS(const Sphere* pA, const Sphere* pB);
// 右上判定
bool Collision_IsHitRightTopStoS(const Sphere* pA, const Sphere* pB);
// 右下判定
bool Collision_IsHitRightBottomStoS(const Sphere* pA, const Sphere* pB);
#endif // COLLISION_H_ 
