#include <algorithm>
#include <string>
#include <iostream>

#include "GameEngine.hpp"

#include <common/text2D.hpp>

// VERY BAD!
const size_t MAX_DETALIZATION_LVL = 4;
float SPEED = 1024.0f;
size_t CURR_DETALIZATION_LVL = 4;
float PARTICLE_SPEED = 0.8f;

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *game_engine = static_cast<GameEngine *>(glfwGetWindowUserPointer(window));

    Camera &camera = game_engine->GetCamera();

    float curr_mov_speed = camera.GetMovementSpeed();
    float curr_mouse_speed = camera.GetMouseSpeed();

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        SPEED *= 1.25f;
        camera.SetMovementSpeed(curr_mov_speed * 1.25f);
        camera.SetMouseSpeed(curr_mouse_speed * 1.25f);
        PARTICLE_SPEED *= 1.15;
    } else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        SPEED = std::fmax(10.f, SPEED * 0.75f);
        camera.SetMovementSpeed(curr_mov_speed * 0.75f);
        camera.SetMouseSpeed(curr_mouse_speed * 0.75f);
        PARTICLE_SPEED = std::fmax(0.0005f, PARTICLE_SPEED * 0.75f);
    } else if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        CURR_DETALIZATION_LVL = CURR_DETALIZATION_LVL < 4? CURR_DETALIZATION_LVL + 1: MAX_DETALIZATION_LVL;
    } else if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        CURR_DETALIZATION_LVL = CURR_DETALIZATION_LVL != 0 ? CURR_DETALIZATION_LVL - 1 : 0;
    }
}

Camera &GameEngine::GetCamera() {
    return camera;
}

GameEngine::GameEngine(const Window &window_,
                       const Camera &camera_,
                       const Model &enemy_model,
                       const Model &floor_cell_model_,
                       const Model &sky_model,
                       int floor_size_,
                       int enemies_count_,
                       float spawn_radius_,
                       float collide_dist_) : window(window_),
                                              camera(camera_),
                                              enemy_model_(enemy_model),
                                              floor_cell_model(floor_cell_model_),
                                              sky_model_(sky_model),
                                              floor_size(floor_size_),
                                              enemies_count(enemies_count_),
                                              spawn_radius(spawn_radius_),
                                              collide_dist(collide_dist_) {
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    for (size_t i = 0; i <= MAX_DETALIZATION_LVL; ++i) {
        auto model_path = "assets/models/" + std::to_string(i) + ".obj";
        Model fireball(&model_path[0],
                       "assets/textures/fireball1.DDS",
                       "assets/shaders/TransformVertexShader.vertexshader",
                       "assets/shaders/TextureFragmentShader.fragmentshader");
        projectile_models.push_back(fireball);
    }


    initText2D("assets/textures/holstein.DDS");

    // Init enemies
    for (size_t i = 0; i < enemies_count; i++) {
        glm::vec3 pos = GetRandHorizVec(0, spawn_radius);
        glm::vec3 dir = glm::normalize(camera.GetPos() - pos);
        active_enemies.emplace_back(enemy_model_, pos, dir);
    }

    // Init floor
    for (int i = -floor_size; i < floor_size; i++) {
        for (int j = -floor_size; j < floor_size; j++) {
            glm::vec3 pos = glm::vec3(float(i), -1.0f, float(j));
            glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
            floor_cells.emplace_back(floor_cell_model, pos, dir, 0.5f);
        }
    }

    // Init sky
    glm::vec3 pos = glm::vec3(0.0, 2.0f, 0.f);
    glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
    sky_.emplace_back(sky_model_, pos, dir, 70.f);

    last_shoot_time = glfwGetTime();
    last_update_time = glfwGetTime();
}

void GameEngine::DrawArray(std::vector<GameObject> &objects_array) {
    if (objects_array.empty()) {
        return;
    }

    Model reference_model = objects_array[0].GetModel();

    glBindVertexArray(reference_model.VAO);

    glUseProgram(reference_model.shader_program);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, reference_model.texture); //можно ли перенести это в
    glUniform1i(reference_model.glsl_texture, 0);
    for (GameObject &obj: objects_array) {
        glm::mat4 mvp_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix() * obj.GetModelMatrix();
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniformMatrix4fv(reference_model.glsl_mvp_matrix, 1, GL_FALSE,
                           &mvp_matrix[0][0]); // transfer mvp matrix to shader
        glDrawArrays(GL_TRIANGLES, 0, reference_model.triangles_count);
    }

    glBindVertexArray(vertex_array_id);
}

