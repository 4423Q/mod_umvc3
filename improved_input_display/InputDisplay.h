#pragma once
#include <d3d9.h>



namespace InputDisplay {
	void init(LPDIRECT3DDEVICE9 pDevice, int screen_width);
	void drawFrame();
	void sendInput(int input);
	void setActive(bool active);
}