#include "Game.hpp"
#include <cmath>

Game::Game()
    : world(b2Vec2(0.0f, 9.8f))
{
    groundBody = nullptr;

    fondo = { 110, 100, 215, 255 };
    textoPrincipal = RAYWHITE;
    textoSecundario = DARKPURPLE;
    sueloColor = Fade(DARKGREEN, 0.7f);

    spawnAngle = 0.0f;

    projectile = nullptr;
    launched = false;

    catapultMode = false;

    launchForce = 0.0f;
    launchAngle = -45.0f;
}

void Game::Init()
{
    InitWindow(
        screenWidth,
        screenHeight,
        "MAVI II - La Catapulta de Mavix"
    );

    SetTargetFPS(60);

    // Suelo estático

    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;

    groundDef.position.Set(
        screenWidth / 2.0f,
        screenHeight - 40.0f
    );

    groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(
        screenWidth / 2.0f,
        20.0f
    );

    groundBody->CreateFixture(
        &groundShape,
        0.0f
    );


    // Crear cajas iniciales

    CreateInitialBoxes();

    // Crear círculos iniciales

    CreateInitialCircles();

    // Crear proyectil

    CreateProjectile();
}

void Game::CreateInitialBoxes()
{
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
}

void Game::CreateInitialCircles()
{
    for (int i = 0; i < 3; i++)
    {
        circles.emplace_back(
            world,
            650.0f + i * 60.0f,
            60.0f + i * 30.0f,
            20.0f,
            Fade(ORANGE, 0.95f)
        );
    }
}

void Game::CreateProjectile()
{
    projectile = new PhysicsCircle(
        world,
        120.0f,               // Esta a esta altura la catapulta ya que al crearse muchas cajas y estas caigan a la izquierda 
        screenHeight / 2.0f,  // Esta queda sin poder usarse osea queda tosqueada o atascada
        20.0f,                
        Fade(YELLOW, 0.95f)
    );

    // Esfera estatica esperando ser lanzada 
    projectile->GetBody()->SetAwake(false);

    launched = false;
    launchForce = 0.0f;
}

void Game::ResetProjectile()
{
    if (projectile != nullptr)
    {
        world.DestroyBody(
            projectile->GetBody()
        );

        delete projectile;
        projectile = nullptr;
    }

    CreateProjectile();
}

void Game::Update()
{
    // TAB -> cambiar entre modos
    if (IsKeyPressed(KEY_TAB))
    {
        catapultMode = !catapultMode;
    }


    // MODO CAJAS

    if (!catapultMode)
    {
        if (IsKeyDown(KEY_RIGHT))
            spawnAngle += 0.02f;

        if (IsKeyDown(KEY_LEFT))
            spawnAngle -= 0.02f;

        if (IsKeyPressed(KEY_SPACE))
        {
            boxes.emplace_back(
                world,
                screenWidth / 2.0f,
                60.0f,
                50.0f,
                50.0f,
                spawnAngle,
                Fade(RED, 0.95f)
            );
        }
    }


    // MODO CATAPULTA

    else
    {
        if (IsKeyDown(KEY_RIGHT))
            launchAngle += 2.0f;

        if (IsKeyDown(KEY_LEFT))
            launchAngle -= 2.0f;

        if (launchAngle > 180.0f)
            launchAngle = -180.0f;

        if (launchAngle < -180.0f)
            launchAngle = 180.0f;

        if (IsKeyDown(KEY_SPACE) && !launched)
        {
            launchForce += 2.0f;

            if (launchForce > 100.0f)
                launchForce = 100.0f;
        }

        if (IsKeyReleased(KEY_SPACE) && !launched)
        {
            float radians = launchAngle * DEG2RAD;

            projectile->GetBody()->SetAwake(true);

            projectile->GetBody()->SetLinearVelocity(
                b2Vec2(0.0f, 0.0f)
            );

            projectile->GetBody()->SetAngularVelocity(0.0f);

            b2Vec2 velocity(
                cos(radians) * launchForce * 2.5f,
                sin(radians) * launchForce * 2.5f
            );

            projectile->GetBody()->SetLinearVelocity(
                velocity
            );

            launched = true;
        }

        if (IsKeyPressed(KEY_R))
        {
            ResetProjectile();
        }
    }

    // Simulación física
    world.Step(
        1.0f / 60.0f,
        8,
        3
    );

    // Evita que el proyectil se salga de la pantalla (con rebote)
    if (projectile != nullptr)
    {
        b2Vec2 pos = projectile->GetBody()->GetPosition();
        b2Vec2 vel = projectile->GetBody()->GetLinearVelocity();

        float radius = projectile->GetRadius();

        if (pos.x - radius < 0)
        {
            pos.x = radius;
            vel.x *= -0.7f;
        }

        if (pos.x + radius > screenWidth)
        {
            pos.x = screenWidth - radius;
            vel.x *= -0.7f;
        }

        if (pos.y - radius < 0)
        {
            pos.y = radius;
            vel.y *= -0.7f;
        }

        projectile->GetBody()->SetTransform(
            pos,
            projectile->GetBody()->GetAngle()
        );

        projectile->GetBody()->SetLinearVelocity(vel);
    }
}

