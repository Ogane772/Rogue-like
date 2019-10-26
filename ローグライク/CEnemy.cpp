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
#include "CEnemy_Gen.h"
#include "CEnemy_Knight.h"
#include "CEnemy_Piel.h"
#include "CEnemy_Dwarf.h"
#include "CEnemy_Haniwa.h"
#include "CEnemy_Sekizou.h"
#include "Cplayer.h"
#include "map.h"
#include "sound.h"
#include "CWepon.h"
#include "wall.h"
#include "bilboard.h"
#include "save_load.h"
#include "Attack.h"
#include "exp.h"
#define _CRTDBG_MAP_ALLOC
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	�萔��`
//=============================================================================
#define ENEMY_HITSTOP (30)//�q�b�g�X�g�b�v�̒���
#define ENEMY_MAX (30)
#define ENEMY_CSV_NAME "CSV/ENEMY_CSV.csv"
#define ENEMY_ON_TURN (50) //�G�l�~�[�o���^�[����
#define ENEMY_COND_ICON_Y (4)
//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

//	�G�l�~�[�ړ��p�\����
int CEnemy::m_EnemyNum[TYPE_MAXENEMY] = {};
int CEnemy::m_EnemyEnableNum = 0;
int CEnemy::m_EnemyMaxNum = 0;
int CEnemy::TurnCount = 0;
int CEnemy::m_ENEMYDATA_MAX = 0;
bool CEnemy::attackflag = false;
CEnemy::ENEMY_Data CEnemy::m_EnemyData[ENEMY_MAX] = { 0 };
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
	for (i = 0; i < ENEMY_MAX; i++)
	{//2�s�ڂ���ǂނ̂�+1����
		end_check = CSV_EnemyLoad(&m_EnemyData[i], i + 1);
		if (m_EnemyData[i].wepon_type == 0)
		{
			break;
		}
		m_ENEMYDATA_MAX++;
	}
}

void CEnemy::Create(int enemy_type, int x, int z)
{
	switch (enemy_type)
	{
	case TYPE_GEN:
	{
		CEnemy_Gen *penemy_gen = new CEnemy_Gen(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_KNIGHT:
	{
		CEnemy_Knight *penemy_knight = new CEnemy_Knight(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_PIEL:
	{
		CEnemy_Piel *penemy_piel = new CEnemy_Piel(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_DWARF:
	{
		CEnemy_Dwarf *penemy_dwarf = new CEnemy_Dwarf(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_HANIWA:
	{
		CEnemy_Haniwa *penemy_haniwa = new CEnemy_Haniwa(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
	case TYPE_SEKIZOU:
	{
		CEnemy_Sekizou *penemy_sekizou = new CEnemy_Sekizou(x, z, m_EnemyData[enemy_type - 1]);
		break;
	}
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
		if (!enemy)
		{
			//�N���G�l�~�[�����Ȃ��Ă��^�[���I������������
			if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION && m_EnemyMaxNum == 0 && i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_END
				|| getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION && m_EnemyMaxNum == 0 && i == MAX_GAMEOBJ - 1 && turnend_count == 0 && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_TURN_CONPLETE)
			{
				TurnCount++;
				if (!getplayer->Get_PlayerHealFlag())
				{
					if (TurnCount % getplayer->Get_OnakaTurn() == 0 && TurnCount != 0)
					{
						getplayer->Player_OnakaDown();
					}
				}
				else
				{
					if (TurnCount % getplayer->Get_OnakaTurn() / 2 == 0 && TurnCount != 0)
					{
						getplayer->Player_OnakaDown();
					}
				}
				CPlayer::Player_NextTurn();
				if (TurnCount % ENEMY_ON_TURN == 0 && TurnCount != 0)
				{
					do
					{
						eposX = mt() % MAX_MAPWIDTH;
						eposZ = mt() % MAX_MAPHEIGHT;
					} while (CMap::Map_GetData(eposZ, eposX).type != 1);

					CMap::MapEnemySingleSet();
				}
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
				if (TurnCount % getplayer->Get_OnakaTurn() == 0 && TurnCount != 0)
				{
					getplayer->Player_OnakaDown();
				}
			}
			else
			{
				if (TurnCount % getplayer->Get_OnakaTurn() / 2 == 0 && TurnCount != 0)
				{
					getplayer->Player_OnakaDown();
				}
			}
			// ���^�[�����ƂɃ����_���œG�𐶐�
			if (TurnCount % ENEMY_ON_TURN == 0 && TurnCount != 0)
			{
				do
				{
					eposX = mt() % MAX_MAPWIDTH;
					eposZ = mt() % MAX_MAPHEIGHT;
				} while (CMap::Map_GetData(eposZ, eposX).type != 1);

				CMap::MapEnemySingleSet();
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
	fscanf(file, "%d,%d,%[^,],%f,%d,%d,%d,%d,%d,%d,%d",&enemydata->enemy_type, &enemydata->wepon_type, enemydata->enemy_name , &enemydata->Hp, &enemydata->str, &enemydata->def, &enemydata->exp, &enemydata->gold, &enemydata->first_floor, &enemydata->end_floor, &enemydata->enemychance);

	fclose(file);



	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}

void CEnemy::Enemy_Update(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();

	//�������ł�Ƃ��͍X�V���Ȃ�
	if (m_Back == false)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;


		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
	}

	if (m_Warp == true)
	{
		Enemy_WarpMove();
	}
	else if (m_Back == true)
	{
		Enemy_BackMove();
	}
	else if (m_Back == false)
	{
		if (m_Condition == SLEEP_CONDITION)
		{
			if (m_TurnCount < ENEMY_SLEEP_HEALTURN)
			{
				enemyturn = ENEMY_TURN_END;
			}
		}
		else if (m_Condition != SLEEP_CONDITION)
		{
			switch (getplayer->Get_PlayerTurn())
			{
			case CPlayer::PLAYER_KEY_INPUT:
			case CPlayer::PLAYER_SERECT_UI:
			case CPlayer::PLAYER_WINDOW:
			case CPlayer::PLAYER_ITEM_WAIT:
				// �Ȃɂ����Ȃ�
				break;

			case CPlayer::PLAYER_MOVE:
			case CPlayer::PLAYER_WARP_WAIT:
				//�v���C���[���ړ����ɃI�u�W�F�N�g�ɓ����������͉���Ȃ�
				//�v���C���[�������U������
				if (enemyturn == ENEMY_ACTION && getplayer->Get_PlayerTurn() == CPlayer::PLAYER_WARP_WAIT)
					enemyturn = ENEMY_TURN_END;

				if (getplayer->m_Judge_player.HitItem || getplayer->Get_PlayerTurn() == CPlayer::PLAYER_RANGEHIT_WAIT)
				{
					if (getplayer->Get_PlayerTurn() == CPlayer::PLAYER_MOVE)
						break;
				}
			case CPlayer::PLAYER_ACT_END:
			case CPlayer::PLAYER_MOVE_END:
			case CPlayer::PLAYER_TURN_END:
				if (!m_Judge_player.HitItem)
				{
					if (getplayer->Get_Condition() <= C3DObj::KURAYAMI_CONDITION)
					{
						Enemy_AI();
					}
				}
				break;
			case CPlayer::PLAYER_TURN_CONPLETE:
				if (!m_Judge_player.HitItem)
				{
					if (getplayer->Get_Condition() >= C3DObj::BAISOKU_CONDITION)
					{
						Enemy_AI();
					}
				}
				break;
			}
		}
	}
	// AI����
	//�ǔ���

}

void CEnemy::Enemy_Draw(void)
{
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);
	DrawDX_Anime(m_mtxWorld, &anime_model);
	//DrawDX_Normal(m_mtxWorld, &Normal_model);
	if (m_Str2 >= CHARGE_BUFF)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + 4, m_Position.z), CTexture::TEX_PLAYER_CHARGE_ICON);
	}
	if (m_Condition == POIZUN_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_POIZUN_ICON);
	}
	if (m_Condition == SLEEP_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_SLEEP_ICON);
	}
	if (m_Condition == KURAYAMI_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_MEKAKUSI_ICON);
	}
	if (m_Condition == MAHUJI_CONDITION)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, D3DXVECTOR3(m_Position.x, m_Position.y + ENEMY_COND_ICON_Y, m_Position.z), CTexture::TEX_PLAYER_MAHUJI_ICON);
	}
}

void CEnemy::Enemy_AI(void)
{
	int i = 0;
	int e = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
	std::random_device rd;
	std::mt19937 mt(rd());
	get_turbo = getplayer->Get_TurboMode();
	// �^�[�����I����Ă�G�̐����J�E���g

	// �����t���[���ɍU���ł���G���J�E���g
	// �U���҂��J�E���g
	int enemyturncount = 0;
	// �v���C���[���ړ����邩�ړ��ȊO�̍s�����I������ꍇ�G�l�~�[���ړ�����
	switch (enemyturn)
	{
	case ENEMY_WAIT:
		// ���ݒn�̍X�V
		//m_Mapz = (m_Position.z - 247.5f) / -5;
		//m_Mapx = (m_Position.x + 247.5f) / 5;
		C3DObj *enemy;
		C3DObj *enemy2;


		for (i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{
				if (i != MAX_GAMEOBJ - 1)
				{
					for (e = 0; e < MAX_GAMEOBJ; e++)
					{
						if (i != e)
						{
							enemy2 = CEnemy::Get_Enemy(e);
							if (enemy2)
							{
								if (enemy->m_Mapx == enemy2->m_Mapx && enemy->m_Mapz == enemy2->m_Mapz - 1)
								{
									enemy->m_Judge_enemy.HitBottom = true;
									enemy2->m_Judge_enemy.HitTop = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx && enemy->m_Mapz == enemy2->m_Mapz + 1)
								{
									enemy->m_Judge_enemy.HitTop = true;
									enemy2->m_Judge_enemy.HitBottom = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx + 1 && enemy->m_Mapz == enemy2->m_Mapz)
								{
									enemy->m_Judge_enemy.HitLeft = true;
									enemy2->m_Judge_enemy.HitRight = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx - 1 && enemy->m_Mapz == enemy2->m_Mapz)
								{
									enemy->m_Judge_enemy.HitRight = true;
									enemy2->m_Judge_enemy.HitLeft = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx - 1 && enemy->m_Mapz == enemy2->m_Mapz - 1)
								{
									enemy->m_Judge_enemy.HitBottomRight = true;
									enemy2->m_Judge_enemy.HitTopLeft = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx - 1 && enemy->m_Mapz == enemy2->m_Mapz + 1)
								{
									enemy->m_Judge_enemy.HitTopRight = true;
									enemy2->m_Judge_enemy.HitBottomLeft = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx + 1 && enemy->m_Mapz == enemy2->m_Mapz + 1)
								{
									enemy->m_Judge_enemy.HitTopLeft = true;
									enemy2->m_Judge_enemy.HitBottomRight = true;
								}
								if (enemy->m_Mapx == enemy2->m_Mapx + 1 && enemy->m_Mapz == enemy2->m_Mapz - 1)
								{
									enemy->m_Judge_enemy.HitBottomLeft = true;
									enemy2->m_Judge_enemy.HitTopRight = true;
								}
							}
						}
					}
				}

			}
		}
		// �ʘH�ł̎΂ߍU���h�~
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type != 2 || !m_Judge_enemy.Hitnaname)
		{
			// �v���C���[���ʘH�ɂ���ꍇ�΂ߍU�����Ȃ�
			if (!m_Judge_enemy.Hitnaname || CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2)
			{
				// 1�}�X��Ƀv���C���[������ꍇ�U������
				if (m_Judge_enemy.HitEnemy)
				{
					enemyturn = ENEMY_ACTION;
					attackframe = 0;
					break;
				}
			}
		}

		//===================================================
		// �G�l�~�[AI
		//===================================================

		//===================================================
		// �ړI�n�����߂�
		//===================================================
		// �܂����������鑫��𒲂ׂ�					// �t���A
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 1)
		{
			// �v���C���[�������t���A�ɂ��邩���ׂ�
			if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 1 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group
				|| CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 3 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group)
			{
				// �v���C���[�̈ʒu��ړI�n�ɂ���
				m_Goalz = getplayer->m_Mapz;
				m_Goalx = getplayer->m_Mapx;
			}	// �ړI�n�ɒ������ꍇ
			else if (m_Mapz == m_Goalz&&
				m_Mapx == m_Goalx)
			{
				// �t���A���̖ړI�n�����߂�
				do
				{
					m_Goalz = mt() % MAX_MAPHEIGHT;
					m_Goalx = mt() % MAX_MAPWIDTH;
				} while (CMap::Map_GetData(m_Goalz, m_Goalx).type != 1 &&
					CMap::Map_GetData(m_Goalz, m_Goalx).type != 3 ||
					CMap::Map_GetData(m_Goalz, m_Goalx).Group != CMap::Map_GetData(m_Mapz, m_Mapx).Group ||
					m_Goalz == m_Mapz && m_Goalx == m_Mapx);
			}
		}

		// �܂����������鑫��𒲂ׂ�					// �t���A�o����
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 3)
		{
			// �X�C�b�`
			if (!m_Goway)
				m_Goway = true;
			else
				m_Goway = false;

			// �v���C���[�������t���A�ɂ��邩���ׂ�
			if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 1 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group
				|| CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type == 3 && CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).Group == CMap::Map_GetData(m_Mapz, m_Mapx).Group)
			{
				// �v���C���[�̈ʒu��ړI�n�ɂ���
				m_Goalz = getplayer->m_Mapz;
				m_Goalx = getplayer->m_Mapx;
			}
			else
			{
				// �ʘH�̂ق��ɐi��
				if (m_Goway)
				{
					// �ړI�n��ʘH��
					// ��̒ʘH
					if (CMap::Map_GetData(m_Mapz - 1, m_Mapx).type == 2 && !m_Judge_enemy.HitTop)
					{
						m_Goalz = m_Mapz - 1;
						m_Goalx = m_Mapx;
					}
					// ���̒ʘH
					if (CMap::Map_GetData(m_Mapz + 1, m_Mapx).type == 2 && !m_Judge_enemy.HitBottom)
					{
						m_Goalz = m_Mapz + 1;
						m_Goalx = m_Mapx;
					}
					// ���̒ʘH
					if (CMap::Map_GetData(m_Mapz, m_Mapx - 1).type == 2 && !m_Judge_enemy.HitLeft)
					{
						m_Goalz = m_Mapz;
						m_Goalx = m_Mapx - 1;
					}
					// �E�̒ʘH
					if (CMap::Map_GetData(m_Mapz, m_Mapx + 1).type == 2 && !m_Judge_enemy.HitRight)
					{
						m_Goalz = m_Mapz;
						m_Goalx = m_Mapx + 1;
					}

				}
				// �t���A�̂ق��ɐi��
				else
				{
					// �t���A���̖ړI�n�����߂�
					do
					{
						m_Goalz = mt() % MAX_MAPHEIGHT;
						m_Goalx = mt() % MAX_MAPWIDTH;
					} while (CMap::Map_GetData(m_Goalz, m_Goalx).type != 1 &&
						CMap::Map_GetData(m_Goalz, m_Goalx).type != 3 ||
						CMap::Map_GetData(m_Goalz, m_Goalx).Group != CMap::Map_GetData(m_Mapz, m_Mapx).Group ||
						m_Goalz == m_Mapz && m_Goalx == m_Mapx);
				}
			}
		}

		// �܂����������鑫��𒲂ׂ�					// �ʘH
		if (CMap::Map_GetData(m_Mapz, m_Mapx).type == 2)
		{
			// ���ɖ߂炸�i�ݑ�����
			// ��ɐi��ł�ꍇ
			if (m_Angle == 0.0f)
			{
				if (!m_Judge_enemy.HitTop)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
				// ����
				else if (!m_Judge_enemy.HitLeft)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
				else if (!m_Judge_enemy.HitRight)// �E��
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
				else if (!m_Judge_enemy.HitBottom)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
			}

			// �E�ɐi��ł�ꍇ
			if (m_Angle == 1.6f)
			{
				if (!m_Judge_enemy.HitRight)// �E��
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
				// 
				else if (!m_Judge_enemy.HitTop)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitBottom)// �E��
				{
					m_Goalz = m_Mapz + 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitLeft)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
			}

			// ���ɐi��ł�ꍇ
			if (m_Angle == 3.2f)
			{
				if (!m_Judge_enemy.HitBottom)
				{
					m_Goalz = m_Mapz + 1;
					m_Goalx = m_Mapx;
				}
				// ����
				else if (!m_Judge_enemy.HitRight)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
				else if (!m_Judge_enemy.HitLeft)// �E��
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
				else if (!m_Judge_enemy.HitTop)
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
			}

			// ���ɐi��ł�ꍇ
			if (m_Angle == 4.8f)
			{
				if (!m_Judge_enemy.HitLeft)// �E��
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx - 1;
				}
				// ����
				else if (!m_Judge_enemy.HitBottom)
				{
					m_Goalz = m_Mapz + 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitTop)// �E��
				{
					m_Goalz = m_Mapz - 1;
					m_Goalx = m_Mapx;
				}
				else if (!m_Judge_enemy.HitRight)
				{
					m_Goalz = m_Mapz;
					m_Goalx = m_Mapx + 1;
				}
			}
		}

		// 1�}�X��Ƀv���C���[�����Ȃ��ꍇ�ړ�����
		//===================================================
		// ��ړ�
		//===================================================
		if (m_Goalz < m_Mapz && m_Goalx == m_Mapx)
		{
			if (!m_Judge_enemy.HitTop)
			{
				Enemy_TopMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopLeft)
			{
				Enemy_TopLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopRight)
			{
				Enemy_TopRightMoveCheck();
				break;
			}
		}

		//===================================================
		// ���ړ�
		//===================================================
		if (m_Goalz > m_Mapz && m_Goalx == m_Mapx)
		{
			if (!m_Judge_enemy.HitBottom)
			{
				Enemy_BottomMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomRight)
			{
				Enemy_BottomRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomLeft)
			{
				Enemy_BottomLeftMoveCheck();
				break;
			}
		}

		//===================================================
		// �E�ړ�
		//===================================================
		if (m_Goalz == m_Mapz && m_Goalx > m_Mapx)
		{
			if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopRight)
			{
				Enemy_TopRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomRight)
			{
				Enemy_BottomRightMoveCheck();
				break;
			}
		}

		//===================================================
		// ���ړ�
		//===================================================
		if (m_Goalz == m_Mapz && m_Goalx < m_Mapx)
		{
			if (!m_Judge_enemy.HitLeft)
			{
				Enemy_LeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottomLeft)
			{
				Enemy_BottomLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTopLeft)
			{
				Enemy_TopLeftMoveCheck();
				break;
			}
		}

		//===================================================
		// ����ړ�
		//===================================================
		if (m_Goalz < m_Mapz && m_Goalx < m_Mapx)
		{
			if (!m_Judge_enemy.HitTopLeft)
			{
				Enemy_TopLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitLeft)
			{
				Enemy_LeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTop)
			{
				Enemy_TopMoveCheck();
				break;
			}
		}

		//===================================================
		// �����ړ�
		//===================================================
		if (m_Goalz > m_Mapz && m_Goalx < m_Mapx)
		{
			if (!m_Judge_enemy.HitBottomLeft)
			{
				Enemy_BottomLeftMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitBottom)
			{
				Enemy_BottomMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitLeft)
			{
				Enemy_LeftMoveCheck();
				break;
			}
		}

		//===================================================
		// �E���ړ�
		//===================================================
		if (m_Goalz > m_Mapz && m_Goalx > m_Mapx)
		{
			if (!m_Judge_enemy.HitBottomRight)
			{
				Enemy_BottomRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
		}

		//===================================================
		// �E��ړ�
		//===================================================
		if (m_Goalz < m_Mapz && m_Goalx > m_Mapx)
		{
			if (!m_Judge_enemy.HitTopRight)
			{
				Enemy_TopRightMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitTop)
			{
				Enemy_TopMoveCheck();
				break;
			}
			else if (!m_Judge_enemy.HitRight)
			{
				Enemy_RightMoveCheck();
				break;
			}
		}

		enemyturn = ENEMY_MOVE_END;
		if (m_TimeWarp)
		{
			m_Warp = true;
		}
		break;
		//===================================================
		// �ړ���
		//===================================================
	case ENEMY_MOVE:
		if (get_turbo)
		{
			Enemy_TurboMove();
		}
		else
		{
			Enemy_Move();
		}
		break;

		//===================================================
		// �ړ�����
		//===================================================
	case ENEMY_MOVE_END:
		enemyturn = ENEMY_TURN_END;
		Animation_Change(IDLE, m_IdleAnimeTime);
		
		break;

		//===================================================
		// �U����
		//===================================================
	case ENEMY_ACTION:
		if (attackflag)	// 1�t���[����2�̈ȏ㓯���ɍU�������Ȃ�
			break;

		attackflag = true;

		//if (enemyturn == ENEMY_MOVE)
		//{
		//	enemyturncount++;
		//}
		if (enemyturncount == 0)
		{
			// �G�l�~�[�p�U���֐�
			Enemy_Act();
		}
		break;

		//===================================================
		// �U������
		//===================================================
	case ENEMY_ACT_END:
		enemyturn = ENEMY_TURN_END;
		Animation_Change(IDLE, 0.005f);
		
		break;
	}
	m_Oldmz = m_Mapz;
	m_Oldmx = m_Mapx;
	m_Mapz = (int)(m_EnemyMyColision.position.z - 247.5f) / -5;
	m_Mapx = (int)(m_EnemyMyColision.position.x + 247.5f) / 5;
	CMap::MapEnemyPosSet(m_Mapz, m_Mapx, m_Oldmz, m_Oldmx);
}

