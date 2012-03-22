#ifndef PETROULESUTILITIES_H
#define PETROULESUTILITIES_H

#include "widgets/itoolbarsearchwidget.h"
#include "inactivityeventfilter.h"
#include "integratedapplication.h"
#include "linuxsysteminfo.h"
#include "platforminformation.h"
#include "qversion.h"
#include "translationutils.h"
#include "windowmanager.h"
#include "dialogs/nativedialogs.h"
#include "dialogs/nativefiledialog.h"
#include "dialogs/nativeopendirectorydialog.h"
#include "dialogs/nativeopenfiledialog.h"
#include "dialogs/nativeprintdialog.h"
#include "dialogs/nativesavefiledialog.h"
#include "dialogs/nativesystemdialog.h"
#include "win32/desktopwindowmanager.h"
#ifdef Q_WS_WIN
#include "win32/explorerstyle.h"
#endif
#include "win32/integratedmainwindow.h"
#include "win32/jumplist.h"
#include "win32/thumbbar.h"
#include "win32/windowsfileregistration.h"

#define PETROULESUTILITIES_VERSION_STR "1.0"

#endif // PETROULESUTILITIES_H
