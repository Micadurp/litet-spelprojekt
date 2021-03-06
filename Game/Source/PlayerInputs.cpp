#include "PlayerInputs.h"

PlayerInputs::PlayerInputs()
{
	m_keyboard = nullptr;
	m_mouse = nullptr;

	m_isEscapePressed = false;
	m_isMousePressed = false;

	m_escapeStillPressed = false;
}

PlayerInputs::~PlayerInputs()
{
}

bool PlayerInputs::Initialize(HWND &wndHandle, HINSTANCE &hInstance)
{
	HRESULT hr;

	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

	// Initialize keyboard device
	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(hr))
	{
		MessageBox(wndHandle, L"Input keyboard device", L"Failed to create", MB_OK);
		return false;
	}

	// Initialize mouse device
	hr = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(hr))
	{
		MessageBox(wndHandle, L"Input mouse device", L"Failed to create", MB_OK);
		return false;
	}

	// Set keyboard data format, expected input
	hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(wndHandle, L"Keyboard data format", L"Failed to set", MB_OK);
		return false;
	}

	hr = m_keyboard->SetCooperativeLevel(wndHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
	{
		return false;
	}

	// Set mouse data format, expected input
	hr = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
	{
		MessageBox(wndHandle, L"Mouse data format", L"Failed to set", MB_OK);
		return false;
	}

	hr = m_mouse->SetCooperativeLevel(wndHandle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (FAILED(hr))
	{
		return false;
	}

	m_mouse->Acquire();
	m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseLastState);

	return true;
}

void PlayerInputs::ShutDown()
{
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard = 0;
	}

	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

void PlayerInputs::Update()
{
	DIMOUSESTATE mouseCurrentState;
	BYTE keyboardState[256];

	// Get access to input devices
	m_keyboard->Acquire();
	m_mouse->Acquire();

	// Store mouse current state
	m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);

	// Store keyboard current state
	m_keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);


	// ------------------------------
	//
	// PLAYER INPUTS
	//
	// ------------------------------

	// Keyboard inputs --------------
	if (keyboardState[DIK_A] & 0x80)
	{
		m_keyboardStateObject.key_a_pressed = true;
	}
	else
	{
		m_keyboardStateObject.key_a_pressed = false;
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		m_keyboardStateObject.key_d_pressed = true;
	}
	else
	{
		m_keyboardStateObject.key_d_pressed = false;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		m_keyboardStateObject.key_w_pressed = true;
	}
	else
	{
		m_keyboardStateObject.key_w_pressed = false;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		m_keyboardStateObject.key_s_pressed = true;
	}
	else
	{
		m_keyboardStateObject.key_s_pressed = false;
	}

	if (keyboardState[DIK_SPACE] & 0x80 && !m_spaceStillPressed)
	{
		m_keyboardStateObject.key_space_pressed = true;
	}
	else
	{
		m_keyboardStateObject.key_space_pressed = false;
	}
	if (keyboardState[DIK_RETURN] & 0x80)
	{
		m_keyboardStateObject.key_enter_pressed = true;
	}
	else
	{
		m_keyboardStateObject.key_enter_pressed = false;
	}
	if (keyboardState[DIK_ESCAPE] & 0x80 && !m_escapeStillPressed)
	{
		m_keyboardStateObject.key_escape_pressed = !m_keyboardStateObject.key_escape_pressed;
	}
	// ------------------------------

	// Mouse inputs -----------------
	// Mouse movement controls rotation
	if (mouseCurrentState.lX != m_mouseLastState.lX || mouseCurrentState.lY != m_mouseLastState.lY)
	{
		m_mouseStateObject.x_pos += m_mouseLastState.lX * 0.001f;
		m_mouseStateObject.y_pos += m_mouseLastState.lY * 0.001f;

	}

	// Mouse left click event
	if (mouseCurrentState.rgbButtons[0])
	{
		m_mouseStateObject.btn_left_pressed = true;
	}
	else
	{
		m_mouseStateObject.btn_left_pressed = false;
	}

	// Mouse right click event
	if (mouseCurrentState.rgbButtons[1])
	{
		m_mouseStateObject.btn_right_pressed = true;
	}
	else
	{
		m_mouseStateObject.btn_right_pressed = false;
	}
	// ------------------------------

	m_mouseLastState = mouseCurrentState;
	UpdateInputStates(keyboardState, m_mouseLastState);

	// Release access to input devices
	//m_keyboard->Unacquire();
	//m_mouse->Unacquire();
}

void PlayerInputs::UpdateInputStates(BYTE* keyboardState, DIMOUSESTATE mouseState)
{
	if (keyboardState[DIK_SPACE] & 0x80)
	{
		m_spaceStillPressed = true;
	}
	else
	{
		m_spaceStillPressed = false;
	}

	if (keyboardState[DIK_RETURN] & 0x80)
	{
		m_returnStillPressed = true;
	}
	else
	{
		m_returnStillPressed = false;
	}

	if (keyboardState[DIK_ESCAPE] & 0x80)
	{
		m_escapeStillPressed = true;
	}
	else
	{
		m_escapeStillPressed = false;
	}
}

KeyboardStateStruct PlayerInputs::GetKeyboardState() const
{
	return m_keyboardStateObject;
}

MouseStateStruct PlayerInputs::GetMouseState() const
{
	return m_mouseStateObject;
}

bool PlayerInputs::IsEscapePressed() const
{
	return m_isEscapePressed;
}
bool PlayerInputs::LeftMouseClick() const
{
	return m_isMousePressed;
}