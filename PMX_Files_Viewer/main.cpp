#include "common.h"
#include "camera.h"
#include "model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_press_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// camera
Config camConfig("resource/configs/camera.yml");
Config modelConfig("resource/configs/model.yml");
Camera camera(camConfig);

float lastX = 0, lastY = 0;
bool firstMouse = true;
bool mousePress = false;

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MoYan", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_press_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    GLCall(glEnable(GL_DEPTH_TEST));


    {
        // model
        // -----
        std::shared_ptr<Shader> modelShader = std::make_shared<Shader>("resource/shaders/model.vs.shader", "resource/shaders/model.fs.shader");

        cv::String modelPath;
        modelConfig.get("modelPath", modelPath);
        Model ourModel(std::string(modelPath), modelShader);

        unsigned int matricesUniBuffer;
        GLCall(glGenBuffers(1, &matricesUniBuffer));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 3, NULL, GL_DYNAMIC_DRAW));
        GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUniBuffer, 0, sizeof(float) * 16 * 3));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        GLCall(glUniformBlockBinding(modelShader->ID, glGetUniformBlockIndex(modelShader->ID, "Matrices"), 0));

        while (!glfwWindowShouldClose(window)) {
            // input
            // -----
            processInput(window);
            GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            // render
            // ------
            glm::mat4 projMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
            glm::mat4 viewMatrix = camera.GetViewMatrix();
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0., -10., -20.));

            GLCall(glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer));
            GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float) * 16, glm::value_ptr(projMatrix)));
            GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16, sizeof(float) * 16, glm::value_ptr(viewMatrix)));
            GLCall(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float) * 16 * 2, sizeof(float) * 16, glm::value_ptr(modelMatrix)));

            modelShader->setVec3("lightColor", glm::vec3(1.));
            modelShader->setVec3("lightPos", glm::vec3(50. * cos(glfwGetTime()), 50., 50. * sin(glfwGetTime())));
            modelShader->setVec3("viewPos", camera.Position);

            ourModel.Draw();

            // swap buffers and poll IO events 
            // -------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT);
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        camera.ProcessKeyboard(UP);
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, static_cast<int>(1.0 * width * SCREEN_HEIGHT / SCREEN_WIDTH));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (mousePress) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_press_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mousePress = true;
    } else if (action == GLFW_RELEASE) {
        mousePress = false;
        firstMouse = true;
    }
}