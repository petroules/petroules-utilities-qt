#ifndef ITOOLBARSEARCHWIDGET_H
#define ITOOLBARSEARCHWIDGET_H

#include <QtGui>

class IToolbarSearchWidget : public QWidget
{
    Q_OBJECT

public:
    virtual QString text() const = 0;
    virtual void setText(const QString &text) = 0;
    virtual void clear() = 0;
    virtual bool isTextHidden() const = 0;
    virtual void setTextHidden(bool hide) = 0;

signals:
    void searchRequested(QString);

protected:
    explicit IToolbarSearchWidget(QWidget *parent = NULL);
};

#endif // ITOOLBARSEARCHWIDGET_H
