#ifndef WINDOWMANAGER_MAC_P_H
#define WINDOWMANAGER_MAC_P_H

#include "windowmanager.h"

#ifdef Q_WS_MAC
#import <ApplicationServices/ApplicationServices.h>
#import <Cocoa/Cocoa.h>

QMap<int, QString> coreGraphicsWindowLevels()
{
    QMap<int, QString> levels;
    levels.insert(kCGBaseWindowLevel, "kCGBaseWindowLevel");
    levels.insert(kCGMinimumWindowLevel, "kCGMinimumWindowLevel");
    levels.insert(kCGDesktopWindowLevel, "kCGDesktopWindowLevel");
    levels.insert(kCGDesktopIconWindowLevel, "kCGDesktopIconWindowLevel");
    levels.insert(kCGBackstopMenuLevel, "kCGBackstopMenuLevel");
    levels.insert(kCGNormalWindowLevel, "kCGNormalWindowLevel");
    levels.insert(kCGFloatingWindowLevel, "kCGFloatingWindowLevel");
    levels.insert(kCGTornOffMenuWindowLevel, "kCGTornOffMenuWindowLevel");
    levels.insert(kCGDockWindowLevel, "kCGDockWindowLevel");
    levels.insert(kCGMainMenuWindowLevel, "kCGMainMenuWindowLevel");
    levels.insert(kCGStatusWindowLevel, "kCGStatusWindowLevel");
    levels.insert(kCGModalPanelWindowLevel, "kCGModalPanelWindowLevel");
    levels.insert(kCGPopUpMenuWindowLevel, "kCGPopUpMenuWindowLevel");
    levels.insert(kCGDraggingWindowLevel, "kCGDraggingWindowLevel");
    levels.insert(kCGScreenSaverWindowLevel, "kCGScreenSaverWindowLevel");
    levels.insert(kCGCursorWindowLevel, "kCGCursorWindowLevel");
    levels.insert(kCGOverlayWindowLevel, "kCGOverlayWindowLevel");
    levels.insert(kCGHelpWindowLevel, "kCGHelpWindowLevel");
    levels.insert(kCGUtilityWindowLevel, "kCGUtilityWindowLevel");
    levels.insert(kCGAssistiveTechHighWindowLevel, "kCGAssistiveTechHighWindowLevel");
    levels.insert(kCGMaximumWindowLevel, "kCGMaximumWindowLevel");
    return levels;
}

/*!
    Returns a string representation of the given CoreGraphics window level.
 */
QString stringForWindowLevel(CGWindowLevel level)
{
    // See CoreGraphics/CGWindowLevel.h
    QMap<int, QString> levels = coreGraphicsWindowLevels();
    if (levels.contains(level))
    {
        return levels.value(level);
    }

    else return QString::number(level);
}

#endif

#endif // WINDOWMANAGER_MAC_P_H
