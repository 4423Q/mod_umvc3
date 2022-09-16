#pragma once
#include <d3d9.h>



namespace InputDisplay {
	void init(LPDIRECT3DDEVICE9 pDevice);
	void drawFrame();
	void run();
}