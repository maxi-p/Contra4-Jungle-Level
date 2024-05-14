#include "Physics.h"
#include "Components.h"
#include "Entity.h"
#include <cmath>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 delta( abs(a->getComponent<CTransform>().pos.x - b->getComponent<CTransform>().pos.x),
                abs(a->getComponent<CTransform>().pos.y - b->getComponent<CTransform>().pos.y));
    float oX = a->getComponent<CBoundingBox>().size.x/2 + b->getComponent<CBoundingBox>().size.x/2 - delta.x;
    float oY = a->getComponent<CBoundingBox>().size.y/2 + b->getComponent<CBoundingBox>().size.y/2 - delta.y;
    return Vec2(oX, oY);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 delta( abs(a->getComponent<CTransform>().prevPos.x - b->getComponent<CTransform>().prevPos.x),
                abs(a->getComponent<CTransform>().prevPos.y - b->getComponent<CTransform>().prevPos.y));
    float oX = a->getComponent<CBoundingBox>().size.x/2 + b->getComponent<CBoundingBox>().size.x/2 - delta.x;
    float oY = a->getComponent<CBoundingBox>().size.y/2 + b->getComponent<CBoundingBox>().size.y/2 - delta.y;
    return Vec2(oX, oY);

    return Vec2(0, 0);
}
