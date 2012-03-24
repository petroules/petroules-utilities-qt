#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <limits>
#include "widgets/searchlineedit.h"

MainWindow::MainWindow(QWidget *parent) :
    IntegratedMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->updateState();

    // Toolbar looks a little better with some space between individual items
#ifdef Q_WS_WIN
    this->ui->toolBar->layout()->setSpacing(8);
#endif

    // Get current style name and list of all available styles
    QString currentStyleName = QApplication::style()->objectName();
    QStringList styles = QStyleFactory::keys();
#ifdef Q_WS_WIN
    styles.push_front("Explorer");
#endif

    // Add styles combo box
    stylesComboBox = new QComboBox();
    foreach (QString style, styles)
    {
        stylesComboBox->addItem(style);
    }

    // Update the combo box selection to reflect the current style
    stylesComboBox->setCurrentIndex(styles.indexOf(QRegExp(QRegExp::escape(currentStyleName), Qt::CaseInsensitive)));

    this->ui->toolBar->addWidget(this->stylesComboBox);
    QObject::connect(this->stylesComboBox, SIGNAL(currentIndexChanged(QString)), SLOT(switchStyle(QString)));

    // Native fullscreen mode shortcuts
#ifdef Q_WS_MAC
    ui->actionEnterFullScreen->setShortcut(QKeySequence(Qt::CTRL | Qt::META | Qt::Key_F));
#else
    ui->actionEnterFullScreen->setShortcut(Qt::Key_F11);
#endif

    // Can't hide menu on OS X
#ifndef Q_WS_MAC
    // Add menu check box
    QCheckBox *menuCheckBox = new QCheckBox(tr("Show menu bar"));
    menuCheckBox->setChecked(true);
    this->ui->toolBar->addWidget(menuCheckBox);
    QObject::connect(menuCheckBox, SIGNAL(toggled(bool)), this->ui->menuBar, SLOT(setVisible(bool)));
#endif

    // No point in having DWM stuff here since it won't do anything
#ifdef Q_WS_WIN
    // Add DWM composition toggle check box
    QCheckBox *compositionCheckBox = new QCheckBox(tr("Enable DWM composition"));
    compositionCheckBox->setChecked(DesktopWindowManager::isCompositionEnabled());
    this->ui->toolBar->addWidget(compositionCheckBox);
    QObject::connect(compositionCheckBox, SIGNAL(toggled(bool)), SLOT(toggleComposition(bool)));

    // Call the slot initially to make sure the padding is correct
    this->toggleComposition(DesktopWindowManager::isCompositionEnabled());

    QSpinBox *dwmMarginSpinBox = new QSpinBox();
    dwmMarginSpinBox->setMinimum(-1);
    dwmMarginSpinBox->setMaximum(std::numeric_limits<int>::max());
    dwmMarginSpinBox->setValue(-1);
    this->ui->toolBar->addWidget(new QLabel(tr("DWM frame margins:")));
    this->ui->toolBar->addWidget(dwmMarginSpinBox);
    QObject::connect(dwmMarginSpinBox, SIGNAL(valueChanged(int)), SLOT(changeMargin(int)));

    this->changeMargin(-1);
    DesktopWindowManager::setBlurBehindEnabled(this);
#endif

    // Add spacer
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->ui->toolBar->addWidget(spacer);

    // Add search box
    this->ui->toolBar->addWidget(new SearchLineEdit());

#ifdef Q_WS_WIN
    JumpList list;
    qDebug() << "Maximum number of jump list destinations shown:" << list.maximumShownDestinations();
    //list.appendFrequentCategory();
    list.appendRecentCategory();

    QList<Destination> custom;
    custom.append(JumpListFile(QDir::toNativeSeparators("C:/Users/Jake Petroules/Documents/LoremIpsum2.txt")));

    list.appendCustomCategory("Custom item", custom);

    JumpListLink link1;
    link1.title = "Launch with CDE style";
    link1.toolTip = "Launches the platform integration demo app using Qt's CDE GUI style";
    link1.path = qApp->applicationFilePath();
    link1.arguments = "-style cde";
    link1.iconPath = QDir::toNativeSeparators("C:/Windows/explorer.exe");
    link1.iconIndex = 20;

    JumpListLink link2;
    link2.title = "Launch with Plastique style";
    link2.toolTip = "Launches the platform integration demo app using Qt's Platique GUI style";
    link2.path = qApp->applicationFilePath();
    link2.arguments = "-style plastique";
    link2.iconPath = QDir::toNativeSeparators("C:/Windows/explorer.exe");
    link2.iconIndex = 21;

    QList<Destination> tasks;
    tasks.append(Destination(link1));
    tasks.append(Destination());
    tasks.append(Destination(link2));

    list.appendTasks(tasks);
    list.save();
