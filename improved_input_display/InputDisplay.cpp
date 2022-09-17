#include "InputDisplay.h"
#include <d3dx9core.h>
#include <string>
#include "icon_data.h"
#include "icon_numbers.h"
#include "resource.h"
using namespace std;


namespace InputDisplay {

	struct sBattleSetting {
		void* vtable;
		char pad[0x30];
		struct character {
			void* vtable;
			INT32 mTeam;
			INT32 mType;
			INT32 unknown;
			INT32 mBody;
			INT32 mCpu;
			INT32 assist;
			char pad[0x38];
		};
		character characters[6];
		char pad2[0x104];
		INT32 battle_type;
		char pad3[0x21];
		char input_key_disp;
	};

	struct sRender {
		char pad[0xf8];
		LPHANDLE other_event_handle;
		LPHANDLE render_event_handle;
	};

	struct sMain {
		char pad[0xc6];
		LPHANDLE semaphore_handle;
	};

	struct InputItem {
		int input;
		int frames;
	};

	LPD3DXFONT m_font = NULL;
	LPDIRECT3DDEVICE9 device = NULL;
	LPDIRECT3DTEXTURE9 iconsTexture = NULL;
	LPD3DXSPRITE iconsSprite = NULL;
	LPDIRECT3DTEXTURE9 iconNumbersTexture = NULL;
	LPD3DXSPRITE iconNumbersSprite = NULL;

	int base_y = 230;
	int base_x = 50;

	InputItem buffer[20];
	int buffer_index = 0;
	int max_buffer_size = 20;

	string error;


	int framecount = 0;
	int current_input = 0;
	int sleepytime = 0;

	void sendInput(int input) {
		framecount++;
		if (input != current_input) {
			InputItem newInput = { current_input, framecount };

			buffer_index = (buffer_index + 1) % (max_buffer_size);
			buffer[buffer_index] = newInput;

			current_input = input;
			framecount = 0;
		}
	}


	INT32 rearrangeColour(INT32 colour) {
		if (colour == 0xFFFF00FF)
			return 0x00000000; //Chroma key?
		return
			((colour & 0xFF000000)) |
				((colour & 0x00FF0000) >> 16) |
				((colour & 0x0000FF00)) |
				((colour & 0x000000FF) << 16);
	}


	void init(LPDIRECT3DDEVICE9 pDevice, HMODULE hMod)
	{
		for (int i = 0; i < max_buffer_size; i++) {
			buffer[i] = { -1, -1 };
		}
		device = pDevice;
		D3DXCreateFontW(pDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font);


		D3DXCreateTexture(pDevice, ICON_FRAME_WIDTH, ICON_FRAME_HEIGHT * ICON_FRAME_COUNT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &iconsTexture);
		D3DXCreateTexture(pDevice, ICON_NUMBERS_FRAME_WIDTH, ICON_NUMBERS_FRAME_HEIGHT * ICON_FRAME_COUNT, 1, 0, D3DFMT_A8B8G8R8, D3DPOOL_MANAGED, &iconNumbersTexture);

		uint32_t* remapped_array = new uint32_t[ICON_FRAME_COUNT*ICON_FRAME_HEIGHT*ICON_FRAME_WIDTH];
		

		memcpy(remapped_array, &icon_data, sizeof(icon_data));
		for (int j = 0; j < ICON_FRAME_HEIGHT * ICON_FRAME_WIDTH*ICON_FRAME_COUNT; j++) {
			remapped_array[j] = rearrangeColour(remapped_array[j]);
		}

		D3DLOCKED_RECT rect;
		iconsTexture->LockRect(0, &rect, 0, D3DLOCK_DISCARD);
		unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
		memcpy(dest, remapped_array,sizeof(icon_data));
		iconsTexture->UnlockRect(0);

		D3DLOCKED_RECT rect2;
		iconNumbersTexture->LockRect(0, &rect2, 0, D3DLOCK_DISCARD);
		unsigned char* dest2 = static_cast<unsigned char*>(rect2.pBits);
		memcpy(dest2, &icon_numbers_data, sizeof(icon_numbers_data));
		iconNumbersTexture->UnlockRect(0);



		D3DXCreateSprite(pDevice, &iconsSprite);
		D3DXCreateSprite(pDevice, &iconNumbersSprite);

		
	}

	void drawError() {

		RECT rct;
		rct.left = 50;
		rct.right = 1000;
		rct.top = 50;
		rct.bottom = 300;
		D3DCOLOR fontColour = D3DCOLOR_ARGB(255, 255, 255, 255);
		m_font->DrawTextA(NULL, error.c_str(), -1, &rct, 0, fontColour);

	}

	void drawText(string text, int x, int y, bool enabled) {
	
		int shadow_offset = 1;
		RECT rct_shadow;
		rct_shadow.left = x + shadow_offset;
		rct_shadow.right = x + 100 + shadow_offset;
		rct_shadow.top = y + shadow_offset;
		rct_shadow.bottom = y + 130 + shadow_offset;
		D3DCOLOR fontColour_shadow = D3DCOLOR_ARGB(255, 0,0,0);
		m_font->DrawTextA(NULL, text.c_str(), -1, &rct_shadow, 0, fontColour_shadow);

		RECT rct;
		rct.left = x;
		rct.right = x + 100;
		rct.top = y;
		rct.bottom = y + 130;
		int intensity = enabled ? 255 : 100;
		D3DCOLOR fontColour = D3DCOLOR_ARGB(255, intensity, intensity, intensity);
		m_font->DrawTextA(NULL, text.c_str(), -1, &rct, 0, fontColour);
	}


