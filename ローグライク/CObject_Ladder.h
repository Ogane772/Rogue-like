#ifndef COBJECT_LADDER_H_
#define COBJECT_LADDER_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
#include "CObject.h"
//===================================
// �萔
//===================================

class CObjectLadder : public CObject
{
public:
	//===================================
	// �I�u�W�F�N�g�\����
	//===================================
	CObjectLadder(int x, int z);
	~CObjectLadder();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle, int week_type);

private:
	int type;
	//int g_laddertex;
};
#endif
