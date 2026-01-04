std::array<int,18> Board::m_standardNumberOrder = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
const std::array<HexCoords,19> Board::m_standardCoordinates = {{
        // Center
        { 0,   0},

        // Ring 1 (distance = 1)
        { 1,   0},
        { 1,  -1},
        { 0,  -1},
        {-1,   0},
        {-1,   1},
        { 0,   1},

        // Ring 2 (distance = 2)
        { 1,   1},
        { 2,   0},
        { 2,  -1},
        { 2,  -2},
        { 1,  -2},
        { 0,  -2},
        {-1,  -1},
        {-2,   0},
        {-2,   1},
        {-2,   2},
        {-1,   2},
        { 0,   2}
    }
};
const std::array<HexCoords,6> Board::m_directionCoords = {{
    { 0, -1 },  // top left
    { 1, -1 },  // top right
    { 1,  0 },  // right
    { 0,  1 },  // bottom right
    { -1, 1 },  // bottom left
    { -1, 0 }   // left
}};
static const std::vector<TileDef> m_basicMap = {
    { -1, -2, ResourceType::Wood,   11 },
    {  0, -2, ResourceType::Wool,  12 },
    {  1, -2, ResourceType::Brick,   9 },

    { -2, -1, ResourceType::Wheat,   4 },
    { -1, -1, ResourceType::Ore,     6 },
    {  0, -1, ResourceType::Wood,    5 },
    {  1, -1, ResourceType::Wool,  10 },

    { -2,  0, ResourceType::Brick,   8 },
    { -1,  0, ResourceType::Wheat,   3 },
    {  0,  0, ResourceType::Desert,  0 },
    {  1,  0, ResourceType::Wheat,  11 },
    {  2,  0, ResourceType::Ore,     4 },

    { -1,  1, ResourceType::Wool,   9 },
    {  0,  1, ResourceType::Wood,   10 },
    {  1,  1, ResourceType::Ore,     3 },
    {  2,  1, ResourceType::Brick,   5 },

    {  0,  2, ResourceType::Wool,   6 },
    {  1,  2, ResourceType::Wheat,   8 },
    {  2,  2, ResourceType::Wood,    2 }
};
