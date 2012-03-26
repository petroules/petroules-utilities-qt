#include "windowmanager_mac_p.h"
#include "mac/machelpers.h"

#ifdef Q_WS_MAC
void WindowManager::setTopMostMac(QWidget *const window, bool topMost)
{
    [qt_mac_windowForWidget(window) setLevel: (topMost ? NSPopUpMenuWindowLevel : NSNormalWindowLevel)];
    WindowManager::updateTopMostFlags(window, topMost);
}

/*
QList<WId> WindowManager::getTopLevelWindowList()
{
    // Ask the window server for the list of windows
    CFArrayRef windowList = CGWindowListCopyWindowInfo(listOptions, kCGNullWindowID);
    QList<WId> list;

    for (CFIndex i = 0; i < CFArrayGetCount(windowList); i++)
    {
    list.append();
    }

    CFRelease(windowList);
    return list;
}

QPixmap WindowManager::getCompositeImage(QList<WId> list)
{
    CGImageRef windowImage = CGWindowListCreateImage(imageBounds, singleWindowListOptions, windowID, imageOptions);
    QPixmap pixmap = QPixmap::fromMacCGImageRef(windowImage);
    CGImageRelease(windowImage);
    return pixmap;
}
*/

QRect WindowManager::getWindowRect(WId windowId)
{
    CGImageRef screen = NULL;

    // Get a list of all the windows on the system
    CFArrayRef allWindowIDs = CGWindowListCreate(kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    if (allWindowIDs)
    {
        // Create an array to store the sole window ID we find, for further processing
        CFMutableArrayRef windowIDs = CFArrayCreateMutable(NULL, 0, NULL);

        // Get a list containing window information and iterate through
        CFArrayRef windowInfo = CGWindowListCreateDescriptionFromArray(allWindowIDs);
        for (CFIndex i = 0; i < CFArrayGetCount(windowInfo); i++)
        {
            // Retrieve the info dictionary for the window at the current index
            CFDictionaryRef dict = static_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(windowInfo, i));
            CFStringRef name = static_cast<CFStringRef>(CFDictionaryGetValue(dict, kCGWindowName));
            CGRect bounds;
            CFDictionaryRef ref = static_cast<CFDictionaryRef>(CFDictionaryGetValue(dict, kCGWindowBounds));
            CGRectMakeWithDictionaryRepresentation(ref, &bounds);
            //if ()
            {
                //
                CFArrayAppendValue(windowIDs, CFArrayGetValueAtIndex(allWindowIDs, i));
            }

        }

        CFRelease(windowInfo);
        CFRelease(allWindowIDs);

        screen = CGWindowListCreateImageFromArray(CGRectInfinite, windowIDs, kCGWindowImageDefault);
        CFRelease(windowIDs);
    }

    /* ????????????????
    if (!screen)
            return;

    // draw the image
    CGContextRef ctx = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGRect bounds = NSRectToCGRect([self bounds]);
    CGContextDrawImage(ctx, bounds, screen);
    CGImageRelease(screen);

    // draw a transparent black
    if (!color)
            color = CGColorCreateGenericRGB(0.0, 0.0, 0.0, 0.5);
    CGContextSetFillColorWithColor(ctx, color);
    CGContextFillRect(ctx, bounds);
    */

    return QRect();
}

QRect WindowManager::getClientRect(WId windowId)
{
    return QRect();
}

QString WindowManager::getWindowText(WId windowId)
{
    return QString();
}

WId WindowManager::rootWindow()
{
    QApplication::desktop()->winId();
}

WId WindowManager::windowFromPoint(const QPoint &p, WId parent, bool(*filterFunction)(WId))
{
    return rootWindow();
}
#endif
