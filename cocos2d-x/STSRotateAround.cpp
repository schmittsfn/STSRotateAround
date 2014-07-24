/*
 *  STSRotateAround.cpp
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

#include "STSRotateAround.h"

USING_NS_CC;

RotateAround::RotateAround()
: _centerOfRotation(Point::ZERO)
, _startPolarCoord({0.f,0.f})
, _reversed(false)
{
    
}

RotateAround::RotateAround(const cocos2d::Point& centerOfRotation, const bool& reverse)
: _centerOfRotation(centerOfRotation)
, _startPolarCoord({0.f,0.f})
, _reversed(reverse)
{
    
}

RotateAround* RotateAround::create(const float duration,
                                   const cocos2d::Point centerOfRotation,
                                   const bool reverse /*=false*/)
{
	RotateAround* pRet = new RotateAround(centerOfRotation, reverse);
    
	if (pRet && pRet->initWithDuration(duration))
    {
		pRet->autorelease();
        return pRet;
	}
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool RotateAround::initWithDuration(const float& duration)
{
    if (super::initWithDuration(duration)) {
        
        return true;
    }
    return false;
}

void RotateAround::startWithTarget(cocos2d::Node* target)
{
    super::startWithTarget(target);
    if (_target) {
        const Point pos = _target->getPosition();
        const Vec2 vec = Vec2(pos.x - _centerOfRotation.x, pos.y - _centerOfRotation.y);
        _startPolarCoord = convertCartesianToPolar(vec);
    }
}

void RotateAround::update(float time)
{
	CC_UNUSED_PARAM(time);
    
    if(_target) {
        const float elapsed = getElapsed();
        const float percent = elapsed / _duration;
        const float angle = _reversed ? 360.f * -percent : 360.f * percent;
        
        const PolarCoord polarCoord {_startPolarCoord.r, _startPolarCoord.a + angle};
        Point pos = convertPolarToCartesian(polarCoord);
        pos = Point(_centerOfRotation.x + pos.x, _centerOfRotation.y + pos.y);
        _target->setPosition(pos);
    }
}

RotateAround* RotateAround::reverse() const
{
    const bool reverse = true;
    return RotateAround::create(_duration, _centerOfRotation, reverse);
}

RotateAround* RotateAround::clone() const
{
	const bool reverse = true;
    RotateAround* pRet = new RotateAround(_centerOfRotation, reverse);
    
	if (pRet && pRet->initWithDuration(_duration))
    {
		pRet->autorelease();
        return pRet;
	}
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}