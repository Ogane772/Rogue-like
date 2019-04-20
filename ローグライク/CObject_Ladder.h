#ifndef COBJECT_LADDER_H_
#define COBJECT_LADDER_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
#include "CObject.h"
//===================================
// 定数
//===================================
#define OBJECT_MAX (10)

class CObjectLadder : public CObject
{
public:
	//===================================
	// オブジェクト構造体
	//===================================
	CObjectLadder(int x, int z);
	~CObjectLadder();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str);
	void Damage(void);

private:
	int type;
	//int g_laddertex;
};
#endif
