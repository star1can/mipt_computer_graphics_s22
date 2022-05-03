// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/text2D.hpp>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace constants {
    const size_t MAX_DISTANCE = 40;
    const size_t START_ENEMIES = 12;
    const size_t MAX_ENEMIES = 25;
} // constants


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getting it from obj files
std::vector<vec3> default_enemy_vertices;
std::vector<vec3> default_enemy_normals;
std::vector<vec2> default_enemy_uvs;
std::vector<vec3> default_fireball_vertices;
std::vector<vec3> default_fireball_normals;
std::vector<vec2> default_fireball_uvs;


// used due game process
std::vector<vec3> enemy_vertices;
std::vector<vec3> enemy_normals;
std::vector<vec3> fireball_vertices;
std::vector<vec3> fireball_normals;
std::vector<vec3> all_vertices;
std::vector<vec3> all_normals;
std::vector<vec3> floor_vertices;
std::vector<vec3> floor_normals;
std::vector<vec2> enemy_uvs;
std::vector<vec2> fireball_uvs;
std::vector<vec2> all_uvs;
std::vector<vec2> floor_uvs;

int64_t fireballs_shot = 0;
int64_t enemies_hit = 0;
int64_t enemies_count = 0;
int64_t cur_det = 1;

// TODO make it constant from C++ library and move it to constants::
const double cpi = 3.14159265359;

vec3 looking_at(1.0f, 0.0f, 1.0f);
vec3 default_lookat(0.0f, 0.0f, -1.0f);
vec3 default_fireball_center(0.0f, 0.0f, 0.0f);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace support {
    float GetAngle(vec3 v1, vec3 v2) {
        v1[1] = 0;
        v2[1] = 0;
        float angle = acos(dot(normalize(v1), normalize(v2)));
        if (v1[0] * v2[2] - v1[2] * v2[0] < 0) {
            angle *= -1;
        }
        return angle;
    }

    vec3 GetRandVec(int min, int max) {
        double phi = 2 * cpi * rand() / RAND_MAX;
        double costheta = (double) rand() / RAND_MAX;
        double theta = acos(costheta);
        int r = (rand() % (max - min) + min);
        double x = r * sin(theta) * cos(phi);
        double y = r * sin(theta) * sin(phi);
        double z = r * cos(theta);
        return vec3(x, y, z);
    }

    vec3 GetRandHorizVec(int min, int max) {
        double phi = 2 * cpi * rand() / RAND_MAX;
        int r = (rand() % (max - min) + min);
        double x = r * sin(phi);
        double y = 0;
        double z = r * cos(phi);
        return vec3(x, y, z);
    }
} // support

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Zombie {
private:
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;
    vec3 center_;
    float64_t coll_dist_ = 0.8f;

    friend class GameManager;

private:
    void set_position() {

        glm::vec3 offset = support::GetRandHorizVec(10, 25);
        offset[1] = -1.3f;

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, float(glm::radians((float) (rand() % 360))), glm::vec3(0.0f, 1.0f, 0.0f));

        center_ += offset;
        for (auto &vertex: vertices_) {
            vertex = vec3(trans * vec4(vertex, 1));
            vertex += offset;
        }
    }

