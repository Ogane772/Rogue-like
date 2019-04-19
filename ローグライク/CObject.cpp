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
#include "exp.h"
#include "sound.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	定数定義
//=============================================================================
#define ORNAMENT_HITSTOP (100)

//=============================================================================
//	静的変数
//=============================================================================

//=============================================================================
//	生成
//=============================================================================
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
	switch (object_type)
	{
	case TYPE_LADDER:
		CObjectLadder *pladder = new CObjectLadder(x, z);
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

void CObject::ObjectDamage(void)
{
	m_Hp--;
	if (m_Hp <= 0)
	{
		m_TimeKeep = m_FrameCount;
	}

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







