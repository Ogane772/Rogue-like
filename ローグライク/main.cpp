//=============================================================================
//
// �Q�[�����[�v�@[main.cpp]
// Author : WATARU SODA
// Date	  : 2018/09/20
//
//=============================================================================

//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================
#define NOMINMAX
/*#include <Windows.h>
#undef min
#undef max*/

#include <d3dx9.h>
#include "common.h"
#include "scene.h"
#include "input.h"
#include "system_timer.h"
#include "debug_font.h"
#include "gamepad.h"
#include "CObject.h"
#include "CTrap.h"
#include "CWepon.h"
#include "fade.h"
#include "sound.h"
#include "C2DObj.h"

#include "C3DObj.h"
#include "Cplayer.h"
#include "CLight.h"
#include "userInterface.h"
#include "CEnemy.h"
#include "CCamera.h"
#include "save_load.h"
//=============================================================================
//	�萔��`
//=============================================================================
#define CLASS_NAME		("GameWindow")
#define WINDOW_CAPTION	("�^�C�j�[���r�����X")
#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX - WS_THICKFRAME)	// �E�B���h�E�X�^�C��


//	#define��()��t���Ƃ�


//=============================================================================
//	�v���g�^�C�v�錾
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�@�Q�[���̏������֐�
static bool Begin(HINSTANCE hInstance, HWND hWnd);
//�@�Q�[���̍X�V�֐�
static void Update(void);
//�@�Q�[���̕`��֐�
static void Draw(void);
//�@�Q�[���̏C������
static void End(void);

//�@static�t����

//=============================================================================
//	�O���[�o���ϐ��錾
//=============================================================================

static  HWND hWnd;
#if defined(_DEBUG) || defined(DEBUG)
static int g_FrameCount = 0;	// �t���[���J�E���^�[
static int g_BaseFrame = 0;		// FPS�v���̊�ƂȂ�t���[���J�E���g
static double g_BaseTime = 0;	// FPS�v���̊�ƂȂ鎞��
static float g_FPS = 0.0f;		// FPS
#endif // _DEBUG || DEBUG
								//static double g_
								// LPDIRECT3DTEXTURE9 g_pTexture;	//	�e�N�X�`���̃A�h���X��
								//�@static�t����
static double g_StaticFrameTime = 0.0f; // �t���[���Œ�p�v������
static BYTE KeyTbl[256];
//=============================================================================
//	���C��
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpCmdline, int nCmdShow)
{
	// �g�p���Ȃ��ꎞ�ϐ��𖾎�
	UNREFERENCED_PARAMETER(hprevInstance);
	UNREFERENCED_PARAMETER(lpCmdline);

	// �E�B���h�E�N���X�\���̂̐ݒ�
	// �E�B���h�E�N���X�\���̂̐ݒ�
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;                          // �E�B���h�E�v���V�[�W���̎w��
	wc.lpszClassName = CLASS_NAME;                     // �N���X���̐ݒ�
	wc.hInstance = hInstance;                          // �C���X�^���X�n���h���̎w��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // �}�E�X�J�[�\�����w��
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // �E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
													   //�A�C�R���w��
	wc.hIconSm = NULL;
	// �N���X�o�^
	RegisterClassEx(&wc);


	// �E�B���h�E�X�^�C��
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// ��{��`���W
	RECT window_rect = { 0, 0, (LONG)WINDOW_WIDTH, (LONG)WINDOW_HEIGHT };

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɐV���ȋ�`���W���v�Z
	AdjustWindowRect(&window_rect, window_style, FALSE);
	int window_width = 0;
	int window_height = 0;
	// �V����Window�̋�`���W���畝�ƍ������Z�o
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;

	int screen_width = 0;
	int screen_hight = 0;
	// �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
	//�@window���W �v���C�}�����j�^�[�̉�ʉ𑜓x�擾
	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_hight = GetSystemMetrics(SM_CYSCREEN);

	// �f�X�N�g�b�v�̐^�񒆂ɃE�B���h�E�����������悤�ɍ��W���v�Z
	// ��������������A�f�X�N�g�b�v���E�B���h�E���傫���ꍇ�͍���ɕ\��
	int window_x = (((screen_width - window_width) / 2 > (0)) ? ((screen_width - window_width) / 2) : (0));  //�@max(a,b) a>b�Ȃ�a a<b�Ȃ�b
	int window_y = (screen_hight / 2) - (window_height / 2);

	// �E�B���h�E�̐���
	HWND hWnd = CreateWindow(
		CLASS_NAME,     // �E�B���h�E�N���X
		WINDOW_CAPTION, // �E�B���h�E�e�L�X�g
		window_style,   // �E�B���h�E�X�^�C��
		-8,       // �E�B���h�E���Wx
		window_y,       // �E�B���h�E���Wy
		window_width,   // �E�B���h�E�̕�
		window_height,  // �E�B���h�E�̍���
		NULL,           // �e�E�B���h�E�n���h��
		NULL,           // ���j���[�n���h��
		hInstance,      // �C���X�^���X�n���h��
		NULL            // �ǉ��̃A�v���P�[�V�����f�[�^
	);

	if (hWnd == NULL) {
		// �E�B���h�E�n���h�������炩�̗��R�Ő����o���Ȃ�����
		return -1;
	}

	// �w��̃E�B���h�E�n���h���̃E�B���h�E���w��̕��@�ŕ\��
	ShowWindow(hWnd, nCmdShow);


	// �Q�[���̏������@(Direct3D�̏�����)
	if (!Begin(hInstance, hWnd))
	{
		return -1;
	}



	//	�Q�[�����[�v
	MSG msg = {};
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	//	�Q�[�����[�v�@�ߒl�F���b�Z�[�W����������0�ȊO�@GetMessage�Ɠ����@PM_REMOVE�͋C�ɂ��Ȃ�
		{
			TranslateMessage(&msg);		//	�L�[���̓��b�Z�[�W�������H	���ԇ@
			DispatchMessage(&msg);		//�@���b�Z�[�W���E�B���h�E�v���V�[�W���ɑ���	���ԇA
		}
		else {
			// ���݂̃V�X�e�����Ԃ��擾
			double time = SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / 60.0) {
				// 1 / 60 �b�o���Ă��Ȃ����������
				Sleep(0);
			}
			else {
				// �t���[���Œ�p�̌v�����Ԃ��X�V����
				g_StaticFrameTime = time;

				// �Q�[���̍X�V
				Update();
				// �Q�[���̕`��
				Draw();
			}
		}
	} while (msg.message != WM_QUIT);

	//�@�Q�[���̏I������(Direct3D�̏I������)
	End();

	return (int)msg.wParam;
}

