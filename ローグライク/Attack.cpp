#include "CPlayer.h"
#include "CEnemy.h"
#include "userinterface.h"
#include "map.h"
#include "Attack.h"
#include "CWepon.h"
#include <iostream>
#include <random>
#include "save_load.h"
#define WEEK_BAIRITU (1.3f)
#define WEEK_GEKIKA_BAIRITU (1.6f)
#define RESIST_BAIRITU (0.7f)
#define RESIST_GEKIKA_BAIRITU (0.4f)

void CAttack::Attack_PlayerUpdate(int charatype, float vec, int str, float str2)
{
	int e;
	int week_check = 0;
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
			if (enemy->alive)
			{
				atk_check = Attack_Check();
				if (atk_check == ESCAPE_NO)
				{
					week_check = Attack_WeekCheck(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type, enemy->m_WeponType);
					damage = CAttack::Attack_DamegeKeisan(week_check, enemy, str, str2);
				}
				if (vec == DOWN_ANGLE)	// ���U��
				{
					if (enemy->m_Judge_enemy.HitEnemyTop && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyTop && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}			
				}
				if (vec == LEFT_ANGLE)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}
				}
				if (vec == UP_ANGLE)	// ��U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}
				}
				if (vec == RIGHT_ANGLE)	// �E�U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
						break;
					}
					else if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_OK)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
						break;
					}
				}

				// �ʘH����΂ߍU���͓�����Ȃ�
				if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
					CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
				{
					if (vec == RIGHT_TOP_ANGLE)	// �E��U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}

					if (vec == RIGHT_DOWN_ANGLE)	// �E���U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}

					if (vec == LEFT_DOWN_ANGLE)	// �����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}

					if (vec == LEFT_TOP_ANGLE)	// ����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_NO)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
							break;
						}
						else if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_OK)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(0, getplayer->Get_Angle(), week_check));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, 0, enemy->Get_Type());
							break;
						}
					}
				}
			}
		}
	}
	
}

void CAttack::Attack_PlayerSkill(int skill_number, float vec, int str, float str2)
{
	int e;
	int week_check = 0;
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
			if (enemy->alive)
			{
				atk_check = ESCAPE_NO;
				if (atk_check == ESCAPE_NO)
				{
					week_check = Attack_WeekCheck(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type, enemy->m_WeponType);
					damage = CAttack::Attack_DamegeKeisan(week_check, enemy, str, str2);
				}
				if (vec == DOWN_ANGLE)	// ���U��
				{
					if (enemy->m_Judge_enemy.HitEnemyTop && atk_check == ESCAPE_NO)
					{
						Skill_PlayerEffect(enemy, skill_number, damage, week_check);
						break;
					}
				}
				if (vec == LEFT_ANGLE)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
					{
						Skill_PlayerEffect(enemy, skill_number, damage, week_check);
						break;
					}
				}
				if (vec == UP_ANGLE)	// ��U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_NO)
					{
						Skill_PlayerEffect(enemy, skill_number, damage, week_check);
						break;
					}
				}
				if (vec == RIGHT_ANGLE)	// �E�U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_NO)
					{
						Skill_PlayerEffect(enemy, skill_number, damage, week_check);
						break;
					}
				}

				// �ʘH����΂ߍU���͓�����Ȃ�
				if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
					CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
				{
					if (vec == RIGHT_TOP_ANGLE)	// �E��U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_NO)
						{
							Skill_PlayerEffect(enemy, skill_number, damage, week_check);
							break;
						}
					}

					if (vec == RIGHT_DOWN_ANGLE)	// �E���U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							Skill_PlayerEffect(enemy, skill_number, damage, week_check);
							break;
						}
					}

					if (vec == LEFT_DOWN_ANGLE)	// �����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							Skill_PlayerEffect(enemy, skill_number, damage, week_check);
							break;
						}
					}

					if (vec == LEFT_TOP_ANGLE)	// ����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_NO)
						{
							Skill_PlayerEffect(enemy, skill_number, damage, week_check);
							break;
						}
					}
				}
			}
		}
	}
}

