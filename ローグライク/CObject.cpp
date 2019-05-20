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
CObject::OBJECT_Data CObject::m_ObjectData[]
{
	//名前 使用時の説明文 開始出現フロア 終わり出現フロア
	{ "NOITEM" ,"未使用","未使用", 0, 0 },
	{ "梯子" , 0,"未使用","エラー", MAX_MAP },
	{ "やくそう" ,"HPが小回復した！","HPを小回復する", 0, MAX_MAP },
};
int CObject::m_ObjectNum[TYPE_MAX] = {};

CObject::CObject()
{
	m_ObjectNum[TYPE_ALL]++;
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
	CObjectLadder *pladder;
	CObjectYakusou *pyakusou;
	switch (object_type)
	{
	case TYPE_LADDER:
		pladder = new CObjectLadder(x, z);
		break;
	case TYPE_YAKUSOU:
		pyakusou = new CObjectYakusou(x, z);
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
	C3DObj *pornament = C3DObj::Get(index);
	if (pornament)
	{
		if (pornament->Get_3DObjType() == C3DObj::TYPE_OBJECT)
		{

			return pornament;

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







