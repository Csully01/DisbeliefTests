#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>

using namespace std;

int CITY_WIDTH = 31;
int CITY_HEIGHT = 31;

class City
{
public:
    City()
    {
        grid = vector<vector<char>>(CITY_HEIGHT, vector<char>(CITY_WIDTH, '#'));

        // Create streets (unchanged)
        for (int i = 5; i < CITY_HEIGHT - 5; i += 5)
        {
            for (int j = 1; j < CITY_WIDTH - 1; ++j)
            {
                grid[i][j] = ' ';
            }
        }
        for (int j = 5; j < CITY_WIDTH - 5; j += 5)
        {
            for (int i = 1; i < CITY_HEIGHT - 1; ++i)
            {
                grid[i][j] = ' ';
            }
        }

        // Place traffic lights at intersections
        for (int i = 5; i < CITY_HEIGHT - 5; i += 5)
        {
            for (int j = 5; j < CITY_WIDTH - 5; j += 5)
            {
                grid[i][j] = 'R';
            }
        }
    }

    char getCell(int x, int y) { return grid[y][x]; }
    void setCell(int x, int y, char c) { grid[y][x] = c; }

private:
    vector<vector<char>> grid;
};

class Vehicle
{
public:
    int x, y, dx, dy;

    Vehicle(int x, int y, int dx, int dy) : x(x), y(y), dx(dx), dy(dy) {}

    void move()
    {
        x += dx;
        y += dy;
    }

    bool isAtIntersection(City &city)
    {
        return (x % 5 == 0 && y % 5 == 0) && (x > 0 && x < CITY_WIDTH - 1 && y > 0 && y < CITY_HEIGHT - 1);
    }
};

class TrafficSimulation
{
public:
    TrafficSimulation() : simulationTime(0)
    {
        // Initialize with some vehicles
        for (int i = 0; i < 10; ++i)
        {
            addRandomVehicle();
        }
    }

    void run()
    {
        while (simulationTime < 100)
        {
            render();
            update();
            simulationTime++;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        cout << "Simulation complete!" << endl;
    }

private:
    City city;
    vector<Vehicle> vehicles;
    int simulationTime;

    void render()
    {
        system("cls");
        vector<vector<char>> displayGrid(CITY_HEIGHT, vector<char>(CITY_WIDTH, ' '));

        // Copy city to display grid
        for (int y = 0; y < CITY_HEIGHT; ++y)
        {
            for (int x = 0; x < CITY_WIDTH; ++x)
            {
                displayGrid[y][x] = city.getCell(x, y);
            }
        }

        // Add vehicles to display grid
        for (auto &vehicle : vehicles)
        {
            displayGrid[vehicle.y][vehicle.x] = 'C';
        }

        // Display the grid
        for (auto &row : displayGrid)
        {
            for (char cell : row)
            {
                cout << cell;
            }
            cout << endl;
        }
        cout << "Simulation Time: " << simulationTime << endl;
    }

    void update()
    {
        // Update traffic lights
        if (simulationTime % 5 == 0)
        {
            for (int y = 5; y < CITY_HEIGHT - 5; y += 5)
            {
                for (int x = 5; x < CITY_WIDTH - 5; x += 5)
                {
                    city.setCell(x, y, city.getCell(x, y) == 'R' ? 'G' : 'R');
                }
            }
        }

        // Move vehicles
        for (auto &vehicle : vehicles)
        {
            if (vehicle.isAtIntersection(city))
            {
                char light = city.getCell(vehicle.x, vehicle.y);
                if (light == 'G')
                {
                    vehicle.move();
                }
            }
            else
            {
                vehicle.move();
            }
        }

        // Remove vehicles that have left the city
        for (int i = 0; i < vehicles.size(); i++)
        {
            if (vehicles[i].x < 0 || vehicles[i].x >= CITY_WIDTH ||
                vehicles[i].y < 0 || vehicles[i].y >= CITY_HEIGHT)
            {
                vehicles.erase(vehicles.begin() + i);
                i--;
            }
        }

        // Add new vehicles
        if (rand() % 3 == 0)
        {
            addRandomVehicle();
        }
    }

    void addRandomVehicle()
    {
        int x, y, dx, dy;
        if (rand() % 2 == 0)
        {
            // Horizontal street
            y = 5 * (1 + rand() % 5);
            x = (rand() % 2 == 0) ? 0 : CITY_WIDTH - 1;
            dx = (x == 0) ? 1 : -1;
            dy = 0;
        }
        else
        {
            // Vertical street
            x = 5 * (1 + rand() % 5);
            y = (rand() % 2 == 0) ? 0 : CITY_HEIGHT - 1;
            dx = 0;
            dy = (y == 0) ? 1 : -1;
        }
        vehicles.push_back(Vehicle(x, y, dx, dy));
    }
};

int main()
{
    srand(time(nullptr));
    TrafficSimulation simulation;
    simulation.run();
    return 0;
}