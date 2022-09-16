#include "InputDisplay.h"
#include <d3dx9core.h>
#include "umvc3utils.h"
#include <string>
#include "Trampoline.h"
#include "MemoryMgr.h"
#include "icon_data.h"
#include "resource1.h"
using namespace Memory::VP;
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

	int base_y = 230;
	int base_x = 50;

	LPD3DXLINE line = NULL;
	D3DXVECTOR2 points[] = {
		D3DXVECTOR2(base_x + 80, base_y - 20),
		D3DXVECTOR2(base_x + 80, base_y + 640),
	};

	InputItem buffer[12];
	int buffer_index = 0;
	int max_buffer_size = 12;

	string error;


	int framecount = 0;
	int current_input = 0;
	int sleepytime = 0;


	void HookInput(void* whatever, int input)
	{
		framecount++;
		if (input != current_input) {
			InputItem newInput = { current_input, framecount };

			buffer_index = (buffer_index + 1) % (max_buffer_size);
			buffer[buffer_index] = newInput;
			
			current_input = input;
			framecount = 0;
		}
	
		return;
	}


	void init(LPDIRECT3DDEVICE9 pDevice, HMODULE hMod)
	{
		for (int i = 0; i < max_buffer_size; i++) {
			buffer[i] = { -1, -1 };
		}
		device = pDevice;
		D3DXCreateLine(pDevice, &line);
		D3DXCreateFontW(pDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font);
		
		//HRESULT res = D3DXCreateTextureFromFileInMemoryEx(pDevice, &icon_data, sizeof(icon_data), ICON_FRAME_WIDTH * ICON_FRAME_COUNT, ICON_FRAME_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 0, 255), nullptr, nullptr, &iconsTexture);

		HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(IDB_PNG1), L"PNG");
		if (hResource == NULL) {
			error += "Error finding resource... ";
		}

		HRESULT res = D3DXCreateTextureFromResource(pDevice, hMod, MAKEINTRESOURCE(IDB_PNG1), &iconsTexture);
		if (res != D3D_OK) {
			switch (res) {
				case D3DERR_NOTAVAILABLE:
					error += "ERror: notavailable";
					break;
				case D3DERR_INVALIDCALL:
					error += "Error: invalidcall";
					break;
				case D3DXERR_INVALIDDATA:
					error += "Error: invalidData";
					break;
				default:
					error += "Other error?" + to_string(res);

			}
			
		}
		D3DXCreateSprite(pDevice, &iconsSprite);

		Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
		InjectHook(_addr(0x1402f9c36), tramp->Jump(HookInput), PATCH_CALL);
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

	void drawItem(InputItem item, int x, int y) {

		int input = item.input;
		int sep = 20;


		drawText("<", x , y + sep/2, input & 0x2);
		drawText("v", x + sep/2, y + sep, input & 0x8);
		drawText(">", x + sep, y + sep/2, input & 0x1);
		drawText("^", x + sep/2, y, input & 0x4);
		
		drawText("L", x + sep * 3, y, input & 0x10);
		drawText("M", x + sep * 4, y, input & 0x20);
		drawText("H", x + sep * 5, y, input & 0x40);
		drawText("S", x + sep * 3, y + sep, input & 0x80);
		drawText("1", x + sep * 4, y + sep, input & 0x100);
		drawText("2", x + sep * 5, y + sep, input & 0x200);
		drawText(to_string(item.frames), x + sep * 7, y + sep/2, true);
	}

	void drawFrame()
	{
		drawError();

		RECT rct;
		rct.left = 0;
		rct.right = 100;
		rct.top = 0;
		rct.bottom = 100;
		iconsSprite->Begin(D3DXSPRITE_ALPHABLEND);
		D3DXVECTOR3 centre = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 position = D3DXVECTOR3(100, 100, 0);

		iconsSprite->Draw(iconsTexture, &rct, &centre, &position, D3DCOLOR_RGBA(255, 255, 255, 255));
		iconsSprite->End();


		line->SetWidth(200);
		line->Begin();
		line->Draw(points, 2, D3DCOLOR_RGBA(40, 40, 80, 170));
		line->End();

		InputItem current = { current_input, framecount };
		drawItem(current, 50, base_y);

			for (int i = 0; i < max_buffer_size; ++i) {
				int index = buffer_index - i;
				if (index < 0) {
					index = max_buffer_size + index;
				}
				InputItem item = buffer[index];
				if (item.input != -1) {
					drawItem(item, 50, base_y + 50 * (i + 1));
				}
			}
	
	}

	
}