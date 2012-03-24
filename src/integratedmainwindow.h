#ifndef INTEGRATEDMAINWINDOW_H
#define INTEGRATEDMAINWINDOW_H

#include <QMainWindow>

class IntegratedMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IntegratedMainWindow(QWidget *parent = 0);
    virtual ~IntegratedMainWindow();
    void setNativeFullScreenEnabled(bool enable);
    bool isNativeFullScreen() const;
    void setNativeFullScreen(bool set);

private:
    class Private;
    Private *d;
};

#endif // INTEGRATEDMAINWINDOW_H
