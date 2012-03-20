#include "cocoainitializer.h"
#import <Cocoa/Cocoa.h>

class CocoaInitializer::Private
{
public:
    NSAutoreleasePool *pool;
};

CocoaInitializer::CocoaInitializer()
{
    this->d = new CocoaInitializer::Private();
    NSApplicationLoad();
    this->d->pool = [[NSAutoreleasePool alloc] init];
}

CocoaInitializer::~CocoaInitializer()
{
    [this->d->pool drain];
    delete this->d;
}
