#ifndef TITLEMENU_H_
#define TITLEMENU_H_
#include "C3DObj.h"

class CTitleMenu : public C3DObj
{
public:
	static void TitleMenu_Initialize(void);
	static void TitleMenu_Finalize(void);
	static void TitleMenu_Update(void);
	static void TitleMenu_Draw(void);
private:
	static bool g_bIsFade;
	static C2DObj *pTitleMenu;
	static int cursor;
	static int save_delete_cursor;
	static bool save_delete;
	static bool save_delete_on;//セーブデータ消去時true

	static void OK_BUTTON_SELECT(void);
	static void UP_BUTTON_SELECT(void);
	static void DOWN_BUTTON_SELECT(void);
};

#endif