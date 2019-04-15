#include "collision.h"

// 球と球
bool Collision_IsHitStoS(const Sphere* pA, const Sphere* pB)
{
	//判定処理
	return D3DXVec3LengthSq(&(pA->position - pB->position)) < (pA->radius + pB->radius) * (pA->radius + pB->radius);
}

// 球と点
bool Collision_IsHitVtoS(const Vertex* pA, const Sphere* pB)
{
	//判定処理
	return D3DXVec3LengthSq(&(pB->position - pA->position)) < (pB->radius * pB->radius);
}

// 上判定
bool Collision_IsHitTopVtoS(const Vertex* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	return  1.0 < top  && top < 3.0;
}
// 下判定
bool Collision_IsHitBottomVtoS(const Vertex* pA, const Sphere* pB)
{
	float bottom = pA->position.z - pB->position.z;
	return  -1.0 > bottom  && bottom > -3.0;
}
// 右判定
bool Collision_IsHitRightVtoS(const Vertex* pA, const Sphere* pB)
{
	float right = pA->position.x - pB->position.x;
	return  1.0 < right  && right < 3.0;
}
// 左判定
bool Collision_IsHitLeftVtoS(const Vertex* pA, const Sphere* pB)
{
	float left = pA->position.x - pB->position.x;
	return  -1.0 > left  && left > -3.0;
}

// 上判定
bool Collision_IsHitTopStoS(const Sphere* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	float x = pA->position.x - pB->position.x;
	return  1.0 < top  && top < 6.0 && -0.1 < x && x < 0.1;
}
// 下判定
bool Collision_IsHitBottomStoS(const Sphere* pA, const Sphere* pB)
{
	float bottom = pA->position.z - pB->position.z;
	float x = pA->position.x - pB->position.x;
	return  -1.0 > bottom  && bottom > -6.0 && -0.1 < x && x < 0.1;
}
// 右判定
bool Collision_IsHitRightStoS(const Sphere* pA, const Sphere* pB)
{
	float right = pA->position.x - pB->position.x;
	float z = pA->position.z - pB->position.z;
	return  1.0 < right  && right < 6.0 && -0.1 < z && z < 0.1;
}
// 左判定
bool Collision_IsHitLeftStoS(const Sphere* pA, const Sphere* pB)
{
	float left = pA->position.x - pB->position.x;
	float z = pA->position.z - pB->position.z;
	return  -1.0 > left  && left > -6.0 && -0.1 < z && z < 0.1;
}

// 左上判定
bool Collision_IsHitLeftTopStoS(const Sphere* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	float Left = pA->position.x - pB->position.x;
	return  1.0 < top  && top < 6.0 && -1.0 > Left && Left > -6.0;
}
// 左下判定
bool Collision_IsHitLeftBottomStoS(const Sphere* pA, const Sphere* pB)
{
	float Bottom = pA->position.z - pB->position.z;
	float Left = pA->position.x - pB->position.x;
	return  -1.0 > Bottom  && Bottom > -6.0 && -1.0 > Left && Left > -6.0;
}
// 右上判定
bool Collision_IsHitRightTopStoS(const Sphere* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	float Right = pA->position.x - pB->position.x;
	return  1.0 < top  && top < 6.0 && 1.0 < Right && Right < 6.0;
}
// 右下判定
bool Collision_IsHitRightBottomStoS(const Sphere* pA, const Sphere* pB)
{
	float Bottom = pA->position.z - pB->position.z;
	float Right = pA->position.x - pB->position.x;
	return  -1.0 > Bottom  && Bottom > -6.0 && 1.0 < Right && Right < 6.0;
}