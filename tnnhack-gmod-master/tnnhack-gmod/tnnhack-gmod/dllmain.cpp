#include <windows.h>
#include "utils.h"
#include "detours.h"
#include "hooks.h"
#include "sperma_cast.h"
#include "console.h"
#include "var.h"
#include "drawing.h"
#include "Features/esp.h"
#include "Features/aim.h"
#include "Features/movement.h"
#include "Features/antiAim.h"
#include "VMTHooks.h"
#include "Features/chams.h"
#include "Features/misc.h"
#include "Features/bones.h"
#include "interface.h"
#include "config.h"

#define GMOD_WINDOW_NAME "Garry's Mod"
#define __CHECKMEMLEAK(d) if(d) delete d;

bool    g_Keys[256] = {};
HWND    g_MainWindow = NULL;

DWORD WINAPI MainThread(LPVOID dll)
{
    g_MainWindow = FindWindowA(NULL, GMOD_WINDOW_NAME);
    if (!g_MainWindow)
    {
        MessageBoxA(NULL, "Failed to find game window.", "Critical Error", MB_OK | MB_ICONERROR);

        FreeLibraryAndExitThread(zalypa_cast<HMODULE>(dll), 0);
        return 0;
    }

    g_pUtils        = new CUtils();
    g_pDetours      = new CDetours();
    g_pVmtHook      = new CVMTHooks();
    g_pConfigSystem = new CConfigSystem();
    g_pConsole      = new CConsole();
    g_pSettings     = new CHackVars();
    g_pDrawing      = new CDrawing();

    g_pEsp          = new CEsp();
    g_pAimbot       = new CAimbot();
    g_pMovement     = new CMovement();
    g_pAntiAim      = new CAntiAim();
    g_pChams        = new CChams();
    g_pMisc         = new CMisc();
    g_pBones        = new CBones();

    g_pConsole->Init();
    g_pConfigSystem->Init();

    Hooks::Run();

#ifdef _DEBUG

    while (!g_Keys[VK_END])
    {
        Sleep(1);
    }

    Hooks::Shutdown();

    if (g_pConsole->GetEnabled())
    {
        g_pConsole->SetEnabled(false);
        Interfaces::InputSystem()->EnableInput(true);
    }

    g_pConfigSystem->SaveCurrentConfig();

    __CHECKMEMLEAK(g_pUtils)
    __CHECKMEMLEAK(g_pDetours)
    __CHECKMEMLEAK(g_pVmtHook)
    __CHECKMEMLEAK(g_pEsp)
    __CHECKMEMLEAK(g_pConsole)
    __CHECKMEMLEAK(g_pSettings)
    __CHECKMEMLEAK(g_pDrawing)
    __CHECKMEMLEAK(g_pAimbot)
    __CHECKMEMLEAK(g_pMovement)
    __CHECKMEMLEAK(g_pAntiAim)
    __CHECKMEMLEAK(g_pChams)
    __CHECKMEMLEAK(g_pMisc)
    __CHECKMEMLEAK(g_pBones)
    __CHECKMEMLEAK(g_pConfigSystem)

    FreeLibraryAndExitThread(zalypa_cast<HMODULE>(dll), 0);

#endif

    return 0;
}

BOOL APIENTRY DllMain(HMODULE dll, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(dll);
            CreateThread(0, 0, &MainThread, dll, 0, 0);

            break;
        }
        case DLL_PROCESS_DETACH:
        {
            if (g_pConfigSystem)
            {
                g_pConfigSystem->SaveCurrentConfig();
            }

            break;
        }
    }

    return TRUE;
}