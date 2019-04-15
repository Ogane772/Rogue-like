#ifndef SCENE_H_
#define SCENE_H_

typedef enum 
{
	SCENE_INDEX_TITLE,
	SCENE_INDEX_STAGE,
	SCENE_INDEX_GAME,
	SCENE_INDEX_REZULT,
	SCENE_INDEX_MAX
}SCENE_INDEX;

void Scene_Initialize(SCENE_INDEX index);
void Scene_Finalize(void);
void Scene_Draw(void);
void Scene_Update(void);
void Scene_Check(void);

void Scene_Change(SCENE_INDEX index);
#endif
