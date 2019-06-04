#include "CPlayer.h"
#include "CEnemy.h"
#include "userinterface.h"
#include "map.h"
#include "Attack.h"
#include "CWepon.h"
#include <iostream>
#include <random>
#define WEEK_BAIRITU (1.3f)
#define RESIST_BAIRITU (0.7f)

void CAttack::Attack_PlayerUpdate(int charatype, float vec, int str)
{
	int e;
	int week_check = 0;
	C3DObj *getplayer;
	C3DObj *enemy;
	bool atk_check;//回避成功値代入
	int damage = 0;//ダメージ計算式の結果を代入
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
					damage = ENEMY_DAMAGEKEISAN;
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
				if (vec == 0.0f)	// 上攻撃
				{
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
				if (vec == 1.6f)	// 右攻撃
				{
					// 当たっていたらダメージを与える
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
				if (vec == 3.2f)	// 下攻撃
				{
					// 当たっていたらダメージを与える
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
				if (vec == 4.8f)	// 左攻撃
				{
					// 当たっていたらダメージを与える
					if (enemy->m_Judge_enemy.HitEnemyRight && atk_check == ESCAPE_NO)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(damage, getplayer->Get_Angle(), week_check));
						CUserinterface::UI_TextCreate(week_check, CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, CUserinterface::ENEMY, damage, enemy->Get_Type());
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

				// 通路から斜め攻撃は当たらない
				if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
					CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
				{
					if (vec == 0.8f)	// 右上攻撃
					{
						// 当たっていたらダメージを与える
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

					if (vec == 2.4f)	// 右下攻撃
					{
						// 当たっていたらダメージを与える
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

					if (vec == 4.0f)	// 左下攻撃
					{
						// 当たっていたらダメージを与える
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

					if (vec == 5.6f)	// 左上攻撃
					{
						// 当たっていたらダメージを与える
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

void CAttack::Attack_EnemyUpdate(int enemy_wepon_type, int charatype, int str, float angle)
{
	int week_check = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
	bool atk_check;//回避成功値代入
	int damage = 0;
	atk_check = Attack_Check();
	if (atk_check == ESCAPE_NO)
	{
		week_check = Attack_WeekCheck(enemy_wepon_type, CPlayer::GetPlayerWeponData(CPlayer::WEPON_NUMBER)->wepon_type);
		if (getplayer->Get_WMode())
		{
			damage = PLAYER_W_DAMAGEKEISAN;
		}
		else
		{
			damage = PLAYER_NORMAL_DAMAGEKEISAN;
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
