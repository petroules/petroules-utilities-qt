#include "nativesavefiledialog.h"

NativeSaveFileDialog::NativeSaveFileDialog(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) :
    NativeFileDialog(parent, caption, dir, filter, selectedFilter, options)
{
}

QDialog* NativeSaveFileDialog::createDialog()
{
    QFileDialog *dialog = qobject_cast<QFileDialog*>(NativeFileDialog::createDialog());
    if (dialog)
    {
        dialog->setFileMode(QFileDialog::AnyFile);
        dialog->setAcceptMode(QFileDialog::AcceptSave);
    }

    return dialog;
}