#endif

    // TODO: For some reason this connection doesn't work from the signals & slots editor in the .ui file
    connect(ui->menu_View, SIGNAL(aboutToShow()), this, SLOT(updateViewMenuState()));

    setPlatformVisibility();
}

MainWindow::~MainWindow()
{
    delete this->ui;
}

void MainWindow::setPlatformVisibility()
{
    // Set the features page for the current platform
#ifdef Q_WS_WIN
    ui->stackedWidget->setCurrentWidget(ui->windowsPage);
#elif defined(Q_WS_MAC)
    ui->stackedWidget->setCurrentWidget(ui->osxPage);
#endif
}

void MainWindow::updateState()
{
    this->ui->setIconicThumbnailPushButton->setEnabled(this->ui->enableLivePreviewCheckBox->isChecked());
    this->ui->clearIconicThumbnailPushButton->setEnabled(this->ui->enableLivePreviewCheckBox->isChecked());
}

QPixmap MainWindow::sendPreview()
{
    if (ui->iconicThumbnailLabel->pixmap())
    {
        return *ui->iconicThumbnailLabel->pixmap();
    }

    // Blank pixmap
    return IntegratedMainWindow::sendPreview();
}

/*!
    Override this method and return the pixmap you want the taskbar preview window to show.

    The pixmap returned by this method will only be used if setIconicPreviewEnabled(true) is
    called, otherwise Windows will simply generate a pixmap of the window contents (the default).
 */
QPixmap MainWindow::sendThumbnail()
{
    if (ui->iconicThumbnailLabel->pixmap())
    {
        return *ui->iconicThumbnailLabel->pixmap();
    }

    // Blank pixmap
    return IntegratedMainWindow::sendThumbnail();
}

void MainWindow::on_setIconicThumbnailPushButton_clicked()
{
    QPixmap pix(":/res/images/osx-background.jpg");
    this->ui->iconicThumbnailLabel->setPixmap(pix);
    this->invalidateThumbnail();
    this->statusBar()->showMessage(tr("Iconic thumbnail set"), 5000);
}

void MainWindow::on_clearIconicThumbnailPushButton_clicked()
{
    this->ui->iconicThumbnailLabel->setPixmap(QPixmap());
    this->invalidateThumbnail();
    this->statusBar()->showMessage(tr("Iconic thumbnail cleared"), 5000);
}

void MainWindow::on_enableLivePreviewCheckBox_toggled(bool checked)
{
    this->ui->setIconicThumbnailPushButton->setEnabled(checked);
    this->ui->clearIconicThumbnailPushButton->setEnabled(checked);
    this->setIconicPreviewEnabled(checked);
    this->statusBar()->showMessage(tr("Live preview %1").arg(checked ? "enabled" : "disabled"), 5000);
}

void MainWindow::thumbBarButtonClicked()
{
    //ThumbBarButton *button = qobject_cast<ThumbBarButton*>(sender());
    void *button = 0;
    if (button)
    {
        //qDebug() << button->tooltip() << " button (#" << button->id() << ") pressed.";
    }
}

void MainWindow::on_overlayIcon1PushButton_clicked()
{
    if (this->setOverlayIcon(this->ui->overlayIcon1PushButton->icon()))
    {
        this->statusBar()->showMessage(tr("Overlay icon 1 set"), 5000);
    }
}

void MainWindow::on_overlayIcon2PushButton_clicked()
{
    if (this->setOverlayIcon(this->ui->overlayIcon2PushButton->icon()))
    {
        this->statusBar()->showMessage(tr("Overlay icon 2 set"), 5000);
    }
}

void MainWindow::on_overlayIcon3PushButton_clicked()
{
    if (this->setOverlayIcon(this->ui->overlayIcon3PushButton->icon()))
    {
        this->statusBar()->showMessage(tr("Overlay icon 3 set"), 5000);
    }
}

void MainWindow::on_clearOverlayIconPushButton_clicked()
{
    if (this->clearOverlayIcon())
    {
        this->statusBar()->showMessage(tr("Overlay icon cleared"), 5000);
    }
}

void MainWindow::on_progressStateComboBox_currentIndexChanged(int index)
{
    TaskbarProgressState state = NoProgress;

    switch (index)
    {
    case 0:
        state = NoProgress;
        break;
    case 1:
        state = Indeterminate;
        break;
    case 2:
        state = Normal;
        break;
    case 3:
        state = Error;
        break;
    case 4:
        state = Paused;
        break;
    default:
        break;
    }

    if (this->setProgressState(state))
    {
        this->statusBar()->showMessage(tr("Taskbar progress state set to %1").arg(this->ui->progressStateComboBox->currentText()));
    }
}

