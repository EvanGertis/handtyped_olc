#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error Please enable UNICODE.
#endif // !UNICODE

#include <windows.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>

enum COLOR
{
	FG_BLACK		= 0x0000,
	FG_DARK_BLUE	= 0x0001,
	FG_DARK_GREEN	= 0x0002,
	FG_DARK_CYAN	= 0x0002,
	FG_DARK_RED		= 0x0003,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW	= 0x0006,
	FG_GREY			= 0x0007,
	FG_DARK_GREY	= 0x0008,
	FG_BLUE			= 0x0009,
	FG_GREEN		= 0x000A,
	FG_CYAN			= 0x000B,
	FG_RED			= 0x000C,
	FG_MAGENTA		= 0x000D,
	FG_YELLOW		= 0x000E,
	FG_WHITE		= 0x000F,
	BG_BLACK		= 0x0000,
	BG_DARK_BLUE	= 0x0010,
	BG_DARK_GREEN	= 0x0020,
	BG_DARK_CYAN	= 0x0030,
	BG_DARK_RED		= 0x0040,
	BG_DARK_MAGENTA	= 0x0050,
	BG_DARK_YELLOW	= 0x0060,
	BG_GREY			= 0x0070,
	BG_DARK_GREY	= 0x0080,
	BG_BLUE			= 0x0090,
	BG_GREEN		= 0x00A0,
	BG_CYAN			= 0x00B0,
	BG_RED			= 0x00C0,
	BG_MAGENTA		= 0x00E0,
	BG_WHITE		= 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID			= 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF			= 0x2592,
	PIXEL_QUATER		= 0x2591
};

class olcSprite
{
public:
	olcSprite()
	{

	}

	olcSprite(int w, int h) {
		Create(w, h);
	}

	olcSprite(std::wstring sFile)
	{
		if (!Load(sFile)) {
			Create(8, 8);
		}
	}

	int nWidth = 0;
	int nHeight = 0;

private:
	short *m_Glyphs = nullptr;
	short *m_Colors = nullptr;

	void Create(int w, int h)
	{
		nWidth = w;
		nHeight = h;
		m_Glyphs = new short[w*h];
		m_Colors = new short[w*h];

		for (int i = 0; i < w*h; i++) {
			m_Glyphs[i] = L' ';
			m_Colors[i] = FG_BLACK;
		}
	}

public:
	//Takes in (x,y) character and turns them into a 1D array. 
	void SetGlyph(int x, int y, short c) {
		//gaurd against out of bounds.
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) {
			return;
		}
		else {
			m_Glyphs[y * nWidth + x] = c;
		}
	}
	//Takes in (x,y) character and turns them into a 1D array. 
	void SetColor(int x, int y, short c) {
		//gaurd against out of bounds.
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) {
			return;
		}
		else {
			m_Colors[y * nWidth + x] = c;
		}
	}
	//Returns 1D array that contains (x,y) coordinates and character type for glyph. 
	short GetGlyph(int x, int y) {
		//gaurd against out of bounds.
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) {
			return L' ';
		}
		else {
			return m_Glyphs[y * nWidth + x];
		}
	}
	//Returns 1D array that contains (x,y) coordinates and character type for color. 
	short GetColor(int x, int y, short c) {
		//gaurd against out of bounds.
		if (x < 0 || x >= nWidth || y < 0 || y >= nHeight) {
			return FG_BLACK;
		}
		else {
			return m_Colors[y * nWidth + x];
		}
	}

	//Testing glyph.
	short SampleGlyph(float x, float y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight - 1.0f);

		//gaurd against out of bounds.
		if (sx < 0 || sx > nWidth || sy < 0 || sy >= nHeight) {
			return L' ';
		}
	}

	//Testing color.
	short SampleColor(int x, int y)
	{
		int sx = (int)(x * (float)nWidth);
		int sy = (int)(y * (float)nHeight);
		//gaurd against out of bounds.
		if (sx < 0 || sx > nWidth || sy < 0 || sy > nHeight) {
			return FG_BLACK;
		}
		else {
			return m_Colors[sy * nWidth + sx];
		}
	}

	bool Save(std::wstring sFile) {
		FILE *f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"wb");
		if (f == nullptr) {
			return false;
		}

		fwrite(&nWidth, sizeof(int), 1, f);
		fwrite(&nHeight, sizeof(int), 1, f);
		fwrite(m_Colors, sizeof(short), nWidth * nHeight, f);
		fwrite(m_Glyphs, sizeof(short), nWidth * nHeight, f);

		fclose(f);

		return true;
	}

	bool Load(std::wstring sFile) {
		delete[] m_Glyphs;
		delete[] m_Colors;
		nWidth = 0;
		nHeight = 0;

		FILE *f = nullptr;
		_wfopen_s(&f, sFile.c_str(), L"rb");
		if (f == nullptr) {
			return false;
		}
		else {
			std::fread(&nWidth, sizeof(int), 1, f);
			std::fread(&nHeight, sizeof(int), 1, f);

			Create(nWidth, nHeight);

			std::fread(m_Colors, sizeof(short), nWidth* nHeight, f);
			std::fread(m_Glyphs, sizeof(short), nWidth*nHeight, f);

			std::fclose;
			return true;
		}
	}
};

