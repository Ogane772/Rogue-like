#include "CPlayer.h"
#include "CEnemy.h"
#include "userinterface.h"
#include "map.h"
#include "Attack.h"

void CAttack::Attack_PlayerUpdate(int charatype, float vec, int str)
{
	int e;
	C3DObj *getplayer;
	C3DObj *enemy;
	for (e = 0; e < MAX_GAMEOBJ; e++)
	{
		getplayer = CPlayer::Get_Player();
		enemy = CEnemy::Get_Enemy(e);
		if (enemy)
		{
			if (enemy->alive)
			{
				if (vec == 0.0f)	// ��U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyBottom)
					{
						enemy->Damage(str);
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
						break;
					}
				}
				if (vec == 1.6f)	// �E�U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyLeft)
					{
						enemy->Damage(str);
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
						break;
					}
				}
				if (vec == 3.2f)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyTop)
					{
						enemy->Damage(str);
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
						break;
					}
				}
				if (vec == 4.8f)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyRight)
					{
						enemy->Damage(str);
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
						break;
					}
				}

				// �ʘH����΂ߍU���͓�����Ȃ�
				if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
					CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
				{
					if (vec == 0.8f)	// �E��U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft)
						{
							enemy->Damage(str);
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
							break;
						}
					}

					if (vec == 2.4f)	// �E���U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopLeft)
						{
							enemy->Damage(str);
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
							break;
						}
					}

					if (vec == 4.0f)	// �����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopRight)
						{
							enemy->Damage(str);
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
							break;
						}
					}

					if (vec == 5.6f)	// ����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomRight)
						{
							enemy->Damage(str);
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str);
							break;
						}
					}
				}
			}
		}
	}
	
}

void CAttack::Attack_EnemyUpdate(int charatype, int str)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	getplayer->Damage(str);
	// �_���[�W�֐����ŏ����ׂ�����
	CUserinterface::UI_TextCreate(CUserinterface::SRAIM, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, str);
}