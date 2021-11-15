#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct ShaderProgramSource {
    std::string vertex_source{ "" };
    std::string fragment_source{ "" };
};

static ShaderProgramSource parseShader(const std::string& filename) {
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::ifstream fstream(filename);
    std::stringstream sstream[2];
    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(fstream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            sstream[(int)type] << line << '\n';
        }
    }

    return { sstream[0].str(), sstream[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout 
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ")
            << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string& vertex_shader, const std::string& fragment_shader) {
    // creating program
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    // merging two files (strings) into one
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    // linking program from file (string)
    glLinkProgram(program);
    glValidateProgram(program);

    // cleaning
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW has occured." << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    ShaderProgramSource source = parseShader("../data/shaders.txt");
    std::cout << "VERTEX" << std::endl;
    std::cout << source.vertex_source << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.fragment_source << std::endl;
    unsigned int shader = createShader(source.vertex_source, source.fragment_source);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}