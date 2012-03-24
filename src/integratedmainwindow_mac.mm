#include "integratedmainwindow.h"
#include "integratedmainwindow_p.h"
#import "mac/machelpers.h"

// https://developer.apple.com/library/mac/#documentation/General/Conceptual/MOSXAppProgrammingGuide/FullScreenApp/FullScreenApp.html
// https://bugreports.qt.nokia.com/browse/QTBUG-21607

void IntegratedMainWindow::Private::setNativeFullScreenEnabled_mac(bool enable)
{
    [qt_mac_windowForWidget(window) setCollectionBehavior: (enable ? NSWindowCollectionBehaviorFullScreenPrimary : NSWindowCollectionBehaviorDefault)];
}

bool IntegratedMainWindow::Private::isNativeFullScreen_mac() const
{
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
    return [qt_mac_windowForWidget(window) styleMask] & NSFullScreenWindowMask;
#else
    return false;
#endif
}

void IntegratedMainWindow::Private::setNativeFullScreen_mac(bool set)
{
    bool current = window->isNativeFullScreen();

    // If the value we want differs from the current, toggle to obtain the desired setting
    if (current != set)
    {
        [qt_mac_windowForWidget(window) toggleFullScreen: nil];
    }
}
