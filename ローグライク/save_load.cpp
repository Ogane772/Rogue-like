#include "save_load.h"
#include <stdio.h>
#define	FILENAME "savefile.bin"	// 保存ファイル名

SAVE_COUNT save;

void Save(void)
{
	FILE *pFile;

	errno_t error;

	error = fopen_s(&pFile, FILENAME, "wb");
	// ファイルオープン
	if (error != 0)
	{
	}
	else
	{
		fwrite((&save.damage_count), sizeof(save.damage_count), 1, pFile);
		fwrite((&save.floor_count), sizeof(save.floor_count), 1, pFile);
		fwrite((&save.damage_on_count), sizeof(save.damage_on_count), 1, pFile);
		fwrite((&save.danjyon_in), sizeof(save.danjyon_in), 1, pFile);
		fwrite((&save.death_count), sizeof(save.death_count), 1, pFile);
		fwrite((&save.game_clear), sizeof(save.game_clear), 1, pFile);
		fwrite((&save.item_count), sizeof(save.item_count), 1, pFile);
		fwrite((&save.walk_count), sizeof(save.walk_count), 1, pFile);
		fwrite((&save.wepon_chenge_count), sizeof(save.wepon_chenge_count), 1, pFile);
		fclose(pFile);
	}
}

void Load(void)
{
	FILE *pFile;

	errno_t error;

	error = fopen_s(&pFile, FILENAME, "wb");
	// ファイルオープン
	if (error != 0)
	{
	}
	else
	{
		fread((&save.damage_count), sizeof(save.damage_count), 1, pFile);
		fread((&save.floor_count), sizeof(save.floor_count), 1, pFile);
		fread((&save.damage_on_count), sizeof(save.damage_on_count), 1, pFile);
		fread((&save.danjyon_in), sizeof(save.danjyon_in), 1, pFile);
		fread((&save.death_count), sizeof(save.death_count), 1, pFile);
		fread((&save.game_clear), sizeof(save.game_clear), 1, pFile);
		fread((&save.item_count), sizeof(save.item_count), 1, pFile);
		fread((&save.walk_count), sizeof(save.walk_count), 1, pFile);
		fread((&save.wepon_chenge_count), sizeof(save.wepon_chenge_count), 1, pFile);
		fclose(pFile);
	}
}

SAVE_COUNT* Get_SaveCount(void)
{
	return &save;
}