int CAttack::Attack_PlayerThreeRenge(int skill_number, float vec, int str, float str2)
{
	int e;
	int count = 0;
	int week_check = 0;
	C3DObj *getplayer;
	C3DObj *enemy;
	bool atk_check;//��𐬌��l���
	int damage = 0;//�_���[�W�v�Z���̌��ʂ���
	getplayer = CPlayer::Get_Player();
	for (e = 0; e < MAX_GAMEOBJ; e++)
	{//�e��������邱��
		enemy = CEnemy::Get_Enemy(e);
		if (enemy)
		{
			if (enemy->alive)
			{
				atk_check = ESCAPE_NO;
				// �������Ă����瓖�������G�l�~�[���𐔂���
				if (vec == DOWN_ANGLE)	// ���U��
				{
					/*
					if (getplayer->m_Mapx == enemy->m_Mapx && getplayer->m_Mapz + 1 == enemy->m_Mapz)
					{
						count++;
						enemy->Set_RangeHit(true);
					}*/
					// �ʘH����΂ߍU���͓�����Ȃ�
					if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
						CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
					{
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}
					if (enemy->m_Judge_enemy.HitEnemyTop && atk_check == ESCAPE_NO)
					{
						count++;
						enemy->Set_RangeHit(true);
					}
				}
				if (vec == LEFT_ANGLE)	// ���U��
				{
					// �������Ă�����_���[�W��^����
					if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
					{
						count++;
						enemy->Set_RangeHit(true);
					}
					if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
						CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
					{
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}
				}
				if (vec == UP_ANGLE)	// ��U��
				{
					if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_NO)
					{
						count++;
						enemy->Set_RangeHit(true);
					}
					if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
						CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
					{
						if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}
				}
				if (vec == RIGHT_ANGLE)	// �E�U��
				{
					if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_NO)
					{
						count++;
						enemy->Set_RangeHit(true);
					}
					if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
						CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
					{
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}
				}

				// �ʘH����΂ߍU���͓�����Ȃ�
				if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
					CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
				{
					if (vec == RIGHT_TOP_ANGLE)	// �E��U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}

					if (vec == RIGHT_DOWN_ANGLE)	// �E���U��
					{
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}

					if (vec == LEFT_DOWN_ANGLE)	// �����U��
					{
						// �������Ă�����_���[�W��^����
						if (enemy->m_Judge_enemy.HitEnemyTopRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyTopLeft && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}

					if (vec == LEFT_TOP_ANGLE)	// ����U��
					{
						if (enemy->m_Judge_enemy.HitEnemyBottomRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyBottom && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
						if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
						{
							count++;
							enemy->Set_RangeHit(true);
						}
					}
				}
			}
		}
	}
	return count;
}

void CAttack::Attack_EnemyUpdate(int enemy_wepon_type, int charatype, int str, float str2, float angle)
{
	int week_check = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
	bool atk_check;//��𐬌��l���
	int damage = 0;
	atk_check = Attack_Check();
	if (atk_check == ESCAPE_NO)
	{
		week_check = Attack_WeekCheck(enemy_wepon_type, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type);
		if (getplayer->Get_WMode())
		{
			damage = (int)PLAYER_W_DAMAGEKEISAN;
		}
		else
		{
			damage = (int)PLAYER_NORMAL_DAMAGEKEISAN;
		}
		if (week_check == CUserinterface::WEEK_TYPE)
		{
			damage *= (int)WEEK_BAIRITU;
		}
		else if (week_check == CUserinterface::RESIST_TYPE)
		{
			damage *= (int)RESIST_BAIRITU;
		}
		if (damage < 0)
		{
			damage = 1;
		}
	}
	if (atk_check == ESCAPE_OK)
	{
		getplayer->Damage(0, angle, week_check);
		CUserinterface::UI_TextCreate(CUserinterface::ENEMY, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, 0 , charatype);
	}
	else if (atk_check == ESCAPE_NO)
	{
		getplayer->Damage(damage, angle, week_check);
		CUserinterface::UI_TextCreate(week_check, CUserinterface::ENEMY, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, damage, charatype);
	}
}

