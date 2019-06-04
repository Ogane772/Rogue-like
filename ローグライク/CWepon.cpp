//////////////////////////////////////////////////
////
////	装備クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CWepon.h"
#include "sound.h"
#include "CWepon_Sword.h"
#include "CWepon_Sheld.h"
#include "CWepon_Ring.h"
#include "CWepon_BigSword.h"
#include "CWepon_Reipia.h"
#include "CWepon_Meisu.h"
#include "CWepon_Ax.h"
#include "CWepon_Rance.h"
#define _CRTDBG_MAP_ALLOC

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
CWepon::Wepon_Data CWepon::m_WeponData[]
{
	//武器タイプ 名前 使用時の説明文 攻撃力　防御力　開始出現フロア 終わり出現フロア　確率
	{ TYPE_SWORD,"ただの剣" ,"何の変哲もない剣", 1, 0 ,-1,-1,-1 },
	{ TYPE_SHELD, "ただの盾" ,"何の変哲もない盾", 0, 1,-1,-1,-1 },
	{ TYPE_RING,"ただの指輪" ,"何の変哲もない指輪", 1, 1,-1,-1,-1 },
	{ TYPE_SWORD,"ひのきの棒" ,"木を削った棒", 2, 0,0,3,25 },
	{ TYPE_SWORD,"マスターソード" ,"伝説の勇者の剣", 10,0, 0,3,25 },
	{ TYPE_SWORD, "ロトの剣" ,"ロトが使った剣", 10,0, 0,3,25 },
	{ TYPE_SWORD,"銀河の剣" ,"星の力が込められた剣", 15,0, 0,3,25 },
	{ TYPE_BIGSWORD,"銅の大剣" ,"銅で作られた大剣", 15,1, 0,3,50 },
	{ TYPE_BIGSWORD,"銀の大剣" ,"銀で作られた大剣", 16,2, 0,3,50 },
	{ TYPE_REIPIA,"銅のレイピア" ,"銅で作られたレイピア", 5,0, 0,3,50 },
	{ TYPE_REIPIA,"銀のレイピア" ,"銀に輝くレイピア", 5,0, 0,3,50 },
	{ TYPE_MEISU,"メイス" ,"普通のメイス", 6,0, 0,3,50 },
	{ TYPE_MEISU,"怒りのメイス" ,"怒りが込められたメイス", 9,0, 0,3,50 },
	{ TYPE_RANCE,"銅のランス" ,"剣に対して強い槍", 12,1, 0,3,50 },
	{ TYPE_RANCE,"刺突のランス" ,"剣に対して強い鋭い槍", 14,0, 0,3,50 },
	{ TYPE_AX,"銅の斧" ,"刺突に対して強い銅の斧", 15,1, 0,3,50 },
	{ TYPE_AX,"銀の斧" ,"刺突に対して強い斧", 15,1, 0,3,50 },
	{ TYPE_SHELD, "おなべのふた" ,"ないよりはましな盾", 0, 1,0,3,50 },
	{ TYPE_SHELD, "ミドルシールド" ,"軽くて小さい盾", 0, 2,0,3,50 },
	{ TYPE_RING,"小さい力の指輪" ,"力が少し上がる指輪", 1, 1,0,3,50 },
	{ TYPE_RING,"小さい守の指輪" ,"守備が少し上がる指輪", 1, 1,0,3,50 }
};
//ウェポンデータ最大数を求める
int CWepon::m_WEPONDATA_MAX = sizeof(CWepon::m_WeponData) / sizeof(m_WeponData[0]);
int CWepon::m_WeponNum[TYPE_MAX] = {};

CWepon::CWepon()
{
	m_WeponNum[TYPE_ALL]++;
}

CWepon::CWepon(int WeponType)
{
	m_WeponType = WeponType;
	m_WeponIndex = m_WeponNum[TYPE_ALL];
	m_WeponNum[TYPE_ALL]++;
	m_WeponNum[WeponType]++;
}

void CWepon::Create(int object_type, int x, int z)
{
	CWeponSword *psword;
	CWeponBigSword *pbigsword;
	CWeponSheld *psheld;
	CWeponRing *pring;
	CWeponReipia *preipia;
	CWeponMeisu *pmeisu;
	CWeponRance *prance;
	CWeponAx *pax;
	switch (object_type)
	{
	case TYPE_SWORD:
		psword = new CWeponSword(x, z);
		break;
	case TYPE_BIGSWORD:
		pbigsword = new CWeponBigSword(x, z);
		break;
	case TYPE_REIPIA:
		preipia = new CWeponReipia(x, z);
		break;
	case TYPE_RANCE:
		prance = new CWeponRance(x, z);
		break;
	case TYPE_MEISU:
		pmeisu = new CWeponMeisu(x, z);
		break;
	case TYPE_AX:
		pax = new CWeponAx(x, z);
		break;
	case TYPE_SHELD:
		psheld = new CWeponSheld(x, z);
		break;
	case TYPE_RING:
		pring = new CWeponRing(x, z);
		break;
	}
}

void CWepon::Wepon_Finalize(int Index)
{

}
//=============================================================================
//	破棄
//=============================================================================

CWepon::~CWepon()
{
	m_WeponNum[TYPE_ALL]--;
}

C3DObj *CWepon::Get_Wepon(int index)
{
	C3DObj *pwepon = C3DObj::Get(index);
	if (pwepon)
	{
		if (pwepon->Get_3DObjType() == C3DObj::TYPE_WEPON)
		{

			return pwepon;

		}
	}
	return NULL;
}


C3DObj *CWepon::Get_AllWepon(int Index)
{
	C3DObj *pWepon = C3DObj::Get(Index);
	if (pWepon)
	{
		if (pWepon->Get_3DObjType() == C3DObj::TYPE_WEPON)
		{

			return pWepon;

		}
	}
	return NULL;
}

void CWepon::DeleteAllWepon(void)
{
	int i;
	C3DObj *wepon;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		wepon = CWepon::Get_Wepon(i);

		if (wepon)
		{
			delete wepon;
		}
	}
}

C3DObj *CWepon::Get_Map_Wepon(int Index)
{
	C3DObj *pWepon = C3DObj::Get(Index);
	if (pWepon)
	{
		if (pWepon->Get_3DObjType() == C3DObj::TYPE_WEPON)
		{
			if (pWepon->Get_DrawCheck())
			{
				return pWepon;
			}
		}
	}
	return NULL;
}







