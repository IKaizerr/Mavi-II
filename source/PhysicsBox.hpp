#pragma once
#include <box2d.h>
#include "raylib.h"

class PhysicsBox
{
private:
    b2Body* body;
    float width;
    float height;
    Color color;

public:
    PhysicsBox(b2World& world, float x, float y, float w, float h, float angle, Color c);

    void Draw() const;

    b2Body* GetBody() const;
};