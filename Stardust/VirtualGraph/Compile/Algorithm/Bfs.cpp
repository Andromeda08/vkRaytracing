#include "Bfs.hpp"

#include <queue>
#include <Math/Graph/Vertex.hpp>

namespace Nebula::Editor
{
    std::set<int32_t> Bfs::execute(const node_ptr& root)
    {
        std::set<int32_t> visited;
        std::queue<std::shared_ptr<Graph::Vertex>> Q;

        Q.push(root);
        visited.insert(root->id());

        while (!Q.empty())
        {
            auto current = Q.front();
            Q.pop();

            for (const auto& w : current->get_outgoing_edges())
            {
                if (!visited.contains(w->id()))
                {
                    visited.insert(w->id());
                    Q.push(w);
                }
            }
        }

        return visited;
    }
}