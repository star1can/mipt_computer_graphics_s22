#include <iostream>

#include "GameEngine.hpp"
#include "GameObject.hpp"
#include "Model.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>




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

    for (GameObject &obj : objects_array)
    {
        glm::mat4 mvp_matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix() * obj.GetModelMatrix();
        glUniform1i(reference_model.glsl_texture, 0);          // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniformMatrix4fv(reference_model.glsl_mvp_matrix, 1, GL_FALSE, &mvp_matrix[0][0]); // transfer mvp matrix to shader
        glDrawArrays(GL_TRIANGLES, 0, reference_model.triangles_count);
    }

    glBindVertexArray(0);
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

GameEngine::GameEngine(const Window &window_,
                       const Camera &camera_,
                       const Model &enemie_model_,
                       const Model &projectile_model_,
                       const Model &floor_cell_model_,
                       int floor_size_,
                       int enemies_count_,
                       float spawn_radius_) : window(window_),
                                              camera(camera_),
                                              enemie_model(enemie_model_),
                                              projectile_model(projectile_model_),
                                              floor_cell_model(floor_cell_model_),
                                              floor_size(floor_size_),
                                              enemies_count(enemies_count_),
                                              spawn_radius(spawn_radius_)
{

    // Init enemies
    for (size_t i = 0; i < enemies_count; i++)
    {
        glm::vec3 pos = GetRandHorizVec(0, spawn_radius);
        glm::vec3 dir = camera.GetPos() - pos;
        active_enemies.emplace_back(GameObject(enemie_model, pos, dir));
    }

    // Init floor
    for (int i = - floor_size; i < floor_size; i++)
    {
        for (int j = - floor_size; j < floor_size; j++)
        {
            glm::vec3 pos = glm::vec3(float(i), -1.0f, float(j));
            glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
            floor_cells.emplace_back(GameObject(floor_cell_model, pos, dir, 0.5f));
        }
    }
}


void GameEngine::UpdateProjectiles()
{
    // DrawArray(active_projectiles);
}

void GameEngine::Update(){
    camera.Update();

    DrawArray(floor_cells);
    DrawArray(active_enemies);
}