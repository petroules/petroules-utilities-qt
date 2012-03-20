#include "nativedialogs.h"

QMessageBox::StandardButton NativeDialogs::critical(QWidget *parent, const QString &title, const QString &text, const QString &informativeText, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return NativeDialogs::messageBox(parent, QMessageBox::Critical, title, text, informativeText, buttons, defaultButton);
}

QMessageBox::StandardButton NativeDialogs::information(QWidget *parent, const QString &title, const QString &text, const QString &informativeText, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return NativeDialogs::messageBox(parent, QMessageBox::Information, title, text, informativeText, buttons, defaultButton);
}

QMessageBox::StandardButton NativeDialogs::warning(QWidget *parent, const QString &title, const QString &text, const QString &informativeText, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return NativeDialogs::messageBox(parent, QMessageBox::Warning, title, text, informativeText, buttons, defaultButton);
}

QMessageBox::StandardButton NativeDialogs::messageBox(QWidget *parent, QMessageBox::Icon icon, const QString &title, const QString &text, const QString &informativeText, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    QMessageBox box(icon, title, text, buttons, parent,
#ifdef Q_WS_MAC
        Qt::Sheet
#else
        Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint
#endif
    );

    box.setWindowModality(Qt::WindowModal);
    box.setInformativeText(informativeText);
    box.setDefaultButton(defaultButton);

    return static_cast<QMessageBox::StandardButton>(box.exec());
}
