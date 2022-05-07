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

    Model enemie_model;
    Model projectile_model;
    Model floor_cell_model;

    int enemies_count;
    int floor_size;
    float spawn_radius;
    float collide_dist;

    double last_shoot_time;
    double last_update_time;

    Window window;
    Camera camera;

    // draw models with same VAO
    void DrawArray(std::vector<GameObject> &objects_array);

    glm::vec3 GetRandVec(int min_length, int max_length);

    glm::vec3 GetRandHorizVec(int min, int max);

    void Shoot();

    void UpdateProjectiles();

public:
    GameEngine(const Window &window_,
               const Camera &camera_,
               const Model &enemie_model_,
               const Model &projectile_model_,
               const Model &floor_cell_model_,
               int floor_size_ = 10,
               int enemies_count_ = 20,
               float spawn_radius_ = 20.0,
               float collide_dist = 0.5f);
    
    void Update();    
};