#include "collision.h"

// ���Ƌ�
bool Collision_IsHitStoS(const Sphere* pA, const Sphere* pB)
{
	//���菈��
	return D3DXVec3LengthSq(&(pA->position - pB->position)) < (pA->radius + pB->radius) * (pA->radius + pB->radius);
}

// ���Ɠ_
bool Collision_IsHitVtoS(const Vertex* pA, const Sphere* pB)
{
	//���菈��
	return D3DXVec3LengthSq(&(pB->position - pA->position)) < (pB->radius * pB->radius);
}

// �㔻��
bool Collision_IsHitTopVtoS(const Vertex* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	return  1.0 < top  && top < 3.0;
}
// ������
bool Collision_IsHitBottomVtoS(const Vertex* pA, const Sphere* pB)
{
	float bottom = pA->position.z - pB->position.z;
	return  -1.0 > bottom  && bottom > -3.0;
}
// �E����
bool Collision_IsHitRightVtoS(const Vertex* pA, const Sphere* pB)
{
	float right = pA->position.x - pB->position.x;
	return  1.0 < right  && right < 3.0;
}
// ������
bool Collision_IsHitLeftVtoS(const Vertex* pA, const Sphere* pB)
{
	float left = pA->position.x - pB->position.x;
	return  -1.0 > left  && left > -3.0;
}

// �㔻��
bool Collision_IsHitTopStoS(const Sphere* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	float x = pA->position.x - pB->position.x;
	return  1.0 < top  && top < 6.0 && -0.1 < x && x < 0.1;
}
// ������
bool Collision_IsHitBottomStoS(const Sphere* pA, const Sphere* pB)
{
	float bottom = pA->position.z - pB->position.z;
	float x = pA->position.x - pB->position.x;
	return  -1.0 > bottom  && bottom > -6.0 && -0.1 < x && x < 0.1;
}
// �E����
bool Collision_IsHitRightStoS(const Sphere* pA, const Sphere* pB)
{
	float right = pA->position.x - pB->position.x;
	float z = pA->position.z - pB->position.z;
	return  1.0 < right  && right < 6.0 && -0.1 < z && z < 0.1;
}
// ������
bool Collision_IsHitLeftStoS(const Sphere* pA, const Sphere* pB)
{
	float left = pA->position.x - pB->position.x;
	float z = pA->position.z - pB->position.z;
	return  -1.0 > left  && left > -6.0 && -0.1 < z && z < 0.1;
}

// ���㔻��
bool Collision_IsHitLeftTopStoS(const Sphere* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	float Left = pA->position.x - pB->position.x;
	return  1.0 < top  && top < 6.0 && -1.0 > Left && Left > -6.0;
}
// ��������
bool Collision_IsHitLeftBottomStoS(const Sphere* pA, const Sphere* pB)
{
	float Bottom = pA->position.z - pB->position.z;
	float Left = pA->position.x - pB->position.x;
	return  -1.0 > Bottom  && Bottom > -6.0 && -1.0 > Left && Left > -6.0;
}
// �E�㔻��
bool Collision_IsHitRightTopStoS(const Sphere* pA, const Sphere* pB)
{
	float top = pA->position.z - pB->position.z;
	float Right = pA->position.x - pB->position.x;
	return  1.0 < top  && top < 6.0 && 1.0 < Right && Right < 6.0;
}
// �E������
bool Collision_IsHitRightBottomStoS(const Sphere* pA, const Sphere* pB)
{
	float Bottom = pA->position.z - pB->position.z;
	float Right = pA->position.x - pB->position.x;
	return  -1.0 > Bottom  && Bottom > -6.0 && 1.0 < Right && Right < 6.0;
}