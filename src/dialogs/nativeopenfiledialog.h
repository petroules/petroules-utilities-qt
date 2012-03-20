#ifndef NATIVEOPENFILEDIALOG_H
#define NATIVEOPENFILEDIALOG_H

#include "petroules-utilities-global.h"
#include "nativefiledialog.h"

class PETROULESUTILITIESSHARED_EXPORT NativeOpenFileDialog : public NativeFileDialog
{
    Q_OBJECT

public:
    explicit NativeOpenFileDialog(QWidget *parent = 0, bool multiselect = false, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
    virtual QDialog* createDialog();
    QStringList selectedFiles() const;

private:
    bool m_multiselect;
};

#endif // NATIVEOPENFILEDIALOG_H
