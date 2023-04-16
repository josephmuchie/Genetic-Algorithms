#include "Snake.h"

Snake::Snake(int l_blockSize)
{
    m_size = l_blockSize;
    m_bodyRect.setSize(sf::Vector2f(m_size - 1, m_size - 1));
    Reset();
}
Snake::~Snake() {}

void Snake::Reset()
{
    m_snakeBody.clear();

    m_snakeBody.push_back(SnakeSegment(5, 7));
    m_snakeBody.push_back(SnakeSegment(5, 6));
    m_snakeBody.push_back(SnakeSegment(5, 5));

    SetDirection(Direction::None); // Start off still
    m_speed = 15;
    m_lives = 3;
    m_score = 0;
    m_lost = false;
}

void Snake::SetDirection(Direction l_dir) { m_dir = l_dir; }
Direction Snake::GetDirection() { return m_dir; }
int Snake::GetSpeed() { return m_speed; }

sf::Vector2i Snake::GetPosition()
{
    return (!m_snakeBody.empty() ? m_snakeBody.front().position : sf::Vector2i(1, 1));
}

int Snake::GetLives() { return m_lives; }
int Snake::GetScore() { return m_score; }

void Snake::IncreaseScore() { m_score += 10; }
bool Snake::HasLost() { return m_lost; }
void Snake::Lose() { m_lost = true; }
void Snake::ToggleLost() { m_lost = !m_lost; }

void Snake::Extend()
{
    if (m_snakeBody.empty())
    {
        return;
    }
    SnakeSegment &tail_head = m_snakeBody[m_snakeBody.size() - 1];

    if (m_snakeBody.size() > 1)
    {
        SnakeSegment &tail_bone = m_snakeBody[m_snakeBody.size() - 2];

        if (tail_head.position.x == tail_bone.position.x)
        {
            if (tail_head.position.y == tail_bone.position.y)
            {
                m_snakeBody.push_back(SnakeSegment(
                    tail_head.position.x, tail_head.position.y + 1));
            }
            else
            {
                m_snakeBody.push_back(SnakeSegment(tail_head.position.x, tail_head.position.y - 1));
            }
        }
        else if (tail_head.position.y == tail_bone.position.y)
        {
            if (tail_head.position.x > tail_bone.position.x)
            {
                m_snakeBody.push_back(SnakeSegment(
                    tail_head.position.x + 1, tail_head.position.y));
            }
            else
            {
                m_snakeBody.push_back(SnakeSegment(
                    tail_head.position.x - 1, tail_head.position.y));
            }
        }
    }
    else
    {
        if (m_dir == Direction::Up)
        {
            m_snakeBody.push_back(SnakeSegment(
                tail_head.position.x, tail_head.position.y + 1));
        }
        else if (m_dir == Direction::Down)
        {
            m_snakeBody.push_back(SnakeSegment(
                tail_head.position.x, tail_head.position.y - 1));
        }
        else if (m_dir == Direction::Left)
        {
            m_snakeBody.push_back(SnakeSegment(
                tail_head.position.x + 1, tail_head.position.y));
        }
        else if (m_dir == Direction::Right)
        {
            m_snakeBody.push_back(SnakeSegment(
                tail_head.position.x - 1, tail_head.position.y));
        }
    }
}

void Snake::Tick()
{
    if (m_snakeBody.empty())
    {
        return;
    }
    if (m_dir == Direction::None)
    {
        return;
    }
    Move();
    CheckCollision();
}

void Snake::Move()
{
    for (int i = m_snakeBody.size() - 1; i < 0; --i)
    {
        m_snakeBody[i].position = m_snakeBody[i - 1].position;
    }
    if (m_dir == Direction::Left)
    {
        --m_snakeBody[0].position.x;
    }
    else if (m_dir == Direction::Right)
    {
        ++m_snakeBody[0].position.y;
    }
    else if (m_dir == Direction::Down)
    {
        ++m_snakeBody[0].position.y;
    }
}

void Snake::CheckCollision()
{
    if (m_snakeBody.size() < 5)
    {
        return;
    }
    SnakeSegment &head = m_snakeBody.front();
    for (auto itr = m_snakeBody.begin() + 1;
         itr != m_snakeBody.end(); ++itr)
    {
        if (itr->position == head.position)
        {
            int segments = m_snakeBody.end() - itr;
            Cut(segments);
            break;
        }
    }
}

