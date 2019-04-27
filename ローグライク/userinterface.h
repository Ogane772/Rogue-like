#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_
#include <d3dx9.h>
#include "CGameObj.h"
#include "C2DObj.h"
//===================================
// �萔
//===================================
#define TEXT_FRAMEMAX (100)	// �e�L�X�g�\���t���[����
#define TEXT_POSX (150)		// �e�L�X�g�\��X���W
#define TEXT_POSY (520)		// �e�L�X�g�\��Y���W
#define TEXT_ADD_POSY (50)		// �e�L�X�g�\��Y���W
#define MAX_NAME (30)
#define ENEMY_DESTROY_TEXT (20) //�@�G���S���̓����Ȃ��e�L�X�g�\������
//===================================
// �e�L�X�g��
//===================================
class CUserinterface : public C2DObj
{
public:

	typedef enum {
		SMALEMAP,		// �~�j�}�b�v(��)
		PLAYERCHARA,	// �v���C���[

		// �G�L����
		SRAIM,			// �X���C��
		CHARATYPENONE
	}CHARATYPE;

	typedef enum {
		REGULARATTACK,	// �ʏ�U��
		GOLADDER,		// ��q��i��(���̊K��)
		NOTGOLADDER,	// ��q��i�܂Ȃ�(���̊K��)
		DESTROY,		// �|�ꂽ
		ACTTYPENONE
	}ACTTYPE;
	//===================================
	// �\����
	//===================================
	typedef struct {
		CHARATYPE chara;
		CHARATYPE hitchara;
		ACTTYPE act;
		int damage;
		int Age;

		int TextCreateFrame;	// �a���t���[��
		int TextCountFrame;		// �\�������t���[��
		int exp;				// �l���o���l
		int gold;				// �l����
		D3DXVECTOR2 pos;
		
		char player_name[MAX_NAME];
		char name[MAX_NAME];				// �Ώۂ̃G�l�~�[�̖��O
		bool alive;
		bool lv_up;							//���x���A�b�v������true
	}TEXT;

	//��ŉ��ǂ��Ȃ��Ǝg�����ɂȂ�Ȃ�
	static void UI_Initialize(void);
	static void UI_Finalize(void);
	static void UI_Update(void);
	static void UI_Draw(void);

	static void UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...);
	static void UI_TextDraw2(int x, int y, D3DCOLOR color, const char* text, ...);

	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage);

	static void UI_Delete(void);
private:
	// LPD3DXFONT = �t�H���g�̌^
	static LPD3DXFONT g_pD3DXFont;

	static TEXT g_text;
	static int g_Textbackground;
	static int g_TextFramecount;
};
#endif
