#ifndef SYSTEM_H
#define SYSTEM_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "TextClass.h"
#include "GamePlay.h"
#include "Direct3D.h"
#include "Menu.h"

//move this
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class System
{
private:
	struct LevelInfo
	{
		wstring scene;
		wstring collision;
		wstring kristall;
		wstring winPlane;
	};
	
	vector<LevelInfo> levels;
	size_t currentLevel;

	int screenWidth;
	int screenHeight;

	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	double countsPerSecond;
	__int64 counterStart;

	int frameCount;
	int fps;

	__int64 frameTimeOld;
	double frameTime;

	enum GameState
	{
		gMenu,
		gGamePlay,
		gPause,
		gDeath,
		gWin,
		gLoading,
		gHowToPlay
	}
	gameState;

	GameState prevState;

	Menu* menu;
	Menu* pauseMenu;
	Menu* deathScreen;
	Menu* howToPlay;
	Menu* winScreen;
	Menu* loadScreen;
	GamePlay* gamePlay;
	Direct3D* direct3D;
	TextClass* timer;

	float lastTime;

public:
	System();
	virtual ~System();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame(double _time);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	void StartTimer();
	double GetTime();
	double GetFrameTime();


};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static System* ApplicationHandle = 0;

#endif