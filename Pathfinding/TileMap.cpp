#include "TileMap.h"

using namespace AI;

namespace
{
    inline int ToIndex(int x, int y, int columns)
    {
        return x + (y * columns);
    }
}

void TileMap::LoadTiles(const char* fileName)
{
    if (std::filesystem::exists(fileName) == false)
        return;

    std::fstream file(fileName);

    int count = 0;
    std::string buffer;

    file >> buffer; // ">>" use the first text section and put them in buffer
    file >> count;

    mTiles.clear();
    mTiles.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        int isBlocked = 0;
        file >> buffer;
        file >> isBlocked;

        Tile& tile = mTiles.emplace_back();
        tile.isBlocked = isBlocked == 1;
        tile.textureID = X::LoadTexture(buffer.c_str());
    }

    file.close();

    mTileWidth = X::GetSpriteWidth(mTiles[0].textureID);
    mTileHeight = X::GetSpriteHeight(mTiles[0].textureID);
}

void TileMap::LoadMap(const char* fileName)
{
    if (std::filesystem::exists(fileName) == false)
        return;

    std::fstream file(fileName);
    int columns = 0, rows = 0;
    std::string buffer;

    file >> buffer;
    file >> columns;
    file >> buffer;
    file >> rows;

    mColumns = columns;
    mRows = rows;

    mMap.resize(columns * rows);
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < columns; ++x)
        {
            char tileType{};
            file >> tileType;
            mMap[ToIndex(x, y, columns)] = tileType - '0';
        }
    }

    file.close();

    auto GetNeighbor = [&](int c, int r) -> AI::GridBasedGraph::Node*
        {
            if (IsBlocked(c, r))
                return nullptr;

            return mGraph.GetNode(c, r);
        };

    mGraph.Initialize(mColumns, mRows);
    for (int r = 0; r < mRows; ++r)
    {
        for (int c = 0; c < mColumns; ++c)
        {
            if (IsBlocked(c, r))
                continue;

            GridBasedGraph::Node* node = GetNeighbor(c, r);
            node->neighbors[GridBasedGraph::Direction::North] = GetNeighbor(c, r - 1);
            node->neighbors[GridBasedGraph::Direction::South] = GetNeighbor(c, r + 1);
            node->neighbors[GridBasedGraph::Direction::East] = GetNeighbor(c + 1, r);
            node->neighbors[GridBasedGraph::Direction::West] = GetNeighbor(c - 1, r);
            node->neighbors[GridBasedGraph::Direction::NorthEast] = GetNeighbor(c + 1, r - 1);
            node->neighbors[GridBasedGraph::Direction::NorthWest] = GetNeighbor(c - 1, r - 1);
            node->neighbors[GridBasedGraph::Direction::SouthEast] = GetNeighbor(c + 1, r + 1);
            node->neighbors[GridBasedGraph::Direction::SouthWest] = GetNeighbor(c - 1, r + 1);
        }
    }
}

void TileMap::Render() const
{
    X::Math::Vector2 position;
    for (int y = 0; y < mRows; ++y)
    {
        for (int x = 0; x < mColumns; ++x)
        {
            int index = ToIndex(x, y, mColumns);
            X::DrawSprite(mTiles[mMap[index]].textureID, position, X::Pivot::TopLeft);
            position.x += mTileWidth;
        }

        position.x = 0.0f;
        position.y += mTileHeight;
    }

    for (int r = 0; r < mRows; ++r)
    {
        for (int c = 0; c < mColumns; ++c)
        {
            const auto node = mGraph.GetNode(c, r);
            for (const auto neighbor : node->neighbors)
            {
                if (neighbor == nullptr)
                    continue;

                const auto a = GetPixelPosition(node->column, node->row);
                const auto b = GetPixelPosition(neighbor->column, neighbor->row);

                X::DrawScreenLine(a, b, X::Colors::Blue);
            }
        }
    }

    for (int r = 0; r < mRows; ++r)
    {
        for (int c = 0; c < mColumns; ++c)
        {
            const auto node = mGraph.GetNode(c, r);
            if (node->parent != nullptr)
            {
                const auto a = GetPixelPosition(node->column, node->row);
                const auto b = GetPixelPosition(node->parent->column, node->parent->row);

                X::DrawScreenLine(a, b, X::Colors::Orange);
            }
        }
    }
}

