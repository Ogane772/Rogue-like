#ifndef SAVELORD_H_
#define SAVELORD_H_
//武器変数
typedef struct {
	int danjyon_in;//冒険回数
	int floor_count;//合計突破階層数
	int game_clear;//ゲームクリア数
	int walk_count;//歩いた回数
	int item_count;//アイテムを使用した回数
	int death_count;//やられた階数
	int damage_count;//与えた総ダメージ
	int damage_on_count;//受けた総ダメージ
	int enemy_death_count;//倒した敵の数
	int wepon_chenge_count;//武器を切り替えた階数
}SAVE_COUNT;
void Save(void);
void Load(void);
SAVE_COUNT* Get_SaveCount(void);
#endif