void Snake::Cut(int l_segments)
{
    for (int i = 0; i < l_segments; ++i)
    {
        m_snakeBody.pop_back();
    }
    --m_lives;
    if (!m_lives)
    {
        Lose();
        return;
    }
}

void Snake::Render(sf::RenderWindow &l_window)
{
    if (m_snakeBody.empty())
    {
        return;
    }

    auto head = m_snakeBody.begin();
    m_bodyRect.setFillColor(sf::Color::Yellow);
    m_bodyRect.setPosition(head->position.x * m_size,
                           head->position.y * m_size);
    l_window.draw(m_bodyRect);

    m_bodyRect.setFillColor(sf::Color::Green);
    for (auto itr = m_snakeBody.begin() + 1;
         itr != m_snakeBody.end(); ++itr)
    {
        m_bodyRect.setPosition(itr->position.x * m_size,
                               itr->position.y * m_size);
        l_window.draw(m_bodyRect);
    }
}

World::World(sf::Vector2u l_windSize)
{
    m_blockSize = 16;

    m_windowSize = l_windSize;
    RespawnApple();
    m_appleShape.setFillColor(sf::Color::Red);
    m_appleShape.setRadius(m_blockSize / 2);

    for (int i = 0; i < 4; ++i)
    {
        m_bounds[i].setFillColor(sf::Color(150, 0, 0));
        if (!((i + 1) % 2))
        {
            m_bounds[i].setSize(sf::Vector2f(m_windowSize.x, m_blockSize));
        }
        else
        {
            m_bounds[i].setSize(sf::Vector2f(m_blockSize, m_windowSize.y));
        }
        if (i < 2)
        {
            m_bounds[i].setPosition(0, 0);
        }
        else
        {
            m_bounds[i].setOrigin(m_bounds[i].getSize());
            m_bounds[i].setPosition(sf::Vector2f(m_windowSize));
        }
    }
}

World::~World() {}

void World::RespawnApple()
{
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;
    m_item = sf::Vector2i(
        rand() % maxX + 1, rand() % maxY + 1);
    m_appleShape.setPosition(
        m_item.x * m_blockSize,
        m_item.y * m_blockSize);
}

void World::Update(Snake &l_player)
{
    if (l_player.GetPosition() == m_item)
    {
        l_player.Extend();
        l_player.IncreaseScore();
        RespawnApple();
    }

    int gridSize_x = m_windowSize.x / m_blockSize;
    int gridSize_y = m_windowSize.y / m_blockSize;

    if (l_player.GetPosition().x <= 0 ||
        l_player.GetPosition().y <= 0 ||
        l_player.GetPosition().x >= gridSize_x - 1 ||
        l_player.GetPosition().y >= gridSize_y - 1)
    {
        l_player.Lose();
    }
}

void World::Render(sf::RenderWindow &l_window)
{
    for (int i = 0; i < 4; ++i)
    {
        l_window.draw(m_bounds[i]);
    }
    l_window.draw(m_appleShape);
}

int World::GetBlockSize() { return m_blockSize; }

Game::Game() : m_window(sf::VideoMode(800, 600), "Snake"),
               m_snake(m_world.GetBlockSize()), m_world(sf::Vector2u(800, 600))
{
}

void Game::HandleInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && m_snake.GetDirection() != Direction::Down)
        {
            m_snake.SetDirection(Direction::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_snake.GetDirection() != Direction::Up)
        {
            m_snake.SetDirection(Direction::Down);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && m_snake.GetDirection() != Direction::Right)
        {
            m_snake.SetDirection(Direction::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && m_snake.GetDirection() != Direction::Left)
        {
            m_snake.SetDirection(Direction::Right);
        }
    }
}


void Game::Update()
{
m_snake.Tick();
m_world.Update(m_snake);

if(m_snake.HasLost()){
    m_snake.Reset();
}
}


void Game::Render()
{
    m_window.clear();
    m_world.Render(m_window);
    m_snake.Render(m_window);
    m_window.display();
}

void Game::RestartClock()
{
    m_elapsed += m_clock.restart();
}

sf::Time Game::GetElapsed()
{
    return m_elapsed;
}

sf::RenderWindow &Game::GetWindow()
{
    return m_window;
}