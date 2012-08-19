#ifndef AEROBLURBEHIND_H
#define AEROBLURBEHIND_H

#include "petroules-utilities-global.h"

class QWidget;
class WindowNotifier;

class PETROULES_UTILITIES_API DesktopWindowManager
{
public:
    static bool setBlurBehindEnabled(QWidget *widget, bool enable = true);
    static bool extendFrameIntoClientArea(QWidget *widget, int left = -1, int top = -1, int right = -1, int bottom = -1);
    static bool isCompositionEnabled();
    static bool enableComposition(bool enable);
    static QColor colorizationColor();

private:
    static WindowNotifier* windowNotifier();
    DesktopWindowManager() { }
};

#endif // AEROBLURBEHIND_H
