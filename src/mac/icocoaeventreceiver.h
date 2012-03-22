#ifndef ICOCOAEVENTRECEIVER_H
#define ICOCOAEVENTRECEIVER_H

#include <QtCore>

class ICocoaEventReceiver
{
public:
    ICocoaEventReceiver(void* object = NULL, const QString &eventName = QString());
    void* object() const;
    void setObject(void* object);
    QString eventName() const;
    void setEventName(const QString &eventName);

public slots:
    virtual void eventReceived(const QString &name, void* object, QMap<QString, void*> *userInfo) = 0;

private:
    class Private;
    Private *d;
    void* m_object;
    QString m_eventName;
};

#endif // ICOCOAEVENTRECEIVER_H
