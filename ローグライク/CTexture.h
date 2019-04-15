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
		TEX_WHITE,
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
		TEX_RESULT,
		TEX_MESSAGE_WINDOW,
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
