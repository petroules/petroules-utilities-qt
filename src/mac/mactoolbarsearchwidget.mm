#include "mactoolbarsearchwidget.h"
#include "icocoaeventreceiver.h"
#include "machelpers.h"
#import <QMacCocoaViewContainer>
#import <Cocoa/Cocoa.h>

class MacToolbarSearchWidget::Private : public QMacCocoaViewContainer
{
    // Do not use Q_OBJECT with pimpl - it will generate a *_moc.cpp file which won't correctly compile with Objective-C++

public:
    Private(QWidget *parent = NULL)
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

MacToolbarSearchWidget::MacToolbarSearchWidget(QWidget *parent)
    : IToolbarSearchWidget(parent), ICocoaEventReceiver(NULL, qt_mac_NSStringToQString(NSControlTextDidChangeNotification)), m_textBuffer(QString()), m_isTextHidden(false), m_menu(NULL)
{
    this->d = new MacToolbarSearchWidget::Private(this);
    this->d->move(2, 2);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->setObject(this->d->searchField());
}

MacToolbarSearchWidget::~MacToolbarSearchWidget()
{
    delete this->d;
}

QSize MacToolbarSearchWidget::sizeHint() const
{
    return this->d->sizeHint() + QSize(6, 2);
}

QMenu* MacToolbarSearchWidget::menu() const
{
    return this->m_menu;
}

void MacToolbarSearchWidget::setMenu(QMenu *menu)
{
    if (menu)
    {
        NSMenu *nsMenu = menu->macMenu(0);
        [[this->d->searchField() cell] setSearchMenuTemplate: nsMenu];
        this->m_menu = menu;
    }
}

bool MacToolbarSearchWidget::isEnabled() const
{
    NSSearchField *searchField = this->d->searchField();
    bool isSearchFieldEnabled = [searchField isEnabled] ? true : false;

    bool isWidgetEnabled = QWidget::isEnabled();
    if (isSearchFieldEnabled != isWidgetEnabled)
    {
        qWarning() << "Enabled/disabled states of NSSearchField and Qt wrapper class do not match. Search field is "
                   << isSearchFieldEnabled << " and widget is " << isWidgetEnabled << ". Returning widget's state.";
    }

    return isWidgetEnabled;
}

void MacToolbarSearchWidget::setEnabled(bool enable)
{
    NSSearchField *searchField = this->d->searchField();
    [searchField setEnabled: enable ? YES : NO];
    QWidget::setEnabled(enable);
}

QString MacToolbarSearchWidget::text() const
{
    NSSearchField *searchField = this->d->searchField();
    NSString *string = [searchField stringValue];
    QString text = qt_mac_NSStringToQString(string);
    return text;
}

void MacToolbarSearchWidget::setText(const QString &text)
{
    NSSearchField *searchField = this->d->searchField();
    NSString *string = qt_mac_QStringToNSString(text);
    [searchField setStringValue: string];
}

void MacToolbarSearchWidget::clear()
{
    this->setText(QString());
}

bool MacToolbarSearchWidget::isTextHidden() const
{
    return this->m_isTextHidden;
}

void MacToolbarSearchWidget::setTextHidden(bool hide)
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

void MacToolbarSearchWidget::eventReceived(const QString &name, void *object, QMap<QString, void *> *userInfo)
{
    Q_UNUSED(name);
    Q_UNUSED(object);
    Q_UNUSED(userInfo);
    emit this->textChanged();
    emit this->searchRequested(this->text());
}
