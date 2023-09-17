#include "Precompiled.h"
#include "Dijkstra.h"

namespace AI
{
    bool Dijkstra::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost)
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
            node = mOpenList.front();
            mOpenList.pop_front();

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
                    if (neighbor == nullptr || neighbor->closed)
                        continue;

                    float cost = node->cost + getCost(node, neighbor);

                    if (neighbor->opened == false)
                    {
                        mOpenList.push_back(neighbor);
                        neighbor->opened = true;
                        neighbor->parent = node;
                        neighbor->cost = cost;
                    }
                    else if (cost < neighbor->cost)
                    {
                        // edge relaxation
                        neighbor->parent = node;
                        neighbor->cost = cost;
                    }
                }
            }

            auto sortCost = [](const GridBasedGraph::Node* a, const GridBasedGraph::Node* b)
            {
                return a->cost < b->cost;
            };

            mOpenList.sort(sortCost);

            // Add the processed node to the closed list
            mClosedList.push_back(node);
            node->closed;
        }

        return found;
    }
}