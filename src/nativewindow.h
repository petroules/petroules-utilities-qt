#ifndef NATIVEWINDOW_H
#define NATIVEWINDOW_H

#include "petroules-utilities-global.h"

class PETROULESUTILITIESSHARED_EXPORT NativeWindow : public QWidget
{
public:
    explicit NativeWindow(WId wid);
    virtual ~NativeWindow();
};

#endif // NATIVEWINDOW_H
