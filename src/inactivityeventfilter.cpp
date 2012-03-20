#include "inactivityeventfilter.h"

/*!
    \class InactivityEventFilter

    The InactivityEventFilter class provides an event filter which emits a signal whenever any key
    or mouse events are generated.

    This class is intended for use as part of an idle timer mechanism.
 */

/*!
    Constructs a new InactivityEventFilter.
 */
InactivityEventFilter::InactivityEventFilter(QObject *parent) :
    QObject(parent)
{
}

/*!
    Reimplemented from QObject::eventFilter(QObject*, QEvent*) to filter the desired events.

    \sa InactivityEventFilter
 */
bool InactivityEventFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress ||
        event->type() == QEvent::KeyRelease ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseMove)
    {
        emit this->resetIdleTimer(object);
    }

    return QObject::eventFilter(object, event);
}
