#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
//#include <Application/Application.hpp>
#include <RenderGraph/Input.hpp>
#include <RenderGraph/Node.hpp>
#include <RenderGraph/Output.hpp>
#include <RenderGraph/Scene.hpp>
#include <Vulkan/Context.hpp>
#include <Vulkan/v2/Descriptor.hpp>
#include <Vulkan/Rendering/Pipeline.hpp>

namespace sd::rg
{
    class RenderGraphEditor;

    class CompositionNode : public Node
    {
    public:
        CompositionNode(const sdvk::CommandBuffers& command_buffers, const sdvk::Context& context,
                        const sdvk::Swapchain& swapchain, RenderGraphEditor& rge);

        void execute(const vk::CommandBuffer& command_buffer) override;

        void compile() override;

        void draw() override;

        const Input& get_input(int32_t id) override;

        const Output& get_output(int32_t id) override;

    private:
        void _init_inputs();

        void _init_resources();

        void _update_descriptors(uint32_t current_frame);

        void _check_features();

        static constexpr auto n_frames_in_flight = 2;

        static constexpr std::string_view s_vertex_shader   = "composite.vert.spv";
        static constexpr std::string_view s_fragment_shader = "composite.frag.spv";

        struct Features
        {
            bool ambient_occlusion {false};
        } m_features;

        struct Renderer
        {
            std::array<vk::Framebuffer,       n_frames_in_flight>  sc_framebuffers;
            std::unique_ptr<sdvk::Descriptor2<n_frames_in_flight>> descriptor;

            std::array<vk::ClearValue, 2>     clear_values;
            vk::Pipeline                      pipeline;
            vk::PipelineLayout                pipeline_layout;
            vk::RenderPass                    render_pass;
            vk::Sampler                       sampler;
        } m_renderer;

    public:
        // A CompositionNode expects min. 1 input.
        // [1] Scene render image
        // [2] AO Image
        std::vector<std::unique_ptr<Input>>  m_inputs;

        // No outputs.
        std::vector<std::unique_ptr<Output>> m_outputs;

        RenderGraphEditor&          m_graph_editor;
        const sdvk::CommandBuffers& m_command_buffers;
        const sdvk::Context&        m_context;
        const sdvk::Swapchain&      m_swapchain;
    };
}
