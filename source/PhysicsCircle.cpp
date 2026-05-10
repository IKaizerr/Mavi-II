#include "PhysicsCircle.hpp"

PhysicsCircle::PhysicsCircle(
    b2World& world,
    float x,
    float y,
    float r,
    Color c
)
{
    radius = r;
    color = c;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);

    body = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = radius;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.6f;

    body->CreateFixture(&fixtureDef);
}

void PhysicsCircle::Draw() const
{
    b2Vec2 pos = body->GetPosition();

    DrawCircleV(
        { pos.x, pos.y },
        radius,
        color
    );

    DrawCircleLines(
        (int)pos.x,
        (int)pos.y,
        radius,
        BROWN
    );
}

b2Body* PhysicsCircle::GetBody() const
{
    return body;
}

float PhysicsCircle::GetRadius() const
{
    return radius;
}