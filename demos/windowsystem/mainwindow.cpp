#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <petroules-utilities.h>
#include <limits>

class MainWindow::Private
{
public:
    Private()
    {
    }

    QList<NativeWindowInfo*> windowInfoList;
    QList<NativeWindowInfo*> windowInfoSelection;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    d = new Private();
    ui->setupUi(this);

    QList<QSpinBox*> boxes;
    boxes << ui->boundsLeftSpinBox << ui->boundsTopSpinBox << ui->boundsWidthSpinBox << ui->boundsHeightSpinBox;
    foreach (QSpinBox *box, boxes)
    {
        box->setMinimum(std::numeric_limits<int>::min());
        box->setMaximum(std::numeric_limits<int>::max());
    }

    QTimer *windowListTimer = new QTimer(this);
    connect(windowListTimer, SIGNAL(timeout()), this, SLOT(updateWindowList()));
    windowListTimer->start(50);

    QTimer *compositeViewTimer = new QTimer(this);
    connect(compositeViewTimer, SIGNAL(timeout()), this, SLOT(updateCompositeView()));
    compositeViewTimer->start(50);
}

MainWindow::~MainWindow()
{
    delete d;
    delete ui;
}

void MainWindow::updateWindowList()
{
    // Get the current selection
    QList<NativeWindowInfo*> selectedWindows;
    QList<WId> selectedWindowIds;
    QList<QTreeWidgetItem*> selectedItems = ui->windowListTreeWidget->selectedItems();
    foreach (QTreeWidgetItem *selectedItem, selectedItems)
    {
        NativeWindowInfo *info = reinterpret_cast<NativeWindowInfo*>(selectedItem->data(0, Qt::UserRole).value<void*>());
        selectedWindows += info;
        selectedWindowIds += info->windowId();
    }

    ui->windowListTreeWidget->setHeaderLabel("Windows");
    ui->windowListTreeWidget->clear();

    d->windowInfoList = WindowManager::windowList();
    foreach (NativeWindowInfo* window, d->windowInfoList)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        if (!window->windowName().isEmpty())
        {
            item->setText(0, window->windowName());
        }
        else
        {
            // If the window's untitled, make its text italic
            QFont font = item->font(0);
            font.setItalic(true);
            item->setFont(0, font);

            item->setText(0, "(untitled)");
        }

        item->setData(0, Qt::UserRole, qVariantFromValue((void*)window));
        ui->windowListTreeWidget->addTopLevelItem(item);
    }

    // Update selection
    for (int i = 0; i < ui->windowListTreeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = ui->windowListTreeWidget->topLevelItem(i);
        NativeWindowInfo *info = reinterpret_cast<NativeWindowInfo*>(item->data(0, Qt::UserRole).value<void*>());
        if (selectedWindowIds.contains(info->windowId()))
        {
            item->setSelected(true);
        }
    }
}

void MainWindow::on_windowListTreeWidget_itemSelectionChanged()
{
    QList<QTreeWidgetItem*> items = ui->windowListTreeWidget->selectedItems();
    QList<NativeWindowInfo*> infos;
    foreach (QTreeWidgetItem *item, items)
    {
        infos.append(reinterpret_cast<NativeWindowInfo*>(item->data(0, Qt::UserRole).value<void*>()));
    }

    populateEditor(d->windowInfoSelection = infos);
}

void MainWindow::populateEditor(NativeWindowInfo *info)
{
    if (!info)
    {
        // TODO: clear the editor out
        return;
    }

    ui->nativeWindowIdLabel->setText(QString::number(WindowManager::widToInteger(info->windowId())));
    ui->windowTitleLineEdit->setText(info->windowName());

    ui->windowLevelComboBox->clear();
    QString layer = info->levelName();
    if (layer.isEmpty())
    {
        layer = QString::number(info->level());
    }
    else
    {
        layer += QString(" (%1)").arg(info->level());
    }

    ui->windowLevelComboBox->addItem(layer);

    ui->boundsLeftSpinBox->setValue(info->bounds().left());
    ui->boundsTopSpinBox->setValue(info->bounds().top());
    ui->boundsWidthSpinBox->setValue(info->bounds().width());
    ui->boundsHeightSpinBox->setValue(info->bounds().height());

    updateCompositeView();
}

void MainWindow::populateEditor(const QList<NativeWindowInfo *> &infos)
{
    if (infos.count() > 1)
    {
        // do stuff here
    }
    else if (infos.count() == 1)
    {
        populateEditor(infos.at(0));
    }
    else
    {
        populateEditor(NULL); // clear
    }
}

void MainWindow::updateCompositeView()
{
    QGraphicsScene *scene = ui->graphicsView->scene();
    if (!scene)
    {
        ui->graphicsView->setScene(scene = new QGraphicsScene(ui->graphicsView));
    }

    scene->clear();
    scene->setSceneRect(QRectF());
    ui->graphicsView->resetMatrix();
    ui->graphicsView->resetTransform();

    if (d->windowInfoSelection.count() > 1)
    {
        scene->addText("Multiple windows not yet implemented");
    }
    else if (d->windowInfoSelection.count() == 1)
    {
        NativeWindowInfo *info = d->windowInfoSelection.first();
        if (info && info->windowId() != WindowManager::nullWindowId())
        {
            QGraphicsItem *screenshot = ui->graphicsView->scene()->addPixmap(WindowManager::getCompositeImage(info->windowId()));
            ui->graphicsView->fitInView(screenshot, Qt::KeepAspectRatio);
            ui->graphicsView->centerOn(screenshot);
        }
    }
    else
    {
        scene->addText("No selection");
    }
}
