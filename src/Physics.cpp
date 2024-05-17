#include "Physics.h"
#include "Components.h"
#include "Entity.h"
#include <iostream>
#include <cmath>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 posA = a->getComponent<CTransform>().pos;
    Vec2 posB = b->getComponent<CTransform>().pos;

    Vec2 delta( abs(posA.x - posB.x), abs(posA.y - posB.y));

    Vec2 halfSizeA = a->getComponent<CBoundingBox>().halfSize;
    Vec2 halfSizeB = b->getComponent<CBoundingBox>().halfSize;

    Vec2 overlap = halfSizeA + halfSizeB - delta;
    return overlap;
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 prevPosA = a->getComponent<CTransform>().prevPos;
    Vec2 prevPosB = b->getComponent<CTransform>().prevPos;

    Vec2 delta( abs(prevPosA.x - prevPosB.x), abs(prevPosA.y - prevPosB.y));

    Vec2 halfSizeA;
    Vec2 halfSizeB = b->getComponent<CBoundingBox>().halfSize;
    
    if ( a->getComponent<CBoundingBox>().switching == true )
    {
        halfSizeA = a->getComponent<CBoundingBox>().prevHalfSize;
    }
    else 
    {
        halfSizeA = a->getComponent<CBoundingBox>().halfSize;
    }
    Vec2 overlap = halfSizeA + halfSizeB - delta;
    return overlap;
}
