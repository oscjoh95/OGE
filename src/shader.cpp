#include <shader.h>

// Constructor
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // Make sure that ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // 1. Read shader code from files
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Check for vertex compilation errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Check for fragment compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 3. Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // Check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the shaders, they won't be needed after they are linked
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

/**
 * Sets the shader as the active shader.
 * 
 * @returns void
 */
void Shader::use() {
    glUseProgram(ID);
}

/**
 * Sets a bool uniform in the shader.
 *
 * @param name The name of the uniform to set
 * @param value value to set the uniform to
 * 
 * @returns void
 */
void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
/**
 * Sets an int uniform in the shader.
 *
 * @param name The name of the uniform to set
 * @param value value to set the uniform to
 * 
 * @returns void
 */
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
/**
 * Sets a float uniform in the shader.
 *
 * @param name The name of the uniform to set
 * @param value value to set the uniform to
 * 
 * @returns void
 */
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
/**
 * Sets a mat4 uniform of floats in the shader.
 *
 * @param name The name of the uniform to set
 * @param value A mat4 of floats to set the uniform to
 * 
 * @returns void
 */
void Shader::setMat4(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
/**
 * Sets a vec3 uniform of floats in the shader.
 *
 * @param name The name of the uniform to set
 * @param value A vec3 of floats to set the uniform to
 * 
 * @returns void
 */
void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
/**
 * Sets a vec3 uniform of floats in the shader.
 *
 * @param name The name of the uniform to set
 * @param x The x value of the vector
 * @param y The y value of the vector 
 * @param z The z value of the vector 
 * 
 * @returns void
 */
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &glm::vec3(x, y, z)[0]);
}