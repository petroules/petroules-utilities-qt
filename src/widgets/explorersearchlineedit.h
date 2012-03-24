#ifndef EXPLORERSEARCHLINEEDIT_H
#define EXPLORERSEARCHLINEEDIT_H

#include <QLineEdit>

class ExplorerSearchLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit ExplorerSearchLineEdit(QWidget *parent = NULL);
    virtual ~ExplorerSearchLineEdit();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateSearchButton(const QString &text);

private:
    QString styleSheetForCurrentState() const;
    QString buttonStyleSheetForCurrentState() const;

    class Private;
    Private *d;
};

#endif // EXPLORERSEARCHLINEEDIT_H
