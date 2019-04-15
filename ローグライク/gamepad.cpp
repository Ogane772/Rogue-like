//=============================================================================
//
// �Q�[���p�b�h���͏��� [gamepad.cpp]
//
//=============================================================================
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <d3dx9.h>
#include <stdio.h>
#include "gamepad.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_SPEED 6

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static bool initialize(HINSTANCE hInstance);
static void finalize(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3D9 pD3d = NULL;
LPDIRECT3DDEVICE9 pDevice = NULL;
LPDIRECTINPUT8 pDinput = NULL;
LPDIRECTINPUTDEVICE8 pJoyDevice = NULL;

//=============================================================================
// �Q�[���p�b�h���͏����̏�����
//=============================================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	//�����񋓂����ꍇ�A���[�U�[�ɑI���E�m�F������
	//TCHAR szConfirm[MAX_PATH + 1];
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if (FAILED(pDinput->CreateDevice(pdidInstance->guidInstance,
		&pJoyDevice, NULL)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

//=============================================================================
// �Q�[���p�b�h���͏����̏I������
//=============================================================================
void Gamepad_Finalize(void)
{
	if (pDinput != NULL) {

		// DirectInput�I�u�W�F�N�g�̊J��
		pDinput->Release();
		pDinput = NULL;
	}
	if (pJoyDevice != NULL) {

		// DirectInput�I�u�W�F�N�g�̊J��
		pJoyDevice->Release();
		pJoyDevice = NULL;
	}
}

//BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext )
//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{

	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -MOVE_SPEED;
		diprg.lMax = MOVE_SPEED;

		if (FAILED(pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//=============================================================================
// �Q�[���p�b�h�̏�����
//=============================================================================
HRESULT Gamepad_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// �uDirectInput�v�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return E_FAIL;
	}
	//���p�\�ȃQ�[���R���g���[���[�̗񋓊֐������s
	if (FAILED(pDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
		NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (!pJoyDevice)
	{
		return E_FAIL;
	}
	// �f�o�C�X���W���C�X�e�B�b�N�ɐݒ�
	if (FAILED(pJoyDevice->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if (FAILED(pJoyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	//�A�v���P�[�V�����Ŏg�p����R���g���[���[�̃v���p�e�B��񋓂��Đݒ肷��
	if (FAILED(pJoyDevice->EnumObjects(EnumObjectsCallback,
		NULL, DIDFT_ALL)))
	{
		return E_FAIL;
	}
	// �f�o�C�X���u�擾�v����
	pJoyDevice->Acquire();

	return true;
}

LPDIRECTINPUTDEVICE8* JoyDevice_Get(void)
{
	return &pJoyDevice;
}