// �E�B���h�E�v���V�[�W��(�R�[���o�b�N�֐�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSE���b�Z�[�W�̑��M
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd); // �w��̃E�B���h�E��WM_DESTROY���b�Z�[�W�𑗂�
		}
		return 0;	// DefWindowProc�֐��Ƀ��b�Z�[�W�𗬂����I�����邱�Ƃɂ���ĉ����Ȃ��������Ƃɂ���

	case WM_DESTROY:	// �E�B���h�E�̔j�����b�Z�[�W
		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�̑��M
		return 0;
	};
	// �ʏ탁�b�Z�[�W�����͂��̊֐��ɔC����
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//�@�Q�[���̏������֐�
bool Begin(HINSTANCE hInstance, HWND hWnd)
{
	//�@Direct3D�̏�����
	if (!CGameObj::Device_Initialize(hWnd))
	{
		return false;
	}
	/*
	if (!InitSound(hWnd))
	{
		return false;
	}*/
	// �L�[�{�[�h�̏�����
	if (!Keyboard_Initialize(hInstance, hWnd))
	{
		return false;
	}
	//�Q�[���p�b�h�̏�����
	if (!Gamepad_Initialize(hInstance, hWnd))
	{
		return false;
	}


	// �X�v���C�g�̏�����
	C2DObj::Sprite_Initialize();
	// �e�N�X�`���̏�����
	C2DObj::Texture_Load();

	// �T�E���h�̏�����
	InitSound(hWnd);

	// �t�F�[�h�̏�����
	Fade_Initialize();

	//�Z�[�u�f�[�^�̃��[�h
	Load();

	//	�V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();
	g_BaseFrame = 0;
	g_BaseTime = 0;

	// �V�X�e���^�C�}�[�̋N��
	SystemTimer_Start();

#if defined(_DEBUG) || defined(DEBUG)
	// �f�o�b�N�t�H���g���W���[���̏�����
	DebugFont_Initialize();
	//DebugPrimitive_Initialize();
	// FPS�v���p�ϐ��Q�̏�����
	g_FrameCount = g_BaseFrame = 0;
	g_BaseTime = SystemTimer_GetTime();
	g_FPS = 0.0f;
#endif // _DEBUG || DEBUG

	// �t���[���Œ�p�v������
	//g_StaticFrameTime = SystemTimer_GetTime();

	//CSV�̓ǂݍ���
	CObject::ObjectDataInit();
	CWepon::WeponDataInit();
 	CEnemy::EnemyDataInit();
	CTrap::TrapDataInit();

	CUserinterface::UI_Initialize();
											//CCamera::Camera_Create();				//	�J��������					//	�n�ʐ���																									//CMeshField_Cylinder::MeshField_Cylinder_Create(CTexture::TEX_FLORR, 10.0f, SYLINDERSIZE, 20, 1,true);	//	���J�x����


	//�@�V�[���̏�����
	Scene_Initialize(SCENE_INDEX_TITLE);

	return true;
}
//�@�Q�[���̍X�V�֐�
void Update(void)
{
	// �L�[�{�[�h�̍X�V
	Keyboard_Update();
	// �V�[���̍X�V
	Scene_Update();
	// �t�F�[�h�̍X�V
	Fade_Update();


	//�@�v��
#if defined(_DEBUG) || defined(DEBUG)

	// ���݂̃V�X�e�����Ԃ��擾
	double time = SystemTimer_GetTime();

	// �O���FPS�v�����Ԃ���K�莞�Ԍo���Ă�����FPS�v��
	if (time - g_BaseTime > 1.0)
	{
	// FPS�v�Z(�O�񂩂�̌o�߃t���[�������o�ߎ���)
	g_FPS = (float)((g_FrameCount - g_BaseFrame )/ (time - g_BaseTime));

	// FPS�v���̊�ƂȂ�ϐ��̍X�V
	g_BaseTime = time;
	g_BaseFrame = g_FrameCount;

	}

	// �t���[���J�E���g�̍X�V
	g_FrameCount++;
#endif // _DEBUG || DEBUG
}
//�@�Q�[���̕`��֐�
void Draw(void)
{

	if (!CGameObj::m_pD3DDevice)
	{
		return;
	}

	// ��ʂ̃N���A
	CGameObj::m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR(/*0xffafa690*/0xff454341), 1.0f, 0);		//��ʃN���A
																																	/*
																																	D3DCLEAR_TARGET		�F�̃N���A
																																	D3DCLEAR_ZBUFFER	�X�e���V���N���A
																																	D3DCOLOR_RGBA		���F�ŃN���A���邩

																																	��l�����@0.0f����Ԏ�O1.0f����ԉ�
																																	*/

																																	//�@�`�揈���J�n
	CGameObj::m_pD3DDevice->BeginScene();
