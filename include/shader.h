#ifndef SHADER_H
#define SHADER_H

#include "../src/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
    unsigned int ID;

    Shader(const char *v_path, const char *f_path);

    // activate shader
    void use() const;

    // utility for uniform functions
    void set_bool(const string &name, bool value) const;

    void set_int(const string &name, int value) const;

    void set_float(const string &name, float value) const;

private:
    void check_compile_errors(unsigned int s, string t) {
        int success;
        char infoLog[1024];
        if (t != "PROGRAM") {
            glGetShaderiv(s, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(s, 1024, nullptr, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << t << "\n" << infoLog <<
                        "\n -- --------------------------------------------------- -- " << endl;
            }
        } else {
            glGetProgramiv(s, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(s, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << t << "\n" << infoLog <<
                        "\n -- --------------------------------------------------- -- " << endl;
            }
        }
    }
};

#endif //SHADER_H
