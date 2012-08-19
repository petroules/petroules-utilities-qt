#ifndef INTEGRATEDMAINWINDOW_H
#define INTEGRATEDMAINWINDOW_H

#include "petroules-utilities-global.h"

class ThumbBar;

class PETROULES_UTILITIES_API IntegratedMainWindow : public QMainWindow
{
    Q_OBJECT

    friend class ThumbBar;

public:
    /*!
        Enumerates possible states of the taskbar progress bar.
     */
    enum TaskbarProgressState
    {
        /*!
            Indicates that there is no progress information to display and sets the taskbar button to its default state.
         */
        NoProgress = 0,

        /*!
            Indicates that the progress bar should cycle along the length of the button repeatedly.
         */
        Indeterminate = 1,

        /*!
            Indicates normal progress, where the taskbar button will fill with green from left to right in proportion to
            the amount of the operation that has been completed.
         */
        Normal = 2,

        /*!
            Same as \a Normal, except that the taskbar button fills with red.
         */
        Error = 4,

        /*!
            Same as \a Normal, except that the taskbar button fills with yellow.
         */
        Paused = 8
    };

    explicit IntegratedMainWindow(QWidget *parent = NULL);
    virtual ~IntegratedMainWindow();

    bool taskbarExtensionsSupported() const;
    bool dwmSupported() const;

    // Overlay icons
    bool setOverlayIcon(const QIcon &icon, const QString &accessibilityDescription = QString());
    bool setOverlayIcon(const QPixmap &pixmap, const QString &accessibilityDescription = QString());
#ifdef Q_WS_WIN
    bool setOverlayIcon(HICON hIcon, const QString &accessibilityDescription = QString());
#endif
    bool clearOverlayIcon();
    inline static QSize overlayIconSize() { return QSize(16, 16); }

    // Progress bar
    bool setProgressValue(float value);
    bool setProgressValue(int value, int max = 100);
    bool setProgressValue(int value, int min, int max);
    bool setProgressState(TaskbarProgressState state);

    // Iconic previews
    void setIconicPreviewEnabled(bool enable);
    bool drawWindowFrameForIconicPreviews() const;
    void setDrawWindowFrameForIconicPreviews(bool draw);

    // Mark fullscreen window
    bool markFullscreenWindow(bool fullscreen = true);

    // Thumb bar
    ThumbBar* thumbBarManager() const;

    // Mac OS X fullscreen
    bool isNativeFullScreenEnabled() const;
    void setNativeFullScreenEnabled(bool enable);
    bool isNativeFullScreen() const;
    void setNativeFullScreen(bool set);

public slots:
    bool activateTab();

protected:
    // Iconic previews
    virtual QPixmap sendThumbnail();
    void invalidateThumbnail();
    virtual QPixmap sendPreview();

#ifdef Q_WS_WIN
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
#ifdef Q_WS_WIN
    void sendThumbnailInternal();
    void sendPreviewInternal();
    void initialize(long *result);
#endif
    Q_DISABLE_COPY(IntegratedMainWindow)
    class Private;
    Private *d;
};

#endif // INTEGRATEDMAINWINDOW_H