Path TileMap::FindPathBFS(int startX, int startY, int endX, int endY)
{
    Path path;
    BFS bfs;

    if (bfs.Run(mGraph, startX, startY, endX, endY))
    {
        const auto& closedList = bfs.GetClosedList();
        auto node = closedList.back();

        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->column, node->row));
            node = node->parent;
        }

        std::reverse(path.begin(), path.end());
    }

    return path;
}

Path TileMap::FindPathDFS(int startX, int startY, int endX, int endY)
{
    Path path;
    DFS dfs;

    if (dfs.Run(mGraph, startX, startY, endX, endY))
    {
        const auto& closedList = dfs.GetClosedList();
        auto node = closedList.back();

        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->column, node->row));
            node = node->parent;
        }

        std::reverse(path.begin(), path.end());
    }

    return path;
}

Path TileMap::FindPathDijkstra(int startX, int startY, int endX, int endY)
{
    Path path;
    Dijkstra dijkstra;

    auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbor) -> float
        {
            if (node->column != neighbor->column && node->row != neighbor->row)
            {
                return 2.5f;
            }

            return 1;
        };

    if (dijkstra.Run(mGraph, startX, startY, endX, endY, getCost))
    {
        const auto& closedList = dijkstra.GetClosedList();
        auto node = closedList.back();

        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->column, node->row));
            node = node->parent;
        }

        std::reverse(path.begin(), path.end());
    }

    return path;
}

Path TileMap::FindPathAStar(int startX, int startY, int endX, int endY)
{
    Path path;
    AStar aStar;

    auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbor) -> float
        {
            if (node->column != neighbor->column && node->row != neighbor->row)
            {
                return 1.73f;
            }

            return 1;
        };

    auto getHeuristic = [](const GridBasedGraph::Node* neighbor, const GridBasedGraph::Node* endNode) -> float
        {
            float D = 1.0f;
            float dx = abs(neighbor->column - endNode->column);
            float dy = abs(neighbor->row - endNode->row);

            return D * (dx + dy);
        };

    if (aStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
    {
        const auto& closedList = aStar.GetClosedList();
        auto node = closedList.back();

        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->column, node->row));
            node = node->parent;
        }

        std::reverse(path.begin(), path.end());
    }

    return path;
}

bool TileMap::IsBlocked(int x, int y) const
{
    if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
    {
        const int index = ToIndex(x, y, mColumns);
        if (index < mMap.size())
        {
            int tile = mMap[index];
            return mTiles[tile].isBlocked;
        }
    }

    return true;
}

const AI::GridBasedGraph::Node* TileMap::GetClosetNode(int x, int y)
{
    X::Math::Vector2 mousePos(x, y);
    const AI::GridBasedGraph::Node* clickedNode = nullptr;
    float closestDist = FLT_MAX;

    for (int r = 0; r < mRows; ++r)
    {
        for (int c = 0; c < mRows; ++c)
        {
            const auto node = mGraph.GetNode(c, r);
            float distanceSqr = X::Math::DistanceSqr(GetPixelPosition(node->column, node->row), mousePos);
            if (distanceSqr < closestDist)
            {
                closestDist = distanceSqr;
                clickedNode = node;
            }
        }
    }

    return clickedNode;
}

X::Math::Vector2 TileMap::GetPixelPosition(int x, int y) const
{
    return {
        (x + 0.5f) * mTileWidth,
        (y + 0.5f) * mTileHeight
    };
}

// 2D map - 5 columns x 4 rows
// [0][0][0][0][0]
// [0][0][0][0][0]
// [0][0][0][X][0]   X is at (3, 2)
// [0][0][0][0][0]

// Stored as 1D - 5x4 = 20 slots
// [0][0][0][0][0] [0][0][0][0][0] [0][0][0][X][0] [0][0][0][0][0]
//
// index = column + (row * columnCount)
//       = 3 + (2 * 5)
//       = 13