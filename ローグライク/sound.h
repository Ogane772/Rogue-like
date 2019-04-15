//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_
/*#define NOMINMAX
#include <windows.h>
#undef min
#undef max*/
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	TITLE_BGM,			// ゲームBGM
	CHUTO_BGM,
	MAIN_BGM,
	RESULT_BGM,
	START_SE,
	NORMALHIT_SE,
	BIGHIT_SE,
	POPCORN_SE,
	COASTER_SE,
	CUP_SE,
	FREEFALL_SE,
	KENSETU_SE,
	GAME_END_SE,
	GAMEOVER_SE,
	COASTER_ON_SE,
	ENTER_SE,
	FREE_WALL_UP_SE,
	HUSEN_SE,
	WHEEL_SE,
	BARUP_SE,
	CLEARBONUS_SE,
	MONSTERHIT_SE,
	OBJECTHIT_SE,
	SCOREHYOUJI_SE,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
