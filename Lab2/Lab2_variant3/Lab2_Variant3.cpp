#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <mutex>

// Для Windows (керування курсором)
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// --- КОНСТАНТИ ---
const int WIDTH = 60;
const int HEIGHT = 20;
const int SPEED = 1;
const int DELAY_MS = 100;

//  Глобальний м'ютекс для синхронізації доступу до даних
mutex dataMutex;

class Fish {
public:
    char symbol;
    int x, y;
    int directionX;
    int directionY;

    Fish(char symbol, int x, int y, int dx, int dy)
        : symbol(symbol), x(x), y(y), directionX(dx), directionY(dy) {
    }

    // Рух по осі X (Золоті рибки)
    void moveX() {
        while (true) {
            {
                lock_guard<mutex> lock(dataMutex); // Автоматичне блокування
                x += directionX * SPEED;

                // Перевірка меж (відбивання)
                if (x >= WIDTH - 2 || x <= 1) {
                    directionX *= -1;
                }
            } // Тут lock знищується і м'ютекс автоматично розблоковується

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }

    // Рух по осі Y (Гуппі)
    void moveY() {
        while (true) {
            {
                lock_guard<mutex> lock(dataMutex);
                y += directionY * SPEED;

                // Перевірка меж
                if (y >= HEIGHT - 2 || y <= 1) {
                    directionY *= -1;
                }
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
};

void hideCursor() {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
#endif
}

void setCursorPosition(int x, int y) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
#endif
}

int main() {
    hideCursor();

    static Fish gold1('G', 5, 5, 1, 0);
    static Fish gold2('G', WIDTH - 5, 15, -1, 0);
    static Fish guppy1('y', 20, 2, 0, 1);
    static Fish guppy2('y', 40, HEIGHT - 2, 0, -1);

    vector<thread> threads;
    // Запуск потоків
    threads.emplace_back(&Fish::moveX, &gold1);
    threads.emplace_back(&Fish::moveX, &gold2);
    threads.emplace_back(&Fish::moveY, &guppy1);
    threads.emplace_back(&Fish::moveY, &guppy2);

    while (true) {
        setCursorPosition(0, 0);

        vector<string> field(HEIGHT, string(WIDTH, ' '));

        // Малюємо рамку
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1) field[i][j] = '#';
                else if (j == 0 || j == WIDTH - 1) field[i][j] = '#';
            }
        }

        {
            lock_guard<mutex> lock(dataMutex);

            auto placeFish = [&](Fish& f) {
                if (f.x > 0 && f.x < WIDTH && f.y > 0 && f.y < HEIGHT) {
                    field[f.y][f.x] = f.symbol;
                }
                };

            placeFish(gold1);
            placeFish(gold2);
            placeFish(guppy1);
            placeFish(guppy2);
        } 
        for (const auto& row : field) {
            cout << row << endl;
        }

        cout << "Thread Safe Aquarium (Variant 3). Ctrl+C to stop." << endl;
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }

    //  Join потрібен, щоб main чекав завершення потоків
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    return 0;
}
