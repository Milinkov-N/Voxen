#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "systems/Window.h"
#include "systems/Events.h"
#include "systems/Camera.h"

#include "graphics/VoxelRenderer.h" 
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Mesh.h"
#include "voxels/voxel.h"
#include "voxels/Chunk.h"

#include "loaders/png_loading.h"

typedef unsigned int uint;

const int WIDTH = 720;
const int HEIGHT = 480;

float vertices[] = {
    // x     y     z     u     v
    -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

     1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int attrs[] = { 3, 2, 0 /* <- null terminator */ };

int main() {
    /* Initialize the window with GLFW and GLEW */
    Window::initialize(WIDTH, HEIGHT, "Hello, Voxen");

    /* Initialize the events system */
    Events::initialize();

    Shader* shader = load_shader("res/main.glslv", "res/main.glslf");
    if (shader == nullptr) {
        std::cerr << "Failed to load shader" << std::endl;
        Window::terminate();
        return 1;
    }

    Texture* texture = load_texture("res/block.png");
    if (texture == nullptr) {
        std::cerr << "Failed to load texture" << std::endl;
        delete shader;
        Window::terminate();
        return 1;
    }

    VoxelRenderer vr(1024 * 1024 * 8);
    Chunk* chunk = new Chunk();
    Mesh* mesh   = vr.render(chunk);

    glClearColor(0.6f, 0.62f, 0.65f, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera* camera = new Camera(glm::vec3(0, 0, 20), glm::radians(90.0f));

    glm::mat4 model(1.0f);
    //model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
    //matrix = glm::translate(matrix, glm::vec3(0, 0.25f, 0));
    //matrix = glm::rotate(
    //    matrix,
    //    glm::radians(25.0f),
    //    glm::vec3(1, 0, 1)
    //);

    float lastTime = (float)glfwGetTime();
    float delta = 0.0f;

    float camX = 0.0f;
    float camY = 0.0f;

    float speed = 3;

    /* Loop until the user closes the window */
    while (!Window::shouldClose()) {
        float currentTime = (float)glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;

        /* Pressing escape key closes the window  */
        if (Events::jpressed(GLFW_KEY_ESCAPE)) {
            Window::setShouldClose(true);
        }
        if (Events::jpressed(GLFW_KEY_TAB)) {
            Events::toggleCursor();
        }

        /* On left mouse click changes the backgorund color to bright voilet */
        if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
            glClearColor(0.5f, 0.25f, 0.75f, 0);
        }

        if (Events::pressed(GLFW_KEY_W)) {
            camera->position += camera->front * delta * speed;
        }
        if (Events::pressed(GLFW_KEY_S)) {
            camera->position -= camera->front * delta * speed;
        }
        if (Events::pressed(GLFW_KEY_D)) {
            camera->position += camera->right * delta * speed;
        }
        if (Events::pressed(GLFW_KEY_A)) {
            camera->position -= camera->right * delta * speed;
        }

        if (Events::_cursor_locked) {
            camX += -(float)Events::deltaX / Window::width * 2;
            camY += -(float)Events::deltaY / Window::height * 2;

            if (camY < -glm::radians(89.0f)) {
                camY = -glm::radians(89.0f);
            }
            if (camY > glm::radians(89.0f)) {
                camY = glm::radians(89.0f);
            }

            camera->rotation = glm::mat4(1.0f);
            camera->rotate(camY, camX, 0);
        }

        /* ===== Render here ===== */

        glClear(GL_COLOR_BUFFER_BIT);

        // Draw VAO
        shader->use();
        shader->uniformMatrix("model", model);
        shader->uniformMatrix(
            "projview",
            camera->getProjection() * camera->getView()
        );
        texture->bind();
        mesh->draw(GL_TRIANGLES);

        /* ===== End of Render ===== */

        /* Swap front and back buffers */
        Window::swapBuffers();

        /* Poll for and process events */
        Events::pullEvents();
    }

    delete shader;
    delete texture;
    delete mesh;
    delete chunk;

    Window::terminate();
    return 0;
}