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
#include "CObject_Kaki.h"
#define _CRTDBG_MAP_ALLOC
#define OBJECT_CSV_NAME "CSV/ITEM_CSV.csv"
#define MAX_OBJECT 99
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
CObject::OBJECT_Data CObject::m_ObjectData[MAX_OBJECT] = { 0 };
//�A�C�e���f�[�^�ő吔�����߂�
int CObject::m_ITEMDATA_MAX = 2;
int CObject::m_ObjectNum[OBJECTTYPE_NONE] = {};

CObject::CObject()
{
	m_ObjectNum[TYPE_ALL]++;
}

void CObject::ObjectDataInit(void)
{
	int i = 0, j = 0;
	int end_check = 0;//CSV�̍s�̏I��肩�m�F
	m_ObjectData[0] = { OBJECTTYPE_NONE,"NOITEM" ,"���g�p","���g�p", 0, 0 ,-1 };
	m_ObjectData[1] = { TYPE_LADDER,"��q" ,"���g�p","�G���[", -1,-1,-1 };
	for (i = 2; i < MAX_OBJECT; i++)
	{//2�s�ڂ���ǂނ̂�+1����
		end_check = CSV_ObjectLoad(&m_ObjectData[i], j + 1);
		if (m_ObjectData[i].object_type == 0)
		{
			break;
		}
		//�I�u�W�F�N�g�^�C�v��1����邽�ߒ���
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
//	�j��
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
		MessageBox(NULL, "ITEM_CSV��ǂݍ��߂܂���ł���\nCSV��ۑ����Ȃ����Β���\��������܂�", "�G���[", MB_OK);
		exit(1);
		return -1;
	}
	//�f�[�^��ǂݍ���
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



