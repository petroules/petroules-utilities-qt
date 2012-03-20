#include "nativeprintdialog.h"

#ifndef QT_NO_PRINTER

NativePrintDialog::NativePrintDialog(QPrinter *printer, QWidget *parent) :
    NativeSystemDialog(parent), m_printer(printer)
{
}

QDialog* NativePrintDialog::createDialog()
{
    return new QPrintDialog(this->m_printer, this->parentWidget());
}

void NativePrintDialog::open()
{
    QPrintDialog *printDialog = qobject_cast<QPrintDialog*>(this->m_dialog);
    if (printDialog)
    {
        printDialog->open(this, SLOT(dummy()));
    }
}

#endif
