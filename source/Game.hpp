#pragma once
#include "raylib.h"
#include <box2d.h>
#include <vector>
#include "PhysicsBox.hpp"
#include "PhysicsCircle.hpp"

class Game
{
private:
    const int screenWidth = 1000;
    const int screenHeight = 600;

    Color fondo;
    Color textoPrincipal;
    Color textoSecundario;
    Color sueloColor;

    // Mundo físico
    b2World world;

    // Suelo
    b2Body* groundBody;


    std::vector<PhysicsBox> boxes;
    std::vector<PhysicsCircle> circles;

    float spawnAngle;

    // Catapulta

    PhysicsCircle* projectile;
    bool launched;

    // Modos
    bool catapultMode;

    // Fuerza y ángulo
    float launchForce;
    float launchAngle;

    void Init();
    void Update();
    void Draw();

    void CreateInitialBoxes();
    void CreateInitialCircles();

    void CreateProjectile();
    void ResetProjectile();

public:
    Game();
    void Run();
};