void Game::Draw()
{
    BeginDrawing();
    ClearBackground(fondo);

    float groundTop =
        (screenHeight - 40.0f) - 20.0f;

    // Suelo visual
    DrawRectangle(
        0,
        (int)groundTop,
        screenWidth,
        40,
        sueloColor
    );

    // Dibujar cajas
    for (const auto& box : boxes)
    {
        box.Draw();
    }

    // Dibujar círculos iniciales
    for (const auto& circle : circles)
    {
        circle.Draw();
    }

    // Dibujar proyectil
    if (projectile != nullptr)
    {
        projectile->Draw();

        if (catapultMode && !launched)
        {
            b2Vec2 pos =
                projectile->GetBody()->GetPosition();

            float radians =
                launchAngle * DEG2RAD;

            Vector2 endPos =
            {
                pos.x + cos(radians) * launchForce,
                pos.y + sin(radians) * launchForce
            };

            DrawLineEx(
                { pos.x, pos.y },
                endPos,
                4.0f,
                RAYWHITE
            );
        }
    }

    // Preview de caja
    if (!catapultMode)
    {
        Rectangle previewRect =
        {
            screenWidth / 2.0f,
            60.0f,
            50.0f,
            50.0f
        };

        Vector2 origin =
        {
            25.0f,
            25.0f
        };

        DrawRectanglePro(
            previewRect,
            origin,
            spawnAngle * RAD2DEG,
            Fade(RED, 0.3f)
        );
    }

    // Panel superior
    DrawRectangle(
        90,
        70,
        820,
        90,
        Fade(BLACK, 0.18f)
    );

    DrawText(
        "Bienvenidos a Modelos y Algoritmos para Videojuegos II",
        120,
        90,
        28,
        textoPrincipal
    );

    DrawText(
        "Raylib dibuja. Box2D simula.",
        320,
        125,
        22,
        textoSecundario
    );

    if (catapultMode)
    {
        DrawText(
            "MODO CATAPULTA",
            360,
            170,
            30,
            YELLOW
        );
    }
    else
    {
        DrawText(
            "MODO CAJAS",
            390,
            170,
            30,
            SKYBLUE
        );
    }

    DrawText(
        TextFormat("Angulo: %.0f", launchAngle),
        30,
        30,
        25,
        WHITE
    );

    DrawText(
        TextFormat("Fuerza: %.0f", launchForce),
        30,
        60,
        25,
        WHITE
    );

    DrawText(
        "TAB cambia modo",
        380,
        470,
        20,
        RAYWHITE
    );

    DrawText(
        "Modo cajas: Flechas + ESPACIO",
        310,
        500,
        20,
        RAYWHITE
    );

    DrawText(
        "Modo catapulta: Flechas + Mantener/Soltar ESPACIO",
        170,
        530,
        20,
        RAYWHITE
    );

    DrawText(
        "R reinicia proyectil",
        360,
        560,
        20,
        RAYWHITE
    );

    EndDrawing();
}

void Game::Run()
{
    Init();

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    CloseWindow();
}