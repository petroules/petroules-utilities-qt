#ifndef MACTOOLBARSEARCHWIDGET_H
#define MACTOOLBARSEARCHWIDGET_H

#include "petroules-utilities-global.h"

#ifdef Q_WS_MAC
#include "mac/icocoaeventreceiver.h"
#endif

class ExplorerSearchLineEdit;

class PETROULESUTILITIESSHARED_EXPORT SearchLineEdit : public QWidget
#ifdef Q_WS_MAC
, public ICocoaEventReceiver
#endif
{
    Q_OBJECT

public:
    explicit SearchLineEdit(QWidget *parent = NULL);
    QSize sizeHint() const;
    QMenu* menu() const;
    void setMenu(QMenu *menu);
    QString text() const;
    void setText(const QString &text);
    void clear();
    bool isTextHidden() const;
    void setTextHidden(bool hide);

signals:
    void textChanged();
    void searchRequested(QString);

protected:
#ifdef Q_WS_MAC
    bool event(QEvent *event);
#endif
    void eventReceived(const QString &name, void *object, QMap<QString, void *> *userInfo);

private:
#ifdef Q_WS_MAC
    class Private;
    Private *d;
#else
    ExplorerSearchLineEdit *d;
#endif
    QString m_textBuffer;
    bool m_isTextHidden;
    QMenu *m_menu;
};

#endif // MACTOOLBARSEARCHWIDGET_H
