#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;

    static vk::VertexInputBindingDescription binding_description()
    {
        vk::VertexInputBindingDescription description;
        description.setBinding(0);
        description.setStride(sizeof(Vertex));
        description.setInputRate(vk::VertexInputRate::eVertex);
        return description;
    }

    static std::vector<vk::VertexInputAttributeDescription> attribute_description()
    {
        std::vector<vk::VertexInputAttributeDescription> attrs(4);

        attrs[0].setBinding(0);
        attrs[0].setLocation(0);
        attrs[0].setFormat(vk::Format::eR32G32B32Sfloat);
        attrs[0].setOffset(offsetof(Vertex, position));

        attrs[1].setBinding(0);
        attrs[1].setLocation(1);
        attrs[1].setFormat(vk::Format::eR32G32B32Sfloat);
        attrs[1].setOffset(offsetof(Vertex, color));

        attrs[2].setBinding(0);
        attrs[2].setLocation(2);
        attrs[2].setFormat(vk::Format::eR32G32B32Sfloat);
        attrs[2].setOffset(offsetof(Vertex, normal));

        attrs[3].setBinding(0);
        attrs[3].setLocation(3);
        attrs[3].setFormat(vk::Format::eR32G32Sfloat);
        attrs[3].setOffset(offsetof(Vertex, uv));

        return attrs;
    }
};

const std::vector<Vertex> test_vertices = {
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
    // Front face [4, 5, 6, 6, 7, 4]
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    // Left face [8, 9, 10, 10, 11, 8]
    {{-0.5f, -0.5f, -0.5f}, {0, 0, 1}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f}, {0, 0, 1}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.5f}, {0, 0, 1}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {0, 0, 1}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    // Right face [12, 13, 14, 14, 15, 12]
    {{0.5f, -0.5f, 0.5f}, {1, 0, 1}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {1, 0, 1}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {1, 0, 1}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, 0.5f}, {1, 0, 1}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    // Bottom face [16, 17, 18, 18, 19, 16]
    {{-0.5f, -0.5f, -0.5f}, {0, 1, 1}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0, 1, 1}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f}, {0, 1, 1}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f, 0.5f}, {0, 1, 1}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
    // Top face [20, 21, 22, 22, 23, 20]
    {{-0.5f, 0.5f, 0.5f}, {1, 1, 0}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f}, {1, 1, 0}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {1, 1, 0}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1, 1, 0}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
};

const std::vector<uint32_t> test_indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

/*
const std::vector<Vertex> test_vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},

    {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f,  0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
};

const std::vector<uint32_t> test_indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};
*/