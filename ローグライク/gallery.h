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
};

#endif