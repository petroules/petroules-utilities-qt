#ifndef SYSTEMKEYBOARDREADWRITE_H
#define SYSTEMKEYBOARDREADWRITE_H

#include "petroules-utilities-global.h"
#include <QObject>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

class PETROULESUTILITIESSHARED_EXPORT SystemKeyboardReadWrite : public QObject
{
    Q_OBJECT

public:
    SystemKeyboardReadWrite();
    ~SystemKeyboardReadWrite();
    unsigned char* keyboardState() const;
    bool isConnected() const;
    bool setConnected(bool state);

signals:
    void keyPressed(unsigned char *keysDepressed, unsigned long keyPressed);

private:
#ifdef Q_WS_WIN
    static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);
#endif

    class Private;
    Private *d;
};

#endif // SYSTEMKEYBOARDREADWRITE_H