#if defined(_DEBUG) || defined(DEBUG)
	//DebugPrimitive_BatchBegin();
#endif // _DEBUG || DEBUG

	// �V�[���̕`��
	Scene_Draw();
	// �t�F�[�h�̕`��
	Fade_Draw();

#if defined(_DEBUG) || defined(DEBUG)
	// FPS�̃f�o�b�N�\��
	DebugFont_Draw(8, 8, "FPS : %.2f", g_FPS);
	//DebugPrimitive_BatchRun();
#endif // _DEBUG || DEBUG



	//�@�`�揈���I��
	CGameObj::m_pD3DDevice->EndScene();
	// �o�b�N�o�b�t�@���t���b�v�i�^�C�~���O��D3DPRESENT_PARAMETERS�̐ݒ�ɂ��j
	CGameObj::m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	Scene_Check();		//	�V�[���̃`�F�b�N
}
//�@�Q�[���̏C������
void End(void)
{
	//DebugPrimitive_Fainalize();
#if defined(_DEBUG) || defined(DEBUG)
	// �f�o�b�N�t�H���g���W���[���̏I������
	//DebugPrimitive_Fainalize();
	//DebugFont_Finalize();
#endif // _DEBUG || DEBUG

	// �V�[���I������
	Scene_Finalize();

	CUserinterface::UI_Finalize();

	C3DObj::DeleteAll();			//	3D�I�u�W�F�N�g�S����
	CGameObj::DeleteAll2D();			//	2D�I�u�W�F�N�g�S����
	C3DObj::Model_Finalize();
	// �T�E���h�I������
	UninitSound();

	// �e�N�X�`���̉��
	//Texture_Release();
	C2DObj::Texture_Release();

	// �X�v���C�g�̊J��
	//Sprite_Finalize();
	C2DObj::Sprite_Finalize();
	DebugFont_Finalize();
	// �L�[�{�[�h�I������
	Keyboard_Finalize();
	//�Q�[���p�b�h�I������
	Gamepad_Finalize();
	// �f�o�C�X�̊J��
	CGameObj::Device_Finalize();
	_CrtDumpMemoryLeaks();

}