#include "color_button.hpp"
#include <windowsx.h>

static BOOL s_bDialogInit = FALSE;
static COLOR_BUTTON s_color_button_1;
static COLOR_BUTTON s_color_button_2;

static BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    s_color_button_1.SetHWND(GetDlgItem(hwnd, psh1));
    s_color_button_1.SetColor(RGB(255, 0, 0));

    s_color_button_2.SetHWND(GetDlgItem(hwnd, psh2));
    s_color_button_2.SetColor(RGB(0, 255, 0));

    s_bDialogInit = TRUE;
    return TRUE;
}

static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case psh1:
        if (codeNotify == BN_CLICKED)
        {
            if (s_color_button_1.DoChooseColor())
            {
                
            }
        }
        break;
    case psh2:
        if (codeNotify == BN_CLICKED)
        {
            if (s_color_button_2.DoChooseColor())
            {
            }
        }
        break;
    case edt1:
        if (codeNotify == EN_CHANGE)
        {
        }
        break;
    case edt2:
        if (codeNotify == EN_CHANGE)
        {
        }
        break;
    }
}

static void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem)
{
    if (!s_color_button_1.OnParentDrawItem(hwnd, lpDrawItem))
    {
        s_color_button_2.OnParentDrawItem(hwnd, lpDrawItem);
    }
}

static void OnDestroy(HWND hwnd)
{
    s_bDialogInit = FALSE;
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_DRAWITEM, OnDrawItem);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
    }
    return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
    return 0;
}
