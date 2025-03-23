#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <climits>


const int dataSize = 1000000; // Розмір масиву
const int numThreads = 4;     // Кількість потоків

void generateData(std::vector<int>& data) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(-100, 200);
    for (int& num : data) {
        num = dist(rng);
    }
}

// --- Послідовна версія ---
void serialVersion(const std::vector<int>& data) {
    int count = 0;
    int maxVal = INT_MIN;

    auto start = std::chrono::high_resolution_clock::now();

    for (int num : data) {
        if (num > 10) {
            count++;
            if (num > maxVal) {
                maxVal = num;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;


    std::cout << "Without parallelization" <<  std::endl;

    std::cout << "Number of elements > 10: " << count << std::endl;
    if (count > 0)
        std::cout << "Largest element > 10: " << maxVal << std::endl;
    else
        std::cout << "No items > 10 found." << std::endl;

    std::cout << "Execution time: " << elapsed.count() << " second" << std::endl;

}

// --- Паралельна версія з м’ютексом ---
void parallelWithMutex(const std::vector<int>& data) {
    int count = 0;
    int maxVal = INT_MIN;
    std::mutex mtx;

    auto worker = [&](int start, int end) {
        int localCount = 0;
        int localMax = INT_MIN;

        for (int i = start; i < end; ++i) {
            if (data[i] > 10) {
                localCount++;
                if (data[i] > localMax) {
                    localMax = data[i];
                }
            }
        }

        std::lock_guard<std::mutex> lock(mtx);
        count += localCount;
        if (localMax > maxVal) {
            maxVal = localMax;
        }
    };

    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    int chunkSize = data.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startIdx = i * chunkSize;
        int endIdx = (i == numThreads - 1) ? data.size() : (i + 1) * chunkSize;
        threads.emplace_back(worker, startIdx, endIdx);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Parallel version with mutex" << "\n";

    std::cout << "Number of elements > 10: " << count << std::endl;
    if (count > 0)
        std::cout << "Largest element > 10: " << maxVal << std::endl;
    else
        std::cout << "No items > 10 found." << std::endl;

    std::cout << "Execution time: " << elapsed.count() << " second" << std::endl;
}

int main() {
    std::vector<int> data(dataSize);
    generateData(data);

    std::cout << "Виконання для масиву з " << dataSize << " елементів\n\n";

    serialVersion(data);
    parallelWithMutex(data);

    return 0;
}