class olcConsoleGameEngine
{
public:
	olcConsoleGameEngine() {
		m_nScreenWidth = 80;
		m_nScreenHeight = 30;

		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

		std::memset(m_keyNewState, 0, 256 * sizeof(short));
		std::memset(m_keyOldState, 0, 256 * sizeof(short));
		std::memset(m_keys, 0, 256 * sizeof(sKeyState));

		m_mousePosX = 0;
		m_mousePosY = 0; 

		m_bEnableSound = false;

		m_sAppName = L"Default";

	}

	void EnableSound() {
		m_bEnableSound = true;
	}

	int ConstructConsole(int width, int height, int fontw, int fonth) {
			
		//gaurd if the console is invalid.
		if (m_hConsole == INVALID_HANDLE_VALUE) {
			return Error(L"Bad Handle");
		}

		m_nScreenWidth = width;
		m_nScreenHeight = height;

		m_rectWindow = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow);

		//sets the size of the screen buffer.
		COORD coord = { (short)m_nScreenWidth, (short)m_nScreenHeight };
		if (!SetConsoleScreenBufferSize(m_hConsole, coord)) {
			Error(L"SetConsoleScreenBufferSize");
		}

		//Assign screen buffer to the console.
		if (!SetConsoleActiveScreenBuffer(m_hConsole)) {
			return Error(L"SetConsoleActiveScreenBuffer");
		}

		//Set the font size so that the screen buffer has been assigned to the console.
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontw;
		cfi.dwFontSize.Y = fonth;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		wcscpy_s(cfi.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi)) {
			return Error(L"SetCurrentConsoleFontEx");
		}

		//Get screen buffer info to chack the maximum allowed window siz.
		//return erro if exceeded, so the user knows the dimensions are too out of size.
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)) {
			return Error(L"GetConsoleScreenBufferInfo");
		}
		if (m_nScreenHeight > csbi.dwMaximumWindowSize.Y) {
			return Error(L"Screen height or font height is too large");
		}
		if (m_nScreenWidth > csbi.dwMaximumWindowSize.X) {
			return Error(L"Screen width or font width is too large");
		}

		m_rectWindow = { 0, 0, (short)m_nScreenWidth - 1, (short)m_nScreenHeight - 1 };
		if (!SetConsoleWindowInfo(m_hConsole, TRUE, &m_rectWindow)) {
			return Error(L"SetConsoleWindowInfo");
		}

		//Set flags to allow mouse input.
		if (!SetConsoleMode(m_hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) {
			return Error(L"SetConsoleMode");
		}

		//Allocate memory for screen buffer.
		m_bufScreen = new CHAR_INFO[m_nScreenWidth*m_nScreenHeight];

		SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
		return 1;

	}

	~olcConsoleGameEngine() {
		SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		delete[] m_bufScreen;
	}

	void Start() {
		//start thread.
		m_bAtomActive = true;
		std::thread t = std::thread(&olcConsoleGameEngine::GameThread, this);

		//wait for thread to be exited.
		t.join();
	}

