#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

const int WIDTH = 60;
const int HEIGHT = 20;
const int SPEED = 1;
const int DELAY_MS = 100;

class Fish {
public:
    char symbol;    
    int x, y;       
    int directionX; 
    int directionY; 

    Fish(char symbol, int x, int y, int dx, int dy)
        : symbol(symbol), x(x), y(y), directionX(dx), directionY(dy) {
    }

    // Рух по осі X (для Золотих рибок)
    void moveX() {
        while (true) {
            x += directionX * SPEED;

            // розворот
            if (x >= WIDTH - 2 || x <= 1) {
                directionX *= -1;
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }

    // Рух по осі Y (для Гуппі)
    void moveY() {
        while (true) {
            y += directionY * SPEED;

            //розворот
            if (y >= HEIGHT - 2 || y <= 1) {
                directionY *= -1;
            }

            this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
        }
    }
};

// Функція, щоб прибрати миготливий курсор
void hideCursor() {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
#endif
}

// Функція малювання поверх старого тексту (щоб не миготіло)
void setCursorPosition(int x, int y) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
#endif
}

int main() {
    hideCursor();

    // Створюємо рибок на фіксованих місцях
    // 'G' - Goldfish (рухаються вліво-вправо)
    static Fish gold1('G', 5, 5, 1, 0);           // Ряд 5
    static Fish gold2('G', WIDTH - 5, 15, -1, 0); // Ряд 15

    // 'y' - Guppy (рухаються вгору-вниз)
    static Fish guppy1('y', 20, 2, 0, 1);         // Стовпчик 20
    static Fish guppy2('y', 40, HEIGHT - 2, 0, -1); // Стовпчик 40

    // Запускаємо потоки
    vector<thread> threads;
    threads.emplace_back(&Fish::moveX, &gold1);
    threads.emplace_back(&Fish::moveX, &gold2);
    threads.emplace_back(&Fish::moveY, &guppy1);
    threads.emplace_back(&Fish::moveY, &guppy2);

    // Головний цикл малювання
    while (true) {
        setCursorPosition(0, 0); // Повертаємося на початок екрану

        // 1. Очищаємо поле пробілами
        vector<string> field(HEIGHT, string(WIDTH, ' '));

        // 2. Малюємо рамку
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT - 1) field[i][j] = '#';
                else if (j == 0 || j == WIDTH - 1) field[i][j] = '#';
            }
        }

        // 3. Ставимо рибок на поле
        auto placeFish = [&](Fish& f) {
            if (f.x > 0 && f.x < WIDTH && f.y > 0 && f.y < HEIGHT) {
                field[f.y][f.x] = f.symbol;
            }
            };

        placeFish(gold1);
        placeFish(gold2);
        placeFish(guppy1);
        placeFish(guppy2);

        // 4. Вивід готового кадру
        for (const auto& row : field) {
            cout << row << endl;
        }

        cout << "Ctrl+C to stop." << endl;
        this_thread::sleep_for(chrono::milliseconds(DELAY_MS));
    }

    // Завершення
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    return 0;
}