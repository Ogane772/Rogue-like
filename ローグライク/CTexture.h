//////////////////////////////////////////////////
////
////	�e�N�X�`���N���X
////
//////////////////////////////////////////////////

#ifndef _CTEXTURE_H_
#define _CTEXTURE_H_

#include "CGameObj.h"

#define TEXTURE_MANAGE_MAX   (512)
#define TEXTURE_INVALID_INDEX (-1)

class CTexture : virtual public CGameObj
{
public:
	static int Texture_SetLoadFile(const char* pFileName, int width, int height);

	// �e�N�X�`���̓ǂݍ���
	// 
	// �߂�l:�ǂݍ��߂Ȃ�������
	// 

	static int Texture_Load(void);

	// �e�N�X�`���̎擾
	static LPDIRECT3DTEXTURE9 Texture_GetTexture(int index);

	// �e�N�X�`���̕��̎擾
	static int Texture_GetWidth(int index);

	// �e�N�X�`���̍����̎擾
	static int Texture_GetHeight(int index);

	// �e�N�X�`���̉��
	// 
	//	����: indices = ����������e�N�X�`���ԍ�
	//					���������z��̐擪�A�h���X
	//		  count   = ����������e�N�X�`���ԍ�
	//                  �̌�
	//
	static int Texture_Release(int indices[], int count);

	// �e�N�X�`���̑S���
	static void Texture_ReleaseAll(void);
private:
	//	�e�N�X�`���t�@�C�����Ǘ��p���R�[�h�\����
	typedef struct TextureFile_tag
	{
		char filename[TEXTURE_FILENAME_MAX];
		int width;
		int height;
	}TextureFile;

	static TextureFile g_Files[TEXTURE_MANAGE_MAX];

	static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_MANAGE_MAX];
};
#endif // !1
