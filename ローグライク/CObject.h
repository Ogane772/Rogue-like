#ifndef OBJECT_H_
#define OBJECT_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
//===================================
// �萔
//===================================
#define OBJECT_MAXTIPS (99)//�������̍ő咷��
class CObject :virtual public C3DObj
{
public:
	typedef struct {
		int object_type;
		char object_name[MAX_NAME];
		char object_effect[OBJECT_MAXTIPS];//�I�u�W�F�N�g�g�p��
		char object_tips[OBJECT_MAXTIPS];//�I�u�W�F�N�g������
		int first_floor;//�o���J�n�t���A
		int end_floor;//�o���I���t���A
		int itemchance;//�A�C�e���o����
	}OBJECT_Data;
	typedef enum {
		TYPE_ALL = 0,

		TYPE_LADDER,//��q
		TYPE_YAKUSOU,//��
		TYPE_ZAXTUSOU,//�G��
		OBJECTTYPE_NONE
	}OBJECTTYPE;

	CObject();
	CObject::CObject(int ObjectType);
	~CObject();
	void Object_Finalize(int Index);


	static void Create(int object_type, int x, int z);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int Get_ITEMDATAMAX(void) { return m_ITEMDATA_MAX; }
	static OBJECT_Data* Get_Item_Data(int index) { return &m_ObjectData[index]; }
	static int Get_ObjectNum(int OrnamType) { return m_ObjectNum[OrnamType]; }
	static int Get_ObjectIndex(int EnemyType) { return m_ObjectNum[EnemyType] - 1; }
	int Get_Type(void) { return m_Type; }
	static char* Get_ObjectName(int index) { return m_ObjectData[index].object_name; }
	static char* Get_ObjectEffect(int index) { return m_ObjectData[index].object_effect; }
	static C3DObj *Get_Object(int index);
	bool Get_DrawCheck(void) { return alive; }
	static void CObject::DeleteAllObject(void);
	static C3DObj *Get_Map_Object(int Index);
protected:
	int m_ObjectIndex;

	static C3DObj *Get_AllObject(int index);

private:
	static int m_ObjectNum[TYPE_MAX];
	static int m_ITEMDATA_MAX;//�A�C�e���f�[�^�̍ő吔
	static OBJECT_Data m_ObjectData[];
};
#endif
