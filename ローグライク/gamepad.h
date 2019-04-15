//=============================================================================
//
// �Q�[���p�b�h���͏��� [gamepad.h]
//
//=============================================================================
#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#define NOMINMAX
#include <Windows.h>
#undef min
#undef max
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

HRESULT Gamepad_Initialize(HINSTANCE hInstance, HWND hWnd);
void Gamepad_Finalize(void);
LPDIRECTINPUTDEVICE8* JoyDevice_Get(void);//�R���g���[���[�擾����Ԃ�
#endif