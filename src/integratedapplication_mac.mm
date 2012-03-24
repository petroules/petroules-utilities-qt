#include "integratedapplication.h"
#include "integratedapplication_p.h"
#import "mac/machelpers.h"
#import <Cocoa/Cocoa.h>

/*
@interface CocoaIntegratedApplication : NSApplication
- (id)sharedApplication;
- (BOOL)applicationShouldHandleReopen:(NSApplication *)theApplication hasVisibleWindows:(BOOL)flag;
@end

@implementation CocoaIntegratedApplication
- (id)sharedApplication
{
    return [[CocoaIntegratedApplication alloc] init];
}
- (BOOL)applicationShouldHandleReopen:(NSApplication *)theApplication hasVisibleWindows:(BOOL)flag
{
    Q_UNUSED(theApplication);

    if (qiApp)
    {
        return qiApp->dockIconClicked(flag);
    }

    return YES;
}
@end

void IntegratedApplication::preInitialization()
{
    if (!qiApp)
    {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        id app = [CocoaIntegratedApplication sharedApplication];
        [NSApp setDelegate: app];
        [pool drain];
    }
    else
    {
        qDebug() << "IntegratedApplication::preInitialization() must be called before the application instance is created.";
    }
}*/

// Objective-C bridge class to handle dock icon click events and pass them to our IntegratedApplication class
@interface DockIconClickEventHandler : NSObject
{
@public
    IntegratedApplication* macApplication;
}

- (void)handleDockClickEvent:(NSAppleEventDescriptor*)event withReplyEvent:(NSAppleEventDescriptor*)replyEvent;

@end

@implementation DockIconClickEventHandler

- (void)handleDockClickEvent:(NSAppleEventDescriptor*)event withReplyEvent:(NSAppleEventDescriptor*)replyEvent
{
    Q_UNUSED(event);
    Q_UNUSED(replyEvent);

    if (macApplication)
    {
        macApplication->handleReopen();
    }
}

@end

void IntegratedApplication::Private::constructorObjc()
{
    eventMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyDownMask handler:^(NSEvent *incomingEvent) {
        return incomingEvent;
    }];

    DockIconClickEventHandler* handler = static_cast<DockIconClickEventHandler*>(dockIconClickEventHandler = [[DockIconClickEventHandler alloc] init]);
    handler->macApplication = integratedApplication;
}

void IntegratedApplication::Private::destructorObjc()
{
    [NSEvent removeMonitor: eventMonitor];
}

void IntegratedApplication::Private::setupCocoaEventHandler()
{
    // TODO: This apparently uses a legacy API and we should be using the
    // applicationShouldHandleReopen:hasVisibleWindows: method on
    // NSApplicationDelegate but this isn't possible without nasty runtime
    // reflection hacks until Qt is fixed. If this breaks, shout at them :)
    [[NSAppleEventManager sharedAppleEventManager]
     setEventHandler: dockIconClickEventHandler
     andSelector: @selector(handleDockClickEvent:withReplyEvent:)
     forEventClass: kCoreEventClass
     andEventID: kAEReopenApplication];
}

void IntegratedApplication::Private::setBadgeText_mac(const QString &text)
{
    [[NSApp dockTile] setBadgeLabel: qt_mac_QStringToNSString(text)];
}
