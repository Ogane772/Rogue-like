#ifndef STAGE_H_
#define STAGE_H_
#include "CGameObj.h"
//===================================
// 定数
//===================================
#define FRAME_MAX (100)	   // 階層表示ふえーむ
#define STAGE_LEVELMAX (3) // ステージの最上階
//後で改良しないと使い物にならない
class CStage :public CGameObj
{
public:
	static int Level;
	static void Stage_Initialize(void);
	static void Stage_Finalize(void);
	static void Stage_Draw(void);
	static void Stage_Update(void);

	static void Stage_SetLevel(int level) { Level = level; }
	static int Stage_GetLevel(void) { return Level; }
private:
	static int g_TextureIndex;
	static int displayframe;
};
#endif
