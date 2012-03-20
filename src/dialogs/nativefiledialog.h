#ifndef NATIVEFILEDIALOG_H
#define NATIVEFILEDIALOG_H

#include "petroules-utilities-global.h"
#include "nativesystemdialog.h"

class PETROULESUTILITIESSHARED_EXPORT NativeFileDialog : public NativeSystemDialog
{
    Q_OBJECT

public:
    explicit NativeFileDialog(QWidget *parent = 0, const QString &caption = QString(), const QString &dir = QString(), const QString &filter = QString(), QString *selectedFilter = 0, QFileDialog::Options options = 0);
    virtual QDialog* createDialog();
    virtual void open();
    QString selectedFile() const;

protected slots:
    virtual void finished(int result);

protected:
    QString m_caption;
    QString m_dir;
    QString m_filter;
    QString *m_selectedFilter;
    QFileDialog::Options m_options;
};

#endif // NATIVEFILEDIALOG_H
