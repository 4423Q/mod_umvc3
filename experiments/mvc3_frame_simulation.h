#pragma once

#include "umvc3_sMvc3Main.h"

namespace Mvc3FrameSimulation {
	struct Mvc3GameState {
		sCharacter sChar;
	};

	void setToggleMode(int toggleMode);
	void setFakePad(int idx, int ls);
	void setPadToTeam(int pad_idx, int team_idx);
	void InstallHook();
	void StartMatch();
	void stopRecording();
	void startRecording(int pad_idx);
	void stopPlaying();
	void startPlaying(int pad_idx);
	void OnGameReady(void (*cb)());
	void OnLocalPlayerInput(bool (*cb)(int, int));
	void OnAdvanceFrameComplete(void (*cb)());
	void OnFrameComplete(void (*cb)());
	Mvc3GameState* getState();
	void AdvanceFrame();
	void setNextInputP1(int input);
	void setNextInputP2(int input);
	void startGame();
}