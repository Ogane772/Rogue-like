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
	 CTitle::Title_Initialize,
	 CTitleMenu::TitleMenu_Initialize,
	 CGallery::Gallery_Initialize,
	 CStage::Stage_Initialize,
	 Game_Initialize,
	 CRezult::Rezult_Initialize,
};

static const SceneFunc g_Finalize[] =
{
	CTitle::Title_Finalize,
	CTitleMenu::TitleMenu_Finalize,
	CGallery::Gallery_Finalize,
	CStage::Stage_Finalize,
	Game_Finalize,
	CRezult::Rezult_Finalize,
};

static const SceneFunc g_Update[] =
{
	CTitle::Title_Update,
	CTitleMenu::TitleMenu_Update,
	CGallery::Gallery_Update,
	CStage::Stage_Update,
	Game_Update,
	CRezult::Rezult_Update,
};

static const SceneFunc g_Draw[] =
{
	CTitle::Title_Draw,
	CTitleMenu::TitleMenu_Draw,
	CGallery::Gallery_Draw,
	CStage::Stage_Draw,
	Game_Draw,
	CRezult::Rezult_Draw,
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