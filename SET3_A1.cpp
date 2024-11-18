#include <iostream>
#include <cmath>
#include <random>
#include <clocale>
#include <iomanip>
#include <vector>
#include <fstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// проверка на принадлежность одному кругу
bool isInCircle(double x, double y, double cx, double cy, double r) {
    return (x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r;
}

// проверка на принадлежность сразу всем кругам
bool isInIntersection(double x, double y) {
    return isInCircle(x, y, 1.0, 1.0, 1.0) &&
        isInCircle(x, y, 1.5, 2.0, sqrt(5) / 2) &&
        isInCircle(x, y, 2.0, 1.5, sqrt(5) / 2);
}

int main() {
    setlocale(LC_ALL, "ru_RU");

    // Размеры области
    double xMin = 0.0, xMax = 2.5, yMin = 0.0, yMax = 2.5;

    // Точная площадь
    double exactArea = 0.25 * M_PI + 1.25 * asin(0.8) - 1;

    vector<int> pointsCount; // Количество точек
    vector<double> estimatedAreas; // Приближенные площади
    vector<double> relativeErrors; // Относительные ошибки

    // Генератор случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> disX(xMin, xMax);
    uniform_real_distribution<> disY(yMin, yMax);

    // Эксперименты с количеством точек от 100 до 100000 с шагом 500
    for (int numPoints = 100; numPoints <= 100000; numPoints += 500) {
        int pointsInside = 0;

        // Генерация точек и подсчет попаданий
        for (int i = 0; i < numPoints; ++i) {
            double x = disX(gen);
            double y = disY(gen);
            if (isInIntersection(x, y)) {
                ++pointsInside;
            }
        }

        // Площадь прямоугольника
        double rectArea = (xMax - xMin) * (yMax - yMin);

        double estimatedArea = (static_cast<double>(pointsInside) / numPoints) * rectArea;

        double error = fabs(estimatedArea - exactArea) / exactArea;
        pointsCount.push_back(numPoints);
        estimatedAreas.push_back(estimatedArea);
        relativeErrors.push_back(error);
    }

    // Сохраняем результаты в файл для построения графиков в Python
    ofstream file("results.txt");
    file << "PointsCount EstimatedArea RelativeError" << endl;
    for (size_t i = 0; i < pointsCount.size(); ++i) {
        file << pointsCount[i] << " " << estimatedAreas[i] << " " << relativeErrors[i] << endl;
    }

    file.close();

    return 0;
}
