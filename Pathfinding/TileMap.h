#pragma once
#include <XEngine.h>
#include <AI.h>

using Path = std::vector<X::Math::Vector2>;

struct Tile
{
    X::TextureId textureID = 0;
    bool isBlocked = false;
};

class TileMap
{
public:
    void LoadTiles(const char* fileName);
    void LoadMap(const char* fileName);

    void Render() const;

    Path FindPathBFS(int startX, int startY, int endX, int endY);
    Path FindPathDFS(int startX, int startY, int endX, int endY);
    Path FindPathDijkstra(int startX, int startY, int endX, int endY);
    Path FindPathAStar(int startX, int startY, int endX, int endY);

    bool IsBlocked(int x, int y) const;

    int GetColumns() const { return mColumns; }
    int GetRows() const { return mRows; }

    const AI::GridBasedGraph::Node* GetClosetNode(int x, int y);

    X::Math::Vector2 GetPixelPosition(int x, int y) const;

private:

    AI::GridBasedGraph mGraph;
    std::vector<int> mMap;
    std::vector<Tile> mTiles;

    int mColumns{};
    int mRows{};

    int mTileWidth{};
    int mTileHeight{};
};