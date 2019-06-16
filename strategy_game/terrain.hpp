#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "node.hpp"
#include "buffer.hpp"
#include "texture.hpp"

namespace scene
{

struct terrain_vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

class terrain : public node
{
public:
	terrain(float x = 0.0f, float y = 0.0f, float z = 0.0f);

	glm::vec3 calculate_camera_position(
        const glm::vec3& world_position, float position_offset) const;
	std::vector<terrain_vertex> vertices;

	void rock(const shader& shader)
	{
		albedo.uniform(shader, "albedo", 0);
		normal.uniform(shader, "normal", 1);
		metallic.uniform(shader, "metallic", 2);
		roughness.uniform(shader, "roughness", 3);
	}
	void snow(const shader& shader)
	{
		albedo1.uniform(shader, "albedo", 0);
		normal1.uniform(shader, "normal", 1);
		metallic1.uniform(shader, "metallic", 2);
		roughness1.uniform(shader, "roughness", 3);
	}

	void wood(const shader& shader)
	{
		albedo2.uniform(shader, "albedo", 0);
		normal.uniform(shader, "normal", 1);
		metallic2.uniform(shader, "metallic", 2);
		roughness2.uniform(shader, "roughness", 3);
	}

private:
	std::vector<int> heights;
	int draw_count{0};

	glm::mat4 inverse_transform{1.0f};

	vertex_array terrain_array;
	buffer terrain_vbo;
	buffer terrain_ebo{GL_ELEMENT_ARRAY_BUFFER};
	texture albedo{ "resources/textures/slatecliffrock-albedo.png" };
	texture normal{ "resources/textures/slatecliffrock_Normal.png" };
	texture metallic{ "resources/textures/slatecliffrock_Metallic.png" };
	texture roughness{ "resources/textures/slatecliffrock_Roughness2.png" };

	texture albedo1{ "resources/textures/snow-packed12-Base_Color.png" };
	texture normal1{ "resources/textures/snow-packed12-Normal-dx.png" };
	texture metallic1{ "resources/textures/snow-packed12-Metallic.png" };
	texture roughness1{ "resources/textures/snow-packed12-Roughness.png" };

	texture albedo2{ "resources/textures/branches_twisted_albedo.png" };
	texture normal2{ "resources/textures/branches_twisted_Normal-ue.png" };
	texture metallic2{ "resources/textures/branches_twisted_metalness.png" };
	texture roughness2{ "resources/textures/branches_twisted_Roughness.png" };

	int width = 0;
	int height = 0;
	int channels = 0;
	float height_offset;

	void update_current(float delta_time,
		const glm::mat4& world_transform, glm::mat4& transform) override;

	void render_current(const shader& shader,
		const glm::mat4& world_transform) const override;

};

}

#endif
