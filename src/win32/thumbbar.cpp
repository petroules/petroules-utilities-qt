#include "thumbbar.h"
#include "integratedmainwindow.h"
#include "integratedmainwindow_p.h"
#include "windowstaskbarintegration_includes.h"

#ifdef Q_WS_WIN
DEFINE_GUID(CLSID_TaskbarList,0x56fdf344,0xfd6d,0x11d0,0x95,0x8a,0x0,0x60,0x97,0xc9,0xa0,0x90);
DEFINE_GUID(IID_ITaskbarList,0x56FDF342,0xFD6D,0x11d0,0x95,0x8A,0x00,0x60,0x97,0xC9,0xA0,0x90);
DEFINE_GUID(IID_ITaskbarList2,0x602D4995,0xB13A,0x429b,0xA6,0x6E,0x19,0x35,0xE4,0x4F,0x43,0x17);
DEFINE_GUID(IID_ITaskbarList3,0xea1afb91,0x9e28,0x4b86,0x90,0xE9,0x9e,0x9f,0x8a,0x5e,0xef,0xaf);
DEFINE_GUID(IID_ITaskbarList4,0xc43dc798,0x95d1,0x4bea,0x90,0x30,0xbb,0x99,0xe2,0x98,0x3a,0x1a);
#endif

#ifdef Q_WS_WIN
struct ThumbBar::ThumbButtonProperties
{
    ThumbButtonProperties() : drawBorder(true), interactive(true), dismissOnClick(false) { }
    bool drawBorder;
    bool interactive;
    bool dismissOnClick;
};
#endif

class ThumbBar::Private
{
#ifdef Q_WS_WIN
public:
    Private(IntegratedMainWindow *window) : window(window), buttonList(0), buttonListSize(0)
    {
        m_taskbar = window->d->m_taskbar;
    }

    IntegratedMainWindow *window;
    ITaskbarList3 *m_taskbar;
    QList<QAction*> actions;
    LPTHUMBBUTTON buttonList;
    int buttonListSize;
    QMap<QAction*, ThumbButtonProperties*> properties;
#endif
};

ThumbBar::ThumbBar(IntegratedMainWindow *window) :
    QObject(window), d(NULL)
{
#ifdef Q_WS_WIN
    this->d = new Private(window);
#endif
}

ThumbBar::~ThumbBar()
{
#ifdef Q_WS_WIN
    if (this->d->buttonList)
    {
        free(reinterpret_cast<void*>(this->d->buttonList));
    }

    delete this->d;
#endif
}

QList<QAction*> ThumbBar::actions() const
{
#ifdef Q_WS_WIN
    return this->d->actions;
#else
    return QList<QAction*>();
#endif
}

void ThumbBar::addAction(QAction *action)
{
#ifdef Q_WS_WIN
    this->d->actions.append(action);
#else
    Q_UNUSED(action);
#endif
}

QAction* ThumbBar::addAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *member)
{
#ifdef Q_WS_WIN
    QAction *action = new QAction(icon, text, NULL);
    QObject::connect(action, SIGNAL(triggered()), receiver, member);
    this->d->actions.append(action);
    return action;
#else
    Q_UNUSED(icon);
    Q_UNUSED(text);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
    return NULL;
#endif
}

QAction* ThumbBar::addAction(const QIcon &icon, const QString &text)
{
#ifdef Q_WS_WIN
    QAction *action = new QAction(icon, text, NULL);
    this->d->actions.append(action);
    return action;
#else
    Q_UNUSED(icon);
    Q_UNUSED(text);
    return NULL;
#endif
}

QAction* ThumbBar::addSeparator()
{
#ifdef Q_WS_WIN
    QAction *action = new QAction(0);
    action->setSeparator(true);
    this->d->actions.append(action);
    return action;
#else
    return NULL;
#endif
}

void ThumbBar::setDrawBorder(QAction *action, bool border)
{
#ifdef Q_WS_WIN
    if (!this->d->properties.contains(action))
    {
        this->d->properties.insert(action, new ThumbButtonProperties());
    }

    this->d->properties.value(action)->drawBorder = border;
#else
    Q_UNUSED(action);
    Q_UNUSED(border);
#endif
}

void ThumbBar::setInteractive(QAction *action, bool interactive)
{
#ifdef Q_WS_WIN
    if (!this->d->properties.contains(action))
    {
        this->d->properties.insert(action, new ThumbButtonProperties());
    }

    this->d->properties.value(action)->interactive = interactive;
#else
    Q_UNUSED(action);
    Q_UNUSED(interactive);
#endif
}

void ThumbBar::setDismissOnClick(QAction *action, bool dismiss)
{
#ifdef Q_WS_WIN
    if (!this->d->properties.contains(action))
    {
        this->d->properties.insert(action, new ThumbButtonProperties());
    }

    this->d->properties.value(action)->dismissOnClick = dismiss;
#else
    Q_UNUSED(action);
    Q_UNUSED(dismiss);
#endif
}

