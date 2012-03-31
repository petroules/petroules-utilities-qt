#include "nativewindow.h"

NativeWindow::NativeWindow(WId wid)
{
    QWidget::create(wid, false, false);
}

NativeWindow::~NativeWindow()
{
    QWidget::destroy(false, false);
}
