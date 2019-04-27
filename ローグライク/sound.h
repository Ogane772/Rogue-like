//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_
/*#define NOMINMAX
#include <windows.h>
#undef min
#undef max*/
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	LVUP_SE,
	PLAYERDAMAGE_SE,
	ENEMYDAMAGE_SE,
	FLOOR_SE,//��q���������
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
