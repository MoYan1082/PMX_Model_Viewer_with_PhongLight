#ifndef COMMON_H
#define COMMON_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <memory>
#include <assert.h>
#include <windows.h>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720
#define INF             100000000

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

#endif // COMMON_H