#include <SFML/Graphics.hpp>

const sf::Keyboard::Key controls[4] = {
    sf::Keyboard::W,
    sf::Keyboard::S,
    sf::Keyboard::Up,
    sf::Keyboard::Down
};

//Parameters
const sf::Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
const float time_step = 0.017f; //60 fps

bool is_player1_serving = true;
const float initial_velocity_x = 100.f; //horizontal velocity
const float initial_velocity_y = 60.f; //vertical velocity
const float velocity_multiplier = 1.1f; //how much the ball will speed up everytime it hits a paddle. Here, 10% every time.
//Objects of the game
sf::CircleShape ball;
sf::RectangleShape paddles[2];
sf::Vector2f ball_velocity = { (is_player1_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };
void init() {
    // Set size and origin of paddles
    for (sf::RectangleShape &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius);
    // reset paddle position
    paddles[0].setPosition(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f);
    paddles[1].setPosition(gameWidth - (paddleOffsetWall + paddleSize.x / 2.f), gameHeight / 2.f);
    // reset Ball Position
    ball.setPosition(gameWidth/2, gameHeight/2);
}
void reset() {
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    ball_velocity = { (is_player1_serving ? initial_velocity_x : -initial_velocity_x), initial_velocity_y };
    is_player1_serving = !is_player1_serving;
}
void update(float dt) {
    // handle paddle movement
    float leftDirection = 0.0f;
    float rightDirection = 0.0f;
    if (sf::Keyboard::isKeyPressed(controls[0])) {
        leftDirection--;
    }
    if (sf::Keyboard::isKeyPressed(controls[1])) {
        leftDirection++;
    }
    if (sf::Keyboard::isKeyPressed(controls[2])) {
        rightDirection--;
    }
    if (sf::Keyboard::isKeyPressed(controls[3])) {
        rightDirection++;
    }
    ball.move(ball_velocity * dt);
    paddles[0].move(sf::Vector2f(0.f, leftDirection * paddleSpeed * dt));
    paddles[1].move(sf::Vector2f(0.f, rightDirection * paddleSpeed * dt));
    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by + ballRadius > gameHeight) { // bottom wall
        // bottom wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, -10.f));
    }
    else if (by - ballRadius < 0) { //top wall
        // top wall
        ball_velocity.x *= velocity_multiplier;
        ball_velocity.y *= -velocity_multiplier;
        ball.move(sf::Vector2f(0.f, 10.f));
    }
    else if (bx > gameWidth) {
        // right wall
        reset();
    }
    else if (bx < 0) {
        // left wall
        reset();
    }
    else if (
        //ball is inline or behind paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        //ball is below top edge of paddle AND
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        //ball is above bottom edge of paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5)) {
        ball_velocity.x = -ball_velocity.x * velocity_multiplier;
        ball.move(sf::Vector2f(10.f, 0.f));
            }
    else if (
        bx + ballRadius > gameWidth - (paddleSize.x + paddleOffsetWall) &&
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5f) &&
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5f)) {
        ball_velocity.x = -ball_velocity.x * velocity_multiplier;
        ball.move(sf::Vector2f(-10.f, 0.f));
    }
}

void render(sf::RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

void clean() {
    // nothing to free in this basic example
}

int main() {
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "PONG");

    //initialise and load
    init();

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        const float dt = clock.restart().asSeconds();

        window.clear();
        update(dt);
        render(window);
        //wait for the time_step to finish before displaying the next frame.
        sf::sleep(sf::seconds(time_step));
        //Wait for Vsync
        window.display();
    }
    clean();
    return 0;
}
