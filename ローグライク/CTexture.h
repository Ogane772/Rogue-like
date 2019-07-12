//////////////////////////////////////////////////
////
////	�e�N�X�`���N���X
////
//////////////////////////////////////////////////

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include "CGameObj.h"

#define TEXTURE_FILENAME_MAX (64)

class CTexture : virtual public CGameObj
{
public:
	CTexture();
	~CTexture();

	enum
	{
		TEX_TITLE,
		TEX_BLUE,
		TEX_WOOD,
		TEX_KUSA,
		TEX_BLOCK,
		TEX_LADDER,
		TEX_1F,
		TEX_2F,
		TEX_3F,
		TEX_4F,
		TEX_5F,
		TEX_SHADOW,
		TEX_HIT,
		TEX_HIT_WEEK,
		TEX_HIT_RESIST,
		TEX_RESULT,
		TEX_MESSAGE_WINDOW,
		TEX_PLAYER_ICON,
		TEX_ENEMY_ICON,
		TEX_LADDER_ICON,
		TEX_ITEM_ICON,
		TEX_HPGAGE_WAKU,
		TEX_HPGAGE_GREENBAR,
		TEX_HPGAGE_REDBAR,
		TEX_HPGAGE_POIZUNBAR,
		TEX_CURSOL,
		TEX_CURSOL_GRY,
		TEX_SWORD_ICON,
		TEX_BIGSWORD_ICON,
		TEX_REIPIA_ICON,
		TEX_RANCE_ICON,
		TEX_MEISU_ICON,
		TEX_AX_ICON,
		TEX_SHELD_ICON,
		TEX_RING_ICON,
		TEX_WIDTH_WINDOW,
		TEX_ITEM_YAKUSOU,
		TEX_WINDOW_MARU,
		TEX_PLAYER_YAJIRUSI,
		TEX_PLAYER_YAJIRUSI2,
		TEX_PLAYER_YAJIRUSI3,
		TEX_PLAYER_MEKAKUSI_ICON,
		TEX_PLAYER_POIZUN_ICON,
		TEX_PLAYER_SLEEP_ICON,
		TEX_PLAYER_CHARGE_ICON,
		TEX_PLAYER_MAHUJI_ICON,
		TEX_BLACK,
		TEX_TITLE_WINDOW,
		TEX_HPGAGE_ONAKA,
		TEX_KAKI_ICON,
		TEX_AISYOU_ICON,
		TEX_BUKI_MARU,
		TEX_TRAP_DAMAGE,
		TEX_TRAP_ICON,
		TEX_DAMAGETRAP_ICON,
		TEX_WARPTRAP_ICON,
		TEX_KUHUKUTRAP_ICON,
		TEX_SLEEPTRAP_ICON,
		TEX_POIZUNTRAP_ICON,
		TEX_KURAYAMITRAP_ICON,
		TEX_DONSOKUTRAP_ICON,
		TEX_JYOSOUTRAP_ICON,
		TEX_SABITRAP_ICON,
		TEX_MAX,
	};


	static int Texture_Load(void);	//	�e�N�X�`���ǂݍ���
	static void Texture_Release(void);	//	�e�N�X�`���j��
	static void Texture_Load(int index);	//	�w�肵���e�N�X�`���ǂݍ���
	static void Texture_Release(int index);	//	�w�肵���e�N�X�`���j��
	static LPDIRECT3DTEXTURE9 Texture_GetTexture(int index) { return m_pTextures[index]; }	//	�e�N�X�`���擾
	static char* Texture_GetFailName(int index) { return TEXTURE_FILES[index].filename; }
	static int Texture_GetWidth(int index) { return TEXTURE_FILES[index].width; }	// �e�N�X�`����
	static int Texture_GetHeight(int index) { return TEXTURE_FILES[index].height; }	//	�e�N�X�`������
protected:




private:
	//	�e�N�X�`���f�[�^�̍\����
	typedef struct TextureFile_tag
	{
		char filename[TEXTURE_FILENAME_MAX];	//	�t�@�C����
		int width;	//	��
		int height;	//	����
	}TextureFileData;
	static TextureFileData TEXTURE_FILES[];	//	�e�N�X�`���\���̐錾
	static int TEXTURE_MAX;	//	�e�N�X�`���\���̑���
	static LPDIRECT3DTEXTURE9 m_pTextures[];	//	�e�N�X�`���|�C���^
};
#endif // !1