void CEnemy::Enemy_Act(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	if (attackframe == 0)
	{
		Animation_Change(ATTACK, m_AttackAnimeTime);
	}
	if (attackframe < 8)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		// ���ɍU��
		if (m_Judge_enemy.HitEnemyLeft)
		{
			m_Angle = 4.8f;
			if (attackframe < 4)
			{
				m_Position -= m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// ��ɍU��
		if (m_Judge_enemy.HitEnemyTop)
		{
			m_Angle = 0.0f;
			if (attackframe < 4)
			{
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// �E�ɍU��
		if (m_Judge_enemy.HitEnemyRight)
		{
			m_Angle = 1.6f;
			if (attackframe < 4)
			{
				m_Position += m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Right * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// ���ɍU��
		if (m_Judge_enemy.HitEnemyBottom)
		{
			m_Angle = 3.2f;
			if (attackframe < 4)
			{
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// �E��ɍU��
		if (m_Judge_enemy.HitEnemyTopRight)
		{
			m_Angle = 0.8f;
			if (attackframe < 4)
			{
				m_Position += m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// �E���ɍU��
		if (m_Judge_enemy.HitEnemyBottomRight)
		{
			m_Angle = 2.4f;
			if (attackframe < 4)
			{
				m_Position += m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position -= m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// �����ɍU��
		if (m_Judge_enemy.HitEnemyBottomLeft)
		{
			m_Angle = 4.0f;
			if (attackframe < 4)
			{
				m_Position -= m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}

		// ����ɍU��
		if (m_Judge_enemy.HitEnemyTopLeft)
		{
			m_Angle = 5.6f;
			if (attackframe < 4)
			{
				m_Position -= m_Right * 1.0f;
				m_Position += m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
			else
			{
				m_Position += m_Right * 1.0f;
				m_Position -= m_Front * 1.0f;
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld *= m_mtxTranslation;
			}
		}
	}
	attackframe++;
	if (attackframe == 5)
	{
		EnemySkill();
		//�U���X�L���̂݁�������
		//m_Str2 = 1.0;
		// �����Ă�����A�U���́A�U�������L�����̖��O��n��
		//	CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Angle);
		//	CAttack::Attack_EnemySkill(CAttack::HIGH_ATTACK_SKILL, m_WeponType, m_Type, m_Str, m_Angle);
	}
	//�v���C���[�����񂾂�\�����ԉ���
	if (attackframe == 20 && getplayer->Get_Hp() <= 0)
	{
		add_time = 25;
	}
	if (attackframe == 55 + add_time)
	{
		add_time = 0;
		enemyturn = ENEMY_ACT_END;
		attackframe = 0;
		if (getplayer->Get_Hp() <= 0)
		{
			getplayer->Set_PlayerTurn(CPlayer::PLAYER_DESTROY);
		}
		else
		{
			if (m_TimeWarp)
			{
				m_Warp = true;
			}
		}
	}
}

/*�����̎�*/

void CEnemy::Enemy_TurboMove(void)
{
	if (walkf == 0 || walkf == 56 || walkf == 24 || walkf == 30 || walkf == 40 || walkf == 100)
	{
		Animation_Change(WALK, m_WalkAnimeTime);
	}
	if (walkf <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// �����蔻��̐ݒ�
		//g_player.col.position = g_player.pos;
		walkf++;
		if (walkf == 5)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf >= 96)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// �����蔻��̐ݒ�
		//g_player.col.position = g_player.pos;
		walkf--;
		if (walkf == 95)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf == 56)
		Enemy_TurbolefttopMove();
	if (walkf == 40)
		Enemy_TurboleftbottomMove();
	if (walkf == 30)
		Enemy_TurborightbottomMove();
	if (walkf == 24)
		Enemy_TurborighttopMove();
}

void CEnemy::Enemy_Move(void)
{
	//0,56,24,30,40,105(turbo=100)
	if (walkf == 0 || walkf == 56 || walkf == 24 || walkf == 30 || walkf == 40 || walkf == 105)
	{
		Animation_Change(WALK, m_WalkAnimeTime);
	}
	if (walkf <= WALK_COUNT)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// �����蔻��̐ݒ�
		//g_player.col.position = g_player.pos;
		walkf++;
		if (walkf == WALK_COUNT + 1)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf >= 96)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;

		// �����蔻��̐ݒ�
		//g_player.col.position = g_player.pos;
		walkf--;
		if (walkf == 95)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf == 56)
		Enemy_lefttopMove();
	if (walkf == 40)
		Enemy_leftbottomMove();
	if (walkf == 30)
		Enemy_rightbottomMove();
	if (walkf == 24)
		Enemy_righttopMove();
}

void CEnemy::Enemy_lefttopMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 0.5f;

		m_Position -= vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_righttopMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 0.5f;

		m_Position += vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_leftbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 0.5f;

		m_Position -= vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_rightbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT))
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 0.5f;

		m_Position += vec2enemy * 0.5f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == (WALK_COUNT)+1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurbolefttopMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 1.0f;

		m_Position -= vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurborighttopMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position += vecenemy * 1.0f;

		m_Position += vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurboleftbottomMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 1.0f;

		m_Position -= vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_TurborightbottomMove(void)
{
	if (nanawalk <= 4)
	{
		m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
		D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
		D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

		m_mtxWorld = m_mtxTranslation * m_mtxRotation;
		m_Position -= vecenemy * 1.0f;

		m_Position += vec2enemy * 1.0f;

		D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
		m_mtxWorld *= m_mtxTranslation;
		nanawalk++;

		if (nanawalk == 5)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy::Enemy_LeftMoveCheck(void)
{
	m_Angle = 4.8f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;

	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	if (get_turbo)
	{
		walkf = 100;
	}
	else
	{
		walkf = 100 + WALK_COUNT - 4;
	}
	vecenemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_RightMoveCheck(void)
{
	m_Angle = 1.6f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;


	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 0;
	vecenemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BottomMoveCheck(void)
{
	m_Angle = 3.2f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;

	m_Position += m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	if (get_turbo)
	{
		walkf = 100;
	}
	else
	{
		walkf = 100 + WALK_COUNT - 4;
	}
	vecenemy = m_Front;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_TopMoveCheck(void)
{

	m_Angle = 0.0f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;
	//g_player.col.position = g_player.walkpos;

	m_Position -= m_Front * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 0;
	vecenemy = m_Front;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}

}

void CEnemy::Enemy_TopLeftMoveCheck(void)
{
	m_Angle = 5.6f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;


	m_Position -= m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 56;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_TopRightMoveCheck(void)
{
	m_Angle = 0.8f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position += m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;


	m_Position -= m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 24;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BottomLeftMoveCheck(void)
{
	m_Angle = 4.0f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;


	m_Position += m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 40;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BottomRightMoveCheck(void)
{
	m_Angle = 2.4f;

	m_Rotation = D3DXVECTOR3(0, m_Angle, 0);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;
	m_Position -= m_Front * 5.0f;
	m_Position += m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	// �����蔻��̐ݒ�
	m_EnemyMyColision.position = m_Position;

	m_Position += m_Front * 5.0f;
	m_Position -= m_Right * 5.0f;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	walkf = 30;
	nanawalk = 0;
	vecenemy = m_Front;
	vec2enemy = m_Right;
	enemyturn = ENEMY_MOVE;

	if (get_turbo)
	{
		Enemy_TurboMove();
	}
	else
	{
		Enemy_Move();
	}
}

void CEnemy::Enemy_BackMove(void)
{//��Ő�����ѐ�Ƀ����X�^�[�������Ƃ��̏�������������
	C3DObj *getplayer;
	getplayer = CPlayer::Get_Player();
	if (getplayer && this)
	{
		//�������Ă����ɔ��
		if (getplayer->Get_Angle() == 3.2f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapz = m_Mapz - 1;
				}
				m_Position.z += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 3.2f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz - 1, m_Mapx).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.z -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//������Ă��牺�ɔ��
		else if (getplayer->Get_Angle() == 0.0f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapz = m_Mapz + 1;
				}
				m_Position.z -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 0.0f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz + 1, m_Mapx).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.z += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//�������Ă���E�ɔ��
		else if (getplayer->Get_Angle() == 4.8f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz, m_Mapx + 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx + 1;
				}
				m_Position.x += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 4.8f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz, m_Mapx + 1).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.x -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//�E�����Ă��獶�ɔ��
		else if (getplayer->Get_Angle() == 1.6f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz, m_Mapx - 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx - 1;
				}
				m_Position.x -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 1.6f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz, m_Mapx - 1).type == 0)
			{//�ǂɂԂ�������~�߂�
			 //�t���[����0�i�܂�Ǎہj��������ړ����Ȃ�
				if (m_BackFrame != 0)
				{
					m_Position.x += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//�E������Ă��獶���ɔ��
		else if (getplayer->Get_Angle() == 0.8f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx - 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx - 1;
					m_Mapz = m_Mapz + 1;
				}
				m_Position.x -= 0.5f;
				m_Position.z -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 0.8f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz + 1, m_Mapx - 1).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.x += 0.5f;
					m_Position.z += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//��������Ă���E���ɔ��
		else if (getplayer->Get_Angle() == 5.6f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz + 1, m_Mapx + 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx + 1;
					m_Mapz = m_Mapz + 1;
				}
				m_Position.x += 0.5f;
				m_Position.z -= 0.5f;
				m_Rotation = D3DXVECTOR3(0, 5.6f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz + 1, m_Mapx + 1).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.x -= 0.5f;
					m_Position.z += 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//���������Ă���E��ɔ��
		else if (getplayer->Get_Angle() == 4.0f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx + 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx + 1;
					m_Mapz = m_Mapz - 1;
				}
				m_Position.x += 0.5f;
				m_Position.z += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 4.0f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz - 1, m_Mapx + 1).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.x -= 0.5f;
					m_Position.z -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
		//�E�������Ă��獶��ɔ��
		else if (getplayer->Get_Angle() == 2.4f)
		{//�ǂɂԂ���܂Ŕ��
			if (CMap::Map_GetData(m_Mapz - 1, m_Mapx - 1).type != 0)
			{
				if (m_BackFrame != 0 && m_BackFrame % 10 == 0)
				{
					m_Mapx = m_Mapx - 1;
					m_Mapz = m_Mapz - 1;
				}
				m_Position.x -= 0.5f;
				m_Position.z += 0.5f;
				m_Rotation = D3DXVECTOR3(0, 2.4f, 0);
				D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
				D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
				m_mtxWorld = m_mtxRotation * m_mtxTranslation;
				m_EnemyMyColision.position = m_Position;
				m_BackFrame++;
			}
			else if (CMap::Map_GetData(m_Mapz - 1, m_Mapx - 1).type == 0)
			{//�ǂɂԂ�������~�߂�
				if (m_BackFrame != 0)
				{
					m_Position.x += 0.5f;
					m_Position.z -= 0.5f;
					m_EnemyMyColision.position = m_Position;
				}
				CUserinterface::UI_TextCreateBack(CUserinterface::PLAYERCHARA, CUserinterface::BACKENEMY_ATTACK, CUserinterface::ENEMY, 5, m_Type);
				m_Back = false;
				m_BackFrame = 0;
				getplayer->SetEnemyBack(false);
				getplayer->Set_RivalFlag(Damage(5, getplayer->Get_Angle(), CUserinterface::NORMAL_TYPE));
			}
		}
	}
}

void CEnemy::Enemy_WarpMove(void)
{
	if (m_BackFrame <= 20)
	{
		m_Position.y += 1;
	}
	if (m_BackFrame == 21)
	{
		CMap::WorpEnemySet(this, m_Mapz, m_Mapx);
	}
	if (m_BackFrame> 20 && m_BackFrame < WAIT_FRAME - 5)
	{
		m_Position.y -= 1;
		if (m_Position.y < 0)
		{
			m_Position.y = 0.0f;
		}
	}
	if (m_BackFrame == WAIT_FRAME - 5)
	{
		m_Position.y = 0.0f;
		m_BackFrame = 0;
		m_Warp = false;
		m_TimeWarp = false;
	}
	else
	{
		m_BackFrame++;
	}
}

bool CEnemy::Damage(int str, float angle, int week_type)
{
	// ��Ƀ_���[�W�G�t�F�N�g���쐬
	if (str > 0)
	{
		Sleep(HITSTOP);
		if (week_type == CUserinterface::NORMAL_TYPE)
		{
			Exp_Set(HIT, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		}
		if (week_type == CUserinterface::WEEK_TYPE)
		{
			Exp_Set(HIT_WEEK, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		}
		if (week_type == CUserinterface::RESIST_TYPE)
		{
			Exp_Set(HIT_RESIST, m_Position.x, m_Position.y, m_Position.z, 3.0f, 0.0f);
		}
		PlaySound(ENEMYDAMAGE_SE);
	}
	else//�U�����~�X�̎�
	{
		PlaySound(MISS_SE);
	}
	//�U�����󂯂����Ɍ���
	if (angle == LEFT_ANGLE)
	{
		m_Angle = 1.6f;
	}
	if (angle == UP_ANGLE)
	{
		m_Angle = 3.2f;
	}
	if (angle == RIGHT_ANGLE)
	{
		m_Angle = 4.8f;
	}
	if (angle == DOWN_ANGLE)
	{
		m_Angle = 0.0f;
	}
	if (angle == LEFT_DOWN_ANGLE)
	{
		m_Angle = 0.8f;
	}
	if (angle == RIGHT_DOWN_ANGLE)
	{
		m_Angle = 5.6f;
	}
	if (angle == RIGHT_TOP_ANGLE)
	{
		m_Angle = 4.0f;
	}
	if (angle == LEFT_TOP_ANGLE)
	{
		m_Angle = 2.4f;
	}
	// ��Ƀ_���[�W�v�Z�����쐬
	m_Hp -= str;
	// �̗͂�0�ȉ��œ|���
	if (m_Hp <= 0)
	{
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->enemy_death_count += 1;
		Enemy_Destroy();
		return DEATH;
	}
	return NORMAL;
}

bool CEnemy::PoizunDamage(int str)
{
	//�ŃG�t�F�N�g�Đ����邱��
	// ��Ƀ_���[�W�v�Z�����쐬
	m_Hp -= str;
	// �̗͂�0�ȉ��œ|���
	if (m_Hp <= 0)
	{
		SAVE_COUNT *save;
		save = Get_SaveCount();
		save->enemy_death_count += 1;
		Enemy_Destroy();
		return DEATH;
	}
	return NORMAL;
}

void CEnemy::Enemy_Destroy(void)
{
	Enemy_ItemDrop();
	alive = false;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Type = CUserinterface::CHARATYPENONE;
	delete this;
}

void CEnemy::Enemy_SetWorpPos(int pposZ, int pposX)
{
	m_Position = D3DXVECTOR3(-247.5f + pposX * 5, 0.0f, 247.5f - pposZ * 5);
	m_EnemyMyColision.position = m_Position;
	m_EnemyMyColision.radius = ENEMY_RADIUS;
	// �}�b�v�ʒu��o�^
	m_Mapx = pposX;
	m_Mapz = pposZ;

	m_Goalx = pposX;
	m_Goalz = pposZ;

	//������������
	m_Angle = 3.2f;
	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;
}

void CEnemy::Enemy_PoizunDamageStart(void)
{
	if (m_Condition == POIZUN_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_POIZUN_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
		Enemy_PoizunDamage();
	}
}

void CEnemy::Enemy_PoizunDamage(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	//���ꂪ�Ȃ��ƃv���C���[�Ɍo���l���炸
	//�łŎ��񂾂�UI���o������
	//�����v���C���[�̃G�l�~�[�f�X��true��������HP1�őς���悤�ɂ��邱�Ɓi1�^�[����1�̈ȏ�E���Ȃ��j
	if (!getplayer->Get_EnemyDeath())
	{
		if (m_Hp - ENEMY_POINZU_DAMAGE <= 0)
		{
			getplayer->SetEnemyDeath(true);
			CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::ENEMY_POIZUN_DEATH, CUserinterface::ENEMY, 0, m_Type);
		}
	}
	//�_���[�W�������ŗp�̂ɕς��邱��
	getplayer->Set_RivalFlag(PoizunDamage((int)ENEMY_POINZU_DAMAGE));
}

void CEnemy::Enemy_ConditionCount(void)
{
	if (m_Condition == SLEEP_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_SLEEP_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
	}
	if (m_Condition == KURAYAMI_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_KURAYAMI_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
	}
	if (m_Condition == MAHUJI_CONDITION)
	{
		m_TurnCount++;
		if (m_TurnCount == ENEMY_TOKUGI_HEALTURN)
		{
			m_Condition = NORMAL_CONDITION;
		}
	}
}

void CEnemy::Enemy_ItemDrop(void)
{
	//�A�C�e���������Ă��炻�̏�ɃA�C�e����u��
	if (set_item != 0)
	{
		CMap::MapItemPosSet(set_item, m_Mapx, m_Mapz);
	}
}