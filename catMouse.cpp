#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class Entity {
protected:
    int x, y;
public:
    Entity(int x, int y) : x(x), y(y) {}
    virtual void move(const vector<vector<char>>& maze) = 0;
    int getX() const { return x; }
    int getY() const { return y; }
};

class Mouse : public Entity {
public:
    Mouse(int x, int y) : Entity(x, y) {}
    void move(const vector<vector<char>>& maze) override {
        int dx = rand() % 3 - 1;
        int dy = rand() % 3 - 1;
        if (x + dx >= 0 && x + dx < maze[0].size() && y + dy >= 0 && y + dy < maze.size() 
            && maze[y + dy][x + dx] != '#') {
            x += dx;
            y += dy;
        }
    }
};

class Cat : public Entity {
public:
    Cat(int x, int y) : Entity(x, y) {}

    void move(const vector<vector<char>>& maze) override {
        extern int mouseX;
        extern int mouseY;

        int dx = 0;
        int dy = 0;

        // Move towards the mouse
        if (mouseX > x) dx = 1;  // Move right
        else if (mouseX < x) dx = -1;  // Move left

        if (mouseY > y) dy = 1;  // Move down
        else if (mouseY < y) dy = -1;  // Move up

        if (x + dx >= 0 && x + dx < maze[0].size() && y + dy >= 0 && y + dy < maze.size() && maze[y + dy][x + dx] != '#') {
            x += dx;
            y += dy;
        }
    }
};

int mouseX = 0;
int mouseY = 0;

class Game {
private:
    vector<vector<char>> maze;
    Mouse mouse;
    Cat cat;
    int foodX, foodY;

public:
    Game(int width, int height) : 
        maze(height, vector<char>(width, ' ')),
        mouse(1, 1),
        cat(width - 2, height - 2),
        foodX(width / 2),
        foodY(height / 2)
    {
        // Add maze with walls
        for (int i = 0; i < height; ++i) {
            maze[i][0] = maze[i][width - 1] = '#';
        }
        for (int j = 0; j < width; ++j) {
            maze[0][j] = maze[height - 1][j] = '#';
        }
        
        // Add some random walls
        for (int i = 0; i < width * height / 10; ++i) {
            int x = rand() % (width - 2) + 1;
            int y = rand() % (height - 2) + 1;
            maze[y][x] = '#';
        }
        
        // Food isn't on a wall
        while (maze[foodY][foodX] == '#') {
            foodX = rand() % (width - 2) + 1;
            foodY = rand() % (height - 2) + 1;
        }
    }

    void run() {
        int turns = 0;
        while (turns < 100) {
            mouse.move(maze);

            mouseX = mouse.getX();
            mouseY = mouse.getY();

            cat.move(maze);

            // Check for game end
            if (mouse.getX() == foodX && mouse.getY() == foodY) {
                cout << "Mouse found food after " << turns << " turns!\n";
                return;
            }
            if (mouse.getX() == cat.getX() && mouse.getY() == cat.getY()) {
                cout << "Cat caught the mouse after " << turns << " turns!\n";
                return;
            }
            displayState();
            ++turns;
        }
        cout << "Game ended after 100 turns.\n";
    }

    void displayState() {
        for (int i = 0; i < maze.size(); ++i) {
            for (int j = 0; j < maze[i].size(); ++j) {
                if (i == mouse.getY() && j == mouse.getX())
                    cout << 'M';
                else if (i == cat.getY() && j == cat.getX())
                    cout << 'C';
                else if (i == foodY && j == foodX)
                    cout << 'F';
                else
                    cout << maze[i][j];
            }
            cout << '\n';
        }
        cout << "\n";
    }
};

int main() {
    srand(time(nullptr));
    Game game(20, 10);
    game.run();
    return 0;
}
