//////////////////////////////////////////////////
////
////	�G�l�~�[�N���X
////
//////////////////////////////////////////////////


//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "CEnemy.h"
#include "CEnemy_Srime.h"
#include "Cplayer.h"
#include "common.h"
#include "CTexture.h"
#include "map.h"
#include "sound.h"
#include "exp.h"
#include "wall.h"
#define _CRTDBG_MAP_ALLOC
#define ENEMY_FIELDMAX (60.0f)//�t�B�[���h�ɕ�����Enemy�ő吔
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	�萔��`
//=============================================================================

#define CHASE_SPEED (0.015f)
#define ENEMY_HITSTOP (30)//�q�b�g�X�g�b�v�̒���
//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

//	�G�l�~�[�ړ��p�\����
int CEnemy::m_EnemyNum[TYPE_MAX] = {};
int CEnemy::m_EnemyEnableNum = 0;
int CEnemy::m_EnemyMaxNum;
int CEnemy::TurnCount;
//=============================================================================
//	����
//=============================================================================

CEnemy::CEnemy()
{
	m_EnemyNum[TYPE_ALL]++;
}

CEnemy::CEnemy(int EnemyType)
{
	m_Type = EnemyType;
	m_EnemyIndex = m_EnemyNum[TYPE_ALL];
	m_EnemyNum[EnemyType]++;
	m_EnemyMaxNum++;
}


//=============================================================================
//	�j��
//=============================================================================

CEnemy::~CEnemy()
{
	m_EnemyMaxNum--;
	m_EnemyNum[TYPE_ALL]--;
	//SkinMesh.cHierarchy.DestroyFrame(Thing.pFrameRoot);
	//SkinMesh.cHierarchy.DestroyMeshContainer()
}

void CEnemy::Create(int enemy_type, int x, int z)
{
	switch (enemy_type)
	{
	case TYPE_SRIME:
		CEnemy_Srime *penemy_srime = new CEnemy_Srime(x, z);
		break;
	}
}

void CEnemy::Enemy_Finalize(int Index)
{

}

void CEnemy::EnemyTurnEnd(void)
{
	int i;
	int turnend_count;//�^�[���G���h�J�E���g
	turnend_count = 0;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		C3DObj *getplayer = CPlayer::Get_Player();
		C3DObj *enemy = CEnemy::Get_Enemy(i);
		//�N���G�l�~�[�����Ȃ��Ă��^�[���I����
		if (i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->turn == CPlayer::PLAYER_TURN_END)
		{
			TurnCount++;
			CPlayer::Player_NextTurn();
			if (TurnCount % 50 == 49)
			{
				int eposX;
				int eposZ;
				do
				{
					eposX = rand() % MAX_MAPWIDTH;
					eposZ = rand() % MAX_MAPHEIGHT;
				} while (CMap::Map_GetData(eposZ, eposX).type != 1);

				Create(TYPE_SRIME, eposX, eposZ);
			}
		}
		// �G�l�~�[�����Ȃ��ꍇ�̓R���e�B�j���[
		if (!enemy)
		{
			continue;
		}
		if (enemy->enemyturn == ENEMY_TURN_END)
		{
			turnend_count++;
		}
		// �G�S���̃^�[�����I��
		if (turnend_count == m_EnemyMaxNum && getplayer->turn == CPlayer::PLAYER_TURN_END)
		{
			TurnCount++;
			CPlayer::Player_NextTurn();

			// ���^�[�����ƂɃ����_���œG�𐶐�
			if (TurnCount % 50 == 49)
			{
				int eposX;
				int eposZ;
				do
				{
					eposX = rand() % MAX_MAPWIDTH;
					eposZ = rand() % MAX_MAPHEIGHT;
				} while (CMap::Map_GetData(eposZ, eposX).type != 1);

				//Create(TYPE_SRIME, eposX, eposZ);
			}
		}
	}
}

void CEnemy::EnemyVsWall(JUDGE *enemy_judge, Sphere *m_EnemyMyColision)
{
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (CWall::Wall_GetData(i).wuse)
		{
			for (int c = 0; c < WALL_WIDTH_MAX; c++)
			{
				// �G�l�~�[�ƕǂ̓����蔻��
				if (Collision_IsHitVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
				{

					if (Collision_IsHitTopVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitTop = true;
						enemy_judge->HitTopLeft = true;
						enemy_judge->HitTopRight = true;
					}

					if (Collision_IsHitBottomVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitBottom = true;
						enemy_judge->HitBottomLeft = true;
						enemy_judge->HitBottomRight = true;
					}

					if (Collision_IsHitRightVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitRight = true;
						enemy_judge->HitTopRight = true;
						enemy_judge->HitBottomRight = true;
					}

					if (Collision_IsHitLeftVtoS(CWall::Wall_GetCollision(i, c), m_EnemyMyColision))
					{
						enemy_judge->HitLeft = true;
						enemy_judge->HitTopLeft = true;
						enemy_judge->HitBottomLeft = true;
					}

				}
			}
		}
	}
}

C3DObj *CEnemy::Get_Enemy(int index)
{
	C3DObj *penemy = C3DObj::Get(index);
	if (penemy)
	{
		if (penemy->Get_3DObjType() == C3DObj::TYPE_ENEMY)
		{
			return penemy;
		}
	}
	return NULL;
}

void CEnemy::DeleteAllEnemy(void)
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		C3DObj *enemy = CEnemy::Get_Enemy(i);

		if (enemy)
		{
			delete enemy;
		}
	}
}