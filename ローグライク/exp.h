#ifndef EXP_H_
#define EXP_H_
enum Effect
{
	HIT,//当たり判定候補
};
void Exp_Init(void);
void Exp_Update(void);
void Exp_Draw(void);
void Exp_Set(int exp_number, float x, float y, float z, float size, float rotation);//再生したいエフェクトをセット
void Exp_Create(float x, float y, float z, float size, float rotation, int texture_index, int anime_pattan, int anime_time, int one_width, int one_height, int width_patan);
void Effect_Destroy(int i);
bool Effect_IsEnable(int i);
#endif  //EXP_H_