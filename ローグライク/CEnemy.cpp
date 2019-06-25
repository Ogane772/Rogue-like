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
#include "map.h"
#include "sound.h"
#include "CWepon.h"
#include "wall.h"
#define _CRTDBG_MAP_ALLOC
#define ENEMY_FIELDMAX (60.0f)//�t�B�[���h�ɕ�����Enemy�ő吔
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	�萔��`
//=============================================================================

#define CHASE_SPEED (0.015f)
#define ENEMY_HITSTOP (30)//�q�b�g�X�g�b�v�̒���
#define ENEMY_MAX (30)
#define ENEMY_CSV_NAME "CSV/ENEMY_CSV.csv"
//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

//	�G�l�~�[�ړ��p�\����
int CEnemy::m_EnemyNum[TYPE_MAXENEMY] = {};
int CEnemy::m_EnemyEnableNum = 0;
int CEnemy::m_EnemyMaxNum = 0;
int CEnemy::TurnCount = 0;
bool CEnemy::attackflag = false;
CEnemy::ENEMY_Data CEnemy::m_EnemyData[ENEMY_MAX] = { 0 };
int CEnemy::m_ENEMY_MAX;
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

void CEnemy::EnemyDataInit(void)
{
	int i = 0;
	int end_check = 0;//CSV�̍s�̏I��肩�m�F
	for (i = 0; i < MAX_WEPON; i++)
	{//2�s�ڂ���ǂނ̂�+1����
		end_check = CSV_EnemyLoad(&m_EnemyData[i], i + 1);
		if (m_EnemyData[i].wepon_type == 0)
		{
			break;
		}
		m_ENEMY_MAX++;
	}
}

void CEnemy::Create(int enemy_type, int x, int z)
{
	switch (enemy_type)
	{
	case TYPE_SRIME:
		CEnemy_Srime *penemy_srime = new CEnemy_Srime(x, z , m_EnemyData[0]);
		break;
	}
}

void CEnemy::Enemy_Finalize(int Index)
{

}

void CEnemy::EnemyTurnEnd(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int i;
	int turnend_count;//�^�[���G���h�J�E���g
	int eposX;
	int eposZ;
	attackflag = false;
	C3DObj *getplayer;
	C3DObj *enemy;
	turnend_count = 0;
	getplayer = CPlayer::Get_Player();
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{	
		enemy = CEnemy::Get_Enemy(i);
		//�N���G�l�~�[�����Ȃ��Ă��^�[���I����
		if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION && m_EnemyMaxNum == 0 && i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_END
			|| getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION && m_EnemyMaxNum == 0 && i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_CONPLETE)
		{
			TurnCount++;
			if (TurnCount % 10 == 0 && TurnCount != 0)
			{
				getplayer->Player_OnakaDown();
			}
			CPlayer::Player_NextTurn();
			if (TurnCount % 50 == 49)
			{
				do
				{
					eposX = mt() % MAX_MAPWIDTH;
					eposZ = mt() % MAX_MAPHEIGHT;
				} while (CMap::Map_GetData(eposZ, eposX).type != 1);

				Create(TYPE_SRIME, eposX, eposZ);
			}
		}
		// �G�l�~�[�����Ȃ��ꍇ�̓R���e�B�j���[
		if (!enemy)
		{
			continue;
		}
		if (enemy->Get_EnemyTurn() == ENEMY_TURN_END)
		{
			turnend_count++;
		}
		// �G�S���̃^�[�����I��
		if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION && turnend_count == m_EnemyMaxNum && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_END
			|| getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION && turnend_count == m_EnemyMaxNum && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_CONPLETE)
		{
			TurnCount++;
			CPlayer::Player_NextTurn();
			if (!getplayer->Get_PlayerHealFlag())
			{
				if (TurnCount % 10 == 0 && TurnCount != 0)
				{
					getplayer->Player_OnakaDown();
				}
			}
			else
			{
				if (TurnCount % 5 == 0 && TurnCount != 0)
				{
					getplayer->Player_OnakaDown();
				}
			}
			// ���^�[�����ƂɃ����_���œG�𐶐�
			if (TurnCount % 50 == 49)
			{
				do
				{
					eposX = mt() % MAX_MAPWIDTH;
					eposZ = mt() % MAX_MAPHEIGHT;
				} while (CMap::Map_GetData(eposZ, eposX).type != 1);

				//Create(TYPE_SRIME, eposX, eposZ);
			}
		}
	}

}

void CEnemy::EnemyVsWall(JUDGE *enemy_judge, Sphere *m_EnemyMyColision)
{
	int i;
	int c;
	for (i = 0; i < WALL_MAX; i++)
	{
		if (CWall::Wall_GetData(i).wuse)
		{
			for (c = 0; c < WALL_WIDTH_MAX; c++)
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
	C3DObj *enemy;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		enemy = CEnemy::Get_Enemy(i);

		if (enemy)
		{
			delete enemy;
		}
	}
}

int CEnemy::CSV_EnemyLoad(ENEMY_Data* enemydata, const int num)
{
	int cnt = 0;
	char buf[128];
	int size = 128;
	int data[20] = { 0 };
	FILE* file = NULL;

	if ((file = fopen(ENEMY_CSV_NAME, "r")) == NULL)
	{
		MessageBox(NULL, "ENEMY_CSV��ǂݍ��߂܂���ł���\nCSV��ۑ����Ȃ����Β���\��������܂�", "�G���[", MB_OK);
		exit(1);
		return -1;
	}
	//�f�[�^��ǂݍ���
	while (fgets(buf, size, file) != NULL && num > ++cnt);
	fscanf(file, "%d,%[^,],%f,%d,%d,%d,%d,%d,%d,%d", &enemydata->wepon_type, enemydata->enemy_name , &enemydata->Hp, &enemydata->str, &enemydata->def, &enemydata->exp, &enemydata->gold, &enemydata->first_floor, &enemydata->end_floor, &enemydata->enemychance);

	fclose(file);



	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}