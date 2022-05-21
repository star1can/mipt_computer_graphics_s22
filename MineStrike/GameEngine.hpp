#pragma once

#include <vector>

#include "Window.hpp"
#include "Camera.hpp"
#include "Model.hpp"
// #include "Enemy.hpp"
#include "Projectile.hpp"
#include "GameObject.hpp"

class GameEngine
{
    const float pi = 3.14159265359;

    std::vector<GameObject> floor_cells;
    std::vector<GameObject> active_enemies;
    std::vector<GameObject> active_projectiles;
    std::vector<GameObject> sky_;

    Model enemie_model;
    Model projectile_model;
    Model floor_cell_model;
    Model sky_model_;

    int enemies_killed = 0;
    int enemies_count;
    int floor_size;
    float spawn_radius;
    float collide_dist;
    float speed_ = 2048.0f;
    GLuint vertex_array_id;

    double last_shoot_time;
    double last_update_time;

    Window window;
    Camera camera;

    glm::vec3 GetRandVec(int min_length, int max_length);

    glm::vec3 GetRandHorizVec(int min, int max);


    // draw models with same VAO
    void DrawArray(std::vector<GameObject> &objects_array);

    void Shoot();

    void UpdateProjectiles();

    void UpdateCollisions();

public:
    GameEngine(const Window &window_,
               const Camera &camera_,
               const Model &enemie_model_,
               const Model &projectile_model_,
               const Model &floor_cell_model_,
               const Model &sky_model,
               int floor_size_ = 10,
               int enemies_count_ = 20,
               float spawn_radius_ = 20.0,
               float collide_dist = 0.5f);

    void Update();
};