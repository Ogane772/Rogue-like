#include "CEnemy.h"
#include <d3dx9.h>
#include "CEnemy_Srime.h"
#include "input.h"
#include "CPlayer.h"
#include "sound.h"
#include "userinterface.h"
#include "debug_font.h"
#include "Attack.h"
#include "save_load.h"
#include "map.h"
#include "collision.h"
#include "bilboard.h"
#include "exp.h"
#include "C2DObj.h"
#include "C3DObj.h"
#define MAXHP (20)
#define STR (4)
//=============================================================================
//	����
//=============================================================================


CEnemy_Srime::CEnemy_Srime(int x, int z, ENEMY_Data enemy_data) :CEnemy(TYPE_SRIME), C3DObj(C3DObj::TYPE_ENEMY)
{
	Initialize(x, z, enemy_data);
}

//=============================================================================
//	�j��
//=============================================================================

CEnemy_Srime::~CEnemy_Srime()
{
	NormalModel_Finalize(&Normal_model);
	//delete this;
}


void CEnemy_Srime::Initialize(int x, int z, ENEMY_Data enemy_data)
{
	strcpy_s(name, MAX_NAME, enemy_data.enemy_name);
	m_EnemyIndex = Get_EnemyIndex(TYPE_ALL);
	//Normal_model = GetNormalModel(MODELL_ENEMY_1);
	C3DObj::InitNormalModelLoad(&Normal_model, "asset/model/piel_wepon.x");
	TurnCount = 0;
	m_Condition = NORMAL_CONDITION;
	get_turbo = false;
	m_WeponType = enemy_data.wepon_type;
	add_time = 0;
	alive = true;
	rival_flag = false;
	map_drawflag = false;
	m_Position = D3DXVECTOR3(-247.5f + x * 5, 0.0f, 247.5f - z * 5);
	m_EnemyMyColision.position = m_Position;
	m_EnemyMyColision.radius = ENEMY_RADIUS;
	enemyturn = ENEMY_WAIT;
	m_Type = TYPE_SRIME;
	m_MaxHp = enemy_data.Hp;
	m_Hp = m_MaxHp;
	m_Str = enemy_data.str;
	m_Str2 = 1.0f;
	m_Def = enemy_data.def;
	m_Exp = enemy_data.exp;
	m_Gold = enemy_data.gold;
	m_Goway = false;
	m_Angle = 0.0f;
	walkf = 0;
	attackframe = 0;
	nanawalk = 0;
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&m_Front, &m_Front);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);
	D3DXVec3Normalize(&m_Right, &m_Right);

	D3DXVec3Cross(&m_Up, &m_Front, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	// �}�b�v�ʒu��o�^
	m_Mapx = x;
	m_Mapz = z;

	m_Goalx = x;
	m_Goalz = z;

	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;

	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

}

