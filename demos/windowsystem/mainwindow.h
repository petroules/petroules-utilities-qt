#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

namespace Ui {
class MainWindow;
}

class NativeWindowInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void populateEditor(NativeWindowInfo *info);
    void populateEditor(const QList<NativeWindowInfo*> &infos);

public slots:
    void updateWindowList();
    void updateCompositeView();
    
private slots:
    void on_windowListTreeWidget_itemSelectionChanged();

private:
    class Private;
    Private *d;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
