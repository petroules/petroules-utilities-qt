#ifndef THUMBBARMANAGER_H
#define THUMBBARMANAGER_H

#include "petroules-utilities-global.h"

class IntegratedMainWindow;

class PETROULESUTILITIESSHARED_EXPORT ThumbBar : public QObject
{
    Q_OBJECT

    friend class IntegratedMainWindow;

public:
    ~ThumbBar();
    QList<QAction*> actions() const;
    void addAction(QAction *action);
    QAction* addAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *member);
    QAction* addAction(const QIcon &icon, const QString &text);
    QAction* addSeparator();
    void setDrawBorder(QAction *action, bool border);
    void setInteractive(QAction *action, bool interactive);
    void setDismissOnClick(QAction *action, bool dismiss);

    static int maxButtons() { return 7; }
    static QSize imageSize();

public slots:
    void show();

protected:
#ifdef Q_WS_WIN
    bool winEvent(MSG *message, long *result);
#endif

private slots:
    void setButtonList();
    void setFlagsForAction(QAction *action, int i);
    void updateButtonList();

private:
    explicit ThumbBar(IntegratedMainWindow *mWindow);
    struct ThumbButtonProperties;
    class Private;
    Private *d;
};

#endif // THUMBBARMANAGER_H
