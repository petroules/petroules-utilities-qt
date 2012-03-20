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
}

SystemKeyboardReadWrite::~SystemKeyboardReadWrite()
{
}

#ifdef Q_WS_WIN
LRESULT CALLBACK SystemKeyboardReadWrite::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        // Check for a key down press
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;

            // Number of keys supported
            const int numberKeys = 256;
            byte *keysDepressed = new byte[numberKeys];
            if (GetKeyboardState(keysDepressed))
            {
                qDebug().nospace() << (char)pKeyboard->vkCode;

                //emit this->keyPressed(keysDepressed, pKeyboard->vkCode);
            }
        }
        else if (wParam == WM_KEYUP)
        {
        }
    }

    return false;
}
#endif

bool SystemKeyboardReadWrite::isConnected()
{
#ifdef Q_WS_WIN
    return this->d->keyboardHook;
#else
    return false;
#endif
}

bool SystemKeyboardReadWrite::setConnected(bool state)
{
#ifdef Q_WS_WIN
    if (state)
    {
        if (!this->d->keyboardHook)
        {
            this->d->keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, GetModuleHandle(NULL), 0);
        }
    }
    else
    {
        if (this->d->keyboardHook)
        {
            UnhookWindowsHookEx(this->d->keyboardHook);
            this->d->keyboardHook = NULL;
        }
    }
#else
    Q_UNUSED(state);
#endif

    return this->isConnected();
}
