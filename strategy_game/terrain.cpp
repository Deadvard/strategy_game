#include "terrain.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>

namespace scene
{

terrain::terrain(float x, float y, float z)
	: node(x, y, z)
    , height_offset(y)
{
	auto* begin =
        stbi_load("resources/images/heightmap.png", &width, &height, &channels, 1);
    auto* end = begin + width * height;
    heights.resize(end - begin);
    std::copy(begin, end, heights.begin());
    stbi_image_free(begin);
	
	vertices.resize(heights.size());

	for (auto i = 0u; i < vertices.size(); ++i)
	{
        glm::vec2 v{i % width, i / height};
        vertices[i].position = {v.x, heights[i] * 0.1f, v.y};
        vertices[i].texture = {v.x/4, v.y/4};
	}

	std::vector<glm::vec3> normals(heights.size());
	for (auto j = 0; j < (height-1); ++j)
	{
		for (auto i = 0; i < (width-1); ++i)
		{
			auto index1 = (j * height) + i;
			auto index2 = (j * height) + (i+1);
			auto index3 = ((j+1) * height) + i;

			auto u = vertices[index1].position - vertices[index3].position;
            auto v = vertices[index3].position - vertices[index2].position;

			auto index = (j * (height-1)) + i;
            normals[index] = glm::cross(u, v);

		}
	}

	for (auto j = 0; j < (height-1); ++j)
	{
		for (auto i = 0; i < (width-1); ++i)
		{
            glm::vec3 v{0.0f, 0.0f, 0.0f};
			auto count = 0;

			// Bottom left face.
			if(((i-1) >= 0) && ((j-1) >= 0))
			{
				auto index = ((j-1) * (height-1)) + (i-1);
                v += normals[index];
				++count;
			}
			// Bottom right face.
			if((i < (width-1)) && ((j-1) >= 0))
			{
				auto index = ((j-1) * (height-1)) + i;
                v += normals[index];
				++count;
			}
			// Upper left face.
			if(((i-1) >= 0) && (j < (height-1)))
			{
				auto index = (j * (height-1)) + (i-1);
                v += normals[index];
				++count;
			}
			// Upper right face.
			if((i < (width-1)) && (j < (height-1)))
			{
				auto index = (j * (height-1)) + i;
				v += normals[index];
				++count;
			}

			v = {v.x/count, v.y/count, v.z/count};
			auto index = (j * height) + i;
			vertices[index].normal = glm::normalize(v);
		}
	}

    std::vector<unsigned int> indices((width-2) * (height-2) * 6);
	for (auto i = 0u; i < indices.size()/6; ++i)
	{
        auto pos = (i/(height-2)) * width + (i%(width-2));
        indices[  draw_count] = pos;
        indices[++draw_count] = pos + width;
        indices[++draw_count] = pos + 1;


        indices[++draw_count] = pos + width;
        indices[++draw_count] = pos + width + 1;
        indices[++draw_count] = pos + 1;

        ++draw_count;
	}

	terrain_array.bind();

	terrain_vbo.data(sizeof(terrain_vertex) * vertices.size(),
        &vertices[0], GL_STATIC_DRAW);
	terrain_ebo.data(sizeof(unsigned int) * indices.size(),
        &indices[0], GL_STATIC_DRAW);

	terrain_array.attribute_pointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), nullptr);

    terrain_array.attribute_pointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), buffer_offset<glm::vec3>(1));

	terrain_array.attribute_pointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(terrain_vertex), buffer_offset<glm::vec3>(2));
}

glm::vec3 terrain::calculate_camera_position(
    const glm::vec3& world_position, float position_offset) const
{
    glm::vec4 position{inverse_transform * glm::vec4{world_position, 1.0f}};

    if(position.x > 0 && position.z > 0 && position.x < width
        && position.z < height)
    {
        glm::ivec2 v{position.x, position.z};
        int i = v.x + v.y * width;
        return {world_position.x, heights[i] * 0.1f +
            height_offset + position_offset, world_position.z};
    }

    return world_position;
}

void terrain::update_current(float delta_time,
    const glm::mat4& world_transform, glm::mat4& transform)
{
    inverse_transform = glm::inverse(world_transform);
}

void terrain::render_current(const shader& shader,
    const glm::mat4& world_transform) const
{
	shader.uniform("model", glm::mat4(1.f));
	
	terrain_array.bind();
	glDrawElements(GL_TRIANGLES, draw_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}
