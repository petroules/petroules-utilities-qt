#include "integratedmainwindow.h"
#include "integratedmainwindow_p.h"

IntegratedMainWindow::IntegratedMainWindow(QWidget *parent) :
    QMainWindow(parent), d(new Private())
{
    d->window = this;
}

IntegratedMainWindow::~IntegratedMainWindow()
{
    delete d;
}

void IntegratedMainWindow::setNativeFullScreenEnabled(bool enable)
{
#ifdef Q_WS_MAC
    d->setNativeFullScreenEnabled_mac(enable);
#else
    Q_UNUSED(enable);
#endif
}

bool IntegratedMainWindow::isNativeFullScreen() const
{
#ifdef Q_WS_MAC
    return d->isNativeFullScreen_mac();
#else
    return false;
#endif
}

void IntegratedMainWindow::setNativeFullScreen(bool set)
{
#ifdef Q_WS_MAC
    return d->setNativeFullScreen_mac(set);
#else
    Q_UNUSED(set);
#endif
}
