// VFI18.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "VFI18.h"
#include "mainwindow.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MainWindow appWindow(hInstance);

    CLASSATTRIBS classattribs;
    classattribs.idIcon = ICON_LARGE_VFI18;
    classattribs.idSmallIcon = ICON_SMALL_VFI18;
    classattribs.idMenu = MENU_VFI18;
    classattribs.idAccelerators = ACCELERATORS_VFI18;
    classattribs.idClass = STRING_CLASS;
    classattribs.idTitle = STRING_TITLE;

    if (!appWindow.SetWindowClassAttributes(&classattribs))
    {
        return FALSE;
    }

    if (!appWindow.RegisterCreate(hInstance, nullptr))
    {
        return FALSE;
    }

    if (!appWindow.CreateChildren())
    {
        return FALSE;
    }

    return appWindow.Go(nCmdShow);
 }