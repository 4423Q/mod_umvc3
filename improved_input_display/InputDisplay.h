#pragma once
#include <d3d9.h>



namespace InputDisplay {
	void init(LPDIRECT3DDEVICE9 pDevice, HMODULE hMod);
	void drawFrame();
	void sendInput(int input);
	void setActive(bool active);
}