void CEnemy_Srime::Update(void)
{
	C3DObj *getplayer = CPlayer::Get_Player();
		
	m_Rotation = D3DXVECTOR3(0.0f, m_Angle, 0.0f);
	D3DXMatrixRotationY(&m_mtxRotation, m_Rotation.y);
	D3DXMatrixTranslation(&m_mtxTranslation, m_Rotation.x, m_Position.y, m_Rotation.z);

	m_mtxWorld = m_mtxTranslation * m_mtxRotation;


	D3DXMatrixTranslation(&m_mtxTranslation, m_Position.x, m_Position.y, m_Position.z);
	m_mtxWorld *= m_mtxTranslation;

	switch (getplayer->Get_PlayerTurn())
	{
	case CPlayer::PLAYER_KEY_INPUT:
	case CPlayer::PLAYER_SERECT_UI:
	case CPlayer::PLAYER_WINDOW:
	case CPlayer::PLAYER_ITEM_WAIT:
		// �Ȃɂ����Ȃ�
		break;

	case CPlayer::PLAYER_MOVE:
		//�v���C���[���ړ����ɃI�u�W�F�N�g�ɓ����������͉���Ȃ�
		if (getplayer->m_Judge_player.HitItem)
		{
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
	// AI����
	//�ǔ���
	
}

void CEnemy_Srime::Draw(void)
{
	
	CBilboard::Shadow_Draw(m_mtxWorld, m_Position);
	DrawDX_Normal(m_mtxWorld, &Normal_model);
	if (m_Str2 >= CHARGE_BUFF)
	{
		CBilboard::Hukidasi_Draw(m_mtxWorld, m_Position, CTexture::TEX_PLAYER_CHARGE_ICON);
	}
	//DebugFont_Draw(200, 150, "�G�l�~�[���[�h%d", enemyturn);
	//DebugFont_Draw(200, 250, "Attackfream = %d", attackframe);
	//DebugFont_Draw(0, 300, "Angle = %f", m_Angle);
	//DebugFont_Draw(0, 300, "�G�l�~�[ nanawalk = %d", nanawalk);
	//DebugFont_Draw(0, 330, "�G�l�~�[ walk = %d", walkf);
	/*
	DebugFont_Draw(200, 20, "�G�l�~�[�o����%d", m_EnemyNum[TYPE_SRIME]);
	DebugFont_Draw(200, 60, "m_Judge_enemy.HitTop = %d", m_Judge_enemy.HitTop);
	DebugFont_Draw(200, 100, "m_Judge_enemy.HitBottom  = %d", m_Judge_enemy.HitBottom);
	DebugFont_Draw(200, 140, "m_Judge_enemy.HitRight   = %d", m_Judge_enemy.HitRight);
	DebugFont_Draw(200, 180, "m_Judge_enemy.HitLeft   = %d", m_Judge_enemy.HitLeft);
	DebugFont_Draw(200, 220, "m_Judge_enemy.HitTopLeft   = %d", m_Judge_enemy.HitTopLeft);
	DebugFont_Draw(200, 260, "m_Judge_enemy.HitBottomRight   = %d", m_Judge_enemy.HitBottomRight);
	DebugFont_Draw(200, 300, "m_Judge_enemy.HitBottomLeft   = %d", m_Judge_enemy.HitBottomLeft);
	DebugFont_Draw(200, 340, "m_Judge_enemy.HitEnemy   = %d", m_Judge_enemy.HitEnemy);
	DebugFont_Draw(200, 380, "m_Judge_enemy.HitEnemyTop   = %d", m_Judge_enemy.HitEnemyTop);
	DebugFont_Draw(200, 420, "m_Judge_enemy.HitEnemyBottom   = %d", m_Judge_enemy.HitEnemyBottom);
	DebugFont_Draw(200, 460, "m_Judge_enemy.HitEnemyRight   = %d", m_Judge_enemy.HitEnemyRight);
	DebugFont_Draw(200, 500, "m_Judge_enemy.HitEnemyLeft   = %d", m_Judge_enemy.HitEnemyLeft);
	DebugFont_Draw(200, 540, "m_Judge_enemy.HitEnemyTopLeft   = %d", m_Judge_enemy.HitEnemyTopLeft);
	DebugFont_Draw(200, 580, "m_Judge_enemy.HitEnemyBottomRight   = %d", m_Judge_enemy.HitEnemyBottomRight);
	DebugFont_Draw(200, 620, "m_Judge_enemy.HitEnemyTopRight   = %d", m_Judge_enemy.HitEnemyTopRight);
	DebugFont_Draw(200, 660, "m_Judge_enemy.HitEnemyBottomLeft   = %d", m_Judge_enemy.HitEnemyBottomLeft);
	DebugFont_Draw(200, 700, "m_Judge_enemy.Hitnaname   = %d", m_Judge_enemy.Hitnaname);
	DebugFont_Draw(200, 740, "Attackfream = %d", attackframe);
	//DebugFont_Draw(200, 150, "�G�l�~�[���[�h%d", enemyturn);
	*/
}

void CEnemy_Srime::Enemy_AI(void)
{
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

		// 
		for (int i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);

			if (enemy)
			{
				if (i != MAX_GAMEOBJ - 1)
				{
					for (int e = 0; e < MAX_GAMEOBJ; e++)
					{
						enemy2 = CEnemy::Get_Enemy(e);
						if (enemy2)
							C3DObj::Collision_EnemyVSEnemy(&enemy->m_Judge_enemy, &enemy->m_EnemyMyColision, &enemy2->m_Judge_enemy, &enemy2->m_EnemyMyColision);

					}
				}
			}
		}

		for (int i = 0; i < MAX_GAMEOBJ; i++)
		{
			enemy = CEnemy::Get_Enemy(i);
			if (enemy)
			{
				if (enemy->m_Mapx == m_Mapx && enemy->m_Mapz == m_Mapz - 1)
					m_Judge_enemy.HitBottom = true;

				if (enemy->m_Mapx == m_Mapx && enemy->m_Mapz == m_Mapz + 1)
					m_Judge_enemy.HitTop = true;

				if (enemy->m_Mapx == m_Mapx + 1 && enemy->m_Mapz == m_Mapz)
					m_Judge_enemy.HitRight = true;

				if (enemy->m_Mapx == m_Mapx - 1 && enemy->m_Mapz == m_Mapz)
					m_Judge_enemy.HitLeft = true;

				if (enemy->m_Mapx == m_Mapx - 1 && enemy->m_Mapz == m_Mapz - 1)
					m_Judge_enemy.HitTopLeft = true;

				if (enemy->m_Mapx == m_Mapx - 1 && enemy->m_Mapz == m_Mapz + 1)
					m_Judge_enemy.HitBottomLeft = true;

				if (enemy->m_Mapx == m_Mapx + 1 && enemy->m_Mapz == m_Mapz + 1)
					m_Judge_enemy.HitBottomRight = true;

				if (enemy->m_Mapx == m_Mapx + 1 && enemy->m_Mapz == m_Mapz - 1)
					m_Judge_enemy.HitTopRight = true;

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
		break;
	}
}

void CEnemy_Srime::Enemy_Act(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 2);
	int attack_number = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
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
		//attack_number = random(mt);
		attack_number = 0;
		switch (attack_number)
		{
		case 0:
			//CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Angle);
			if (getplayer->Get_Condition() == NORMAL_CONDITION)
			{
				CAttack::Attack_EnemySkill(CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			}
			else
			{
				CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str,m_Str2, m_Angle);
			}
			//m_Str2 = CHARGE_BUFF;
			break;
		case 1:
			//CAttack::Attack_EnemySkill(CAttack::HIGH_ATTACK_SKILL, m_WeponType, m_Type, m_Str, m_Angle, ESCAPE_CHECK_OK);
			CAttack::Attack_EnemySkill(CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			//m_Str2 = CHARGE_BUFF;
			break;
		case 2:
			CAttack::Attack_EnemySkill(CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			//m_Str2 = CHARGE_BUFF;
			break;
		}
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
	}
}

/*�����̎�*/

void CEnemy_Srime::Enemy_TurboMove(void)
{
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
	
	if (walkf== 56)
	Enemy_TurbolefttopMove();
	if (walkf== 40)
	Enemy_TurboleftbottomMove();
	if (walkf== 30)
	Enemy_TurborightbottomMove();
	if (walkf== 24)
	Enemy_TurborighttopMove();
}

void CEnemy_Srime::Enemy_Move(void)
{
	if (walkf<= WALK_COUNT)
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
		if (walkf== WALK_COUNT + 1)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf>= 96)
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
		if (walkf== 95)
			enemyturn = ENEMY_MOVE_END;
	}

	if (walkf== 56)
		Enemy_lefttopMove();
	if (walkf== 40)
		Enemy_leftbottomMove();
	if (walkf== 30)
		Enemy_rightbottomMove();
	if (walkf== 24)
		Enemy_righttopMove();
}

