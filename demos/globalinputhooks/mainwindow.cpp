#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "systemkeyboardreadwrite.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SystemKeyboardReadWrite *k = new SystemKeyboardReadWrite();
    k->setConnected(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
