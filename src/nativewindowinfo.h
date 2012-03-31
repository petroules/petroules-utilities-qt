#ifndef NATIVEWINDOWINFO_H
#define NATIVEWINDOWINFO_H

#include <QtGui>

class NativeWindowInfo : public QObject
{
    Q_OBJECT

public:
    enum WindowLevel
    {
        TODO
    };

    explicit NativeWindowInfo(QObject *parent = 0);
    virtual ~NativeWindowInfo();
    WId windowId() const;
    void setWindowId(WId wid);
    int backingStoreType() const;
    void setBackingStoreType(int type);
    NativeWindowInfo::WindowLevel level() const;
    void setLevel(NativeWindowInfo::WindowLevel level);
    QString levelName() const;
    void setLevelName(const QString &name);
    QRect bounds() const;
    void setBounds(QRect bounds);
    int sharingState() const;
    void setSharingState(int state);
    float alpha() const;
    void setAlpha(float alpha);
    int ownerPid() const;
    void setOwnerPid(int pid);
    long long memoryUsage() const;
    void setMemoryUsage(long long usage);

    // On Mac these are optional...
    int workspace() const;
    void setWorkspace(int workspace);
    QString ownerName() const;
    void setOwnerName(const QString &name);
    QString windowName() const;
    void setWindowName(const QString &name);
    bool isOnScreen() const;
    void setOnScreen(bool onScreen);
    bool isBackingLocationVideoMemory() const;
    void setIsBackingLocationVideoMemory(bool backingLocationVideoMemory);

private:
    class Private;
    Private *d;
};

#endif // NATIVEWINDOWINFO_H
