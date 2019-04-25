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
	getplayer = CPlayer::Get_Player();
	for (e = 0; e < MAX_GAMEOBJ; e++)
	{	
		enemy = CEnemy::Get_Enemy(e);
		if (enemy)
		{
			if (enemy->alive)
			{
				if (vec == 0.0f)	// ãUŒ‚
				{
					// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
					if (enemy->m_Judge_enemy.HitEnemyBottom)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(str));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
						break;
					}
				}
				if (vec == 1.6f)	// ‰EUŒ‚
				{
					// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
					if (enemy->m_Judge_enemy.HitEnemyLeft)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(str));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
						break;
					}
				}
				if (vec == 3.2f)	// ‰ºUŒ‚
				{
					// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
					if (enemy->m_Judge_enemy.HitEnemyTop)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(str));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
						break;
					}
				}
				if (vec == 4.8f)	// ¶UŒ‚
				{
					// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
					if (enemy->m_Judge_enemy.HitEnemyRight)
					{
						getplayer->Set_Attack_End(ATTACK_END);
						getplayer->Set_RivalFlag(enemy->Damage(str));
						CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
						break;
					}
				}

				// ’Ê˜H‚©‚çŽÎ‚ßUŒ‚‚Í“–‚½‚ç‚È‚¢
				if (CMap::Map_GetData(getplayer->m_Mapz, getplayer->m_Mapx).type != 2 &&
					CMap::Map_GetData(enemy->m_Mapz, enemy->m_Mapx).type != 2)
				{
					if (vec == 0.8f)	// ‰EãUŒ‚
					{
						// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
						if (enemy->m_Judge_enemy.HitEnemyBottomLeft)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(str));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
							break;
						}
					}

					if (vec == 2.4f)	// ‰E‰ºUŒ‚
					{
						// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
						if (enemy->m_Judge_enemy.HitEnemyTopLeft)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(str));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
							break;
						}
					}

					if (vec == 4.0f)	// ¶‰ºUŒ‚
					{
						// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
						if (enemy->m_Judge_enemy.HitEnemyTopRight)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(str));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
							break;
						}
					}

					if (vec == 5.6f)	// ¶ãUŒ‚
					{
						// “–‚½‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ð—^‚¦‚é
						if (enemy->m_Judge_enemy.HitEnemyBottomRight)
						{
							getplayer->Set_Attack_End(ATTACK_END);
							getplayer->Set_RivalFlag(enemy->Damage(str));
							CUserinterface::UI_TextCreate(CUserinterface::PLAYERCHARA, CUserinterface::REGULARATTACK, enemy->type, str, enemy->Get_Name());
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
	// ƒ_ƒ[ƒWŠÖ”“à‚Å‘‚­‚×‚«‚©‚à
	CUserinterface::UI_TextCreate(CUserinterface::SRAIM, CUserinterface::REGULARATTACK, CUserinterface::PLAYERCHARA, str, getplayer->Get_Name());
}