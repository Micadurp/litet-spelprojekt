#include "PlayerInputs.h"

PlayerInputs::PlayerInputs()
{
	//m_yawPitch = { 0.0f, 0.0f };		// x = yaw rotation, y = pitch
	//m_movement = { 0.0f, 0.0f };		// x = left/right, y = forward/backward

	m_keyboard = nullptr;
	m_mouse = nullptr;

	m_isEscapePressed = false;

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

void PlayerInputs::Update()
{
	DIMOUSESTATE mouseCurrentState;
	BYTE keyboardState[256];

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
	if (keyboardState[DIK_D] & 0x80)
	{
		m_keyboardStateObject.key_d_pressed = true;
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		m_keyboardStateObject.key_w_pressed = true;
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		m_keyboardStateObject.key_s_pressed = true;
	}
	if (keyboardState[DIK_SPACE] & 0x80)
	{
		// Do stuff
	}
	if (keyboardState[DIK_RETURN] & 0x80)
	{
		// Do stuff
	}
	if (keyboardState[DIK_ESCAPE] & 0x80 && !m_escapeStillPressed)
	{
		m_isEscapePressed = !m_isEscapePressed;
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

	// Mouse right click event
	if (mouseCurrentState.rgbButtons[1])
	{
		m_mouseStateObject.btn_right_pressed = true;
	}
	// ------------------------------

	m_mouseLastState = mouseCurrentState;
	UpdateInputStates(keyboardState, m_mouseLastState);
}

void PlayerInputs::UpdateInputStates(BYTE* keyboardState, DIMOUSESTATE mouseState)
{
	m_escapeStillPressed = keyboardState[DIK_ESCAPE] & 0x80;
	m_returnStillPressed = keyboardState[DIK_RETURN] & 0x80;
	m_spaceStillPressed = keyboardState[DIK_SPACE] & 0x80;

	
	if (!keyboardState[DIK_A] & 0x80)
	{
		m_keyboardStateObject.key_a_pressed = false;
	}
	if (!keyboardState[DIK_D] & 0x80)
	{
		m_keyboardStateObject.key_d_pressed = false;
	}
	if (!keyboardState[DIK_W] & 0x80)
	{
		m_keyboardStateObject.key_w_pressed = false;
	}
	if (!keyboardState[DIK_S] & 0x80)
	{
		m_keyboardStateObject.key_s_pressed = false;
	}


	if (!mouseState.rgbButtons[0])
	{
		m_mouseStateObject.btn_left_pressed = false;
	}

	if (!mouseState.rgbButtons[1])
	{
		m_mouseStateObject.btn_right_pressed = false;
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

void PlayerInputs::ReleaseCOM()
{
	m_keyboard->Unacquire();
	m_mouse->Unacquire();
	m_directInput->Release();
}