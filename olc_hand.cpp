/*
FILE NAME: olc_hand.cpp
AUTHOR: E. M. GERTIS
CREDIT: Jaxid9 OneLoneCoder.
DATE: 03/23/2019
PURPOSE: TEST THE HAND TYPED IMPLEMENTATION OF THE OLC 
		 GAME ENGINE.
*/

#include "pch.h"
#include "olc.h"
#include "olc_hand.h"

class testEngine : public olcConsoleGameEngine
{

public: 
	testEngine() {
		m_sAppName = L"Game Engine";
	}

	virtual bool OnUserCreate() {

		for (int i = 0; i < 20; i++)
		{
			for ( int j = 0; j < 20; j++)
			{
				Draw(i, j, PIXEL_SOLID, FG_WHITE);
			}

		}

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) {

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				Draw( (int)fElapsedTime % 100, j, PIXEL_SOLID, FG_WHITE);
			}

		}
		
		return true;
	}

	bool OnUserDestroy() {
		
		return true;
	}
};

int main()
{
	testEngine te;
	if (te.ConstructConsole(120, 128, 4, 4)) {
		te.Start();
	}

	return 0;
}


