#ifndef GALLERY_H_
#define GALLERY_H_
#include "C3DObj.h"
#define GALLERY_MODELMAX (10)

class CGallery : public C3DObj
{
public:
	static NormalModelData Gallery_model[GALLERY_MODELMAX];

	static void Gallery_Initialize(void);
	static void Gallery_CameraInitialize(void);
	static void Gallery_ModelInitialize(void);
	static void Gallery_Finalize(void);
	static void Gallery_Update(void);
	static void Gallery_Draw(void);
	static void Gallery_CameraDraw(void);
	static void Gallery_ModelDraw(void);
	static void Tips_Draw(void);
	static void Gallery_JIXTUSEKI_Draw(void);
private:
	static bool g_bIsFade;
	static int tFlag;
	static C2DObj *pGallery;
	static int cursor;
	static bool tipsflag;//データ集開いてるときtrue
	static bool galleryflag;//ギャラリー開いてるときtrue

	static void OK_BUTTON_SELECT(void);
	static void UP_BUTTON_SELECT(void);
	static void DOWN_BUTTON_SELECT(void);
};

#endif