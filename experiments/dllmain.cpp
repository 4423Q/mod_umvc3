// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
#include "mvc3_frame_simulation.h"
#include <iostream>
#include <string>

using namespace Memory::VP;


DWORD WINAPI Initialise(LPVOID lpreserved) {

    sMvc3Manager* manager;

    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    printf("Initialise() | Begin!\n");
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "go") {
            std::cout << "GO TIME" << std::endl;
            Mvc3FrameSimulation::setToggleMode(1);
            Mvc3FrameSimulation::setLifeSupport(3, 1);

            Mvc3FrameSimulation::setPadToTeam(0, 0);

            Mvc3FrameSimulation::setPadToTeam(3, 1);


            Mvc3FrameSimulation::StartMatch();
        }
        if (line == "alternate") {
            Mvc3FrameSimulation::setToggleMode(2);
            std::cout << "alternating render" << std::endl;
        }
        if (line == "normalrender") {
            Mvc3FrameSimulation::setToggleMode(0);
            std::cout << "normal render" << std::endl;
        }
        if (line == "myrender") {
            Mvc3FrameSimulation::setToggleMode(1);
            std::cout << "my render" << std::endl;
        }
        if (line == "press") {
            Mvc3FrameSimulation::queueInput(3, 0x80);
        }
    }
    return TRUE;
}

bool CheckGame()
{
    char* gameName = (char*)(_addr(0x140B12D10));

    if (strcmp(gameName, "umvc3") == 0)
    {
        return true;
    }
    else
    {
        MessageBoxA(0, "Invalid game version!\nmod_enable_damage_counter only supports latest Steam executable.", 0, MB_ICONINFORMATION);
        return false;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (CheckGame()) {
            Mvc3FrameSimulation::InstallHook();
            CreateThread(nullptr, 0, Initialise, hModule, 0, nullptr);

        }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

