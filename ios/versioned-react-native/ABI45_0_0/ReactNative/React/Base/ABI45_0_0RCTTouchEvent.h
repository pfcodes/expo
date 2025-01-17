/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <Foundation/Foundation.h>

#import <ABI45_0_0React/ABI45_0_0RCTEventDispatcherProtocol.h>

/**
 * Represents a touch event, which may be composed of several touches (one for every finger).
 * For more information on contents of passed data structures see ABI45_0_0RCTTouchHandler.
 */
@interface ABI45_0_0RCTTouchEvent : NSObject <ABI45_0_0RCTEvent>

- (instancetype)initWithEventName:(NSString *)eventName
                         ABI45_0_0ReactTag:(NSNumber *)ABI45_0_0ReactTag
                     ABI45_0_0ReactTouches:(NSArray<NSDictionary *> *)ABI45_0_0ReactTouches
                   changedIndexes:(NSArray<NSNumber *> *)changedIndexes
                    coalescingKey:(uint16_t)coalescingKey NS_DESIGNATED_INITIALIZER;
@end
