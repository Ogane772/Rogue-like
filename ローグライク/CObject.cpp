//////////////////////////////////////////////////
////
////	障害物クラス
////
//////////////////////////////////////////////////

//=============================================================================
//	インクルードファイル
//=============================================================================

#include "CObject.h"
#include "CObject_Ladder.h"
#include "CObject_Yakusou.h"
#include "exp.h"
#include "map.h"
#include "sound.h"
#include "CObject_Kaki.h"
#define _CRTDBG_MAP_ALLOC
#define OBJECT_CSV_NAME "CSV/ITEM_CSV.csv"
#define MAX_OBJECT 99
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
CObject::OBJECT_Data CObject::m_ObjectData[MAX_OBJECT] = { 0 };
//アイテムデータ最大数を求める
int CObject::m_ITEMDATA_MAX = 2;
int CObject::m_ObjectNum[OBJECTTYPE_NONE] = {};

CObject::CObject()
{
	m_ObjectNum[TYPE_ALL]++;
}

void CObject::ObjectDataInit(void)
{
	int i = 0, j = 0;
	int end_check = 0;//CSVの行の終わりか確認
	m_ObjectData[0] = { OBJECTTYPE_NONE,"NOITEM" ,"未使用","未使用", 0, 0 ,-1 };
	m_ObjectData[1] = { TYPE_LADDER,"梯子" ,"未使用","エラー", -1,-1,-1 };
	for (i = 2; i < MAX_OBJECT; i++)
	{//2行目から読むので+1する
		end_check = CSV_ObjectLoad(&m_ObjectData[i], j + 1);
		if (m_ObjectData[i].object_type == 0)
		{
			break;
		}
		//オブジェクトタイプが1つずれるため直す
		m_ObjectData[i].object_type += 1;
		m_ITEMDATA_MAX++;
		j++;
	}
}

CObject::CObject(int ObjectType)
{
	m_ObjectType = ObjectType;
	m_ObjectIndex = m_ObjectNum[TYPE_ALL];
	m_ObjectNum[TYPE_ALL]++;
	m_ObjectNum[ObjectType]++;
}

void CObject::Create(int object_type, int x, int z)
{
	CObjectLadder *pladder = NULL;
	CObjectYakusou *pyakusou = NULL;
	CObjectKaki *pkaki = NULL;
	switch (object_type)
	{
	case TYPE_LADDER:
		pladder = new CObjectLadder(x, z);
		break;
	case TYPE_YAKUSOU:
	case TYPE_ZAXTUSOU:
	case TYPE_BAIKA:
	case TYPE_DOKUKESI:
	case TYPE_PATTIRI:
	case TYPE_IYASI:
	case TYPE_GEKIKA:
	case TYPE_WARP:
	case TYPE_SIAWASE:
	case TYPE_TOKU_YAKUSOU:
	case TYPE_KASOKU:
	case TYPE_YOSOKU:
	case TYPE_TANKEN:
	case TYPE_KENMASOU:
		pyakusou = new CObjectYakusou(x, z, object_type);
		break;
	case TYPE_PUTI_KAKI:
	case TYPE_BIG_KAKI:
		pkaki = new CObjectKaki(x, z, object_type);
		break;
	}
}

void CObject::Object_Finalize(int Index)
{

}
//=============================================================================
//	破棄
//=============================================================================

CObject::~CObject()
{
	m_ObjectNum[TYPE_ALL]--;
}

C3DObj *CObject::Get_Object(int index)
{
	C3DObj *pobject = C3DObj::Get(index);
	if (pobject)
	{
		if (pobject->Get_3DObjType() == C3DObj::TYPE_OBJECT)
		{

			return pobject;

		}
	}
	return NULL;
}


C3DObj *CObject::Get_AllObject(int Index)
{
	C3DObj *pObject = C3DObj::Get(Index);
	if (pObject)
	{
		if (pObject->Get_3DObjType() == C3DObj::TYPE_OBJECT)
		{

			return pObject;

		}
	}
	return NULL;
}

void CObject::DeleteAllObject(void)
{
	int i;
	C3DObj *object;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		object = CObject::Get_Object(i);

		if (object)
		{
			delete object;
		}
	}
}

C3DObj *CObject::Get_Map_Object(int Index)
{
	C3DObj *pObject = C3DObj::Get(Index);
	if (pObject)
	{
		if (pObject->Get_3DObjType() == C3DObj::TYPE_OBJECT)
		{
			if (pObject->Get_DrawCheck())
			{
				return pObject;
			}
		}
	}
	return NULL;
}

int CObject::CSV_ObjectLoad(OBJECT_Data* object, const int num)
{
	int cnt = 0;
	char buf[128];
	int size = 128;
	int data[20] = { 0 };
	FILE* file = NULL;

	if ((file = fopen(OBJECT_CSV_NAME, "r")) == NULL)
	{
		MessageBox(NULL, "ITEM_CSVを読み込めませんでした\nCSVを保存しなおせば直る可能性があります", "エラー", MB_OK);
		exit(1);
		return -1;
	}
	//データを読み込む
	while (fgets(buf, size, file) != NULL && num > ++cnt);
	fscanf(file, "%d,%[^,],%[^,],%[^,],%d,%d,%d", &object->object_type, &object->object_name, &object->object_effect, &object->object_tips, &object->first_floor, &object->end_floor, &object->itemchance);

	fclose(file);


	if (num != cnt)
	{
		for (cnt = 0; cnt < size; ++cnt)
			buf[cnt] = 0;
		return -2;
	}

	return 0;
}



