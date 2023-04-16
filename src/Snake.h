#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct SnakeSegment{
    SnakeSegment(int x, int y) : position(x,y){}
    sf::Vector2i position;
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction{None, Up, Down, Left, Right};

class Snake{
public:
    Snake(int l_blockSize);
    ~Snake();

    //Helper methods
    void SetDirection(Direction l_dir);
    Direction GetDirection();
    int GetSpeed();
    sf::Vector2i GetPosition();
    int GetLives();
    int GetScore();
    void IncreaseScore();
    bool HasLost();

    void Lose(); 
    void ToggleLost();

    void Extend(); //Grow snake
    void Reset(); //Reset to starting position

    void Move();
    void Tick();
    void Cut(int l_segments); //Method for cutting snake
    void Render(sf::RenderWindow& l_window);
private:
    void CheckCollision();

    SnakeContainer m_snakeBody;
    int m_size;
    Direction m_dir;
    int m_speed;
    int m_score;
    int m_lives;
    bool m_lost;
    sf::RectangleShape m_bodyRect;
};

class World{
public:
    World(sf::Vector2u l_windSize);
    ~World();

    int GetBlockSize();

    void RespawnApple();

    void Update(Snake& l_player);
    void Render(sf::RenderWindow& l_window);
private:
    sf::Vector2u m_windowSize;
    sf::Vector2i m_item;
    int m_blockSize;

    sf::CircleShape m_appleShape;
    sf::RectangleShape m_bounds[4];
};

class Game{
public:
    Game();
    void HandleInput();
    void Update();
    void Render();
    void RestartClock();
    sf::Time GetElapsed();
    sf::RenderWindow& GetWindow();
    //~Game();

private:
    sf::RenderWindow m_window;
    World m_world;
    Snake m_snake;
    sf::Clock m_clock;
    sf::Time m_elapsed;
};