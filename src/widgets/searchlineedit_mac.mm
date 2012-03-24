#include "searchlineedit.h"
#include "mac/machelpers.h"
#import <QMacCocoaViewContainer>
#import <Cocoa/Cocoa.h>

class SearchLineEdit::Private : public QMacCocoaViewContainer
{
    // Do not use Q_OBJECT with pimpl - it will generate a *_moc.cpp file which won't correctly compile with Objective-C++

public:
    explicit Private(QWidget *parent = NULL)
        : QMacCocoaViewContainer(0, parent)
    {
        // bug in 4.8.0, will be fixed in 4.8.1
        setAttribute(Qt::WA_NativeWindow);
        setCocoaView([[NSSearchField alloc] init]);
        setMouseTracking(true);
    }

    ~Private()
    {
        [this->searchField() release];
    }

    QSize sizeHint() const
    {
        return QSize(200, 40);
    }

    NSSearchField* searchField() const
    {
        return reinterpret_cast<NSSearchField*>(this->cocoaView());
    }
};

SearchLineEdit::SearchLineEdit(QWidget *parent)
    : QWidget(parent), ICocoaEventReceiver(NULL, qt_mac_NSStringToQString(NSControlTextDidChangeNotification)), m_textBuffer(QString()), m_isTextHidden(false), m_menu(NULL)
{
    this->d = new SearchLineEdit::Private(this);
    this->d->move(2, 2);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->setObject(this->d->searchField());
}

QSize SearchLineEdit::sizeHint() const
{
    return d->sizeHint() + QSize(6, 2);
}

void SearchLineEdit::setMenu(QMenu *menu)
{
    if (menu)
    {
        NSMenu *nsMenu = menu->macMenu(0);
        [[this->d->searchField() cell] setSearchMenuTemplate: nsMenu];
        this->m_menu = menu;
    }
}

QString SearchLineEdit::text() const
{
    NSSearchField *searchField = this->d->searchField();
    NSString *string = [searchField stringValue];
    QString text = qt_mac_NSStringToQString(string);
    return text;
}

void SearchLineEdit::setText(const QString &text)
{
    NSSearchField *searchField = this->d->searchField();
    NSString *string = qt_mac_QStringToNSString(text);
    [searchField setStringValue: string];
}

void SearchLineEdit::eventReceived(const QString &name, void *object, QMap<QString, void *> *userInfo)
{
    Q_UNUSED(name);
    Q_UNUSED(object);
    Q_UNUSED(userInfo);
    emit this->textChanged();
    emit this->searchRequested(this->text());
}

bool SearchLineEdit::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::EnabledChange:
    {
        NSSearchField *searchField = d->searchField();
        [searchField setEnabled: isEnabled() ? YES : NO];
        break;
    }
    default:
        break;
    }

    return QWidget::event(event);
}
