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
#define TEXT_Y (55)//�e�L�X�g��Y���̕�
#define ENEMY_DESTROY_TEXT (20) //�@�G���S���̓����Ȃ��e�L�X�g�\������
#define UI_Y (30)	//���UI�\���ʒu
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
		ENEMY,		
		CHARATYPENONE
	}CHARATYPE;

	typedef enum {
		REGULARATTACK,	// �ʏ�U��
		GOLADDER,		// ��q��i��(���̊K��)
		NOTGOLADDER,	// ��q��i�܂Ȃ�(���̊K��)
		DESTROY,		// �|�ꂽ
		ACTTYPENONE,
		ITEM,			// �A�C�e���ɐG�ꂽ�Ƃ�
		NOITEM,			// �A�C�e�������ĂȂ��Ƃ�
		ITEM_EFFECT,	// �A�C�e���g�p��
		ITEM_ASIMOTO,	// �A�C�e���u�����Ƃ�
		NOITEM_ASIMOTO,	// �A�C�e�����u���Ȃ��Ƃ�
		WEPON_GET,	// ��������ɓ��ꂽ��
		WEPON_W_GETCHENGE,	// ��������ɓ��ꂽ��(�Ў聨����j
		WEPON_NORMAL_GETCHENGE,	// ��������ɓ��ꂽ���i���聨�Ў�j
		SHELD_W_GET,	// ��������ɓ��ꂽ���i����̎����Q�b�g�j
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
		int type;				// ���
		int purasu_number;		// ������+�l
		int attribute_type;			// �ϐ��^�C�v
		D3DXVECTOR2 pos;
		
		char player_name[MAX_NAME];
		char name[MAX_NAME];				// �Ώۂ̃G�l�~�[�̖��O
		char item_log[MAX_NAME];			// �A�C�e�����ʂ̐���
		bool alive;
		bool lv_up;							//���x���A�b�v������true
	}TEXT;
	//�ϐ��̌���
	enum ATTRIBUTE_TYPE{
		WEEK_TYPE,
		NORMAL_TYPE,
		RESIST_TYPE,
	};
	//��ŉ��ǂ��Ȃ��Ǝg�����ɂȂ�Ȃ�
	static void UI_Initialize(void);
	static void UI_Finalize(void);
	static void UI_Update(void);
	static void UI_Draw(void);

	static void UI_TextDraw(int x, int y, D3DCOLOR color, const char* text, ...);
	static void UI_TextDraw2(int x, int y, D3DCOLOR color, const char* text, ...);

	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type);//�A�C�e���p
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, int type, int purasu_number);//�����p
	static void UI_TextCreate(int week_type, CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);
	static void UI_TextCreate(CHARATYPE chara, ACTTYPE act, CHARATYPE hitchara, int damage, int type);

	static void UI_Delete(void);
private:
	static void CommandWindow_Draw(void);
	static void ItemWindow_Draw(void);
	static void RightWindow_Draw(void);//�E�̑������\��
	static void SetItemWindow(void);//�����A�C�e���\��
	static void SetItemOnWindow(void);//�����A�C�e���I���������̃E�B���h�E
	static void SetItemTips(void);//�����A�C�e��������
	static void StatusWindow_Draw(void);
	static void UI_UpDraw(void);
	static void Map_Draw(void);
	static void UI_TextDraw(void);
	// LPD3DXFONT = �t�H���g�̌^
	static LPD3DXFONT g_pD3DXFont;

	static TEXT g_text;
	static int g_Textbackground;
	static int g_TextFramecount;
};
#endif
