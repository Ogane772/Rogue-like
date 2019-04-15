//////////////////////////////////////////////////
////
////	�e�N�X�`���N���X
////
//////////////////////////////////////////////////

//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================

#include "CTexture.h"

//=============================================================================
//	�ÓI�ϐ�
//=============================================================================

CTexture::TextureFile CTexture::g_Files[TEXTURE_MANAGE_MAX];
LPDIRECT3DTEXTURE9 CTexture::g_pTextures[TEXTURE_MANAGE_MAX];

// �ǂݍ��ރe�N�X�`���t�@�C���̎��O�\�������
// 
// ����:pFileName = �t�@�C����
//          width = �e�N�X�`����
//		   height = ���e�N�X�`���̍���
// �߂�l:�\��(�e�N�X�`���Ǘ�)�ԍ�
// 

int CTexture::Texture_SetLoadFile(const char* pFileName, int width, int height)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		// �l�Y�~�Ԃ��R�[�h
		if (g_Files[i].filename[0]) continue;


		// �g���ĂȂ��R�[�h
		strcpy(g_Files[i].filename, pFileName);
		g_Files[i].width = width;
		g_Files[i].height = height;



		return i;
	}

	return TEXTURE_INVALID_INDEX;
}


// �e�N�X�`���̓ǂݍ���
// 
// �߂�l:�ǂݍ��߂Ȃ�������
// 

int CTexture::Texture_Load(void)
{
	int failed_count = 0;
	HRESULT hr;

	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{
		// �l�Y�~�Ԃ��R�[�h
		if (!g_Files[i].filename[0]) continue;

		// �f�[�^�x�[�X�ɋL�ڂ�������

		if (g_pTextures[i]) continue;

		if (!m_pD3DDevice)
		{
			failed_count++;
			continue;
		}



		hr = D3DXCreateTextureFromFile(m_pD3DDevice, g_Files[i].filename, &g_pTextures[i]);

		if (FAILED(hr))
		{
			failed_count++;
			continue;
		}

	}
	//	���S�ɓǂݍ��߂�
	return failed_count;
}

// �e�N�X�`���̎擾
LPDIRECT3DTEXTURE9 CTexture::Texture_GetTexture(int index)
{
	return g_pTextures[index];
}
// �e�N�X�`���̕��̎擾
int CTexture::Texture_GetWidth(int index)
{
	return g_Files[index].width;
}

// �e�N�X�`���̍����̎擾
int CTexture::Texture_GetHeight(int index)
{
	return g_Files[index].height;
}

// �e�N�X�`���̉��
// 
//	����: indices = ����������e�N�X�`���ԍ�
//					���������z��̐擪�A�h���X
//		  count   = ����������e�N�X�`���ԍ�
//                  �̌�
//
int CTexture::Texture_Release(int indices[], int count)
{
	int unrelease_count = 0;

	for (int i = 0; i < count; i++)
	{
		// SAFERELEASE�ł��\
		if (g_pTextures[indices[i]])
		{
			g_pTextures[indices[i]]->Release();
			g_pTextures[indices[i]] = NULL;

		}
		else
		{
			unrelease_count++;
		}
		g_Files[indices[i]].filename[0] = 0; // \0�Ɠ��`
	}

	return unrelease_count;
}

// �e�N�X�`���̑S���
void CTexture::Texture_ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MANAGE_MAX; i++)
	{// SAFERELEASE
		if (g_pTextures[i])
		{
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
		g_Files[i].filename[0] = 0; // \0�Ɠ��`
	}

}













