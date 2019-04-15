#ifndef _DEBUG_FONT_H_
#define _DEBUG_FONT_H_



void DebugFont_Initialize(void);
void DebugFont_Finalize(void);
void DebugFont_Draw(void);
void DebugFont_Draw(int x, int y, const char* pFormat, ...);
#endif
