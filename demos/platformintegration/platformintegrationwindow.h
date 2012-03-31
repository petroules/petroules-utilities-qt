#ifndef PLATFORMINTEGRATIONWINDOW_H
#define PLATFORMINTEGRATIONWINDOW_H

#include <petroules-utilities.h>

namespace Ui
{
    class PlatformIntegrationWindow;
}

class QComboBox;

class PlatformIntegrationWindow : public IntegratedMainWindow
{
    Q_OBJECT

public:
    explicit PlatformIntegrationWindow(QWidget *parent = 0);
    ~PlatformIntegrationWindow();

private:
    void setPlatformVisibility();

private slots:
    void thumbBarButtonClicked();
    void updateState();

    QPixmap sendPreview();
    QPixmap sendThumbnail();

    void on_setIconicThumbnailPushButton_clicked();
    void on_clearIconicThumbnailPushButton_clicked();
    void on_enableLivePreviewCheckBox_toggled(bool checked);

    void on_overlayIcon1PushButton_clicked();
    void on_overlayIcon2PushButton_clicked();
    void on_overlayIcon3PushButton_clicked();
    void on_clearOverlayIconPushButton_clicked();
    void on_progressStateComboBox_currentIndexChanged(int index);
    void on_progressHorizontalSlider_sliderMoved(int position);
    void on_actionPreviousTab_triggered();
    void on_actionNextTab_triggered();
    void on_activateThumbBarPushButton_clicked();

    void on_registerFileExtensionsPushButton_clicked();

    void on_unregisterFileExtensionsPushButton_clicked();

    void switchStyle(QString);

    void toggleComposition(bool);
    void changeMargin(int);

    void on_actionEnableNativeFullScreen_triggered(bool checked);
    void on_actionEnterFullScreen_triggered();
    void updateViewMenuState();

    void on_dockIconBadgeTextLineEdit_textChanged(const QString &text);

private:
    Ui::PlatformIntegrationWindow *ui;
    QComboBox *stylesComboBox;
};

#endif // PLATFORMINTEGRATIONWINDOW_H
