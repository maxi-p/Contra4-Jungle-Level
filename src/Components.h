#pragma once
#include <memory>
#include "Vec2.h"
#include "Animation.h"
#include "Assets.h"

class Component
{
public:
    bool has = false;
};

class CTransform : public Component
{
public:
    Vec2    pos         = { 0.0, 0.0 };
    Vec2    prevPos     = { 0.0, 0.0 };
    Vec2    scale       = { 1.0, 1.0 };
    Vec2    velocity    = { 0.0, 0.0 };
    float   angle       =   0;

    CTransform() {}
    CTransform(const Vec2& p)
        : pos(p) {}
    CTransform (const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
        : pos(p), velocity(sp), scale(sc), angle(a){}
};

class CLifeSpan : public Component
{
public:
    int lifespan        = 0;
    int frameCreated    = 0;
    CLifeSpan() {}
    CLifeSpan(int duration, int frame)
        : lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component
{
public:
    bool up         = false;
    bool down       = false;
    bool left       = false;
    bool right      = false;
    bool shoot      = false;
    bool shift      = false;
    bool canShoot   = true;
    bool canJump    = true;

    CInput() {}
};

class CBoundingBox : public Component
{
public:
    Vec2 size;
    Vec2 halfSize;
    Vec2 prevSize;
    Vec2 prevHalfSize;
    bool switching      = false;
    CBoundingBox() {}
    CBoundingBox(const Vec2& s)
        : size(s), halfSize(s/2) {}
    void setSize(const Vec2& s)
    {
        prevSize = size;
        prevHalfSize = Vec2(halfSize.x,halfSize.y) ;
        size = s;
        halfSize = s/2;
    }
};

class CAnimation : public Component
{
public:
    std::shared_ptr<Animation> animation;
    bool repeat = false;
    CAnimation() {}
    CAnimation(const std::shared_ptr<Animation> animation, bool r)
        : animation(animation), repeat(r) {}
};

class CGravity : public Component
{
public:
    float gravity = 0;
    CGravity() {}
    CGravity(float g) : gravity(g) {}
};

class CState : public Component
{
public:
    std::string state = "DEFAULT";
    std::string subState = "DEFAULT";
    size_t timer = 0;
    CState() {}
    CState(const std::string& state) : state(state) {}
    CState(const std::string& state, const std::string& subState) : state(state), subState(subState) {}
};