void CEnemy_Srime::Enemy_lefttopMove(void)
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_righttopMove(void)
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_leftbottomMove(void)
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_rightbottomMove(void)
{
	if (nanawalk <= (WALK_COUNT) )
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

		if (nanawalk == (WALK_COUNT) + 1)
			enemyturn = ENEMY_MOVE_END;
	}
}

void CEnemy_Srime::Enemy_TurbolefttopMove(void)
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

void CEnemy_Srime::Enemy_TurborighttopMove(void)
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

void CEnemy_Srime::Enemy_TurboleftbottomMove(void)
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

void CEnemy_Srime::Enemy_TurborightbottomMove(void)
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

void CEnemy_Srime::Enemy_LeftMoveCheck(void)
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
	m_Mapx -= 1;

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

void CEnemy_Srime::Enemy_RightMoveCheck(void)
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
	m_Mapx += 1;

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

void CEnemy_Srime::Enemy_BottomMoveCheck(void)
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
	m_Mapz += 1;

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

void CEnemy_Srime::Enemy_TopMoveCheck(void)
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
	m_Mapz -= 1;
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

void CEnemy_Srime::Enemy_TopLeftMoveCheck(void)
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
	m_Mapz -= 1;
	m_Mapx -= 1;

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

void CEnemy_Srime::Enemy_TopRightMoveCheck(void)
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
	m_Mapz -= 1;
	m_Mapx += 1;

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

void CEnemy_Srime::Enemy_BottomLeftMoveCheck(void)
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
	m_Mapz += 1;
	m_Mapx -= 1;

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
void CEnemy_Srime::Enemy_BottomRightMoveCheck(void)
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
	m_Mapz += 1;
	m_Mapx += 1;

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

bool CEnemy_Srime::Damage(int str, float angle, int week_type)
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

void CEnemy_Srime::Enemy_Destroy(void)
{
	alive = false;
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Type = CUserinterface::CHARATYPENONE;
	delete this;
}
