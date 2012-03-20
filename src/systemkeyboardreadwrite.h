#ifndef SYSTEMKEYBOARDREADWRITE_H
#define SYSTEMKEYBOARDREADWRITE_H

#include <QObject>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

#define byte unsigned char
#define DWORD unsigned long

class SystemKeyboardReadWrite : public QObject
{
    Q_OBJECT

public:
    SystemKeyboardReadWrite();
    ~SystemKeyboardReadWrite();
    bool isConnected();
    bool setConnected(bool state);

signals:
    void keyPressed(byte *keysDepressed, DWORD keyPressed);

private:
#ifdef Q_WS_WIN
    static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);
#endif

    class Private;
    Private *d;
};

#endif // SYSTEMKEYBOARDREADWRITE_H
