#ifndef SAVELORD_H_
#define SAVELORD_H_
//����ϐ�
typedef struct {
	int danjyon_in;//�`����
	int floor_count;//���v�˔j�K�w��
	int game_clear;//�Q�[���N���A��
	int walk_count;//��������
	int item_count;//�A�C�e�����g�p������
	int death_count;//���ꂽ�K��
	int damage_count;//�^�������_���[�W
	int damage_on_count;//�󂯂����_���[�W
	int enemy_death_count;//�|�����G�̐�
	int wepon_chenge_count;//�����؂�ւ����K��
}SAVE_COUNT;
void Save(void);
void Load(void);
SAVE_COUNT* Get_SaveCount(void);
#endif