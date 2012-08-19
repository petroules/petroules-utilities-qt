#ifndef NATIVEPRINTDIALOG_H
#define NATIVEPRINTDIALOG_H

#include "petroules-utilities-global.h"
#include "nativesystemdialog.h"

#ifndef QT_NO_PRINTER

class PETROULES_UTILITIES_API NativePrintDialog : public NativeSystemDialog
{
    Q_OBJECT

public:
    explicit NativePrintDialog(QPrinter *printer, QWidget *parent = 0);
    virtual QDialog* createDialog();
    virtual void open();

private:
    QPrinter *m_printer;
};

#endif // QT_NO_PRINTER
#endif // NATIVEPRINTDIALOG_H
