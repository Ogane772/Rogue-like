#ifndef REZULT_H_
#define REZULT_H_
#include "C3DObj.h"

class CRezult : public C3DObj
{
public:
	static void Rezult_Initialize(void);
	static void Rezult_Finalize(void);
	static void Rezult_Draw(void);
	static void Rezult_Update(void);
};
#endif
