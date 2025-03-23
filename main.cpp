#include <iostream>
#include <vector>
#include <chrono>
#include <random>

int main() {
    const int dataSize = 1000000; // розмір масиву
    std::vector<int> data(dataSize);

    // Ініціалізація генератора випадкових чисел
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(-100, 200);

    // Заповнення масиву випадковими числами
    for (int& num : data) {
        num = dist(rng);
    }

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

    std::cout << "Number of elements > 10: " << count << std::endl;
    if (count > 0)
        std::cout << "Largest element > 10: " << maxVal << std::endl;
    else
        std::cout << "No items > 10 found." << std::endl;

    std::cout << "Execution time: " << elapsed.count() << " second" << std::endl;

    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.