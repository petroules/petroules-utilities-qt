#ifndef MACTOOLBARSEARCHWIDGET_H
#define MACTOOLBARSEARCHWIDGET_H

#include "widgets/itoolbarsearchwidget.h"
#include "icocoaeventreceiver.h"

class MacToolbarSearchWidget : public IToolbarSearchWidget, public ICocoaEventReceiver
{
    Q_OBJECT

public:
    explicit MacToolbarSearchWidget(QWidget *parent = NULL);
    ~MacToolbarSearchWidget();
    QSize sizeHint() const;
    QMenu* menu() const;
    void setMenu(QMenu *menu);
    bool isEnabled() const;
    void setEnabled(bool enable);
    QString text() const;
    void setText(const QString &text);
    void clear();
    bool isTextHidden() const;
    void setTextHidden(bool hide);

signals:
    void textChanged();

protected:
    void eventReceived(const QString &name, void *object, QMap<QString, void *> *userInfo);

private:
    class Private;
    Private *d;
    QString m_textBuffer;
    bool m_isTextHidden;
    QMenu *m_menu;
};

#endif // MACTOOLBARSEARCHWIDGET_H
