#ifndef MESSAGEBOXSHEET_H
#define MESSAGEBOXSHEET_H

#include "petroules-utilities-global.h"

class PETROULES_UTILITIES_API NativeDialogs
{
public:
    static QMessageBox::StandardButton critical(QWidget *parent, const QString &title, const QString &text, const QString &informativeText = QString(), QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    static QMessageBox::StandardButton information(QWidget *parent, const QString &title, const QString &text, const QString &informativeText = QString(), QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    static QMessageBox::StandardButton warning(QWidget *parent, const QString &title, const QString &text, const QString &informativeText = QString(), QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

private:
    NativeDialogs();
    static QMessageBox::StandardButton messageBox(QWidget *parent, QMessageBox::Icon icon, const QString &title, const QString &text, const QString &informativeText = QString(), QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
};

#endif // MESSAGEBOXSHEET_H
