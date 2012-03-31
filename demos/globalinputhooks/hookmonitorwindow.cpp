#include "hookmonitorwindow.h"
#include "ui_hookmonitorwindow.h"
#include "systemkeyboardreadwrite.h"

HookMonitorWindow::HookMonitorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HookMonitorWindow)
{
    ui->setupUi(this);

    SystemKeyboardReadWrite *k = new SystemKeyboardReadWrite();
    k->setConnected(true);
}

HookMonitorWindow::~HookMonitorWindow()
{
    delete ui;
}
