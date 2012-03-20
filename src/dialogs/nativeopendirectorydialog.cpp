#include "nativeopendirectorydialog.h"

NativeOpenDirectoryDialog::NativeOpenDirectoryDialog(QWidget *parent, const QString &caption, const QString &dir, QFileDialog::Options options) :
    NativeFileDialog(parent, caption, dir, QString(), 0, options)
{
}

QDialog* NativeOpenDirectoryDialog::createDialog()
{
    QFileDialog *dialog = qobject_cast<QFileDialog*>(NativeFileDialog::createDialog());
    if (dialog)
    {
        dialog->setFileMode(this->m_options & QFileDialog::ShowDirsOnly ? QFileDialog::DirectoryOnly : QFileDialog::Directory);
        dialog->setAcceptMode(QFileDialog::AcceptOpen);
    }

    return dialog;
}
