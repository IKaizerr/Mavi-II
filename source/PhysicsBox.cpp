#include "PhysicsBox.hpp"

PhysicsBox::PhysicsBox(b2World& world, float x, float y, float w, float h, float angle, Color c)
{
    width = w;
    height = h;
    color = c;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    bodyDef.angle = angle; // Aplicar angulo inicial a la caja

    body = world.CreateBody(&bodyDef);

    b2PolygonShape boxShape;
    boxShape.SetAsBox(w / 2.0f, h / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.4f;
    fixtureDef.restitution = 0.2f;

    body->CreateFixture(&fixtureDef);
}

void PhysicsBox::Draw() const
{
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle() * RAD2DEG;

    Rectangle rect = {
        pos.x,
        pos.y,
        width,
        height
    };

    Vector2 origin = { width / 2.0f, height / 2.0f };

    DrawRectanglePro(rect, origin, angle, color);
}

b2Body* PhysicsBox::GetBody() const
{
    return body;
}