void GameEngine::DrawFireballs(std::vector<Fireball> &fireballs_array) {
    for (Fireball &fireball: fireballs_array) {
        Model reference_model = fireball.GetModel();

        glBindVertexArray(reference_model.VAO);

        glUseProgram(reference_model.shader_program);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, reference_model.texture);
        glUniform1i(reference_model.glsl_texture, 0);
        glm::mat4 mvp_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix() * fireball.GetModelMatrix();
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniformMatrix4fv(reference_model.glsl_mvp_matrix, 1, GL_FALSE,
                           &mvp_matrix[0][0]); // transfer mvp matrix to shader
        glDrawArrays(GL_TRIANGLES, 0, reference_model.triangles_count);
    }

    glBindVertexArray(vertex_array_id);
}

glm::vec3 GameEngine::GetRandVec(int min, int max) {
    double phi = 2 * pi * rand() / RAND_MAX;
    double costheta = (double) rand() / RAND_MAX;
    double theta = acos(costheta);
    int r = (rand() % (max - min) + min);
    double x = r * sin(theta) * cos(phi);
    double y = r * sin(theta) * sin(phi);
    double z = r * cos(theta);
    return glm::vec3(x, y, z);
}

glm::vec3 GameEngine::GetRandHorizVec(int min, int max) {
    double phi = 2 * pi * rand() / RAND_MAX;
    int r = (rand() % (max - min) + min);
    double x = r * sin(phi);
    double y = 0;
    double z = r * cos(phi);
    return glm::vec3(x, y, z);
}

void GameEngine::Shoot() {
    active_projectiles.emplace_back(projectile_models[CURR_DETALIZATION_LVL], camera.GetPos() + 0.5f * camera.GetDir(),
                                    camera.GetDir(), 0.25f);

}

void GameEngine::UpdateProjectiles() {
    // Delete far projectiles
    active_projectiles.erase(
            std::remove_if(
                    active_projectiles.begin(),
                    active_projectiles.end(),
                    [](Fireball &projectile) {
                        if (!projectile.IsBlowed()) {
                            return glm::length(projectile.GetPos()) > 50.0f;
                        }
                        else {
                            return glm::length(projectile.GetModel().GetVertices()[0]) > 5.0f;
                        }
                    }
            ),
            active_projectiles.end());

    // UpdateProjectiles
    for (auto &projectile: active_projectiles) {
        if (projectile.IsBlowed()) {
            projectile.BlowingUp(PARTICLE_SPEED);
        } else {
            float sensivity =
                    (glfwGetTime() - last_update_time) * SPEED;
            projectile.SetPos(projectile.GetPos() + sensivity * projectile.GetDir());
        }
    }
}

void GameEngine::UpdateCollisions() {
    // Update collisions
    for (auto &active_projectile: active_projectiles) {
        for (int enemy_idx = 0; enemy_idx < active_enemies.size(); ++enemy_idx) {
            if ((glm::length(active_projectile.GetPos() - active_enemies[enemy_idx].GetPos()) <
                 collide_dist) && !active_projectile.IsBlowed()) {
                active_projectile.BlowUp();
                active_enemies.erase(active_enemies.begin() + enemy_idx);
                ++enemies_killed;
                break;
            }
        }
    }
}


void GameEngine::Update() {
    glfwSetWindowUserPointer(window.GetWindow(), this);
    glfwSetKeyCallback(window.GetWindow(), KeyCallback);

    camera.Update();

    UpdateProjectiles();
    UpdateCollisions();

    // Check mouse for shoot
    if (glfwGetMouseButton(window.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (glfwGetTime() - last_shoot_time > 0.2f) {
            Shoot();
            last_shoot_time = glfwGetTime();
        }
    }

    if ((int) glfwGetTime() % 1000 == 100) {
        for (size_t i = 0; i < 1; i++) {
            glm::vec3 pos = GetRandHorizVec(0, spawn_radius);
            glm::vec3 dir = glm::normalize(camera.GetPos() - pos);
            active_enemies.emplace_back(enemy_model_, pos, dir);
        }
    }

    glBindVertexArray(1);
    DrawArray(floor_cells);
    DrawArray(sky_);
    DrawArray(active_enemies);
    DrawFireballs(active_projectiles);

    std::string prefix("Score:");
    std::string count(std::to_string(enemies_killed));
    printText2D((prefix + count).data(), 10, 500, 40);

    last_update_time = glfwGetTime();
}