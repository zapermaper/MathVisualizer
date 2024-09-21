#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std; 

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
int main()
{
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win= glfwCreateWindow(800, 600, "Test window", NULL, NULL);
    if (win == NULL)
    {
        cout << "Couldnt open a GLFW Window" << endl;
        return -1;
    }
    glfwMakeContextCurrent(win);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to start GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    
    const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

float vertices[] = {
-0.5f, -0.5f, 0.0f,
0.5f, -0.5f, 0.0f,
0.0f, 0.5f, 0.0f
};

unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
glCompileShader(vertexShaderId);

int shaderWorks;
char infoLog[512];
glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &shaderWorks);

if (!shaderWorks){
    glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
    cout<<"Vector shader failed to compile"<<infoLog<<endl;
}

unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
glCompileShader(fragmentShaderId);
glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &shaderWorks);

if (!shaderWorks){
    glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
    cout<<"Fragment Shader failed to compile"<<infoLog<<endl;
}

unsigned int shaderProgramId = glCreateProgram();
glAttachShader(shaderProgramId, vertexShaderId);
glAttachShader(shaderProgramId, fragmentShaderId);
glLinkProgram(shaderProgramId);

glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &shaderWorks);
if(!shaderWorks) {
    glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
    cout<<"Shader program failed to link shaders"<<infoLog<<endl;
}

glDeleteShader(vertexShaderId);
glDeleteShader(fragmentShaderId);

unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glBindVertexArray(0);

while(!glfwWindowShouldClose(win))
{
    processInput(win);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(shaderProgramId);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glfwSwapBuffers(win);
    glfwPollEvents();    
}

glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgramId);

glfwTerminate();
return 0;
}