#include <algorithm>
#include <string>

#include "GameEngine.hpp"

#include <common/text2D.hpp>
#include <external/stb/stb_image.h>

GameEngine::GameEngine(const Window &window_,
                       const Camera &camera_,
                       const Model &enemie_model_,
                       const Model &projectile_model_,
                       const Model &floor_cell_model_,
                       const Model &sky_model,
                       int floor_size_,
                       int enemies_count_,
                       float spawn_radius_,
                       float collide_dist_) : window(window_),
                                              camera(camera_),
                                              enemie_model(enemie_model_),
                                              projectile_model(projectile_model_),
                                              floor_cell_model(floor_cell_model_),
                                              sky_model_(sky_model),
                                              floor_size(floor_size_),
                                              enemies_count(enemies_count_),
                                              spawn_radius(spawn_radius_),
                                              collide_dist(collide_dist_)
{
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    initText2D("assets/textures/holstein.DDS");

    // Init enemies
    for (size_t i = 0; i < enemies_count; i++)
    {
        glm::vec3 pos = GetRandHorizVec(0, spawn_radius);
        glm::vec3 dir = glm::normalize(camera.GetPos() - pos);
        active_enemies.emplace_back(enemie_model, pos, dir);
    }

    // Init floor
    for (int i = -floor_size; i < floor_size; i++)
    {
        for (int j = -floor_size; j < floor_size; j++)
        {
            glm::vec3 pos = glm::vec3(float(i), -1.0f, float(j));
            glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
            floor_cells.emplace_back(floor_cell_model, pos, dir, 0.5f);
        }
    }

    // Init sky
    glm::vec3 pos = glm::vec3(0.0, 2.0f, 0.f);
    glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
    sky_.emplace_back(sky_model_, pos, dir, 30.f);

    last_shoot_time = glfwGetTime();
    last_update_time = glfwGetTime();
}

void GameEngine::DrawArray(std::vector<GameObject> &objects_array)
{
    if (objects_array.empty())
    {
        return;
    }

    Model reference_model = objects_array[0].GetModel();

    glBindVertexArray(reference_model.VAO);

    glUseProgram(reference_model.shader_program);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reference_model.texture); //можно ли перенести это в
    glUniform1i(reference_model.glsl_texture, 0);
    for (GameObject &obj : objects_array)
    {
        glm::mat4 mvp_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix() * obj.GetModelMatrix();
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniformMatrix4fv(reference_model.glsl_mvp_matrix, 1, GL_FALSE,
                           &mvp_matrix[0][0]); // transfer mvp matrix to shader
        glDrawArrays(GL_TRIANGLES, 0, reference_model.triangles_count);
    }

    glBindVertexArray(vertex_array_id);
}



glm::vec3 GameEngine::GetRandVec(int min, int max)
{
    double phi = 2 * pi * rand() / RAND_MAX;
    double costheta = (double)rand() / RAND_MAX;
    double theta = acos(costheta);
    int r = (rand() % (max - min) + min);
    double x = r * sin(theta) * cos(phi);
    double y = r * sin(theta) * sin(phi);
    double z = r * cos(theta);
    return glm::vec3(x, y, z);
}

glm::vec3 GameEngine::GetRandHorizVec(int min, int max)
{
    double phi = 2 * pi * rand() / RAND_MAX;
    int r = (rand() % (max - min) + min);
    double x = r * sin(phi);
    double y = 0;
    double z = r * cos(phi);
    return glm::vec3(x, y, z);
}

void GameEngine::Shoot()
{
    active_projectiles.emplace_back(projectile_model, camera.GetPos() + 0.5f * camera.GetDir(), camera.GetDir(), 0.25f);
}

void GameEngine::UpdateProjectiles()
{
    // Delete far projectiles
    active_projectiles.erase(
        std::remove_if(
            active_projectiles.begin(),
            active_projectiles.end(),
            [](GameObject &projectile)
            { return glm::length(projectile.GetPos()) > 50.0f; }),
        active_projectiles.end());

    // UpdateProjectiles
    for (GameObject &projectile : active_projectiles)
    {
        float sensivity =
            (glfwGetTime() - last_update_time) * speed_;
        projectile.SetPos(projectile.GetPos() + sensivity * projectile.GetDir());
    }
}

void GameEngine::UpdateCollisions()
{
    // Update collisions
    for (int projectile_idx = 0; projectile_idx < active_projectiles.size();)
    {
        bool has_intersection = false;

        // Remove ONE enemie which intersects with projectile
        for (int enemy_idx = 0; enemy_idx < active_enemies.size(); ++enemy_idx)
        {
            if (glm::length(active_projectiles[projectile_idx].GetPos() - active_enemies[enemy_idx].GetPos()) <
                collide_dist)
            {
                active_enemies.erase(active_enemies.begin() + enemy_idx);
                ++enemies_killed;
                has_intersection = true;
                break;
            }
        }

        if (has_intersection)
        {
            active_projectiles.erase(active_projectiles.begin() + projectile_idx);
        }
        else
        {
            ++projectile_idx;
        }
    }
}

void GameEngine::Update()
{
    if (glfwGetKey(window.GetWindow(), GLFW_KEY_Q) == GLFW_PRESS)
    {
        speed_ *= 2;
    }
    else if (glfwGetKey(window.GetWindow(), GLFW_KEY_E) == GLFW_PRESS) {
        speed_ = std::fmax(1.f, speed_ /2);
    }

    // Check mouse for shoot
    if (glfwGetMouseButton(window.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (glfwGetTime() - last_shoot_time > 0.2f)
        {
            Shoot();
            last_shoot_time = glfwGetTime();
        }
    }

    camera.Update();

    UpdateProjectiles();
    UpdateCollisions();

    if ((int)glfwGetTime() % 1000 == 10) {
        for (size_t i = 0; i < 1; i++) {
            glm::vec3 pos = GetRandHorizVec(0, spawn_radius);
            glm::vec3 dir = glm::normalize(camera.GetPos() - pos);
            active_enemies.emplace_back(enemie_model, pos, dir);
        }
    }

    glBindVertexArray(1);
    DrawArray(floor_cells);
    DrawArray(sky_);
    DrawArray(active_enemies);
    DrawArray(active_projectiles);

    std::string prefix("Score:");
    std::string count(std::to_string(enemies_killed));
    printText2D((prefix + count).data(), 10, 500, 40);

    last_update_time = glfwGetTime();
}