#ifdef Q_WS_WIN
bool ThumbBar::winEvent(MSG *message, long *result)
{
    Q_UNUSED(result);

    // Get the index of the pressed taskbar button
    int buttonId = LOWORD(message->wParam) - IDTB_FIRST;
    if (buttonId >= 0 && buttonId < this->d->actions.size())
    {
        this->d->actions.at(buttonId)->trigger();
    }

    return false;
}
#endif

QSize ThumbBar::imageSize()
{
#ifdef Q_WS_WIN
    return QSize(GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
#else
    return QSize();
#endif
}

void ThumbBar::show()
{
#ifdef Q_WS_WIN
    if (!this->d->buttonList)
    {
        this->setButtonList();
    }
#endif
}

/*!
    Updates the taskbar with the current button list. This should be called if the number of buttons has changed.
 */
void ThumbBar::setButtonList()
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return;
    }

    // Make sure we've only got 7 buttons
    Q_ASSERT(d->actions.size() <= ThumbBar::maxButtons());
    while (d->actions.size() > ThumbBar::maxButtons())
    {
        d->actions.removeLast();
    }

    // Set the button list size and allocate memory for the native list
    d->buttonListSize = d->actions.size();
    d->buttonList = reinterpret_cast<LPTHUMBBUTTON>(malloc(sizeof(THUMBBUTTON) * d->buttonListSize));

    // Convert the Qt buttons to native button structs
    for (int i = 0; i < d->actions.size(); i++)
    {
        QAction *action = d->actions.at(i);
        setFlagsForAction(action, i);
        QObject::connect(action, SIGNAL(changed()), SLOT(updateButtonList()));
    }

    d->m_taskbar->ThumbBarAddButtons(d->window->winId(), d->buttonListSize, d->buttonList);
#endif
}

void ThumbBar::setFlagsForAction(QAction *action, int i)
{
#ifdef Q_WS_WIN
    d->buttonList[i].iId = IDTB_FIRST + i;

    // Maximum is 260 WCHARs, so we need to make sure we don't exceed that amount
    if (action->toolTip().length() > 260)
    {
        action->setToolTip(action->toolTip().left(260));
    }

#ifdef Q_CC_MSVC
    // MSVC++ complains about wcscpy being unsafe so we use its preferred function
    wcscpy_s(d->buttonList[i].szTip, action->toolTip().toStdWString().c_str());
#else
    wcscpy(d->buttonList[i].szTip, action->toolTip().toStdWString().c_str());
#endif

    // For image lists... we're not using this since icons are so much easier to deal with
    d->buttonList[i].iBitmap = 0;

    d->buttonList[i].hIcon = !action->isSeparator() ? action->icon().pixmap(ThumbBar::imageSize()).toWinHICON() : 0;

    THUMBBUTTONMASK mask = static_cast<THUMBBUTTONMASK>(0);
    if (!action->icon().isNull() && !action->isSeparator())
    {
        mask = static_cast<THUMBBUTTONMASK>(mask | THB_ICON);
    }

    mask = static_cast<THUMBBUTTONMASK>(mask | THB_FLAGS);

    if (!action->toolTip().isEmpty() && !action->isSeparator())
    {
        mask = static_cast<THUMBBUTTONMASK>(mask | THB_TOOLTIP);
    }

    d->buttonList[i].dwMask = mask;

    THUMBBUTTONFLAGS flags = action->isEnabled() && !action->isSeparator() ? THBF_ENABLED : THBF_DISABLED;

    ThumbButtonProperties *props = d->properties.value(action, new ThumbButtonProperties());
    if (props->dismissOnClick && !action->isSeparator())
    {
        flags = static_cast<THUMBBUTTONFLAGS>(mask | THBF_DISMISSONCLICK);
    }

    if (!props->drawBorder && !action->isSeparator())
    {
        flags = static_cast<THUMBBUTTONFLAGS>(mask | THBF_NOBACKGROUND);
    }

    if (!action->isVisible())
    {
        flags = static_cast<THUMBBUTTONFLAGS>(mask | THBF_HIDDEN);
    }

    if (!props->interactive)
    {
        flags = static_cast<THUMBBUTTONFLAGS>(mask | THBF_NONINTERACTIVE);
    }

    d->buttonList[i].dwFlags = flags;
#else
    Q_UNUSED(action);
    Q_UNUSED(i);
#endif
}

/*!
    Updates the taskbar with the current button list. This should be called if the number of buttons has not changed
    but the properties of individual buttons has.
 */
void ThumbBar::updateButtonList()
{
#ifdef Q_WS_WIN
    if (!d->m_taskbar)
    {
        return;
    }

    for (int i = 0; i < d->buttonListSize; i++)
    {
        this->setFlagsForAction(d->actions.at(i), i);
    }

    d->m_taskbar->ThumbBarUpdateButtons(d->window->winId(), d->buttonListSize, d->buttonList);
#endif
}
