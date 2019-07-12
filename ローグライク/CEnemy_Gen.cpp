#include "CEnemy.h"
#include <d3dx9.h>
#include "Attack.h"
#include "CEnemy_Gen.h"
#include "Cplayer.h"
//=============================================================================
//	生成
//=============================================================================

CEnemy_Gen::CEnemy_Gen(int x, int z, ENEMY_Data enemy_data) :CEnemy(enemy_data.enemy_type), C3DObj(C3DObj::TYPE_ENEMY)
{
	Initialize(x, z, enemy_data);
}

//=============================================================================
//	破棄
//=============================================================================

CEnemy_Gen::~CEnemy_Gen()
{
	AnimeModel_Finalize(&anime_model);
	//NormalModel_Finalize(&Normal_model);
	//delete this;
}


void CEnemy_Gen::Initialize(int x, int z, ENEMY_Data enemy_data)
{
	int i;
	strcpy_s(name, MAX_NAME, enemy_data.enemy_name);
	m_EnemyIndex = Get_EnemyIndex(TYPE_ALL);
	TurnCount = 0;
	m_Condition = NORMAL_CONDITION;
	get_turbo = false;
	m_WeponType = enemy_data.wepon_type;
	add_time = 0;
	alive = true;
	m_RangeHit = false;
	rival_flag = false;
	map_drawflag = false;
	m_Back = false;
	m_Position = D3DXVECTOR3(-247.5f + x * 5, 0.0f, 247.5f - z * 5);
	m_EnemyMyColision.position = m_Position;
	m_EnemyMyColision.radius = ENEMY_RADIUS;
	enemyturn = ENEMY_WAIT;
	m_Type = enemy_data.enemy_type;
	m_MaxHp = enemy_data.Hp;
	m_Hp = m_MaxHp;
	m_Str = enemy_data.str;
	m_Str2 = 1.0f;
	m_Def = enemy_data.def;
	m_Exp = enemy_data.exp;
	m_Gold = enemy_data.gold;
	m_Goway = false;
	m_Warp = false;
	m_TimeWarp = false;
	m_Angle = 0.0f;
	m_TurnCount = 0;
	walkf = 0;
	attackframe = 0;
	nanawalk = 0;
	m_BackFrame = 0;
	m_Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Normalize(&m_Front, &m_Front);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Front);
	D3DXVec3Normalize(&m_Right, &m_Right);

	D3DXVec3Cross(&m_Up, &m_Front, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	// マップ位置を登録
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
	set_item = 0;

	char animefile[TEXTURE_FILENAME_MAX] = {};
	strcpy_s(animefile, C3DObj::Get_AnimeFileName(MODELL_ANIME_GENSAN));

	SkinMesh.InitThing(m_pD3DDevice, &anime_model, animefile);

	//モデル情報取得
	for (i = 0; i < (signed)anime_model.pAnimController->GetNumAnimationSets(); i++)
	{//AnimSetにアニメーション情報格納
		anime_model.pAnimController->GetAnimationSet(i, &pAnimSet[i]);
	}
	//アニメーション情報初期化
	TrackDesc.Weight = 1;
	TrackDesc.Enable = true;
	TrackDesc.Position = 0;//アニメーションタイムリセット
	TrackDesc.Speed = 0.005f;//モーションスピード
	anime_model.pAnimController->SetTrackDesc(0, &TrackDesc);//アニメ情報セット
	anime_model.pAnimController->SetTrackAnimationSet(0, pAnimSet[IDLE]);//初期アニメーションセット
	m_AnimationType = IDLE;
	Animation_Change(IDLE, 0.005f);
}

void CEnemy_Gen::Update(void)
{
	Enemy_Update();
}

void CEnemy_Gen::Draw(void)
{
	Enemy_Draw();

	//DebugFont_Draw(0, 330, "set_item = %d", set_item);
	//DebugFont_Draw(0, 100, "k.pos x = %f k.pos y = %f k.pos z = %f", m_EnemyMyColision.position.x, m_EnemyMyColision.position.y, m_EnemyMyColision.position.z);
	//DebugFont_Draw(0, 150, "e.pos x = %f e.pos y = %f e.pos z = %f", m_Position.x, m_Position.y, m_Position.z);
	//DebugFont_Draw(200, 150, "エネミーモード%d", enemyturn);
	//DebugFont_Draw(200, 25 0, "Attackfream = %d", attackframe);
	//DebugFont_Draw(0, 350, "Angle = %f", m_Angle);
	//DebugFont_Draw(0, 400, "床タイプ = %d", CMap::Map_GetData(m_Mapz, m_Mapx).type);
	//DebugFont_Draw(0, 500, "mapX = %d,mapZ = %d", m_Mapx,m_Mapz);
	//DebugFont_Draw(0, 300, "エネミー nanawalk = %d", nanawalk);
	//DebugFont_Draw(0, 330, "エネミー walk = %d", walkf);
	/*
	DebugFont_Draw(200, 20, "エネミー出現数%d", m_EnemyNum[TYPE_SRIME]);
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
	//DebugFont_Draw(200, 150, "エネミーモード%d", enemyturn);
	*/
}

void CEnemy_Gen::EnemySkill(void)
{
		std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> random(0, 2);
	int attack_number = 0;
	C3DObj *getplayer = CPlayer::Get_Player();
	//attack_number = random(mt);
	switch (attack_number)
	{
	case 0:
		CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Str2, m_Angle);
		//CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Angle);
		/*
		if (set_item == 0)
		{//盗む処理
			CAttack::Attack_EnemySkill(this, CAttack::TOUZOKU_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
			if (set_item != 0)
			{//盗んでいたらワープ処理をセット
				m_TimeWarp = true;
			}
		}
		//if (getplayer->Get_Condition() == NORMAL_CONDITION && m_Condition != MAHUJI_CONDITION)
		//{
		//	CAttack::Attack_EnemySkill(this,CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
		//}
		else
		{
			CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Str2, m_Angle);
		}
		//チャージ倍率増やすときはここに書く
		//m_Str2 = CHARGE_BUFF;
		*/
		break;
	case 1:
		//CAttack::Attack_EnemySkill(CAttack::HIGH_ATTACK_SKILL, m_WeponType, m_Type, m_Str, m_Angle, ESCAPE_CHECK_OK);
		if (getplayer->Get_Condition() == NORMAL_CONDITION && m_Condition != MAHUJI_CONDITION)
		{
			CAttack::Attack_EnemySkill(this, CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
		}
		else
		{
			CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Str2, m_Angle);
		}
		//m_Str2 = CHARGE_BUFF;
		break;
	case 2:
		if (getplayer->Get_Condition() == NORMAL_CONDITION && m_Condition != MAHUJI_CONDITION)
		{
			CAttack::Attack_EnemySkill(this, CAttack::POIZUN_SKILL, m_WeponType, m_Type, m_Str, m_Str2, m_Angle, ESCAPE_CHECK_OK);
		}
		else
		{
			CAttack::Attack_EnemyUpdate(m_WeponType, m_Type, m_Str, m_Str2, m_Angle);
		}
		//m_Str2 = CHARGE_BUFF;
		break;
	}
}



