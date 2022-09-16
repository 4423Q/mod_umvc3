#include "InputDisplay.h"
#include <d3dx9core.h>


LPD3DXFONT m_font = NULL;
LPDIRECT3DDEVICE9 device = NULL;

void InputDisplay::init(LPDIRECT3DDEVICE9 pDevice)
{
	device = pDevice;
	D3DXCreateFontW(pDevice, 17, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font);
}

void InputDisplay::drawFrame()
{
	RECT rct;
	rct.left = 0;
	rct.right = 500;
	rct.top = 100;
	rct.bottom = 400;
	D3DCOLOR fontColour = D3DCOLOR_ARGB(255, 255, 255, 255);
	m_font->DrawTextA(NULL, "S L M H  < v ^ >", -1, &rct, 0, fontColour);
}

void InputDisplay::run()
{
	bool running = true;
	while (running) {
		Sleep(1);
	}
}
