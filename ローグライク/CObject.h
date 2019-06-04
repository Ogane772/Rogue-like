#ifndef OBJECT_H_
#define OBJECT_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
//===================================
// 定数
//===================================
#define OBJECT_MAXTIPS (99)//説明文の最大長さ
class CObject :virtual public C3DObj
{
public:
	typedef struct {
		int object_type;
		char object_name[MAX_NAME];
		char object_effect[OBJECT_MAXTIPS];//オブジェクト使用文
		char object_tips[OBJECT_MAXTIPS];//オブジェクト説明文
		int first_floor;//出現開始フロア
		int end_floor;//出現終わりフロア
		int itemchance;//アイテム出現率
	}OBJECT_Data;
	typedef enum {
		TYPE_ALL = 0,

		TYPE_LADDER,//梯子
		TYPE_YAKUSOU,//薬草
		TYPE_ZAXTUSOU,//雑草
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
	static int m_ITEMDATA_MAX;//アイテムデータの最大数
	static OBJECT_Data m_ObjectData[];
};
#endif
