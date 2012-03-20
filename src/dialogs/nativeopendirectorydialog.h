#ifndef NATIVEOPENDIRECTORYDIALOG_H
#define NATIVEOPENDIRECTORYDIALOG_H

#include "petroules-utilities-global.h"
#include "nativefiledialog.h"

class PETROULESUTILITIESSHARED_EXPORT NativeOpenDirectoryDialog : public NativeFileDialog
{
    Q_OBJECT

public:
    explicit NativeOpenDirectoryDialog(QWidget *parent = 0, const QString &caption = QString(), const QString &dir = QString(), QFileDialog::Options options = 0);
    virtual QDialog* createDialog();
};

#endif // NATIVEOPENDIRECTORYDIALOG_H
