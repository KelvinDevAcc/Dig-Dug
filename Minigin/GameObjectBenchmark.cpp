#include "GameObjectBenchmark.h"
#include <chrono>
#include <iostream>

void GameObjectBenchmark::RunIntBenchmark(int inputValue, int buffer_size, std::vector<float>& timings) {
    int* intBuffer = new int[buffer_size]; 

    for (int i = 1; i <= 1024; i *= 2) {
        long long duration{ 0 };

        for (int l = 0; l <= inputValue; ++l) {
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < buffer_size; j += i) {
                intBuffer[j] *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        duration /= (inputValue > 2) ? (inputValue - 2) : inputValue;
        timings.push_back(static_cast<float>(duration));
    }

    delete[] intBuffer; 
}

void GameObjectBenchmark::RunGameObjectaltBenchmark(int inputValue, int buffer_size, std::vector<float>& timings) {
	auto* GameObject = new GameObject3DAlt[buffer_size]; 

    for (int i = 1; i <= 1024; i *= 2) {
        long long duration{ 0 };

        for (int l = 0; l <= inputValue; ++l) {
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < buffer_size; j += i) {
                GameObject[j].ID *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        duration /= (inputValue > 2) ? (inputValue - 2) : inputValue;
        timings.push_back(static_cast<float>(duration));
    }

    delete[] GameObject; 
}

void GameObjectBenchmark::RunGameObjectBenchmark(int inputValue, int buffer_size, std::vector<float>& timings) {
    auto* GameObject = new GameObject3D[buffer_size]; 

    for (int i = 1; i <= 1024; i *= 2) {
        long long duration{ 0 };

        for (int l = 0; l <= inputValue; ++l) {
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < buffer_size; j += i) {
                GameObject[j].ID *= 2;
            }
            auto end = std::chrono::high_resolution_clock::now();
            duration += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        }

        duration /= (inputValue > 2) ? (inputValue - 2) : inputValue;
        timings.push_back(static_cast<float>(duration));
    }

    delete[] GameObject; 
}
