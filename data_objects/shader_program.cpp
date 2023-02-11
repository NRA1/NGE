#include "shader_program.hpp"

ShaderProgram::ShaderProgram(const char *vertex_path, const char *fragment_path)
{
    std::optional<std::string> result = ResourceLoader::loadText(vertex_path);
    std::string vertex_shader_source;
    bool fallback = !result.has_value();
    if (fallback)
    {
        log() - Critical < "Failed to load vertex shader from file" < vertex_path;
        log() - Critical < "Failing back to vertex shader source fallback";
    }
    else vertex_shader_source = result.value();

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    do
    {
        if (fallback) vertex_shader_source = common_fallbacks::vertex_shader_source_fallback;

        auto cstr_shader_source = vertex_shader_source.c_str();
        glShaderSource(vertex_shader, 1, &cstr_shader_source, nullptr);
        glCompileShader(vertex_shader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
            if (fallback)
            {
                log() - Fatal < "Vertex shader fallback compilation failed. Reason:" < infoLog;
                log() - Fatal < "There are no fallback fallbacks. Faulting";
                Game::fault();
            }
            else
            {
                log() - Critical < "Vertex shader compilation failed. Reason:" < infoLog;
                log() - Critical < "Failing back to vertex shader source fallback and retrying";
                fallback = true;
            }
        }
        else
        {
            if(fallback) log() - Info < "Vertex shader fallback compilation successful.";
            else log() - Info < "Vertex shader compilation successful";
            fallback = false;
        }
    } while (fallback);

    result = ResourceLoader::loadText(fragment_path);
    std::string fragment_shader_source;
    fallback = !result.has_value();
    if (fallback)
    {
        log() - Critical < "Failed to load fragment shader from file" < fragment_path;
        log() - Critical < "Failing back to fragment shader source fallback";
    }
    else fragment_shader_source = result.value();

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    do
    {
        if (fallback) fragment_shader_source = common_fallbacks::fragment_shader_source_fallback;

        auto cstr_shader_source = fragment_shader_source.c_str();
        glShaderSource(fragment_shader, 1, &cstr_shader_source, nullptr);
        glCompileShader(fragment_shader);

        int success;
        char infoLog[512];
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
            if (fallback)
            {
                log() - Fatal < "Fragment shader fallback compilation failed. Reason:" < infoLog;
                log() - Fatal < "There are no fallback fallbacks. Faulting";
                Game::fault();
            }
            else
            {
                log() - Critical < "Fragment shader compilation failed. Reason:" < infoLog;
                log() - Critical < "Failing back to vertex shader source fallback and retrying";
                fallback = true;
            }
        }
        else
        {
            if(fallback) log() - Info < "Fragment shader fallback compilation successful";
            else log() - Info < "Fragment shader compilation successful";
            fallback = false;
        }
    } while (fallback);

    id_ = glCreateProgram();
    glAttachShader(id_, vertex_shader);
    glAttachShader(id_, fragment_shader);
    glLinkProgram(id_);

    int success;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    char infoLog[512];
    if (!success)
    {
        glGetProgramInfoLog(id_, 512, nullptr, infoLog);
        log() - Fatal < "Failed to link shader program. Reason" < infoLog;
        log() - Fatal < "No way to recover. Faulting";
        Game::fault();
    }
    else log() - Info < "Shader program linking successful";

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void ShaderProgram::use() const
{
    glUseProgram(id_);
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id_);
}

void ShaderProgram::setMat4(const std::string &name, Mat4 &value) const
{
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &value[0][0]);
}


