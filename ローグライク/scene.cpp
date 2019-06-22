#include "scene.h"
#include "title.h"
#include "game.h"
#include "stage.h"
#include "rezult.h"
#include "title_menu.h"
#include "gallery.h"

static SCENE_INDEX g_SceneIndex;
static SCENE_INDEX g_SceneNextIndex;

typedef void(*SceneFunc)(void);
static const SceneFunc g_Initialize[] =
{
	 Title_Initialize,
	 TitleMenu_Initialize,
	 CGallery::Gallery_Initialize,
	 CStage::Stage_Initialize,
	 Game_Initialize,
	 Rezult_Initialize,
};

static const SceneFunc g_Finalize[] =
{
	Title_Finalize,
	TitleMenu_Finalize,
	CGallery::Gallery_Finalize,
	CStage::Stage_Finalize,
	Game_Finalize,
	Rezult_Finalize,
};

static const SceneFunc g_Update[] =
{
	Title_Update,
	TitleMenu_Update,
	CGallery::Gallery_Update,
	CStage::Stage_Update,
	Game_Update,
	Rezult_Update,
};

static const SceneFunc g_Draw[] =
{
	Title_Draw,
	TitleMenu_Draw,
	CGallery::Gallery_Draw,
	CStage::Stage_Draw,
	Game_Draw,
	Rezult_Draw,
};




void Scene_Initialize(SCENE_INDEX index)
{
	g_SceneIndex = g_SceneNextIndex = index;

	g_Initialize[g_SceneIndex]();

}
void Scene_Finalize(void)
{
	g_Finalize[g_SceneIndex]();
}
void Scene_Draw(void)
{
	g_Draw[g_SceneIndex]();
}
void Scene_Update(void)
{
	g_Update[g_SceneIndex]();
}
void Scene_Check(void)
{
	if (g_SceneIndex != g_SceneNextIndex)
	{
		Scene_Finalize();
		Scene_Initialize(g_SceneNextIndex);
	}
}

void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}