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

int main()
{
	for (int i = FG_BLACK; i < BG_WHITE; i++) {
		COLOR it = static_cast<COLOR>(i);
		for (int j = PIXEL_SOLID; j < PIXEL_QUATER; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), it);

			PIXEL_TYPE jt = static_cast<PIXEL_TYPE>(j);

			SetPixel(GetDC(GetConsoleWindow()), jt, jt, it);
			std::cout << "Test" << std::endl;
		}
	}
}


