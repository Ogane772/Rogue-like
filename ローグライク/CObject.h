#ifndef OBJECT_H_
#define OBJECT_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
//===================================
// íËêî
//===================================
#define OBJECT_MAX (10)

class CObject :virtual public C3DObj
{
public:
	typedef enum {
		TYPE_ALL = 0,

		TYPE_LADDER,//íÚéq
		OBJECTTYPE_NONE
	}OBJECTTYPE;

	CObject();
	CObject::CObject(int ObjectType);
	~CObject();
	void Object_Finalize(int Index);


	static void Create(int object_type, int x, int z);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Damage(void) = 0;

	static int Get_ObjectNum(int OrnamType) { return m_ObjectNum[OrnamType]; }
	static int Get_ObjectIndex(int EnemyType) { return m_ObjectNum[EnemyType] - 1; }
	int Get_Type(void) { return m_Type; }
	void ObjectDamage(void);
	static int Get_ObjectMaxNum(void) { return m_ORNAMENT_MAX; }
	static C3DObj *Get_Object(int index);
	bool Get_DrawCheck(void) { return alive; }
	static void CObject::DeleteAllObject(void);
	static C3DObj *Get_Map_Object(int Index);
protected:
	int m_ObjectIndex;

	static C3DObj *Get_AllObject(int index);

private:
	int m_Type;			// éÌóﬁ
	static int m_ObjectNum[TYPE_MAX];

	static int m_ORNAMENT_MAX;
};
#endif
