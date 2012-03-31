#include "nativewindowinfo.h"
#include "windowmanager.h"

class NativeWindowInfo::Private
{
public:
    WId windowId;
    int backingStoreType;
    NativeWindowInfo::WindowLevel windowLevel;
    QString levelName;
    QRect bounds;
    int sharingState;
    float alpha;
    int ownerPid;
    long long memoryUsage;

    // On Mac these are optional...
    int workspace;
    QString ownerName;
    QString windowName;
    bool isOnScreen;
    bool isBackingLocationVideoMemory;
};

NativeWindowInfo::NativeWindowInfo(QObject *parent) :
    QObject(parent)
{
    d = new Private();
    d->windowId = WindowManager::nullWindowId();
    d->backingStoreType = 0;
    d->windowLevel = (WindowLevel)0; // TODO???
    d->sharingState = 0;
    d->alpha = 0;
    d->ownerPid = 0;
    d->memoryUsage = 0;
    d->workspace = 0;
    d->isOnScreen = false;
    d->isBackingLocationVideoMemory = false;
}

NativeWindowInfo::~NativeWindowInfo()
{
    delete d;
}

WId NativeWindowInfo::windowId() const
{
    return d->windowId;
}

void NativeWindowInfo::setWindowId(WId wid)
{
    d->windowId = wid;
}

int NativeWindowInfo::backingStoreType() const
{
    return d->backingStoreType;
}

void NativeWindowInfo::setBackingStoreType(int type)
{
    d->backingStoreType = type;
}

NativeWindowInfo::WindowLevel NativeWindowInfo::level() const
{
    return d->windowLevel;
}

void NativeWindowInfo::setLevel(NativeWindowInfo::WindowLevel level)
{
    d->windowLevel = level;
}

QString NativeWindowInfo::levelName() const
{
    return d->levelName;
}

void NativeWindowInfo::setLevelName(const QString &name)
{
    d->levelName = name;
}

QRect NativeWindowInfo::bounds() const
{
    return d->bounds;
}

void NativeWindowInfo::setBounds(QRect bounds)
{
    d->bounds = bounds;
}

int NativeWindowInfo::sharingState() const
{
    return d->sharingState;
}

void NativeWindowInfo::setSharingState(int state)
{
    d->sharingState = state;
}

float NativeWindowInfo::alpha() const
{
    return d->alpha;
}

void NativeWindowInfo::setAlpha(float alpha)
{
    d->alpha = alpha;
}

int NativeWindowInfo::ownerPid() const
{
    return d->ownerPid;
}

void NativeWindowInfo::setOwnerPid(int pid)
{
    d->ownerPid = pid;
}

long long NativeWindowInfo::memoryUsage() const
{
    return d->memoryUsage;
}

void NativeWindowInfo::setMemoryUsage(long long usage)
{
    d->memoryUsage = usage;
}

// On Mac these are optional...
int NativeWindowInfo::workspace() const
{
    return d->workspace;
}

void NativeWindowInfo::setWorkspace(int workspace)
{
    d->workspace = workspace;
}

QString NativeWindowInfo::ownerName() const
{
    return d->ownerName;
}

void NativeWindowInfo::setOwnerName(const QString &name)
{
    d->ownerName = name;
}

/*!
    \property windowName

    On Mac OS X, returns the name of the window as configured in Quartz. Note that few applications set this value.

    On other platforms UNDEFINED.
 */
QString NativeWindowInfo::windowName() const
{
    return d->windowName;
}

void NativeWindowInfo::setWindowName(const QString &name)
{
    d->windowName = name;
}

bool NativeWindowInfo::isOnScreen() const
{
    return d->isOnScreen;
}

void NativeWindowInfo::setOnScreen(bool onScreen)
{
    d->isOnScreen = onScreen;
}

/*!
    \property backingLocationVideoMemory

    On Mac OS X, returns a value identifying whether the window's backing store is located in video memory.

    On other platforms UNDEFINED.
 */
bool NativeWindowInfo::isBackingLocationVideoMemory() const
{
    return d->isBackingLocationVideoMemory;
}

void NativeWindowInfo::setIsBackingLocationVideoMemory(bool backingLocationVideoMemory)
{
    d->isBackingLocationVideoMemory = backingLocationVideoMemory;
}
