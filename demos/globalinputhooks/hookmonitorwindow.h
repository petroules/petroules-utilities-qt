#ifndef HOOKMONITORWINDOW_H
#define HOOKMONITORWINDOW_H

#include <QMainWindow>

namespace Ui {
    class HookMonitorWindow;
}

class HookMonitorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HookMonitorWindow(QWidget *parent = 0);
    ~HookMonitorWindow();

private:
    Ui::HookMonitorWindow *ui;
};

#endif // HOOKMONITORWINDOW_H
