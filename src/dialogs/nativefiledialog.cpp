#include "nativefiledialog.h"

NativeFileDialog::NativeFileDialog(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) :
    NativeSystemDialog(parent), m_caption(caption), m_dir(dir), m_filter(filter), m_selectedFilter(selectedFilter), m_options(options)
{
}

QDialog* NativeFileDialog::createDialog()
{
    QFileDialog *dialog = new QFileDialog(this->parentWidget(), this->m_caption, this->m_dir, this->m_filter);
    dialog->setOptions(this->m_options);

    if (this->m_selectedFilter)
    {
        dialog->selectNameFilter(*this->m_selectedFilter);
    }

    return dialog;
}

void NativeFileDialog::open()
{
    QFileDialog *dialog = qobject_cast<QFileDialog*>(this->m_dialog);
    if (dialog)
    {
        dialog->open(this, SLOT(dummy()));
    }
}

QString NativeFileDialog::selectedFile() const
{
    QFileDialog *dialog = qobject_cast<QFileDialog*>(this->m_dialog);
    if (dialog)
    {
        return dialog->selectedFiles().value(0);
    }

    return QString();
}

void NativeFileDialog::finished(int result)
{
    if (this->m_selectedFilter)
    {
        QFileDialog *dialog = qobject_cast<QFileDialog*>(this->m_dialog);
        if (dialog)
        {
            *this->m_selectedFilter = dialog->selectedFilter();
        }
    }

    NativeSystemDialog::finished(result);
}
