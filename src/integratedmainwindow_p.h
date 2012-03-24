#ifndef INTEGRATEDMAINWINDOW_P_H
#define INTEGRATEDMAINWINDOW_P_H

class IntegratedMainWindow;

class IntegratedMainWindow::Private
{
public:
    IntegratedMainWindow *window;
#ifdef Q_WS_MAC
    void setNativeFullScreenEnabled_mac(bool enable);
    bool isNativeFullScreen_mac() const;
    void setNativeFullScreen_mac(bool set);
#endif
};

#endif // INTEGRATEDMAINWINDOW_P_H