void CAttack::Attack_EnemySkill(int skill_number, int enemy_wepon_type, int charatype, int str, float str2, float angle, bool escape_check)
{
	int week_check = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
	bool atk_check;//��𐬌��l���
	int damage = 0;
	atk_check = Attack_Check();
	if (escape_check == ESCAPE_CHECK_NO)
	{
		atk_check = ESCAPE_NO;
	}
	if (atk_check == ESCAPE_NO)
	{
		week_check = Attack_WeekCheck(enemy_wepon_type, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type);
		if (getplayer->Get_WMode())
		{
			damage = (int)PLAYER_W_DAMAGEKEISAN;
		}
		else
		{
			damage = (int)PLAYER_NORMAL_DAMAGEKEISAN;
		}
		if (week_check == CUserinterface::WEEK_TYPE)
		{
			if (!getplayer->Get_GekikaFlag())
			{
				damage *= (int)WEEK_BAIRITU;
			}
			else
			{
				damage *= (int)WEEK_GEKIKA_BAIRITU;
			}
		}
		else if (week_check == CUserinterface::RESIST_TYPE)
		{
			if (!getplayer->Get_GekikaFlag())
			{
				damage *= (int)RESIST_BAIRITU;
			}
			else
			{
				damage *= (int)RESIST_GEKIKA_BAIRITU;
			}
		}
		if (damage < 0)
		{
			damage = 1;
		}
	}
	if (atk_check == ESCAPE_OK)
	{
		getplayer->Damage(0, angle, week_check);
		CUserinterface::UI_TextCreate(CUserinterface::ENEMY, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, 0, charatype);
	}
	else if (atk_check == ESCAPE_NO)
	{
		Skill_Effect(skill_number, damage, week_check, angle, charatype);
	}
}

