#ifndef HEVENT_H
#define HEVENT_H

#include <QEvent>

static const QEvent::Type HEvent_UpdateItem=(QEvent::Type)QEvent::registerEventType(QEvent::User+100);

class HEvent : public QEvent
{
public:

    explicit HEvent(QEvent::Type type);


    QVariant data;
};

#endif // HEVENT_H
