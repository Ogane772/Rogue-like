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
	bool atk_check;//��𐬌��l���
	int damage = 0;//�_���[�W�v�Z���̌��ʂ���
	getplayer = CPlayer::Get_Player();
	for (e = 0; e < MAX_GAMEOBJ; e++)
	{	
		enemy = CEnemy::Get_Enemy(e);
		if (enemy)
		{
			atk_check = Attack_Check();

			if (enemy->alive)
			{
				damage = ENEMY_DAMAGEKEISAN;
				if (damage < 0)
				{
					damage = 1;
				}
				if (vec == 0.0f)	// ��U��
				{
					if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}			
				}
				if (vec == 1.6f)	// �E�U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}
				}
				if (vec == 3.2f)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyTop && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyTop && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}
				}
				if (vec == 4.8f)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
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
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}

					if (vec == 2.4f)	// �E���U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}

					if (vec == 4.0f)	// �����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}

					if (vec == 5.6f)	// ����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle()));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}
				}
			}
		}
	}
	
}

void CAttack::Attack_EnemyUpdate(int charatype, int str, float angle)
{
	C3DObj *getplayer = CPlayer::Get_Player();
	bool atk_check;//��𐬌��l���
	int damage = 0;
	if (getplayer->Get_WMode())
	{
		damage = PLAYER_W_DAMAGEKEISAN;
	}
	else
	{
		damage = PLAYER_NORMAL_DAMAGEKEISAN;
	}
	if (damage < 0)
	{
		damage = 1;
	}
	atk_check = Attack_Check();
	// �_���[�W�֐����ŏ����ׂ�����
	if (atk_check == ESCAPE_OK)
	{
		getplayer->Damage(0, angle);
		CUserinterface::UI_TextCreate(CUserinterface::ENEMY, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, 0 , charatype);
	}
	else if (atk_check == ESCAPE_NO)
	{
		getplayer->Damage(damage, angle);
		CUserinterface::UI_TextCreate(CUserinterface::ENEMY, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, damage, charatype);
	}
}

bool CAttack::Attack_Check(void)
{
	int atkcheck = rand() % 100 + 1;
	if (atkcheck <= ESCAPE_NUMBER)
	{
		return ESCAPE_OK;
	}
	return ESCAPE_NO;
}