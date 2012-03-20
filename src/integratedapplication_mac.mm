#include "integratedapplication.h"
#import <Cocoa/Cocoa.h>

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
}
