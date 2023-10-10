#include "GraphCompileStrategy.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <format>
#include <string>
#include <VirtualGraph/Common/NodeType.hpp>
#include <VirtualGraph/Compile/Algorithm/Bfs.hpp>
#include <VirtualGraph/Compile/Algorithm/TopologicalSort.hpp>
#include <VirtualGraph/RenderGraph/Resources/ResourceSpecification.hpp>
#include <VirtualGraph/RenderGraph/Resources/ResourceType.hpp>

namespace Nebula::RenderGraph::Compiler
{
    std::string to_string(NodeType type)
    {
        switch (type)
        {
            case NodeType::eAmbientOcclusion:
                return "Ambient Occlusion";
            case NodeType::eAntiAliasing:
                return "Anti-Aliasing";
            case NodeType::eDeferredRender:
                return "Deferred Pass";
            case NodeType::eDenoise:
                return "Denoise";
            case NodeType::eGaussianBlur:
                return "Blur";
            case NodeType::eLightingPass:
                return "Lighting Pass";
            case NodeType::eRayTracing:
                return "Ray Tracing";
            case NodeType::ePresent:
                return "Present";
            case NodeType::eSceneProvider:
                return "Scene Provider";
            case NodeType::eUnknown:
                // Falls Through
            default:
                return "Unknown";
        }
    }

    GraphCompileStrategy::GraphCompileStrategy(const RenderGraphContext& context)
    : m_context(context)
    {
        m_node_factory = std::make_unique<NodeFactory>(context);
    }

    void GraphCompileStrategy::write_logs_to_file(const std::string& file_name)
    {
        auto path = std::format("logs/{}.txt", file_name);
        std::fstream fs(path);
        fs.open(path, std::ios_base::out);
        std::ostream_iterator<std::string> os_it(fs, "\n");
        std::copy(logs.begin(), logs.end(), os_it);
        fs.close();
    }

    CompileResult GraphCompileStrategy::make_failed_result(const std::string& message)
    {
        CompileResult result;

        result.success = false;
        result.failure_message = message;
        result.logs = logs;

        return result;
    }

    void GraphCompileStrategy::write_graph_state_dump(const RenderPath& render_path, const std::string& file_name)
    {
        std::vector<std::string> dump;
        dump.emplace_back("[=====[ Begin Resources ]=====]");
        for (const auto& res : render_path.resources)
        {
            dump.push_back(std::format("[Resource] {}", res.second->name()));
            dump.push_back(std::format("\tType: {}", get_resource_type_str(res.second->type())));
            dump.push_back(std::format("\tValid: {}", res.second->is_valid() ? "true" : "false"));
        }
        dump.emplace_back("[=====[ End Resources ]=====]");

        dump.emplace_back(" ");

        dump.emplace_back("[=====[ Begin Nodes ]=====]");
        for (const auto& node : render_path.nodes)
        {
            dump.push_back(std::format("[Node] {}", node->name()));
            dump.push_back(std::format("\tType: {}", to_string(node->type())));
            dump.emplace_back("\tResources:");

            auto& resources = node->resources();
            for (const ResourceSpecification& rspec : node->get_resource_specs())
            {
                const bool is_connected = resources.contains(rspec.name);
                const std::string in_or_out = (rspec.role == ResourceRole::eInput) ? "Input" : "Output";
                dump.push_back(
                    is_connected
                    ? std::format("\t\t{}: Connected to: {} as {}", rspec.name, resources[rspec.name]->name(), in_or_out)
                    : std::format("\t\t{}: Missing", rspec.name)
                );
            }
        }
        dump.emplace_back("[=====[End Nodes]=====]");

        auto path = std::format("logs/{}.txt", file_name);
        std::fstream fs(path);
        fs.open(path, std::ios_base::out);
        std::ostream_iterator<std::string> os_it(fs, "\n");
        std::copy(dump.begin(), dump.end(), os_it);
        fs.close();
    }

    std::vector<std::shared_ptr<Editor::Node>>
    GraphCompileStrategy::filter_unreachable_nodes(const std::vector<std::shared_ptr<Editor::Node>>& nodes)
    {
        std::vector<std::shared_ptr<Editor::Node>> result;

        std::shared_ptr<Editor::Node> root_node;
        for (const auto& node : nodes)
        {
            if (node->type() == NodeType::eSceneProvider)
            {
                root_node = node;
            }
        }

        if (!root_node)
        {
            throw std::runtime_error("[Error] Graph must contain a SceneProvider node");
        }

        auto bfs = std::make_unique<Algorithm::Bfs>(nodes);
        auto reachable_node_ids = bfs->execute(root_node);
        for (const auto& node : nodes)
        {
            if (reachable_node_ids.contains(node->id()))
            {
                result.push_back(node);
            }
        }

        return result;
    }

    std::vector<std::shared_ptr<Editor::Node>>
    GraphCompileStrategy::get_execution_order(const std::vector<std::shared_ptr<Editor::Node>>& nodes)
    {
        std::vector<std::shared_ptr<Editor::Node>> result;

        auto tsort = std::make_unique<Algorithm::TopologicalSort>(nodes);
        try
        {
            result = tsort->execute();
        }
        catch (const std::runtime_error& ex)
        {
            throw ex;
        }

        // !!! Important !!!
        // An execution order must always end with a "Present" node
        auto last_node = result.back();
        if (last_node->type() != NodeType::ePresent)
        {
            auto present_node = std::find_if(std::begin(result), std::end(result), [](const auto& n){
                return n->type() == NodeType::ePresent;
            });
            std::rotate(present_node, present_node + 1, std::end(result));
        }

        return result;
    }
}
