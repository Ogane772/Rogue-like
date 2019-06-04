//////////////////////////////////////////////////
////
////	��Q���N���X
////
//////////////////////////////////////////////////

//=============================================================================
//	�C���N���[�h�t�@�C��
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
//	�萔��`
//=============================================================================

//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

//=============================================================================
//	����
//=============================================================================
//�A�C�e���o�����͕K�����̊K��100���ɍ��킹�邱�ƁI
//100���ɑ���Ȃ��ꍇ�͎G���ɂ���
CObject::OBJECT_Data CObject::m_ObjectData[]
{
	//���O �g�p���̐����� �J�n�o���t���A �I���o���t���A�@�m��
	{ OBJECTTYPE_NONE,"NOITEM" ,"���g�p","���g�p", 0, 0 ,-1},
	{ TYPE_LADDER,"��q" ,"���g�p","�G���[", -1,-1,-1 },
	{ TYPE_YAKUSOU,"�₭����" ,"HP�����񕜂����I","HP�����񕜂���", 0, MAX_MAP,50 },
	{ TYPE_ZAXTUSOU,"�G��" ,"�������ق�̏����c�ꂽ","�����̑�", 0, MAX_MAP,50 },
};
//�A�C�e���f�[�^�ő吔�����߂�
int CObject::m_ITEMDATA_MAX = sizeof(CObject::m_ObjectData) / sizeof(m_ObjectData[0]);
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
	case TYPE_ZAXTUSOU:
		pyakusou = new CObjectYakusou(x, z, object_type);
		break;
	}
}

void CObject::Object_Finalize(int Index)
{

}
//=============================================================================
//	�j��
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







