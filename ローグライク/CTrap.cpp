//////////////////////////////////////////////////
////
////	ワナクラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "CTrap.h"
#include "exp.h"
#include "map.h"
#include "sound.h"
#include "CTrap_Damage.h"
#include "CTrap_Damage.h"
#include "CTrap_Donsoku.h"
#include "CTrap_Jyosou.h"
#include "CTrap_Kuhuku.h"
#include "CTrap_Kurayami.h"
#include "CTrap_Poizun.h"
#include "CTrap_Sabi.h"
#include "CTrap_Sleep.h"
#include "CTrap_Warp.h"
#define _CRTDBG_MAP_ALLOC
#define TRAP_CSV_NAME "CSV/TRAP_CSV.csv"
#define MAX_TRAP 99
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================

//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================
//アイテム出現率は必ずその階で100％に合わせること！
//100％に足りない場合は雑草にする
CTrap::TRAP_Data CTrap::m_TrapData[MAX_TRAP] = { 0 };
//アイテムデータ最大数を求める
int CTrap::m_TRAPDATA_MAX = 0;
int CTrap::m_TrapNum[TRAPTYPE_NONE] = {};

CTrap::CTrap()
{
	m_TrapNum[TYPE_ALL]++;
}

void CTrap::TrapDataInit(void)
{
	int i = 0;
	int end_check = 0;//CSVの行の終わりか確認
	for (i = 0; i < MAX_TRAP; i++)
	{//2行目から読むので+1する
 		end_check = CSV_TrapLoad(&m_TrapData[i], i);
		if (m_TrapData[i].trap_type == 0)
		{
			break;
		}
		m_TRAPDATA_MAX++;
	}
}

CTrap::CTrap(int TrapType)
{
	m_TrapType = TrapType;
	m_TrapIndex = m_TrapNum[TYPE_ALL];
	m_TrapNum[TYPE_ALL]++;
	m_TrapNum[TrapType]++;
}

void CTrap::Create(int trap_type, int x, int z)
{
	CTrapDamage *pdamage = NULL;
	CTrapKuhuku *pkuhuku = NULL;
	CTrapWarp *pwarp = NULL;
	CTrapSleep *psleep = NULL;
	CTrapPoizun *ppoizun = NULL;
	CTrapKurayami *pkurayami = NULL;
	CTrapDonsoku *pdonsoku = NULL;
	CTrapJyosou *pjyosou = NULL;
	CTrapSabi *psabi = NULL;
	switch (trap_type)
	{
	case TYPE_DAMAGE:
		pdamage = new CTrapDamage(x, z, trap_type);
		break;
	case TYPE_KUHUKU:
		pkuhuku = new CTrapKuhuku(x, z, trap_type);
		break;
	case TYPE_WARP_TRAP:
		pwarp = new CTrapWarp(x, z, trap_type);
		break;
	case TYPE_SLEEP_TRAP:
		psleep = new CTrapSleep(x, z, trap_type);
		break;
	case TYPE_POIZUN_TRAP:
		ppoizun = new CTrapPoizun(x, z, trap_type);
		break;
	case TYPE_KUYARAMI_TRAP:
		pkurayami = new CTrapKurayami(x, z, trap_type);
		break;
	case TYPE_DONSOKU_TRAP:
		pdonsoku = new CTrapDonsoku(x, z, trap_type);
		break;
	case TYPE_ITEMDELETE_TRAP:
		pjyosou = new CTrapJyosou(x, z, trap_type);
		break;
	case TYPE_SABIRU_TRAP:
		psabi = new CTrapSabi(x, z, trap_type);
		break;
	}
}

void CTrap::Trap_Finalize(int Index)
{

}
//=============================================================================
//	破棄
//=============================================================================

CTrap::~CTrap()
{
	m_TrapNum[TYPE_ALL]--;
}

C3DObj *CTrap::Get_Trap(int index)
{
	C3DObj *ptrap = C3DObj::Get(index);
	if (ptrap)
	{
		if (ptrap->Get_3DObjType() == C3DObj::TYPE_TRAP)
		{

			return ptrap;

		}
	}
	return NULL;
}


C3DObj *CTrap::Get_AllTrap(int Index)
{
	C3DObj *pTrap = C3DObj::Get(Index);
	if (pTrap)
	{
		if (pTrap->Get_3DObjType() == C3DObj::TYPE_TRAP)
		{

			return pTrap;

		}
	}
	return NULL;
}

void CTrap::DeleteAllTrap(void)
{
	int i;
	C3DObj *trap;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		trap = CTrap::Get_Trap(i);

		if (trap)
		{
			delete trap;
		}
	}
}

C3DObj *CTrap::Get_Map_Trap(int Index)
{
	C3DObj *pTrap = C3DObj::Get(Index);
	if (pTrap)
	{
		if (pTrap->Get_3DObjType() == C3DObj::TYPE_TRAP)
		{
			if (pTrap->Get_DrawCheck())
			{
				return pTrap;
			}
		}
	}
	return NULL;
}

int CTrap::CSV_TrapLoad(TRAP_Data* trap, const int num)
{
	int cnt = 0;
	char buf[128];
	int size = 128;
	int data[20] = { 0 };
	FILE* file = NULL;

	if ((file = fopen(TRAP_CSV_NAME, "r")) == NULL)
	{
		MessageBox(NULL, "TRAP_CSVを読み込めませんでした\nCSVを保存しなおせば直る可能性があります", "エラー", MB_OK);
		exit(1);
		return -1;
	}
	//データを読み込む
	while (fgets(buf, size, file) != NULL && num > ++cnt);
	fscanf(file, "%d,%[^,],%[^,],%d,%d,%d", &trap->trap_type, &trap->trap_name, &trap->trap_effect, &trap->first_floor, &trap->end_floor, &trap->trapchance);

	fclose(file);



	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}



