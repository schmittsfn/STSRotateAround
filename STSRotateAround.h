/*
 *  STSRotateAround.h
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

#ifndef __STSRotateAround__
#define __STSRotateAround__

#include "CCActionInterval.h"


struct PolarCoord {
    float r; // radius
    float a; // angle
};

/** Converts a polar coordinate to a cartesian coordinate */
inline cocos2d::Point convertPolarToCartesian(const PolarCoord p)
{
    float rad = CC_DEGREES_TO_RADIANS(p.a);
    float x = p.r * cosf(rad);
    float y = p.r * sinf(rad);
    return cocos2d::Point(x,y);
}

/** Converts a cartesian coordinate to a polar coordinate */
inline PolarCoord convertCartesianToPolar(const cocos2d::Point& p)
{
    float r = hypotf(p.x, p.y);
    float rad = atan2f(p.y, p.x);
    float a = CC_RADIANS_TO_DEGREES(rad);
    return {r,a};
}



/**
 @brief Makes the target move around a center of rotation
 */
class RotateAround : public cocos2d::ActionInterval
{
public:
    typedef cocos2d::ActionInterval super;
    
	/**
     * Create the action
     * @param duration: the duration of the flickering
     * @param centerOfRotation: the point around which to rotate
     */
	static RotateAround* create(const float& duration,
                                const cocos2d::Point& centerOfRotation,
                                const bool reverse = false);
    
	//
    // Override
    //
    virtual void startWithTarget(cocos2d::Node* target) override;
	virtual void update(float time) override;
	virtual RotateAround* clone() const override;
	virtual RotateAround* reverse() const override;
    
CC_CONSTRUCTOR_ACCESS:
    RotateAround();
    explicit RotateAround(const cocos2d::Point& centerOfRotation, const bool& reverse);
    virtual ~RotateAround() {};
    
	/** init the action */
	bool initWithDuration(const float& duration);
    
protected:
    const cocos2d::Point    _centerOfRotation;
    PolarCoord              _startPolarCoord; // relative to center of rotation
    const bool              _reversed;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(RotateAround);
    
};

#endif /* defined(__STSRotateAround__) */
