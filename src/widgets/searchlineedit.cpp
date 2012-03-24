#include "searchlineedit.h"
#include "explorersearchlineedit.h"
#include <QToolButton>
#include <QStyle>
#include <QLineEdit>

#ifndef Q_WS_MAC
SearchLineEdit::SearchLineEdit(QWidget *parent)
    : QWidget(parent), m_textBuffer(QString()), m_isTextHidden(false), m_menu(NULL)
{
    this->d = new ExplorerSearchLineEdit(this);
}

QSize SearchLineEdit::sizeHint() const
{
    return d->size();
}
#endif

QMenu* SearchLineEdit::menu() const
{
    return this->m_menu;
}

#ifndef Q_WS_MAC
void SearchLineEdit::setMenu(QMenu *menu)
{
    if (menu)
    {
        this->m_menu = menu;
    }
}

QString SearchLineEdit::text() const
{
    return d->text();
}

void SearchLineEdit::setText(const QString &text)
{
    d->setText(text);
}
#endif

void SearchLineEdit::clear()
{
    this->setText(QString());
}

bool SearchLineEdit::isTextHidden() const
{
    return this->m_isTextHidden;
}

void SearchLineEdit::setTextHidden(bool hide)
{
    if (this->isEnabled())
    {
        qDebug() << "Due to limitations of this widget, it must be disabled before hiding the text. The text will not be hidden.";
        return;
    }

    if (hide)
    {
        this->m_textBuffer = this->text();
        this->clear();
    }
    else
    {
        this->setText(this->m_textBuffer);
        this->m_textBuffer = QString();
    }

    this->m_isTextHidden = hide;
}
