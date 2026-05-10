#pragma once
#include <box2d.h>
#include "raylib.h"

class PhysicsCircle
{
private:
    b2Body* body;
    float radius;
    Color color;

public:
    PhysicsCircle(
        b2World& world,
        float x,
        float y,
        float r,
        Color c
    );

    void Draw() const;

    b2Body* GetBody() const;

    float GetRadius() const;
};