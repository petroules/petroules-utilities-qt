#include "windowmanager_mac_p.h"
#include "mac/machelpers.h"
#include "nativewindowinfo.h"

// https://developer.apple.com/library/mac/#samplecode/SonOfGrab/Introduction/Intro.html
// Hint: Remove the inline modifier on ChangeBits or it won't compile with Lion / Xcode 4

#import <ApplicationServices/ApplicationServices.h>
#import <Cocoa/Cocoa.h>

#ifdef Q_WS_MAC
NativeWindowInfo* mac_NSDictionaryToNativeWindowInfo(const NSDictionary *entry)
{
    NativeWindowInfo *windowInfo = new NativeWindowInfo();

    // Guaranteed to be available
    windowInfo->setWindowId([[entry objectForKey: (id)kCGWindowNumber] intValue]);
    windowInfo->setBackingStoreType([[entry objectForKey: (id)kCGWindowStoreType] intValue]);
    windowInfo->setLevel((NativeWindowInfo::WindowLevel)[[entry objectForKey: (id)kCGWindowLayer] intValue]); // TODO: Don't cast
    windowInfo->setLevelName(stringForWindowLevel(windowInfo->level()));

    CGRect bounds;
    CGRectMakeWithDictionaryRepresentation(reinterpret_cast<CFDictionaryRef>([entry objectForKey: (id)kCGWindowBounds]), &bounds);
    windowInfo->setBounds(QRect(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height));

    windowInfo->setSharingState([[entry objectForKey: (id)kCGWindowSharingState] intValue]);
    windowInfo->setAlpha([[entry objectForKey: (id)kCGWindowAlpha] floatValue]);
    windowInfo->setOwnerPid([[entry objectForKey: (id)kCGWindowOwnerPID] intValue]);
    windowInfo->setMemoryUsage([[entry objectForKey: (id)kCGWindowMemoryUsage] longLongValue]);

    // Might not be available, so we should check
    NSNumber *workspace = reinterpret_cast<NSNumber*>([entry objectForKey: (id)kCGWindowWorkspace]);
    windowInfo->setWorkspace(workspace ? [workspace intValue] : 0);

    // If NSString pointers are null, we get an empty QString so this is OK
    windowInfo->setOwnerName(qt_mac_NSStringToQString([entry objectForKey: (id)kCGWindowOwnerName]));
    windowInfo->setWindowName(qt_mac_NSStringToQString([entry objectForKey: (id)kCGWindowName]));

    CFBooleanRef onScreen = reinterpret_cast<CFBooleanRef>([entry objectForKey: (id)kCGWindowIsOnscreen]);
    windowInfo->setOnScreen(onScreen ? ((BOOL)CFBooleanGetValue(onScreen) == YES) : false);

    CFBooleanRef videoMem = reinterpret_cast<CFBooleanRef>([entry objectForKey: (id)kCGWindowBackingLocationVideoMemory]);
    windowInfo->setIsBackingLocationVideoMemory(videoMem ? ((BOOL)CFBooleanGetValue(videoMem) == YES) : false);

    return windowInfo;
}

NativeWindowInfo* mac_NativeWindowInfoForSingleWindow(WId windowId)
{
    // Create an NSArray containing our single window ID to give to CGWindowListCreateDescriptionFromArray
    CGWindowID *windowIds = (CGWindowID*)calloc(1, sizeof(CGWindowID));
    windowIds[0] = windowId;
    CFArrayRef arr = CFArrayCreate(kCFAllocatorDefault, (const void**)windowIds, 1, NULL);
    free(windowIds);

    // Get info dictionaries for our array of window IDs
    const NSArray *cfWindowList = (NSArray*)CGWindowListCreateDescriptionFromArray(arr);
    CFRelease(arr);

    // If there was only one as we expected, convert it to our structure and send it out
    if (cfWindowList && [cfWindowList count] == 1)
    {
        NSDictionary *entry = [cfWindowList objectAtIndex: 0];
        NativeWindowInfo *info = mac_NSDictionaryToNativeWindowInfo(entry);
        [cfWindowList release];
        return info;
    }

    return NULL;
}

