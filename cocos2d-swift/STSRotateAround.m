/*
 *  STSRotateAround.m
 *
 *  Copyright (c) 2014 Stefan Schmitt.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#import "STSRotateAround.h"


struct PolarCoord {
    float r; // radius
    float a; // angle
};

typedef struct PolarCoord PolarCoord;

/** Converts a cartesian coordinate to a polar coordinate */
static inline PolarCoord convertCartesianToPolar(const CGPoint p)
{
    const float r = hypotf(p.x, p.y);
    const float rad = atan2f(p.y, p.x);
    const float a = CC_RADIANS_TO_DEGREES(rad);
    return (PolarCoord){r,a};
}

/** Converts a polar coordinate to a cartesian coordinate */
static inline CGPoint convertPolarToCartesian(const PolarCoord p)
{
    const float rad = CC_DEGREES_TO_RADIANS(p.a);
    const float x = p.r * cosf(rad);
    const float y = p.r * sinf(rad);
    return CGPointMake(x, y);
}


#pragma mark - STSRotateAround
@interface STSRotateAround ()

@property (nonatomic, assign) CGPoint centerOfRotation;
@property (nonatomic, assign) PolarCoord startPolarCoord;
@property (nonatomic, assign) BOOL reversed;

@end



@implementation STSRotateAround

+ (id)actionWithDuration:(CCTime)duration centerOfRotation:(CGPoint)center
{
	return [[ self alloc] initWithDuration:duration centerOfRotation:center reverse:NO];
}

+ (id)actionWithDuration:(CCTime)duration centerOfRotation:(CGPoint)center reverse:(BOOL)reverse
{
	return [[ self alloc] initWithDuration:duration centerOfRotation:center reverse:reverse];
}

- (id)initWithDuration:(CCTime)duration centerOfRotation:(CGPoint)center reverse:(BOOL)reverse
{
	self =  [super initWithDuration:duration];
    if( self ) {
        _centerOfRotation   = center;
        _reversed           = reverse;
    }
    
	return self;
}

- (id)copyWithZone: (NSZone*) zone
{
	CCAction *copy = [[[self class] allocWithZone: zone] initWithDuration:[self duration]
                                                         centerOfRotation:[self centerOfRotation]
                                                                  reverse:[self reversed]];
	return copy;
}

- (void)startWithTarget:(id)target
{
	NSAssert([target respondsToSelector:@selector(position)], @"target must be subclass of CCNode");
    [super startWithTarget:target];
    if (target) {
        const CGPoint pos = [((CCNode*)target) position];
        const CGPoint vec = CGPointMake(pos.x - _centerOfRotation.x, pos.y - _centerOfRotation.y);
        self.startPolarCoord = convertCartesianToPolar(vec);
    }
}

- (void)update: (CCTime) t
{
	if( ! [self isDone]
       && [self target])
    {
		const float elapsed = [super elapsed];
        const float percent = elapsed / _duration;
        const float angle = _reversed ? 360.f * -percent : 360.f * percent;
        
        const PolarCoord polarCoord = {_startPolarCoord.r, _startPolarCoord.a + angle};
        CGPoint pos = convertPolarToCartesian(polarCoord);
        pos = CGPointMake(_centerOfRotation.x + pos.x, _centerOfRotation.y + pos.y);
        [(CCNode*)[self target] setPosition:pos];
	}
}

- (void)stop
{
	[super stop];
}

- (CCActionInterval*)reverse
{
	return [[self class] actionWithDuration:[self duration]
                           centerOfRotation:[self centerOfRotation]
                                    reverse:YES];
}

@end
