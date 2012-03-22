#include "cocoaeventlistener.h"
#include "icocoaeventreceiver.h"
#include "machelpers.h"

/*!
    Objective-C helper class used to pass events back to a C++ receiver class.
 */
@implementation CocoaEventListener

/*!
    Returns a CocoaEventListener object initialized with the specified receiver.
 */
-(id) initWithReceiver: (ICocoaEventReceiver*) receiver
{
    if ((self = [super init]))
    {
        [self setReceiver: receiver];
    }

    return self;
}

-(void) dealloc
{
    [self setReceiver: NULL];
    [super dealloc];
}

/*!
    Returns the receiver class that will receive events handled by this listener.
 */
-(ICocoaEventReceiver*) receiver
{
    return self->m_receiver;
}

/*!
    Sets the receiver class that will receive events handled by this listener.
 */
-(void) setReceiver: (ICocoaEventReceiver*) receiver
{
    // Remove observation of any previous widget's events
    [[NSNotificationCenter defaultCenter] removeObserver: self];

    if (receiver)
    {
        self->m_receiver = receiver;

        NSString *eventName = nil;
        if (!receiver->eventName().isEmpty())
        {
            eventName = qt_mac_QStringToNSString(receiver->eventName());
        }

        [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(eventReceived:) name: eventName object: reinterpret_cast<id>(receiver->object())];
    }
}

/*!
    Sends notification of a posted event to the receiver.
 */
-(void) eventReceived: (NSNotification*) note
{
    if (self->m_receiver)
    {
        QMap<QString, void*> *userInfo = NULL;
        NSDictionary *cocoaUserInfo = [note userInfo];
        if (cocoaUserInfo != nil)
        {
            userInfo = new QMap<QString, void*>();
            for (NSString *key in cocoaUserInfo)
            {
                userInfo->insert(qt_mac_NSStringToQString(key), [cocoaUserInfo objectForKey: key]);
            }
        }

        self->m_receiver->eventReceived(qt_mac_NSStringToQString([note name]), reinterpret_cast<void*>([note object]), userInfo);
    }
}

@end
