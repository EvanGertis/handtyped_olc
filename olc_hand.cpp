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
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), it);
		std::cout << "yo" << std::endl;
	}
}


