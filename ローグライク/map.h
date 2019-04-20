#ifndef MAP_H_
#define MAP_H_
#include <d3dx9.h>
#include "CTexture.h"
//===================================
// �萔
//===================================
#define MAX_MAPWIDTH (100)
#define MAX_MAPHEIGHT (100)
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
		HAVEITEM,
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

	}MAP;

	static void Map_Initialize(void);
	// �e�N�X�`���ԍ��A���̒����A�c�̒����A���l�p���A�c�l�p��  ,�|�W�V����
	static void Map_Create(void);
	static void Map_Finalize(void);
	static void Map_Draw(void);

	static MAP Map_GetData(int z, int x) { return g_map[z][x]; }
	static void Map_SetData(int z, int x,int alpha) { g_map[z][x].alpha = alpha; }
private:
	//static int g_TexWood2;
	static MAP **g_map, *base_g_map;

	static int GroupWidth;
	static int GroupHeight;
};

#endif
