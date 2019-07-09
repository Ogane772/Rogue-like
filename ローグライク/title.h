#ifndef TITLE_H_
#define TITLE_H_
#include "C3DObj.h"

class CTitle : public C3DObj
{
public:
	static void Title_Initialize(void);
	static void Title_Finalize(void);
	static void Title_Update(void);
	static void Title_Draw(void);
private:
	static bool g_bIsFade;
	static int tFlag;
	static C2DObj *pTitle;
};
#endif