#ifndef NATIVESYSTEMDIALOG_H
#define NATIVESYSTEMDIALOG_H

#include "petroules-utilities-global.h"

class PETROULESUTILITIESSHARED_EXPORT NativeSystemDialog : public QObject
{
    Q_OBJECT

public:
    explicit NativeSystemDialog(QWidget *parent = 0);
    virtual ~NativeSystemDialog();
    inline QWidget* parentWidget() { return qobject_cast<QWidget*>(this->parent()); }
    QDialog::DialogCode exec();

protected:
    virtual QDialog* createDialog() = 0;
    virtual void open();

protected slots:
    void dummy();
    virtual void finished(int result);

protected:
    QDialog *m_dialog;

private:
    QEventLoop *m_eventLoop;
    int m_result;
};

#endif // NATIVESYSTEMDIALOG_H
