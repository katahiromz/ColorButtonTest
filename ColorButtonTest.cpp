// ColorButtonTest.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.
#include <windows.h>
#include <commctrl.h>
#include <windowsx.h>
#include "color_button.hpp"
#include "color_value/color_value.h"
#include <strsafe.h>

static BOOL s_bDialogInit = FALSE;
static COLOR_BUTTON s_color_button_1;
static COLOR_BUTTON s_color_button_2;

static void DoSetColorText(HWND hwnd, INT nItemID, COLORREF value)
{
    char buf[64];
    value = color_value_fix(value);
    color_value_store(buf, 64, value);
    SetDlgItemText(hwnd, nItemID, buf);
}

static BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    s_color_button_1.SetHWND(GetDlgItem(hwnd, psh1));
    s_color_button_2.SetHWND(GetDlgItem(hwnd, psh2));
    s_color_button_1.SetColor(RGB(255, 0, 255));
    s_color_button_2.SetColor(RGB(0, 255, 0));

    DoSetColorText(hwnd, edt1, s_color_button_1.GetColor());
    DoSetColorText(hwnd, edt2, s_color_button_2.GetColor());

    s_bDialogInit = TRUE;
    return TRUE;
}

static void OnOK(HWND hwnd)
{
    char buf[64];

    GetDlgItemTextA(hwnd, edt1, buf, 64);
    uint32_t value1 = color_value_parse(buf);
    if (value1 == uint32_t(-1))
    {
        SendDlgItemMessage(hwnd, edt1, EM_SETSEL, 0, -1);
        SetFocus(GetDlgItem(hwnd, edt1));
        MessageBox(hwnd, TEXT("Invalid color value"), NULL, MB_ICONERROR);
        return;
    }

    GetDlgItemTextA(hwnd, edt2, buf, 64);
    uint32_t value2 = color_value_parse(buf);
    if (value2 == uint32_t(-1))
    {
        SendDlgItemMessage(hwnd, edt2, EM_SETSEL, 0, -1);
        SetFocus(GetDlgItem(hwnd, edt2));
        MessageBox(hwnd, TEXT("Invalid color value"), NULL, MB_ICONERROR);
        return;
    }

    TCHAR szText[128];
    StringCbPrintf(szText, sizeof(szText), TEXT("#%06X, #%06X"), value1, value2);
    MessageBox(hwnd, szText, TEXT("Color Values"), MB_ICONINFORMATION);

    value1 = color_value_fix(value1);
    value2 = color_value_fix(value2);

    EndDialog(hwnd, IDOK);
}

static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (!s_bDialogInit)
        return;

    switch (id)
    {
    case IDOK:
        OnOK(hwnd);
        break;
    case IDCANCEL:
        EndDialog(hwnd, id);
        break;
    case psh1:
        if (codeNotify == BN_CLICKED)
        {
            if (s_color_button_1.DoChooseColor())
            {
                s_bDialogInit = FALSE;
                DoSetColorText(hwnd, edt1, s_color_button_1.GetColor());
                s_bDialogInit = TRUE;
            }
        }
        break;
    case psh2:
        if (codeNotify == BN_CLICKED)
        {
            if (s_color_button_2.DoChooseColor())
            {
                s_bDialogInit = FALSE;
                DoSetColorText(hwnd, edt2, s_color_button_2.GetColor());
                s_bDialogInit = TRUE;
            }
        }
        break;
    case edt1:
        if (codeNotify == EN_CHANGE || codeNotify == EN_KILLFOCUS)
        {
            char buf[64];
            GetDlgItemTextA(hwnd, edt1, buf, 64);
            uint32_t value = color_value_parse(buf);
            if (value != uint32_t(-1))
            {
                value = color_value_fix(value);
                s_color_button_1.SetColor(value);

                if (codeNotify == EN_KILLFOCUS)
                {
                    s_bDialogInit = FALSE;
                    DoSetColorText(hwnd, edt1, value);
                    s_bDialogInit = TRUE;
                }
            }
        }
        break;
    case edt2:
        if (codeNotify == EN_CHANGE || codeNotify == EN_KILLFOCUS)
        {
            char buf[64];
            GetDlgItemTextA(hwnd, edt2, buf, 64);
            uint32_t value = color_value_parse(buf);
            if (value != uint32_t(-1))
            {
                value = color_value_fix(value);
                s_color_button_2.SetColor(value);

                if (codeNotify == EN_KILLFOCUS)
                {
                    s_bDialogInit = FALSE;
                    DoSetColorText(hwnd, edt2, value);
                    s_bDialogInit = TRUE;
                }
            }
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
    InitCommonControls();
    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
    return 0;
}
