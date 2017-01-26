#include "cocoabridge.h"

//#import <Foundation/Foundation.h>

#import <AppKit/AppKit.h>

void CocoaBridge::sendCurrentEvent()
{
   NSEvent *currentEvent = [NSApp currentEvent];
   [NSApp sendEvent:currentEvent];
}