void CAttack::Skill_PlayerEffect(C3DObj *enemy, int skill_number, int damage, int week_check)
{
	//��Ԉُ�̎��̓G�l�~�[�̕��Ɍ����Ȃ��̂Ō����悤�ɂ��邱��
	//UI���o���Ƃ��͕K���_���[�W�v�Z�O�ɂ��邱��
	C3DObj *getplayer = CPlayer::Get_Player();
	switch (skill_number)
	{
	case CWepon::TYPE_HIGHATTACK_SKILL:
		damage = (int)(damage * 1.3f);
		CUserinterface::UI_TextPlayerSkill(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, week_check, CUserinterface::PLAYERCHARA, CUserinterface::SKILLATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
		getplayer->Player_SkillHpDown();
		getplayer->Set_Attack_End(ATTACK_END);
		getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
		break;
	case CWepon::TYPE_BACKATTACK_SKILL:
		getplayer->Player_SkillHpDown();
		CUserinterface::UI_TextPlayerNodamageSkill(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_uieffect, week_check, CUserinterface::PLAYERCHARA, CUserinterface::NODAMAGE_SKILL, CUserinterface::ENEMY,  enemy->Get_Type());
		enemy->SetBackMove(true);
		break;
	case CWepon::TYPE_WARP_SKILL:
		getplayer->Player_SkillHpDown();
		CUserinterface::UI_TextPlayerNodamageSkill(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_uieffect, week_check, CUserinterface::PLAYERCHARA, CUserinterface::NODAMAGE_SKILL, CUserinterface::ENEMY, enemy->Get_Type());
		enemy->Set_Warp(true);
		break;
	case CWepon::TYPE_POIZUN_ATTACK:
		damage = (int)(damage * 1.1f);
		getplayer->Player_SkillHpDown();
		getplayer->Set_Attack_End(ATTACK_END);
		CUserinterface::UI_TextPlayerSkillCondition(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_uieffect,week_check, CUserinterface::PLAYERCHARA, CUserinterface::SKILLATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
		getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
		if (enemy->Get_Condition() != POIZUN_CONDITION)
		{
			enemy->SetCondition(POIZUN_CONDITION);
			enemy->SetTurnCount(0);
		}
		break;
	case CWepon::TYPE_SLEEP_ATTACK:
		damage = (int)(damage * 1.1f);
		getplayer->Player_SkillHpDown();
		getplayer->Set_Attack_End(ATTACK_END);
		CUserinterface::UI_TextPlayerSkillCondition(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_uieffect, week_check, CUserinterface::PLAYERCHARA, CUserinterface::SKILLATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
		getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
		if (enemy->Get_Condition() != SLEEP_CONDITION)
		{
			enemy->SetCondition(SLEEP_CONDITION);
			enemy->SetTurnCount(0);
		}
		break;
	case CWepon::TYPE_KURAYAMI_ATTACK:
		damage = (int)(damage * 1.1f);
		getplayer->Player_SkillHpDown();
		getplayer->Set_Attack_End(ATTACK_END);
		CUserinterface::UI_TextPlayerSkillCondition(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_uieffect, week_check, CUserinterface::PLAYERCHARA, CUserinterface::SKILLATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
		getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
		if (enemy->Get_Condition() != KURAYAMI_CONDITION)
		{
			enemy->SetCondition(KURAYAMI_CONDITION);
			enemy->SetTurnCount(0);
		}
		break;
	case CWepon::TYPE_MAHUJI_ATTACK:
		damage = (int)(damage * 1.1f);
		getplayer->Player_SkillHpDown();
		getplayer->Set_Attack_End(ATTACK_END);
		CUserinterface::UI_TextPlayerSkillCondition(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_uieffect, week_check, CUserinterface::PLAYERCHARA, CUserinterface::SKILLATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
		getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
		if (enemy->Get_Condition() != MAHUJI_CONDITION)
		{
			enemy->SetCondition(MAHUJI_CONDITION);
			enemy->SetTurnCount(0);
		}
		break;
	case CWepon::TYPE_THREE_ATTACK:
		damage = (int)(damage * 1.1f);
		CUserinterface::UI_TextPlayerSkill(CWepon::Get_Wepon_Skill(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_sikibetu_number)->Skill_name, week_check, CUserinterface::PLAYERCHARA, CUserinterface::SKILLATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
		getplayer->Set_Attack_End(ATTACK_END);
		getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));	
		break;
	}
}


void CAttack::Skill_Effect(int skill_number, int damage, int week_check, float angle, int charatype)
{
	//��Ԉُ�̎��̓G�l�~�[�̕��Ɍ����Ȃ��̂Ō����悤�ɂ��邱��
	C3DObj *getplayer = CPlayer::Get_Player();
	switch (skill_number)
	{
	case HIGH_ATTACK_SKILL:
		damage = (int)(damage * 1.3f);
		getplayer->Damage(damage, angle, week_check);
		CUserinterface::UI_TextCreate("�̑S�͎a��I",week_check, CUserinterface::ENEMY, HIGH_ATTACK_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	case HITYU_ATTACK_SKILL:
		getplayer->Damage(damage, angle, week_check);
		CUserinterface::UI_TextCreate("�̕K�����I", week_check, CUserinterface::ENEMY, HITYU_ATTACK_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	case HANI_FURASHU_SKILL:
		damage = 30;
		getplayer->Damage(damage, angle, week_check);
		CUserinterface::UI_TextCreate("�̃n�j�[�t���b�V���I", week_check, CUserinterface::ENEMY, HANI_FURASHU_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	case KIAIDAME_SKILL:
		//�C�����߂̏����̓G�l�~�[���ŏ�������
		CUserinterface::UI_TextCreate("�͗͂𗭂߂��I", week_check, CUserinterface::ENEMY, KIAIDAME_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	case SUNAKEMURI_SKILL:
		if (getplayer->Get_Condition() != KURAYAMI_CONDITION)
		{
			getplayer->SetTurnCount(0);
		}
		getplayer->SetCondition(KURAYAMI_CONDITION);
		CUserinterface::UI_TextCreate("�͍������N�������I", week_check, CUserinterface::ENEMY, SUNAKEMURI_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	case KOMORIUTA_SKILL:
		if (getplayer->Get_Condition() != SLEEP_CONDITION)
		{
			getplayer->SetTurnCount(0);
		}
		getplayer->SetCondition(SLEEP_CONDITION);
		CUserinterface::UI_TextCreate("�͎q��S���̂����I", week_check, CUserinterface::ENEMY, KOMORIUTA_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	case POIZUN_SKILL:
		if (getplayer->Get_Condition() != POIZUN_CONDITION)
		{
			getplayer->SetTurnCount(0);
		}
		getplayer->SetCondition(POIZUN_CONDITION);
		CUserinterface::UI_TextCreate("�͓ł�f�����I", week_check, CUserinterface::ENEMY, POIZUN_SKILL, CUserinterface::PLAYERCHARA, damage, charatype);
		break;
	}
}

bool CAttack::Attack_Check(void)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 100);
	int atkcheck = random(mt);
	if (atkcheck <= ESCAPE_NUMBER)
	{
		return ESCAPE_OK;
	}
	return ESCAPE_NO;
}

int CAttack::Attack_WeekCheck(int attack_wepontype, int damage_wepontype)
{
	if (attack_wepontype == CWepon::TYPE_SWORD || attack_wepontype == CWepon::TYPE_BIGSWORD)
	{
		if (damage_wepontype == CWepon::TYPE_REIPIA || damage_wepontype == CWepon::TYPE_RANCE)
		{
			return CUserinterface::RESIST_TYPE;
		}
		if (damage_wepontype == CWepon::TYPE_SWORD || damage_wepontype == CWepon::TYPE_BIGSWORD)
		{
			return CUserinterface::NORMAL_TYPE;
		}
		if (damage_wepontype == CWepon::TYPE_MEISU || damage_wepontype == CWepon::TYPE_AX)
		{
			return CUserinterface::WEEK_TYPE;
		}
	}
	if (attack_wepontype == CWepon::TYPE_REIPIA || attack_wepontype == CWepon::TYPE_RANCE)
	{
		if (damage_wepontype == CWepon::TYPE_REIPIA || damage_wepontype == CWepon::TYPE_RANCE)
		{
			return CUserinterface::NORMAL_TYPE;
		}
		if (damage_wepontype == CWepon::TYPE_SWORD || damage_wepontype == CWepon::TYPE_BIGSWORD)
		{
			return CUserinterface::WEEK_TYPE;
		}
		if (damage_wepontype == CWepon::TYPE_MEISU || damage_wepontype == CWepon::TYPE_AX)
		{
			return CUserinterface::RESIST_TYPE;
		}
	}
	if (attack_wepontype == CWepon::TYPE_MEISU || attack_wepontype == CWepon::TYPE_AX)
	{
		if (damage_wepontype == CWepon::TYPE_REIPIA || damage_wepontype == CWepon::TYPE_RANCE)
		{
			return CUserinterface::WEEK_TYPE;
		}
		if (damage_wepontype == CWepon::TYPE_SWORD || damage_wepontype == CWepon::TYPE_BIGSWORD)
		{
			return CUserinterface::RESIST_TYPE;
		}
		if (damage_wepontype == CWepon::TYPE_MEISU || damage_wepontype == CWepon::TYPE_AX)
		{
			return CUserinterface::NORMAL_TYPE;
		}
	}
	return CUserinterface::NORMAL_TYPE;
}

int CAttack::Attack_DamegeKeisan(int week_check, C3DObj *enemy, int str, float str2)
{
	C3DObj *getplayer;
	getplayer = CPlayer::Get_Player();
	int damage = 0;

	week_check = Attack_WeekCheck(CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type, enemy->m_WeponType);
	damage = (int)ENEMY_DAMAGEKEISAN;
	if (week_check == CUserinterface::WEEK_TYPE)
	{
		if (!getplayer->Get_GekikaFlag())
		{
			damage *= (int)WEEK_BAIRITU;
		}
		else
		{
			damage *= (int)WEEK_GEKIKA_BAIRITU;
		}
	}
	else if (week_check == CUserinterface::RESIST_TYPE)
	{
		if (!getplayer->Get_GekikaFlag())
		{
			damage *= (int)RESIST_BAIRITU;
		}
		else
		{
			damage *= (int)RESIST_GEKIKA_BAIRITU;
		}
	}
	if (damage < 0)
	{
		damage = 1;
	}
	SAVE_COUNT *save;
	save = Get_SaveCount();
	save->damage_count += damage;
	return damage;
}