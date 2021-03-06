//////////////////////////////////////////////////
////
////	装備クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
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
#define MAX_WEPON (99)
#define WEPON_CSV_NAME "CSV/WEPON_CSV.csv"
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
CWepon::Wepon_Data CWepon::m_WeponData[MAX_WEPON] = { 0 };
CWepon::Wepon_Skill CWepon::m_WeponSkill[] = {
	"","","","",
	"","","","",
	"","","","",
	"","","","",
	"","","","",//この4つは指輪分
"全力斬り","消費HP:小","全力の力で敵を攻撃するぞ","ログ無し",
"鎧砕き","消費HP:小","敵の防御を無視して攻撃するぞ","ログ無し",
"吹っ飛び斬","消費HP:小","目の前の敵を吹っ飛ばすぞ","は吹っ飛んだ！",
"ワープ斬","消費HP:中","目の前の敵をワープさせるぞ","はどこかにワープした！",
"毒のトゲ","消費HP:小","敵を毒にする攻撃をするぞ","を毒にした！",
"睡眠剣","消費HP:大","敵を眠らせる攻撃をするぞ","を眠らせた！",
"目潰し","消費HP:小","敵を暗闇にする攻撃をするぞ","は目が見えなくなった！",
"技封じの術","消費HP:小","敵の特技を封じるぞ","の特技を封印した！",
"破断剣","消費HP:中","前方3方向1度に攻撃できるぞ","ログ無し",
"一閃突き","消費HP:中","前方2マス1度に攻撃できるぞ","ログ無し",
};
//ウェポンデータ最大数を求める
int CWepon::m_WEPONDATA_MAX = 0;
int CWepon::m_WeponNum[WEPONTYPE_NONE] = {};

void CWepon::WeponDataInit(void)
{
	int i = 0;
	int end_check = 0;//CSVの行の終わりか確認
	for (i = 0; i < MAX_WEPON; i++)
	{//2行目から読むので+1する
		end_check = CSV_Wepon_Load(&m_WeponData[i], i + 1);
		if (m_WeponData[i].type == 0)
		{
			break;
		}
		m_WEPONDATA_MAX++;
	}
}

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

int CWepon::CSV_Wepon_Load(Wepon_Data* wepondata, const int num)
{
	int cnt = 0;
	char buf[128];
	int size = 128;
	int data[20] = { 0 };
	FILE* file = NULL;

	if ((file = fopen(WEPON_CSV_NAME, "r")) == NULL)
	{
		MessageBox(NULL, "WEPON_CSVを読み込めませんでした\nCSVを保存しなおせば直る可能性があります", "エラー", MB_OK);
		exit(1);
		return -1;
	}
	//データを読み込む
	while (fgets(buf, size, file) != NULL && num > ++cnt);
	fscanf(file, "%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d,%d", &wepondata->type, wepondata->Wepon_name, wepondata->Wepon_effect, &wepondata->str, &wepondata->def, &wepondata->first_floor, &wepondata->end_floor, &wepondata->weponchance, &wepondata->rare,&wepondata->kantei_type);

	fclose(file);



	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}






