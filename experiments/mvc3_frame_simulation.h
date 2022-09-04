#pragma once

#include "umvc3_sMvc3Main.h"

namespace Mvc3FrameSimulation {
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
	void startGame();
}