public:
    Zombie(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals,
           const std::vector<glm::vec2> &uvs) : vertices_(vertices), normals_(normals), uvs_(uvs) {
        set_position();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Fireball {
private:
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;
    vec3 center_;
    vec3 direction_;
    float64_t coll_dist_ = 0.1f;

    friend class GameManager;

private:
    void set_position() {
        vec3 curr_position = get_position();

        glm::vec3 offset = normalize(looking_at) * 2.0f;
        offset[1] = -1.f;
        offset[2] *= -1.2f;
        offset += curr_position;

        float angle = support::GetAngle(looking_at, default_lookat);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, -angle, glm::vec3(0.0f, 1.0f, 0.0f));

        center_ += offset;
        for (auto &vertex: vertices_) {
            vertex = vec3(trans * vec4(vertex, 1));
            vertex += offset;
        }

        direction_ = center_ - curr_position;
        direction_[1] = 0;
        direction_ *= 0.3f;
    }

public:
    Fireball(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals,
             const std::vector<glm::vec2> &uvs) : vertices_(vertices), normals_(normals), uvs_(uvs) {
        set_position();
    }

    void Move() {
        center_ += direction_;
        for (auto &vertex: vertices_) {
            vertex += direction_;
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<Fireball> fireballs;
std::vector<Zombie> enemies;

// TODO make it better, maybe namespace
class GameManager {
private:
    static void remove_most_dist_fireballs() {
        for (int k = 0; k < fireballs.size(); ++k) {
            if (length(fireballs[k].center_) > constants::MAX_DISTANCE) {
                DeleteFireball(k);
                --k;
            }
        }
    }

public:

    static void Init() {
        for (int i = 0; i < constants::START_ENEMIES; ++i) {
            AddEnemy();
        }
        ConcatAll();
        CreateFloor();
    }

    static void Shoot() {
        fireballs.emplace_back(default_fireball_vertices, default_fireball_normals, default_fireball_uvs);
        ++fireballs_shot;
    }

    static void AddEnemy() {
        enemies.emplace_back(default_enemy_vertices, default_enemy_normals, default_enemy_uvs);
        ++enemies_count;
    }

    static void DeleteEnemy(int num) {
        enemies.erase(enemies.begin() + num);
        --enemies_count;
    }

    static void DeleteFireball(int num) {
        fireballs.erase(fireballs.begin() + num);
    }

    static void RemoveCollisions() {
        for (int f = 0; f < fireballs.size(); ++f) {
            for (int e = 0; e < enemies.size(); ++e) {
                if (distance(fireballs[f].center_, enemies[e].center_) <
                    fireballs[f].coll_dist_ + enemies[e].coll_dist_) {
                    ++enemies_hit;
                    DeleteEnemy(e);
                    DeleteFireball(f);
                    --f;
                    break;
                }
            }
        }
    }

    static void MoveFireballs() {
        remove_most_dist_fireballs();

        for (auto &fireball: fireballs) {
            fireball.Move();
        }
    }

    static void ConcatEnemies() {
        enemy_vertices.clear();
        enemy_uvs.clear();
        enemy_normals.clear();

        for (auto &enemy: enemies) {
            enemy_vertices.insert(enemy_vertices.end(), enemy.vertices_.begin(), enemy.vertices_.end());
            enemy_uvs.insert(enemy_uvs.end(), enemy.uvs_.begin(), enemy.uvs_.end());
            enemy_normals.insert(enemy_normals.end(), enemy.normals_.begin(), enemy.normals_.end());
        }
    }

    static void ConcatFireballs() {
        fireball_vertices.clear();
        fireball_uvs.clear();
        fireball_normals.clear();

        for (auto &fireball: fireballs) {
            fireball_vertices.insert(fireball_vertices.end(), fireball.vertices_.begin(), fireball.vertices_.end());
            fireball_uvs.insert(fireball_uvs.end(), fireball.uvs_.begin(), fireball.uvs_.end());
            fireball_normals.insert(fireball_normals.end(), fireball.normals_.begin(), fireball.normals_.end());
        }
    }

    static void ConcatAll() {
        all_vertices.clear();
        all_uvs.clear();
        all_normals.clear();

        for (auto &enemy: enemies) {
            all_vertices.insert(all_vertices.end(), enemy.vertices_.begin(), enemy.vertices_.end());
            all_uvs.insert(all_uvs.end(), enemy.uvs_.begin(), enemy.uvs_.end());
            all_normals.insert(all_normals.end(), enemy.normals_.begin(), enemy.normals_.end());
        }

        for (auto &fireball: fireballs) {
            all_vertices.insert(all_vertices.end(), fireball.vertices_.begin(), fireball.vertices_.end());
            all_uvs.insert(all_uvs.end(), fireball.uvs_.begin(), fireball.uvs_.end());
            all_normals.insert(all_normals.end(), fireball.normals_.begin(), fireball.normals_.end());
        }

        all_vertices.insert(all_vertices.end(), floor_vertices.begin(), floor_vertices.end());
    }

    static void CreateFloor() {
        floor_vertices.clear();
        floor_uvs.clear();
        floor_normals.clear();
        std::vector<vec3> default_tile_vertices = {
                // 1
                {1.0f,  -2.0f, 1.0f},
                {1.0f,  -2.0f, -1.0f},
                {-1.0f, -2.0f, 1.0f},
                // 2
                {1.0f,  -2.0f, -1.0f},
                {-1.0f, -2.0f, -1.0f},
                {-1.0f, -2.0f, 1.0f},
        };
        std::vector<vec3> tile_vertices(6);
        std::vector<vec3> default_tile_normals = {
                {0.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                // 2
                {0.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
        };

        std::vector<glm::vec2> default_tile_uvs(default_enemy_uvs.begin(), default_enemy_uvs.begin() + 6);

        for (int i = -25; i <= 25; ++i) {
            for (int j = -25; j <= 25; ++j) {
                for (int k = 0; k < 6; ++k) {
                    tile_vertices[k] = default_tile_vertices[k];
                    tile_vertices[k][0] += 2 * i;
                    tile_vertices[k][2] += 2 * j;
                }
                floor_vertices.insert(floor_vertices.end(), tile_vertices.begin(), tile_vertices.end());
                floor_uvs.insert(floor_uvs.end(), default_tile_uvs.begin(), default_tile_uvs.end());
                floor_normals.insert(floor_normals.end(), default_tile_normals.begin(), default_tile_normals.end());
            }
        }
    }
};

void MouseCallback(GLFWwindow *window, int button, int action, int mod) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        GameManager::Shoot();
    }
};

void PopulateBuffers(const std::vector<vec3> &vertices,
                     const std::vector<vec2> &uvs,
                     const std::vector<vec3> &normals) {

    if (!vertices.empty()) {
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

        GLuint uvbuffer;
        glGenBuffers(1, &uvbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

        GLuint normalbuffer;
        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );

        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,                                // attribute
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );

        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
                2,                                // attribute
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );
    }
}


int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Mine Strike", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.2f, 0.2f, 0.7f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

//    // Cull triangles which normal is not towards the camera
//    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("assets/shaders/StandardShading.vertexshader",
                                   "assets/shaders/StandardShading.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

    // Load the texture
    GLuint fireball_texture = loadDDS("assets/textures/fireball.DDS");
    GLuint zombie_texture = loadDDS("assets/textures/zombie.DDS");
    GLuint grass_texture = loadDDS("assets/textures/sand.DDS");

    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

    // Read our .obj file
    assert(loadOBJ("assets/models/zombie.obj", default_enemy_vertices, default_enemy_uvs, default_enemy_normals));
    assert(loadOBJ("assets/models/low_poly_fireball.obj", default_fireball_vertices, default_fireball_uvs,
                   default_fireball_normals));

    GameManager::Init();

    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // Initialize our little text library with the Holstein font
    initText2D("assets/textures/Holstein.DDS");

    // For speed computation
    double fireball_last_time = glfwGetTime();
    double enemy_last_time = glfwGetTime();

    glfwSetMouseButtonCallback(window, MouseCallback);

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // get the 3rd column -- the forward direction
        // used for directing Fireballs
        looking_at = vec3(ViewMatrix[2]);

        // shoot Fireball and spawn enemy each second
        double cur_time = glfwGetTime();
        if (cur_time - enemy_last_time > 1) {
            enemy_last_time = cur_time;
            if (enemies_count < constants::MAX_ENEMIES) {
                GameManager::AddEnemy();
            }
        }
        if (cur_time - fireball_last_time > 0.2f) {
            fireball_last_time = cur_time;
            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (state == GLFW_PRESS) {
                GameManager::Shoot();
            }
        }
        // move_Fireballs
        GameManager::MoveFireballs();
        GameManager::RemoveCollisions();

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glm::vec3 lightPos = glm::vec3(0, 35, 0);
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fireball_texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        GameManager::ConcatEnemies();
        GameManager::ConcatFireballs();
        GameManager::ConcatAll();

        glBindTexture(GL_TEXTURE_2D, zombie_texture);
        PopulateBuffers(enemy_vertices, enemy_uvs, enemy_normals);
        glDrawArrays(GL_TRIANGLES, 0, enemy_vertices.size());

        glBindTexture(GL_TEXTURE_2D, fireball_texture);
        PopulateBuffers(fireball_vertices, fireball_uvs, fireball_normals);
        glDrawArrays(GL_TRIANGLES, 0, fireball_vertices.size());

        glBindTexture(GL_TEXTURE_2D, grass_texture);
        PopulateBuffers(floor_vertices, floor_uvs, floor_normals);
        glDrawArrays(GL_TRIANGLES, 0, floor_vertices.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        char text[256];
        sprintf(text, "%.2f sec", glfwGetTime());
        std::string count(std::to_string(enemies_hit));
        printText2D(count.data(), 10, 500, 100);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteTextures(1, &fireball_texture);
    glDeleteTextures(1, &zombie_texture);
    glDeleteTextures(1, &grass_texture);
    glDeleteVertexArrays(1, &VertexArrayID);
    // Delete the text's VBO, the shader and the texture
    cleanupText2D();
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}