#include "nativesystemdialog.h"

NativeSystemDialog::NativeSystemDialog(QWidget *parent) :
    QObject(parent), m_dialog(NULL), m_eventLoop(new QEventLoop(this)), m_result(QDialog::Rejected)
{
}

NativeSystemDialog::~NativeSystemDialog()
{
    delete this->m_dialog;
}

QDialog::DialogCode NativeSystemDialog::exec()
{
    this->m_dialog = this->createDialog();
    QObject::connect(this->m_dialog, SIGNAL(finished(int)), this, SLOT(finished(int)));
    this->open();

    // Loop until dialog closed and return the dialog result
    this->m_eventLoop->exec(QEventLoop::DialogExec);
    return static_cast<QDialog::DialogCode>(this->m_result);
}

void NativeSystemDialog::open()
{
    if (this->m_dialog)
    {
        this->m_dialog->open();
    }
}

/*!
    Captures the dialog code result from the dialog.
 */
void NativeSystemDialog::finished(int result)
{
    this->m_result = result;

    // Quit the event loop if it's running so we can return to the caller
    if (this->m_eventLoop->isRunning())
    {
        this->m_eventLoop->quit();
    }
}

/*!
    This function does nothing - it exists to prevent a warning from connecting to a NULL slot.
 */
void NativeSystemDialog::dummy()
{
}
