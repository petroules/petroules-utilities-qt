#include "nativeopenfiledialog.h"

NativeOpenFileDialog::NativeOpenFileDialog(QWidget *parent, bool multiselect, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) :
    NativeFileDialog(parent, caption, dir, filter, selectedFilter, options), m_multiselect(multiselect)
{
}

QDialog* NativeOpenFileDialog::createDialog()
{
    QFileDialog *dialog = qobject_cast<QFileDialog*>(NativeFileDialog::createDialog());
    if (dialog)
    {
        dialog->setFileMode(this->m_multiselect ? QFileDialog::ExistingFiles : QFileDialog::ExistingFile);
        dialog->setAcceptMode(QFileDialog::AcceptOpen);
    }

    return dialog;
}

QStringList NativeOpenFileDialog::selectedFiles() const
{
    QFileDialog *dialog = qobject_cast<QFileDialog*>(this->m_dialog);
    if (dialog)
    {
        return dialog->selectedFiles();
    }

    return QStringList();
}