void MainWindow::on_progressHorizontalSlider_sliderMoved(int position)
{
    // Calling setProgressValue will change the progress state to normal
    // if it isn't already set to normal, error or paused; make the UI reflect that
    if (ui->progressStateComboBox->currentIndex() < 2)
    {
        ui->progressStateComboBox->setCurrentIndex(2);
    }

    if (this->setProgressValue(position))
    {
        this->statusBar()->showMessage(tr("Progress bar set to %1/100").arg(position), 5000);
    }
}

void MainWindow::on_actionPreviousTab_triggered()
{
    if (this->ui->windowsTabWidget->currentIndex() > 0)
    {
        this->ui->windowsTabWidget->setCurrentIndex(this->ui->windowsTabWidget->currentIndex() - 1);
    }
}

void MainWindow::on_actionNextTab_triggered()
{
    if (this->ui->windowsTabWidget->currentIndex() < this->ui->windowsTabWidget->count())
    {
        this->ui->windowsTabWidget->setCurrentIndex(this->ui->windowsTabWidget->currentIndex() + 1);
    }
}

void MainWindow::on_activateThumbBarPushButton_clicked()
{
    ThumbBar *thumbBar = this->thumbBarManager();
    if (thumbBar)
    {
        foreach (QAction *action, this->ui->toolBar->actions())
        {
            thumbBar->addAction(action);
        }

        thumbBar->show();
    }
}

void MainWindow::on_registerFileExtensionsPushButton_clicked()
{
    WindowsFileRegistration reg;
    if (!reg.areFileExtensionsRegistered("TaskbarHOL"))
    {
        QMessageBox::critical(this, "Error", "File extensions are not registered.");
    }

    if (!reg.registerFileExtensions("TaskbarHOL", QStringList(".txt")))
    {
        QMessageBox::critical(this, "Error", "Access denied - elevation required.");
    }
}

void MainWindow::on_unregisterFileExtensionsPushButton_clicked()
{
    WindowsFileRegistration reg;
    if (!reg.unregisterFileExtensions("TaskbarHOL", QStringList(".txt")))
    {
        QMessageBox::critical(this, "Error", "Access denied - elevation required.");
    }
}

void MainWindow::switchStyle(QString index)
{
#ifdef Q_WS_WIN
    if (index.toLower() == "explorer")
    {
        QApplication::setStyle(new ExplorerStyle());
    }
    else
#endif
    {
        QApplication::setStyle(index);
    }

    // For some reason this disappears when we set the style...
    this->ui->toolBar->layout()->setSpacing(8);
    this->updateGeometry();
}

void MainWindow::toggleComposition(bool enable)
{
    DesktopWindowManager::enableComposition(enable);

    // Window looks a little better with less padding when we have full window glass
    int margins = enable ? 6 : 9;
    this->centralWidget()->layout()->setContentsMargins(margins, margins, margins, margins);
}

void MainWindow::changeMargin(int margin)
{
    DesktopWindowManager::extendFrameIntoClientArea(this, margin, margin, margin, margin);
}

void MainWindow::on_actionEnableNativeFullScreen_triggered(bool checked)
{
    // Should only change if we're not actually in fullscreen because then we can't exit
    if (!isNativeFullScreen())
    {
        setNativeFullScreenEnabled(checked);
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("You shouldn't attempt to change whether native fullscreen mode is enabled while in native fullscreen mode. Note that setNativeFullScreenEnabled will refuse to change the state in this case for your convenience and to maintain a consistent state."));
    }
}

void MainWindow::on_actionEnterFullScreen_triggered()
{
    if (isNativeFullScreenEnabled())
    {
        setNativeFullScreen(!isNativeFullScreen());
    }
    else
    {
        if (isFullScreen())
        {
            showNormal();
        }
        else
        {
            showFullScreen();
        }
    }
}

void MainWindow::updateViewMenuState()
{
    ui->actionEnableNativeFullScreen->setChecked(isNativeFullScreenEnabled());
    ui->actionEnterFullScreen->setText((isNativeFullScreen() || isFullScreen()) ? tr("Exit Full Screen") : tr("Enter Full Screen"));
}

void MainWindow::on_dockIconBadgeTextLineEdit_textChanged(const QString &text)
{
    qiApp->setBadgeText(text);
}
