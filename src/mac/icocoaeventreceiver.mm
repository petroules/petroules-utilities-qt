#include "icocoaeventreceiver.h"
#include "cocoaeventlistener.h"

class ICocoaEventReceiver::Private
{
public:
    Private(ICocoaEventReceiver *receiver = NULL)
        : listener(nil)
    {
        this->initializeListener(receiver);
    }

    void initializeListener(ICocoaEventReceiver *receiver)
    {
        // Delete any previous listener
        if (this->listener != nil)
        {
            [this->listener release];
            this->listener = nil;
        }

        // Add the current listener
        if (receiver)
        {
            this->listener = [[CocoaEventListener alloc] initWithReceiver: receiver];
        }
    }

private:
    CocoaEventListener *listener;
};

/*!
    \class ICocoaEventReceiver
    Interface for classes that will receive Objective-C events from a Cocoa object.
 */

/*!
    Constructs a new CocoaEventReceiver for the specified Objective-C object and event type.
 */
ICocoaEventReceiver::ICocoaEventReceiver(void *object, const QString &eventName)
    : m_object(object), m_eventName(eventName)
{
    this->d = new ICocoaEventReceiver::Private(this);
}

/*!
    The object to listen for events from. NULL to listen to events from all objects.
 */
void* ICocoaEventReceiver::object() const
{
    return this->m_object;
}

void ICocoaEventReceiver::setObject(void *object)
{
    this->m_object = object;

    // Whenever the object is changed we need to reinitialize the listener
    this->d->initializeListener(this);
}

/*!
    The name of the Objective-C event to listen to. Empty string to listen to all types of events.
 */
QString ICocoaEventReceiver::eventName() const
{
    return this->m_eventName;
}

void ICocoaEventReceiver::setEventName(const QString &eventName)
{
    this->m_eventName = eventName;

    // Whenever the event name is changed we need to reinitialize the listener
    this->d->initializeListener(this);
}
