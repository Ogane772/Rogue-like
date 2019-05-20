#pragma once
#ifndef COBJECT_YAKUSOU_H_
#define COBJECT_YAKUSOU_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
#include "CObject.h"
//===================================
// 定数
//===================================
\

class CObjectYakusou : public CObject
{
public:
	//===================================
	// オブジェクト構造体
	//===================================
	CObjectYakusou(int x, int z);
	~CObjectYakusou();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle);

private:
	int type;
	//int g_laddertex;
};
#endif
