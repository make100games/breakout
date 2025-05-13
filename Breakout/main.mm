//
//  main.m
//  Breakout
//
//  Created by Chris Braunschweiler on 28.04.25.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>
#include "OldRenderer.hpp"
#include <memory>
#include <vector>
#include "Engine.hpp"
#include "KeyInput.hpp"

void runApp() {
    @autoreleasepool {
        NSApplication *app = [NSApplication sharedApplication];

        NSWindow *window = [[NSWindow alloc]
            initWithContentRect:NSMakeRect(100, 100, 800, 600)
                      styleMask:(NSWindowStyleMaskTitled |
                                 NSWindowStyleMaskClosable |
                                 NSWindowStyleMaskResizable)
                        backing:NSBackingStoreBuffered
                          defer:NO];
        [window setTitle:@"OpenGL C++ App"];
        [window makeKeyAndOrderFront:nil];

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
            0
        };

        NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        NSOpenGLContext *context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
        [context setView:window.contentView];
        [context makeCurrentContext];
        [app setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
        
        // Fire up our engine
        std::unique_ptr<Engine> gameEngine = std::make_unique<Engine>();
        gameEngine -> start();

        bool running = true;
        while (running) {
            NSEvent *event;
            while ((event = [app nextEventMatchingMask:NSEventMaskAny
                                              untilDate:nil
                                                 inMode:NSDefaultRunLoopMode
                                                dequeue:YES])) {
                if ([event type] == NSEventTypeKeyDown) {
                    NSString *chars = [event charactersIgnoringModifiers];
                    unichar keyChar = [chars characterAtIndex:0];

                    switch (keyChar) {
                        case NSLeftArrowFunctionKey:
                            gameEngine -> onKeyInput(KeyInput { KeyInteraction::KeyDown, KeyCode::LeftArrow });
                            break;
                        case NSRightArrowFunctionKey:
                            gameEngine -> onKeyInput(KeyInput { KeyInteraction::KeyDown, KeyCode::RightArrow });
                            break;
                        case ' ':   // Space
                            gameEngine -> onKeyInput(KeyInput { KeyInteraction::KeyDown, KeyCode::Space });
                            break;
                        case NSCarriageReturnCharacter:
                            printf("Hit enter key. Shutting down game!\n");
                            running = false;    // Exit if user hits Enter key
                            break;
                    }
                    
                    // 🚫 Skip [app sendEvent:event] to avoid beep that OS makes if event is not handled/consumed and propagated unnecessarily
                    continue;
                }
                if ([event type] == NSEventTypeKeyUp) {
                    NSString *chars = [event charactersIgnoringModifiers];
                    unichar keyChar = [chars characterAtIndex:0];

                    switch (keyChar) {
                        case NSLeftArrowFunctionKey:
                            gameEngine -> onKeyInput(KeyInput { KeyInteraction::KeyUp, KeyCode::LeftArrow });
                            break;
                        case NSRightArrowFunctionKey:
                            gameEngine -> onKeyInput(KeyInput { KeyInteraction::KeyUp, KeyCode::RightArrow });
                            break;
                        case ' ':   // Space
                            gameEngine -> onKeyInput(KeyInput { KeyInteraction::KeyUp, KeyCode::Space });
                            break;
                        default:
                            break;
                    }
                }
                [app sendEvent:event];
                [app updateWindows];
            }

            gameEngine -> update();
            [context flushBuffer];
        }
    }
}

int main(int argc, const char * argv[]) {
    runApp();
    return 0;
}
