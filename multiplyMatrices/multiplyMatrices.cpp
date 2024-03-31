#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <random>

// Функция создания двух матриц
std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> createMatrices(int rows, int cols) {
    std::vector<std::vector<int>> matrix1(rows, std::vector<int>(cols));
    std::vector<std::vector<int>> matrix2(cols, std::vector<int>(rows));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix1[i][j] = dis(gen);
            matrix2[j][i] = dis(gen);
        }
    }

    return { matrix1, matrix2 };
}

// Функция перемножения двух матриц
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2) {
    int rows = matrix1.size();
    int cols = matrix2[0].size();
    int common = matrix2.size();

    std::vector<std::vector<int>> result(rows, std::vector<int>(cols, 0));

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < common; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time taken to multiply matrices: " << elapsed_seconds.count() << " seconds" << std::endl;

    return result;
}

// Функция многопоточного перемножения двух матриц
std::vector<std::vector<int>> multiplyMatricesMultithreaded(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2) {
    int rows = matrix1.size();
    int cols = matrix2[0].size();
    int common = matrix2.size();

    std::vector<std::vector<int>> result(rows, std::vector<int>(cols, 0));

    auto start = std::chrono::high_resolution_clock::now();

    auto multiplyRow = [&](int row) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < common; ++k) {
                result[row][j] += matrix1[row][k] * matrix2[k][j];
            }
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < rows; ++i) {
        threads.emplace_back(multiplyRow, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Time taken for multithreaded matrix multiplication: " << elapsed_seconds.count() << " seconds" << std::endl;

    return result;
}

int main() {
    int rows = 3;
    int cols = 3;

    auto matrices = createMatrices(rows, cols);
    std::vector<std::vector<int>> matrix1 = matrices.first;
    std::vector<std::vector<int>> matrix2 = matrices.second;

    std::cout << "Matrix 1:" << std::endl;
    for (const auto& row : matrix1) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Matrix 2:" << std::endl;
    for (const auto& row : matrix2) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    auto resultSingleThread = multiplyMatrices(matrix1, matrix2);

    std::cout << "Result of single-threaded matrix multiplication:" << std::endl;
    for (const auto& row : resultSingleThread) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    auto resultMultithreaded = multiplyMatricesMultithreaded(matrix1, matrix2);

    std::cout << "Result of multi-threaded matrix multiplication:" << std::endl;
    for (const auto& row : resultMultithreaded) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}