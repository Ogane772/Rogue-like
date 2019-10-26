#ifndef MAP_H_
#define MAP_H_
#include <d3dx9.h>
#include "CTexture.h"
#include "C3DObj.h"
//===================================
// �萔
//===================================
#define MAX_MAPWIDTH (100)
#define MAX_MAPHEIGHT (100)
#define MAX_MAP (3)	//�ŏ�K
#define MAP_FLOOR_CENTER (25)	// �t���A��S�̂̐^�񒆂Ɏ����Ă����߂̒���

#define MAP_AXIS_MAX (4)		// �e���̍ő�t���A��

//#define GroupWidth  (3)		// ���̒Z�a�O���[�v��
//#define GroupHeight (2)		// �c�̒Z�a�O���[�v��


//===================================
// ��
//===================================
class CMap : public CTexture
{
public:
	// ����ɏ���Ă���
	typedef enum {
		NOTHAVE,
		HAVEPLAYER,
		HAVEENEMY,
		HAVELADDER,
		HAVEITEM,
		HAVEWEPON,
		HAVETRAP,
		HAVEMAPNONE
	}HAVEMAP;

	// ��邱�� �}�b�v����� �v���C���[�̏����ʒu�����߂� �K�i(�ړI�n)�̍��W�����߂�
	//===================================
	// �}�b�v�\����
	//===================================
	typedef struct
	{
		D3DXVECTOR3 pos;	// �g������
		bool use;	// ����ɂȂ��Ă��邩 0 = ���ꖳ�� ,1 = �t���A, 2 = �ʘH, 3 = �t���A�̒ʘH�o����
		int type;	// ����̎��
		HAVEMAP have;	// ����ɉ������邩(���邩)
		bool Czwall;	// �ǂ����ꂽ��
		bool Cxwall;	// �ǂ����ꂽ��
		int Group;		// �Z�a�O���[�v
		int alpha; // �~�j�}�b�v�p�̃A���t�@�l����p
		bool Cxtopwall;	// �ʘH��ǂ����ꂽ��
		bool Cxbotwall;	// �ʘH���ǂ����ꂽ��
		bool CzRwall;	// �ʘH�E�ǂ����ꂽ��
		bool CzLwall;	// �ʘH���ǂ����ꂽ��
		int passageNum;	// �ʘH�ԍ�
		bool passagecheck; // ���̃t���A�ɒʘH�����邩

	}MAP;

	static void Map_Initialize(void);
	// �e�N�X�`���ԍ��A���̒����A�c�̒����A���l�p���A�c�l�p��  ,�|�W�V����
	static void Map_Create(void);
	static void Map_Create_B(void);			// �O���[�v�K���֌W�Ȃ��q�����Ă�O���[�v���m�ŒʘH������
	static void Map_CreateOneFloor(void);	// �ꕔ���f�o�b�O�p
	static void Map_Finalize(void);
	static void Map_Draw(void);
	static void MapPlayerSet(void);//�}�b�v�Ƀv���C���[�z�u����
	static void WorpPlayerSet(int z, int x);//���[�v���̃v���C���[�z�u����
	static MAP Map_GetData(int z, int x) { return g_map[z][x]; }
	static void Map_SetHaveData(int z, int x, HAVEMAP have) { g_map[z][x].have = have; }
	static void Map_SetData(int z, int x, int alpha) { g_map[z][x].alpha = alpha; }
	static void WorpEnemySet(C3DObj *enemy, int z, int x);//���[�v�G�l�~�[�z�u����
	static void MapItemPosSet(int item_type, int x, int z);//�w��ꏊ�ɃA�C�e���z�u
	static void MapPlayerPosSet(int mapz, int mapx, int oldz, int oldx);

	static void MapEnemySingleSet(void);
	static void MapEnemyPosSet(int mapz, int mapx, int oldz, int oldx);
private:
	//static int g_TexWood2;
	static void MapWallSet(void);//�}�b�v�ɕǔz�u����
	static void MapItemSet(void);//�}�b�v�ɃA�C�e���z�u����
	static void MapWeponSet(void);//�}�b�v�ɑ����z�u����
	static void MapTrapSet(void);//�}�b�v�Ƀ��i�z�u����
	static void MapEnemySet(void);//�}�b�v�ɃG�l�~�[�z�u����
	
	
	static void MapLadderSet(void);//�}�b�v�ɊK�i�z�u����

	static void MapdeletePassage(int passagenum);// �I�񂾒ʘH�i���o�[���폜

	static MAP **g_map, *base_g_map;

	static int GroupWidth;
	static int GroupHeight;
	static int DeletePassageNum;					// �폜����ʘH��

	typedef struct
	{
		bool use;
		int number;
	}DELETEPASSAGE;

	static DELETEPASSAGE *g_deletepassage;

};

#endif
