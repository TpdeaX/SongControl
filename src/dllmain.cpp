#include "pch.h"
#include <fstream>
#include "Hooks.h"

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HWND* mainWindowHWND = (HWND*)lParam;
    DWORD targetProcessId = GetCurrentProcessId();

    if (processId == targetProcessId)
    {
        *mainWindowHWND = hwnd;
        return FALSE;  // Detener la enumeraci n
    }

    return TRUE;  // Continuar enumerando
}

HWND GetMainWindowHWND()
{
    HWND mainWindowHWND = NULL;

    EnumWindows(EnumWindowsCallback, (LPARAM)&mainWindowHWND);

    return mainWindowHWND;
}


DWORD WINAPI threadFunc(void* hModule) {


    //sharedStateBP().LoadSettings();
    //matdash::create_console();
    //SetCustomCursorInsideWindow(GetMainWindowHWND());

    //// Load the custom cursor image
    //auto cursorImage = LoadCursorFromFileA("C:\\Users\\Joseph\\Downloads\\image-removebg-preview.ico");

    //SetClassLongPtrA(GetMainWindowHWND(), GCLP_HCURSOR, (LONG_PTR)cursorImage);


    //SetCursor(cursorImage);
    sharedStateA().LoadSettings();

    matdash::add_hook<&AppDelegate_trySaveGameHook, matdash::Thiscall>(gd::base + 0x3D5E0);

    matdash::add_hook<&GJSongBrowserHook::customSetupHook, matdash::Thiscall>(gd::base + 0x14ccb0);
    matdash::add_hook<&GJSongBrowserHook::loadPageHook, matdash::Thiscall>(gd::base + 0x14d070);

    matdash::add_hook<&CustomSongWidgetHook::updatePlaybackBtn, matdash::Thiscall>(gd::base + 0x69970);
    matdash::add_hook<&CustomSongWidgetHook::onPlaybackHook, matdash::Thiscall>(gd::base + 0x697B0);

    matdash::add_hook<&MusicDownloadManagerHook::getDownloadedSongsHook, matdash::Thiscall>(gd::base + 0x195640);
    matdash::add_hook<&GJDropDownLayer_exitLayer, matdash::Thiscall>(gd::base + 0x113980);
    matdash::add_hook<&CCKeyboardDispatcher_dispatchKeyboardMSGHook, matdash::Thiscall>(Utils::libcocos_base + 0x99F30);
    matdash::add_hook<&CCNode_addChildHook, matdash::Thiscall>(Utils::libcocos_base + 0x5ECF0);


    //matdash::add_hook<&getColorAction, matdash::Thiscall>(gd::base + 0x11cde0);

    //matdash::add_hook<&PauseLayerHook::createHook, matdash::Thiscall>(gd::base + 0x1E4570);
    //matdash::add_hook<&PauseLayerHook::customSetupHook, matdash::Thiscall>(gd::base + 0x1E4620);
    //matdash::add_hook<&PauseLayerHook::musicSliderChangedHook, matdash::Thiscall>(gd::base + 0x1E5CE0);
    //matdash::add_hook<&PauseLayerHook::sfxSliderChangedHook, matdash::Thiscall>(gd::base + 0x1DDFA0);
    //matdash::add_hook<&PauseLayerHook::createToggleButtonHook, matdash::Thiscall>(gd::base + 0x1E5570);

    //matdash::add_hook<&CurrencyRewardLayer_init, matdash::Thiscall>(gd::base + 0x50130);
    //matdash::add_hook<&MoreOptionsLayer_addToggle, matdash::Thiscall>(gd::base + 0x1DF6B0);
    //matdash::add_hook<&MoreOptionsLayer_init, matdash::Thiscall>(gd::base + 0x1DE8F0);

    //matdash::add_hook<&CustomSongWidgetHook::updateSongInfoHook, matdash::Thiscall>(gd::base + 0x69BF0);

    ////matdash::add_hook<&ChallengeNodeHook::onClaimRewardA, matdash::Thiscall>(gd::base + 0x40380);
    //matdash::add_hook<&AppDelegateHook::trySaveGameH, matdash::Thiscall>(gd::base + 0x3D5E0);

    ////matdash::add_hook<&MenuLayer_init_H, matdash::Thiscall>(gd::base + 0x1907B0);
    ////matdash::add_hook<&MenuLayer_FLAlert_ClickedH, matdash::Thiscall>(gd::base + 0x192150);


    ////matdash::add_hook<&getDownloadProgress, matdash::Thiscall>(gd::base + 0x82d0);
    ////matdash::add_hook<&createHH, matdash::Optcall>(gd::base + 0x33270);

    ////matdash::add_hook<&Hooks::NoNodeI_registerWithTouchDispatcher, matdash::Thiscall>(gd::base + 0x16990);

    ////cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("BE_GameSheet01.plist", "BE_GameSheet01.png");//no working :b
    ////cocos2d::CCTextureCache::sharedTextureCache()->addImage("BE_GameSheet01.png", 0);


    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if(CreateThread(0, 0x100, threadFunc, hModule, 0, 0))
        break;
    }

    return TRUE;
}

