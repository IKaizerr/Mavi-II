#ifndef NDEBUG
#include <vld.h>
#endif

#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "PhysicsBox.hpp"

struct PhysicsCircle
{
    b2Body* body;
    float radius;
    Color color;
};

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "MAVI II - Bienvenida");
    SetTargetFPS(60);

    Color fondo = { 110, 100, 215, 255 };
    Color textoPrincipal = RAYWHITE;
    Color textoSecundario = DARKPURPLE;
    Color sueloColor = Fade(DARKGREEN, 0.7f);

    // Mundo físico
    b2Vec2 gravity(0.0f, 9.8f);
    b2World world(gravity);

    // -----------------------------
    // Suelo estático
    // -----------------------------
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(screenWidth / 2.0f, screenHeight - 40.0f);
    b2Body* groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(screenWidth / 2.0f, 20.0f);
    groundBody->CreateFixture(&groundShape, 0.0f);

    std::vector<PhysicsBox> boxes;
    std::vector<PhysicsCircle> circles;

    float spawnAngle = 0.0f; // Angulo configurable antes de crear la caja

    // -----------------------------
    // Crear algunas cajas dinámicas
    // -----------------------------
    for (int i = 0; i < 4; i++)
    {
        boxes.emplace_back(
            world,
            300.0f + i * 80.0f,
            80.0f + i * 20.0f,
            50.0f,
            50.0f,
            0.0f,
            Fade(SKYBLUE, 0.95f)
        );
    }

    // -----------------------------
    // Crear algunos círculos dinámicos
    // -----------------------------
    for (int i = 0; i < 3; i++)
    {
        b2BodyDef circleDef;
        circleDef.type = b2_dynamicBody;
        circleDef.position.Set(650.0f + i * 60.0f, 60.0f + i * 30.0f);

        b2Body* circleBody = world.CreateBody(&circleDef);

        b2CircleShape circleShape;
        circleShape.m_radius = 20.0f;

        b2FixtureDef circleFixture;
        circleFixture.shape = &circleShape;
        circleFixture.density = 1.0f;
        circleFixture.friction = 0.3f;
        circleFixture.restitution = 0.6f;

        circleBody->CreateFixture(&circleFixture);

        circles.push_back({ circleBody, 20.0f, Fade(ORANGE, 0.95f) });
    }

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) spawnAngle += 0.02f; // Girar a la derecha
        if (IsKeyDown(KEY_LEFT))  spawnAngle -= 0.02f; // Girar a la izquierda

        if (IsKeyPressed(KEY_SPACE))
        {
            boxes.emplace_back(
                world,
                screenWidth / 2.0f,
                60.0f,
                50.0f,
                50.0f,
                spawnAngle,          // Aplicar rotacion al crear la caja 
                Fade(RED, 0.95f)
            );
        }

        // Avanzar simulación
        world.Step(1.0f / 60.0f, 8, 3);

        BeginDrawing();
        ClearBackground(fondo);

        // Suelo visual
        float groundTop = (screenHeight - 40.0f) - 20.0f;
        DrawRectangle(0, (int)groundTop, screenWidth, 40, sueloColor);

        // Dibujar cajas
        for (const auto& box : boxes)
        {
            box.Draw();
        }

        // Dibujar círculos
        for (const auto& circle : circles)
        {
            b2Vec2 pos = circle.body->GetPosition();
            DrawCircleV({ pos.x, pos.y }, circle.radius, circle.color);
            DrawCircleLines((int)pos.x, (int)pos.y, circle.radius, BROWN);
        }

        // Preview de la caja
        Rectangle previewRect = {
            screenWidth / 2.0f,
            60.0f,
            50.0f,
            50.0f
        };

        Vector2 origin = { 25.0f, 25.0f };

        DrawRectanglePro(
            previewRect,
            origin,
            spawnAngle * RAD2DEG,
            Fade(RED, 0.3f) // Transparente para diferenciar
        );

        // Panel superior
        DrawRectangle(90, 70, 820, 90, Fade(BLACK, 0.18f));
        DrawText("Bienvenidos a Modelos y Algoritmos para Videojuegos II", 120, 90, 28, textoPrincipal);
        DrawText("Raylib dibuja. Box2D simula.", 320, 125, 22, textoSecundario);

        // Pie
        DrawText("Primer contacto con simulacion fisica en 2D", 300, 540, 20, RAYWHITE);
        DrawText("Flechas <- -> rotan | ESPACIO crea caja", 280, 510, 20, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}