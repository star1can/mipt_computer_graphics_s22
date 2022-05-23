#pragma once

#include <vector>

#include "Window.hpp"
#include "Camera.hpp"
#include "Model.hpp"
// #include "Enemy.hpp"
#include "Projectile.hpp"
#include "GameObject.hpp"
#include "Fireball.hpp"


class GameEngine
{
    const float pi = 3.14159265359;

    std::vector<GameObject> floor_cells;
    std::vector<GameObject> active_enemies;
    std::vector<Fireball> active_projectiles;
    std::vector<GameObject> sky_;

    Model enemy_model_;
    std::vector<Model> projectile_models;
    Model floor_cell_model;
    Model sky_model_;

    int enemies_killed = 0;
    int enemies_count;
    int floor_size;
    float spawn_radius;
    float collide_dist;
    GLuint vertex_array_id;
    float shoot_delay_ = 0.2f;

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

    void DrawFireballs(std::vector<Fireball> &fireballs);

    std::string GetGraphicsPreset();

public:
    GameEngine(const Window &window_,
               const Camera &camera_,
               const Model &enemy_model,
               const Model &floor_cell_model_,
               const Model &sky_model,
               int floor_size_ = 10,
               int enemies_count_ = 20,
               float spawn_radius_ = 20.0,
               float collide_dist = 0.5f);

    void Update();
    Camera& GetCamera();
    void SetShootDelay(float new_delay);
    float GetShootDelay();
};