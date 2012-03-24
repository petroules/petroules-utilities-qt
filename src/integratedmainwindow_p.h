#ifndef INTEGRATEDMAINWINDOW_P_H
#define INTEGRATEDMAINWINDOW_P_H

#include <QPoint>
#include <QSize>

struct ITaskbarList3;
class ThumbBar;
class IntegratedMainWindow;

class IntegratedMainWindow::Private
{
public:
    Private();
    ~Private();

    IntegratedMainWindow *window;
#ifdef Q_WS_MAC
    bool isNativeFullScreenEnabled_mac() const;
    void setNativeFullScreenEnabled_mac(bool enable);
    bool isNativeFullScreen_mac() const;
    void setNativeFullScreen_mac(bool set);
#endif

    ITaskbarList3 *m_taskbar;
    unsigned int m_taskbarMessageId;
    QSize m_maxThumbnailSize;
    bool m_windowFrameForIconicThumbnails;
    ThumbBar *m_thumbBarManager;

    QPoint offset;
    bool moving;

};

#endif // INTEGRATEDMAINWINDOW_P_H
