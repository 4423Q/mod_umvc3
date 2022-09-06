// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Trampoline.h"
#include "umvc3utils.h"
#include "MemoryMgr.h"
#include "umvc3_sMvc3Main.h"
#include "mvc3_frame_simulation.h"
#include <iostream>
#include <string>
#include "libs/ggponet.h"

using namespace Memory::VP;

GGPOSession* ggpo = NULL;
GGPOPlayerHandle local_player_handle;

bool begin_game(const char*) {
    printf("GGPO SAYS GO");
    Mvc3FrameSimulation::startGame();
    return true;
}

bool advance_frame(int) {
    int allInput[2] = { 0,0 };
    int disconnect_flags;
    int result = ggpo_synchronize_input(ggpo,         // the session object
        allInput,            // array of inputs
        8,
        &disconnect_flags);   // size of all inputs
    if (GGPO_SUCCEEDED(result)) {
        Mvc3FrameSimulation::setNextInputP1(allInput[0]);
        Mvc3FrameSimulation::setNextInputP2(0);
    }
    else {
        printf("Failed to sync %x\n", result);
    }
    Mvc3FrameSimulation::AdvanceFrame();
    return true;
}

bool load_game_state(unsigned char* buffer, int len)
{
    printf("Loading game state\n");
    Mvc3FrameSimulation::setState((Mvc3FrameSimulation::Mvc3GameState*)buffer);
    return true;
}


bool save_game_state(unsigned char** buffer, int* len, int* checksum, int)
{
    printf("Saving 'game state'\n");
    *len = sizeof(Mvc3FrameSimulation::Mvc3GameState);
     *buffer = (unsigned char*)malloc(*len);
    if (!*buffer) {
        printf("Err allocating mem\n");
        return false;
    }
    memcpy(*buffer, Mvc3FrameSimulation::getState(), *len);
    *checksum = 1;
    return true;
}

void free_buffer(void* buffer) {
    free(buffer);
}

bool log_game_state(char* filename, unsigned char* buffer, int)
{
    FILE* fp = nullptr;
    fopen_s(&fp, filename, "w");
    if (fp) {
        Mvc3FrameSimulation::Mvc3GameState* gamestate = (Mvc3FrameSimulation::Mvc3GameState*)buffer;
        fprintf(fp, "GameState object.\n");
        fclose(fp);
    }
    return true;
}

bool on_event_callback(GGPOEvent* info)
{
    return true;
}

bool onLocalInput(int team, int input) {
    if (team == 0) {
        //Send input to GGPO, then reconcile and send to game.
        int localInput[1] = {input};
        int result = ggpo_add_local_input(ggpo,               // the session object
            local_player_handle,  // handle for p1
            &localInput,              // p1's inputs
            4);      // size of p1's inputs

        printf("Setting input! %x\n", localInput[0]);
        /* synchronize the local and remote inputs */
        if (GGPO_SUCCEEDED(result)) {
            int allInput[2] = { 0,0 };
            int disconnect_flags;
            result = ggpo_synchronize_input(ggpo,         // the session object
                allInput,            // array of inputs
                8,
                &disconnect_flags);   // size of all inputs
            if (GGPO_SUCCEEDED(result)) {
                printf("Retrieved Input %x\n", allInput[0]);

                Mvc3FrameSimulation::setNextInputP1(allInput[0]);
                Mvc3FrameSimulation::setNextInputP2(0);
                return true;
            }
            else {
                printf("Failed to sync %x\n", result);
                return false;
            }
        }
        else {
            printf("Failed to set :( %x\n", result);
            return false;
        }

    }
    return true;
}

void onGameReady() {
    printf("GAME READY!!!\n");

    GGPOErrorCode result;
    GGPOSessionCallbacks cb = {};

    cb.begin_game = begin_game;
    cb.advance_frame = advance_frame;
    cb.load_game_state = load_game_state;
    cb.save_game_state = save_game_state;
    cb.free_buffer = free_buffer;
    cb.log_game_state = log_game_state;
    cb.on_event = on_event_callback;


    char name[5] = { 'u','m','v','c','3' };

    /*
        result = ggpo_start_session(&ggpo,         // the new session object
        &cb,           // our callbacks
        "test_app",    // application name
        2,             // 2 players
        sizeof(int),   // size of an input packet
        8001);         // our local udp port
    */

    result = ggpo_start_synctest(&ggpo, &cb, name, 2, 4, 1);
    GGPOPlayer p1;
    p1.type = GGPO_PLAYERTYPE_LOCAL;
    ggpo_add_player(ggpo, &p1, &local_player_handle);
    ggpo_set_frame_delay(ggpo, local_player_handle, 5);

    ggpo_set_disconnect_timeout(ggpo, 0);
    ggpo_set_disconnect_notify_start(ggpo, 0);
}

void onAdvanceFrameComplete() {
    ggpo_advance_frame(ggpo);
}

void onFrameComplete() {
    ggpo_idle(ggpo, 3);
}

DWORD WINAPI Initialise(LPVOID lpreserved) {

    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    printf("Initialise() | Begin!\n");
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "ggpo") {
            std::cout << "GGPO TIME" << std::endl;
            Mvc3FrameSimulation::setToggleMode(3);
            Mvc3FrameSimulation::setFakePad(3, 1);

            Mvc3FrameSimulation::setPadToTeam(0, 0);
            Mvc3FrameSimulation::setPadToTeam(3, 1);

            Mvc3FrameSimulation::StartMatch();
            Mvc3FrameSimulation::OnLocalPlayerInput(onLocalInput);
            Mvc3FrameSimulation::OnGameReady(onGameReady);
            Mvc3FrameSimulation::OnAdvanceFrameComplete(onAdvanceFrameComplete);
            Mvc3FrameSimulation::OnFrameComplete(onFrameComplete);
        }
        if (line == "go") {
            std::cout << "GO TIME" << std::endl;
            Mvc3FrameSimulation::setToggleMode(1);
            Mvc3FrameSimulation::setFakePad(3, 1);

            Mvc3FrameSimulation::setPadToTeam(0, 0);

            Mvc3FrameSimulation::setPadToTeam(3, 1);

            Mvc3FrameSimulation::StartMatch();
            Mvc3FrameSimulation::startGame();
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
        if (line == "test") {
            Mvc3FrameSimulation::setToggleMode(4);
            std::cout << "render only" << std::endl;
        }

        if (line == "record") {
            Mvc3FrameSimulation::startRecording(0);
            std::cout << "Recording!" << std::endl;
        }
        if (line == "recstop") {
            Mvc3FrameSimulation::stopRecording();
            std::cout << "Stopping" << std::endl;

        }
        if (line == "play") {
            Mvc3FrameSimulation::startPlaying(3);
            std::cout << "playing!" << std::endl;
        }
        if (line == "start") {
            Mvc3FrameSimulation::startGame();
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

