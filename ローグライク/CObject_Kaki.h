#pragma once
#ifndef COBJECT_KAKI_H_
#define COBJECT_KAKI_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
#include "CObject.h"
//===================================
// 定数
//===================================

class CObjectKaki : public CObject
{
public:
	//===================================
	// オブジェクト構造体
	//===================================
	CObjectKaki(int x, int z, int object_type);
	~CObjectKaki();

	void Initialize(int x, int z, int object_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

private:
	int type;
};
#endif
