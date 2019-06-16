#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>
#include <GL/glew.h>
#include "shader.hpp"

class texture
{
public:
	texture(const std::string& path);

    texture(const std::vector<std::string>& paths);

    ~texture();

    void uniform(const shader& shader, const std::string& name,
        int texture_index = 0) const;
    void skybox(const shader& shader) const;
    void bind_to_buffer() const;

private:
    unsigned int id;

};

#endif // TEXTURE_HPP
