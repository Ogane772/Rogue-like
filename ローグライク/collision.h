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

// ���Ƌ�
bool Collision_IsHitStoS(const Sphere* pA, const Sphere* pB);

// ���Ɠ_
bool Collision_IsHitVtoS(const Vertex* pA, const Sphere* pB);

// ���Ɠ_ �ǂ̕����ɂԂ����Ă��邩

// �㔻��
bool Collision_IsHitTopVtoS(const Vertex* pA, const Sphere* pB);

// ������
bool Collision_IsHitBottomVtoS(const Vertex* pA, const Sphere* pB);

// �E����
bool Collision_IsHitRightVtoS(const Vertex* pA, const Sphere* pB);

// ������
bool Collision_IsHitLeftVtoS(const Vertex* pA, const Sphere* pB);

// ���Ƌ� �ǂ̕����ɂԂ����Ă��邩

// �㔻��
bool Collision_IsHitTopStoS(const Sphere* pA, const Sphere* pB);
// ������
bool Collision_IsHitBottomStoS(const Sphere* pA, const Sphere* pB);
// �E����
bool Collision_IsHitRightStoS(const Sphere* pA, const Sphere* pB);
// ������
bool Collision_IsHitLeftStoS(const Sphere* pA, const Sphere* pB);

// ���㔻��
bool Collision_IsHitLeftTopStoS(const Sphere* pA, const Sphere* pB);
// ��������
bool Collision_IsHitLeftBottomStoS(const Sphere* pA, const Sphere* pB);
// �E�㔻��
bool Collision_IsHitRightTopStoS(const Sphere* pA, const Sphere* pB);
// �E������
bool Collision_IsHitRightBottomStoS(const Sphere* pA, const Sphere* pB);
#endif // COLLISION_H_ 
