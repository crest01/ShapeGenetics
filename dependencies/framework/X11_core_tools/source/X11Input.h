


#ifndef INCLUDED_X11_INPUT
#define INCLUDED_X11_INPUT

#pragma once

#include "platform.h"


namespace Framework
{
	enum Button
	{
		BUTTON_LEFT = 1U,
		BUTTON_RIGHT = 4U,
		BUTTON_MIDDLE = 2U
	};

	enum Key
	{
		KEY_SPACE = XK_space,
		KEY_ENTER = XK_Return,
		KEY_BACKSPACE = XK_BackSpace,
		KEY_TAB = XK_Tab,
		KEY_INS = XK_Insert,
		KEY_DEL = XK_Delete,
		KEY_HOME = XK_Home,
		KEY_LEFT = XK_Left,
		KEY_UP = XK_Up,
		KEY_RIGHT = XK_Right,
		KEY_DOWN = XK_Down,
		KEY_PAGE_UP = XK_Page_Up,
		KEY_PAGE_DOWN = XK_Page_Down,
		KEY_0 = XK_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_PLUS = XK_plus,
		KEY_MINUS = XK_minus,
		KEY_NUM_0 = XK_0,
		KEY_NUM_1,
		KEY_NUM_2,
		KEY_NUM_3,
		KEY_NUM_4,
		KEY_NUM_5,
		KEY_NUM_6,
		KEY_NUM_7,
		KEY_NUM_8,
		KEY_NUM_9,
		KEY_A = XK_a,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_F1 = XK_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_ESCAPE = XK_Escape
	};
}

#endif  // INCLUDED_X11_INPUT
