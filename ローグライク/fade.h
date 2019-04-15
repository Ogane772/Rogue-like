#ifndef _FADE_H_
#define _FADE_H_


void Fade_Initialize(void);
void Fade_Finalize(void);
void Fade_Update(void);
void Fade_Draw(void);

void Fade_Start(bool bOut, int frame, int r, int g, int b);
bool Fade_IsFade(void);


#endif

