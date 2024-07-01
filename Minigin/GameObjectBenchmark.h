#pragma once

#include <vector>

struct Transform {
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class GameObject3D {
public:
    Transform transform;
    int ID;
};

class GameObject3DAlt {
public:
    Transform* transform;
    int ID;
};

class GameObjectBenchmark final{
public:
    static void RunIntBenchmark(int inputValue, int buffer_size, std::vector<float>& timings);

    static void RunGameObjectBenchmark(int inputValue, int buffer_size, std::vector<float>& timings);
    static void RunGameObjectaltBenchmark(int inputValue, int buffer_size, std::vector<float>& timings);
};

