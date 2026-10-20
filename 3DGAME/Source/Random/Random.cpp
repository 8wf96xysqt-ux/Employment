#include "Random.h"
#include <random>

namespace
{
    // メルセンヌ・ツイスタ
    std::mt19937 g_Random(std::random_device{}());
}

namespace Random
{
    int GetInt(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(g_Random);
    }

    float GetFloat(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(g_Random);
    }
}