void WindowManager::setTopMostMac(QWidget *const window, bool topMost)
{
    [qt_mac_windowForWidget(window) setLevel: (topMost ? NSPopUpMenuWindowLevel : NSNormalWindowLevel)];
    WindowManager::updateTopMostFlags(window, topMost);
}

WId WindowManager::nullWindowId()
{
    return kCGNullWindowID;
}

QList<NativeWindowInfo*> WindowManager::windowList(WindowListOptions listOptions, WId relativeToWindow)
{
#ifndef QT_NO_DEBUG
    QElapsedTimer timer;
    timer.start();
#endif
    QList<NativeWindowInfo*> windowInfoList;

    // Ask the window server for the list of windows
    // We use the same values internally for list options so we can cast
    const NSArray *cfWindowList = reinterpret_cast<const NSArray*>(CGWindowListCopyWindowInfo((CGWindowListOption)listOptions, relativeToWindow));
    if (cfWindowList)
    {
        for (NSDictionary *entry in cfWindowList)
        {
            windowInfoList.append(mac_NSDictionaryToNativeWindowInfo(entry));
        }

        CFRelease(cfWindowList);
    }

#ifndef QT_NO_DEBUG
    qDebug() << "Took" << timer.elapsed() << "ms to enumerate window list";
#endif
    return windowInfoList;
}

QPixmap WindowManager::getCompositeImage(WId windowId, WindowManager::WindowListOptions listOptions, WindowManager::WindowImageTypes imageOptions, QRect bounds)
{
#ifndef QT_NO_DEBUG
    QElapsedTimer timer;
    timer.start();
#endif
    NSRect nsBounds = NSMakeRect(bounds.x(), bounds.y(), bounds.width(), bounds.height());
    if (bounds.isNull())
    {
        nsBounds = CGRectNull;
    }
    else if (bounds.width() == std::numeric_limits<int>::max() && bounds.height() == std::numeric_limits<int>::max())
    {
        nsBounds = CGRectInfinite;
    }

    // We use the same values internally so we can cast
    CGWindowListOption cgListOptions = (CGWindowListOption)listOptions;
    CGWindowImageOption cgImageOptions = (CGWindowImageOption)imageOptions;

    CGImageRef windowImage = CGWindowListCreateImage(nsBounds, cgListOptions, windowId, cgImageOptions);
    QPixmap pixmap = QPixmap::fromMacCGImageRef(windowImage);
    CGImageRelease(windowImage);
#ifndef QT_NO_DEBUG
    qDebug() << "Took" << timer.elapsed() << "ms to create composited image";
#endif
    return pixmap;
}

QRect WindowManager::getWindowRect(WId windowId)
{
    NativeWindowInfo *info = mac_NativeWindowInfoForSingleWindow(windowId);
    return info ? info->bounds() : QRect();
}

QRect WindowManager::getClientRect(WId windowId)
{
    // No real way to get the "client" rect on OS X
    return getWindowRect(windowId);
}

QString WindowManager::getWindowText(WId windowId)
{
    NativeWindowInfo *info = mac_NativeWindowInfoForSingleWindow(windowId);
    return info ? info->windowName() : QString();
}

WId WindowManager::rootWindow()
{
    return QApplication::desktop()->winId();
}

uint WindowManager::idleTime()
{
    // http://developer.apple.com/library/mac/#documentation/Carbon/Reference/QuartzEventServicesRef/Reference/reference.html
    return (uint)CGEventSourceSecondsSinceLastEventType(kCGEventSourceStateCombinedSessionState, kCGAnyInputEventType) * 1000;
}

WId WindowManager::activeWindow()
{
    // TODO: Test this function - it's untested
    ProcessSerialNumber psn;
    pid_t pid;
    if (GetFrontProcess(&psn) == noErr)
    {
        if (GetProcessPID(&psn, &pid) == noErr)
        {
            foreach (NativeWindowInfo *info, windowList())
            {
                if (info->ownerPid() == pid)
                {
                    return info->windowId();
                }
            }
        }
    }

    return nullWindowId();
}
#endif
