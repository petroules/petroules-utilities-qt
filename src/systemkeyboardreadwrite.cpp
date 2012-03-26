#include "systemkeyboardreadwrite.h"
#include <QDebug>
#include <iostream>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

class SystemKeyboardReadWrite::Private
{
public:
#ifdef Q_WS_WIN
    Private() : keyboardHook(NULL) { }
    HHOOK keyboardHook;
#else
    Private() { }
#endif
};

SystemKeyboardReadWrite::SystemKeyboardReadWrite()
    : QObject()
{
    d = new Private();
}

SystemKeyboardReadWrite::~SystemKeyboardReadWrite()
{
}

#ifdef Q_WS_WIN
LRESULT CALLBACK SystemKeyboardReadWrite::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

        // Check for a key down press
        if (wParam == WM_KEYDOWN)
        {
            qDebug().nospace() << (char)pKeyboard->vkCode;
        }
        else if (wParam == WM_KEYUP)
        {
        }
    }

    return false;
}
#endif

byte* SystemKeyboardReadWrite::keyboardState() const
{
#ifdef Q_WS_WIN
    // Number of keys supported
    const int numberKeys = 256;
    byte *keysDepressed = new byte[numberKeys];
    if (GetKeyboardState(keysDepressed))
    {
        return keysDepressed;
    }
#endif

    return NULL;
}

bool SystemKeyboardReadWrite::isConnected() const
{
#ifdef Q_WS_WIN
    return d->keyboardHook;
#else
    return false;
#endif
}

bool SystemKeyboardReadWrite::setConnected(bool state)
{
#ifdef Q_WS_WIN
    if (state)
    {
        if (!d->keyboardHook)
        {
            d->keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, GetModuleHandle(NULL), 0);
        }
    }
    else
    {
        if (d->keyboardHook)
        {
            UnhookWindowsHookEx(d->keyboardHook);
            d->keyboardHook = NULL;
        }
    }
#else
    Q_UNUSED(state);
#endif

    return isConnected();
}