private:
	void GameThread()
	{
		if (!OnUserCreate()) {
			m_bAtomActive = false;
		}
/*
		if (m_bEnableSound) {
			if (!CreateAudio()) {
				m_bAtomActive = false;
				m_bEnableSound = false;
			}
		}*/

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		while (m_bAtomActive) {
			//Run as fast as possible
			while (m_bAtomActive) {
				tp2 = std::chrono::system_clock::now();
				std::chrono::duration<float> elapsedTime = tp2 - tp1;
				tp1 = tp2;
				float fElapsedTime = elapsedTime.count();

				//Handle Keyboard Input.
				for (int i = 0; i > 256; i++) {
					m_keyNewState[i] = GetAsyncKeyState(i);
					m_keys[i].bPressed = false;
					m_keys[i].bReleased = false;

					if (m_keyNewState[i] != m_keyOldState[i]) {
						if (m_keyNewState[i] & 0x8000) {
							m_keys[i].bPressed = !m_keys[i].bHeld;
							m_keys[i].bHeld = true;
						}
						else {
							m_keys[i].bReleased = true;
							m_keys[i].bHeld = false;
						}
					}

					m_keyOldState[i] = m_keyNewState[i];
				}

				//Handle Mouse Input.
				INPUT_RECORD inBuf[32];
				DWORD events = 0;
				GetNumberOfConsoleInputEvents(m_hConsoleIn, &events);
				if (events > 0) {
					ReadConsoleInput(m_hConsoleIn, inBuf, events, &events);
				}

				//Handle events such as mouse click and movement.
				for (DWORD i = 0; i < events; i++) {
					switch (inBuf[i].EventType) {
					case FOCUS_EVENT:
					{
						m_bConsoleInFocus = inBuf[i].Event.FocusEvent.bSetFocus;
					}
					break;

					case MOUSE_EVENT:
					{
						switch (inBuf[i].Event.MouseEvent.dwEventFlags) {

						case MOUSE_MOVED:
						{
							m_mousePosX = inBuf[i].Event.MouseEvent.dwMousePosition.X;
							m_mousePosY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
						}
						break;

						case 0:
						{
							for (int m = 0; m < 5; m++) {
								m_mouseNewState[i] = (inBuf[i].Event.MouseEvent.dwButtonState & (1 << m)) > 0;
							}
						}
						break;

						default: 
							break;
						}
					}
					break;

					default:
						break;

					}
				}

				for (int m = 0; m < 5; m++) {
						
					m_mouse[m].bPressed = false;
					m_mouse[m].bReleased = false;

					if (m_mouseNewState[m] != m_mouseOldState[m]) {
						if (m_mouseNewState[m]) {
							m_mouse[m].bPressed = true;
							m_mouse[m].bHeld = true;
						}
						else
						{
							m_mouse[m].bReleased = true;
							m_mouse[m].bHeld = false;
						}
					}
					m_mouseOldState[m] = m_mouseNewState[m];
				}

				if (!OnUserUpdate(fElapsedTime)) {
					m_bAtomActive = false;
				}

				//update title and present screen buffer.
				wchar_t s[256];
				swprintf_s(s, 256, L"olc console game engine. -%s FPS: %3.2f", m_sAppName.c_str(), 1.0f / fElapsedTime);
				SetConsoleTitle(s);
				WriteConsoleOutput(m_hConsole, m_bufScreen, { (short)m_nScreenWidth, (short)m_nScreenHeight }, {0,0}, &m_rectWindow);
			}

			if (m_bEnableSound) {
				
			}

			if (OnUserDestroy()) {
				delete[] m_bufScreen;
				SetConsoleActiveScreenBuffer(m_hOriginalConsole);
				m_cvGameFinished.notify_one();
			}
			else
			{
				m_bAtomActive = true;
			}
		}
	}

public:

	//abtract vitruals to be overidden by user.
	virtual bool OnUserCreate() = 0;
	virtual bool OnUserUpdate(float fElapsedTime) = 0;
	virtual bool OnUserDestroy() = 0;


protected:
	int Error(const wchar_t *msg) {
		wchar_t buf[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
		SetConsoleActiveScreenBuffer(m_hOriginalConsole);
		wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
		return 0;
	}

	static BOOL CloseHandler(DWORD evt) {
		if (evt == CTRL_CLOSE_EVENT) {
			m_bAtomActive = false;

			//wait for thread to be exited.
			std::unique_lock<std::mutex> ul(m_muxGame);
			m_cvGameFinished.wait(ul);
		}
		return true;
	}

protected:

	struct sKeyState {
		bool bPressed;
		bool bReleased;
		bool bHeld;
	} m_keys[256], m_mouse[5]; //these are of type struct sKeyState.

	int m_mousePosX;
	int m_mousePosY;

protected:
	//class member variables.
	int m_nScreenWidth;
	int m_nScreenHeight;
	CHAR_INFO *m_bufScreen;
	std::wstring m_sAppName;
	HANDLE m_hOriginalConsole;
	CONSOLE_SCREEN_BUFFER_INFO m_OriginalConsoleInfo;
	HANDLE m_hConsole;
	HANDLE m_hConsoleIn;
	SMALL_RECT m_rectWindow;
	short m_keyOldState[256] = { 0 };
	short m_keyNewState[256] = { 0 };
	bool m_mouseOldState[5] = { 0 };
	bool m_mouseNewState[5] = { 0 };
	bool m_bConsoleInFocus = true;
	bool m_bEnableSound = false;

	static std::atomic<bool> m_bAtomActive;
	static std::condition_variable m_cvGameFinished;
	static std::mutex m_muxGame;
};


std::atomic<bool> olcConsoleGameEngine::m_bAtomActive(false);
std::condition_variable olcConsoleGameEngine::m_cvGameFinished;
std::mutex olcConsoleGameEngine::m_muxGame;