#include "Precompiled.h"
#include "DFS.h"

namespace AI
{
    bool DFS::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY)
    {
        graph.ResetSearchParameter();
        mOpenList.clear();
        mClosedList.clear();

        // Add a start node to the openList
        auto node = graph.GetNode(startX, startY);

        mOpenList.push_back(node);
        node->opened = true;
        bool found = false;

        // Search for the end node
        while (!found && mOpenList.empty() == false)
        {
            // DFS is LIFO
            node = mOpenList.back();
            mOpenList.pop_back();

            // Is this the end node?
            if (node->column == endX && node->row == endY)
            {
                // we are done
                found = true;
            }
            else
            {
                // Go through the neighbors and add to the openList
                for (auto neighbor : node->neighbors)
                {
                    if (neighbor == nullptr || neighbor->opened)
                        continue;

                    mOpenList.push_back(neighbor);
                    neighbor->opened = true;
                    neighbor->parent = node;
                }
            }

            // Add the processed node to the closed list
            mClosedList.push_back(node);
            node->closed;
        }

        return found;
    }
}