#pragma once

#include "umvc3_sMvc3Main.h"

namespace Mvc3FrameSimulation {
	void setToggleMode(int toggleMode);
	void setLifeSupport(int idx, int ls);
	void setPadToTeam(int pad_idx, int team_idx);
	void queueInput(int pad_idx, int input);
	void InstallHook();
	void StartMatch();
}