#include "../include/shader.h"

Shader::Shader(const char *v_path, const char *f_path) {
    // 1. retrieve vertex/fragment source code from file path
    string v_code;
    string f_code;
    ifstream vs_file;
    ifstream fs_file;

    // Set exception rules to ifstream objects
    vs_file.exceptions(ifstream::failbit | ifstream::badbit);
    fs_file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        // open files
        vs_file.open("placeholder");
        fs_file.open("placeholder");

        // create and read buffer contents into streams
        stringstream vs_stream, fs_stream;
        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        // close file handlers
        vs_file.close();
        fs_file.close();

        // convert stream into string
        v_code = vs_stream.str();
        f_code = fs_stream.str();
    } catch (ifstream::failure &e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
    }
    const char *vs_code = v_code.c_str();
    const char *fs_code = f_code.c_str();

    // 2. compile shaders
    unsigned int v, f;

    // vertex shader
    v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs_code, nullptr);
    glCompileShader(v);
    check_compile_errors(v, "VERTEX");

    // fragment shader
    f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs_code, nullptr);
    glCompileShader(f);
    check_compile_errors(v, "FRAGMENT");

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, v);
    glAttachShader(ID, f);
    glLinkProgram(ID);
    check_compile_errors(v, "PROGRAM");

    // delete shaders
    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::set_bool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::set_int(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set_float(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