	enum IconType {
		ICONTYPE_BLANK,
		ICONTYPE_H,
		ICONTYPE_S,
		ICONTYPE_L,
		ICONTYPE_M,
		ICONTYPE_A1,
		ICONTYPE_A2,
		ICONTYPE_D,
		ICONTYPE_RIGHT,
		ICONTYPE_U,
		ICONTYPE_LEFT,
		ICONTYPE_DLEFT,
		ICONTYPE_DRIGHT,
		ICONTYPE_URIGHT,
		ICONTYPE_ULEFT
	};

	void drawButtonIcon(IconType iconType, float x, float y, bool enabled) {
		float iconIndex = iconType;
		
		RECT rct;
		rct.left = 0;
		rct.right = 16;
		rct.top = iconIndex * 16;
		rct.bottom = (iconIndex+1) * 16;
		D3DXVECTOR3 centre = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0);

		int brightness = enabled ? 255 : 40;
		iconsSprite->Draw(iconsTexture, &rct, &centre, &position, D3DCOLOR_RGBA(brightness, brightness, brightness, 255));
	}

	void drawDigit(int digit, float x, float y) {
		int offset = digit % 10;
		RECT rct;
		rct.left = 0;
		rct.right = ICON_NUMBERS_FRAME_WIDTH;
		rct.top = offset * ICON_NUMBERS_FRAME_HEIGHT;
		rct.bottom = (offset+1) * ICON_NUMBERS_FRAME_HEIGHT;
		D3DXVECTOR3 centre = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0);
		iconNumbersSprite->Draw(iconNumbersTexture, &rct, &centre, &position, D3DCOLOR_RGBA(255,255,255, 255));
	}

	void drawNumber(int number, float x, float y) {
		float sep = ICON_NUMBERS_FRAME_WIDTH;
		if (number <= 9) {
			drawDigit(number, x, y);
			return;
		}
		if (number <= 99) {
			drawDigit(number / 10, x, y);
			drawDigit(number % 10, x + sep, y);
			return;
		}
		if (number <= 999) {
			drawDigit(number / 100, x, y);
			drawDigit((number / 10) % 10, x + sep, y);
			drawDigit(number % 10, x + sep * 2, y);
			return;
		}
		if (number <= 9999) {
			drawDigit(number / 1000, x, y);
			drawDigit((number / 100) % 10, x + sep, y);
			drawDigit((number / 10) % 10, x + sep * 2, y);
			drawDigit(number % 10, x + sep * 3, y);
			return;
		}
		drawDigit(9, x, y);
		drawDigit(9, x + sep, y);
		drawDigit(9, x + sep * 2, y);
		drawDigit(9, x + sep * 3, y);
	}

	void drawItem(InputItem item, int x, int y) {

		int input = item.input;
		int sep = 20;

		int arrow_x = x;
		int arrow_y = y + sep / 2;
			
			switch (input & 0x0000000f) {
			case 0x1:
				drawButtonIcon(ICONTYPE_RIGHT, arrow_x, arrow_y, true);
				break;
			case 0x2:
				drawButtonIcon(ICONTYPE_LEFT, arrow_x, arrow_y, true);
				break;
			case 0x4:
				drawButtonIcon(ICONTYPE_U, arrow_x, arrow_y, true);
				break;
			case 0x8:
				drawButtonIcon(ICONTYPE_D, arrow_x, arrow_y, true);
				break;
			case 0x4 | 0x1:
				drawButtonIcon(ICONTYPE_URIGHT, arrow_x, arrow_y, true);
				break;
			case 0x4 | 0x2:
				drawButtonIcon(ICONTYPE_ULEFT, arrow_x, arrow_y, true);
				break;
			case 0x8 | 0x1:
				drawButtonIcon(ICONTYPE_DRIGHT, arrow_x, arrow_y, true);
				break;
			case 0x8 | 0x2:
				drawButtonIcon(ICONTYPE_DLEFT, arrow_x, arrow_y, true);
				break;
			}
		
		drawButtonIcon(ICONTYPE_L, x + sep * 2, y, input & 0x10);
		drawButtonIcon(ICONTYPE_M, x + sep * 3, y, input & 0x20);
		drawButtonIcon(ICONTYPE_H, x + sep * 4, y, input & 0x40);
		drawButtonIcon(ICONTYPE_S, x + sep * 2, y + sep, input & 0x80);
		drawButtonIcon(ICONTYPE_A1, x + sep * 3, y + sep, input & 0x100);
		drawButtonIcon(ICONTYPE_A2, x + sep * 4, y + sep, input & 0x200);
		drawNumber(item.frames, x + sep * 5 + 4, y + 4);
	}

	void drawFrame()
	{
		drawError();

		iconsSprite->Begin(D3DXSPRITE_ALPHABLEND);
		iconNumbersSprite->Begin(D3DXSPRITE_ALPHABLEND);

		int num_to_draw = 14;
		InputItem current = { current_input, framecount };
		drawItem(current, 50, base_y);

			for (int i = 0; i < num_to_draw; ++i) {
				int index = buffer_index - i;
				if (index < 0) {
					index = max_buffer_size + index;
				}
				InputItem item = buffer[index];
				if (item.input != -1) {
					drawItem(item, 50, base_y + 40 * (i + 1));
				}
			}

			iconNumbersSprite->End();
		iconsSprite->End();

	}

	
}