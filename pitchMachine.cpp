#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Ball
{
public:
    double x;
    double velocity;
    double angle;
    bool isStrikeZone;

    Ball(double startVelocity, double startAngle)
        : x(0), velocity(startVelocity), angle(startAngle)
    {
        isStrikeZone = (abs(angle) < (10 * M_PI / 180));
    }

    void updatePosition(double time)
    {
        x = velocity * time * cos(angle);
    }
};

class Batter
{
public:
    double reactionTime;
    double swingTiming;
    double swingPosition;

    Batter(double reactionTime)
        : reactionTime(reactionTime), swingTiming(0), swingPosition(0) {}

    void calculateSwing(const Ball &ball)
    {
        swingTiming = ball.x / ball.velocity + reactionTime;
        swingPosition = ball.velocity * swingTiming * cos(ball.angle);
    }

    bool didHit(const Ball &ball) const
    {
        // Check if the batter's swing position is close to the ball's position
        return abs(swingPosition - ball.x) < 0.5;
    }
};

void simulatePitch(double velocity, double angle, int &strikes, int &balls, int &hits)
{
    Ball ball(velocity, angle);
    Batter batter(0.2);

    double time = 0;
    bool hit = false;

    for (int i = 0; i < 100; i++)
    {
        time += 0.01;
        ball.updatePosition(time);

        if (ball.isStrikeZone)
        {
            batter.calculateSwing(ball);
            if (batter.didHit(ball))
            {
                hit = true;
                hits++;
                cout << "The batter hit the ball at position " << ball.x << "!" << endl;
                return;
            }
        }
    }

    // Decide the result based on player's choice and pitch location
    if (ball.isStrikeZone)
    {
        strikes++;
        cout << "Strike, The ball was in the strike zone." << endl;
    }
    else
    {
        balls++;
        cout << "Ball, The pitch was outside the strike zone." << endl;
    }
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    int strikes = 0, balls = 0, hits = 0;

    while (strikes < 3 && balls < 4)
    {
        cout << "Do you want to swing at the next pitch? y/n: ";
        char swingChoice;
        cin >> swingChoice;

        // Randomly generate velocity and angle for the pitch
        double velocity = 30 + (rand() % 20);
        double angle = (rand() % 20 - 10) * (M_PI / 180);

        if (swingChoice == 'y' || swingChoice == 'Y')
        {
            simulatePitch(velocity, angle, strikes, balls, hits);
        }
        else
        {
            if (angle < (10 * M_PI / 180) && angle > (-10 * M_PI / 180))
            {
                strikes++;
                cout << "Strike, You chose not to swing, but it was in the strike zone." << endl;
            }
            else
            {
                balls++;
                cout << "Ball, You chose not to swing, and it was outside the strike zone." << endl;
            }
        }

        cout << "Pitch count: Strikes: " << strikes << ", Balls: " << balls << ", Hits: " << hits << endl;
    }

    if (strikes >= 3)
    {
        cout << "Strike out! Better luck next time!" << endl;
    }
    else
    {
        cout << "You walked! Take your base." << endl;
    }

    return 0;
}
