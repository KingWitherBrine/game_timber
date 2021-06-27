#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];
// Where is the player/branch
// Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

void updateBranches(int seed) {
    // Move all the branches down one space
    for (int j = NUM_BRANCHES-1; j > 0; j--) {
        branchPositions[j] = branchPositions[j-1];
    }
    srand((int)time(0) + seed);
    int r = rand() % 5;

    switch (r) {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }

}
    

int main() {
    // Create a VideoMode object 
    sf::VideoMode vm(1920, 1080);
    
    // Create an open window for the game
    sf::RenderWindow window(vm, "Timber!!!");

    // Create a texture to hold a graphic on the GPU
    sf::Texture bgTexture;

    // Load a graphic into the texture
    bgTexture.loadFromFile("graphics/background.png");

    // Create a sprite
    sf::Sprite bgSprite;

    // Attach the texture to the sprite
    bgSprite.setTexture(bgTexture);

    // Set the background sprite to cover the screen
    bgSprite.setPosition(0, 0);

    // Create the tree sprite
    sf::Texture treeTexture;
    treeTexture.loadFromFile("graphics/tree.png");
    sf::Sprite treeSprite;
    treeSprite.setTexture(treeTexture);
    treeSprite.setPosition(810, 0);

    // Create the bee sprite
    sf::Texture beeTexture;
    beeTexture.loadFromFile("graphics/bee.png");
    sf::Sprite beeSprite;
    beeSprite.setTexture(beeTexture);
    beeSprite.setPosition(0, 800);

    bool beeActive = false;

    float beeSpeed = 0.0f;

    // Create the three cloud sprites
    sf::Texture cloudTexture;
    cloudTexture.loadFromFile("graphics/cloud.png");
    sf::Sprite cloudSprite1;
    sf::Sprite cloudSprite2;
    sf::Sprite cloudSprite3;

    cloudSprite1.setTexture(cloudTexture);
    cloudSprite2.setTexture(cloudTexture);
    cloudSprite3.setTexture(cloudTexture);

    cloudSprite1.setPosition(0, 0);
    cloudSprite2.setPosition(0, 150);
    cloudSprite3.setPosition(0, 300);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables tp control time itself
    sf::Clock clock;

    // Time bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
    sf::Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth/timeRemaining;

    // Track whether the game is running
    bool paused = true;

    // Draw some text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;

    // We need to choose a font
    sf::Font font;
    // Original Font: "fonts/KOMIKAP.ttf"
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    
    // Sent the font to out message
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assign the actual message
    messageText.setString("Press Enter to start the game!");
    scoreText.setString("Score = 0");

    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Choose a color
    messageText.setFillColor(sf::Color::Green);
    scoreText.setFillColor(sf::Color::White);

    // Position the text
    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left +
        textRect.width / 2.0f,
        textRect.top + 
        textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare the 6 branches
    sf::Texture branchTexture;
    branchTexture.loadFromFile("graphics/branch.png");

    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(branchTexture);
        branches[i].setPosition(-2000, -2000);
        
        // Set the sprite's origin to the dead center
        // We can spin it around without changing its position
        branches[i].setOrigin(220, 20);
    }
    // Prepare the player
    sf::Texture playerTexture;
    playerTexture.loadFromFile("graphics/player.png");
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(580, 720);

    // Player starts on the left
    side playerSide = side::LEFT;
    
    // Prepare the gravestone
    sf::Texture RIPTexture;
    RIPTexture.loadFromFile("graphics/rip.png");
    sf::Sprite RIPSprite;
    RIPSprite.setTexture(RIPTexture);
    RIPSprite.setPosition(600, 860);
    
    // Prepare the axe
    sf::Texture axeTexture;
    axeTexture.loadFromFile("graphics/axe.png");
    sf::Sprite axeSprite;
    axeSprite.setTexture(axeTexture);
    axeSprite.setPosition(700, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    sf::Texture logTexture;
    logTexture.loadFromFile("graphics/log.png");
    sf::Sprite logSprite;
    logSprite.setTexture(logTexture);
    logSprite.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    while (window.isOpen()) {
        // Handle the close button
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        /*
        ********************
        Handle Player Input
        ********************
        */

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Start the game
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            paused = false;
            score = 0;
            timeRemaining = 6;
        }

        /*
        ********************
        Update the Scene
        ********************
        */

        if (!paused) {
        
            // Measure time
            sf::Time dt = clock.restart();
            
            // Subtract from the amount of time remaining;
            timeRemaining -= dt.asSeconds();

            // Resize up the time bar
            timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * 
            timeRemaining, timeBarHeight));

            if (timeRemaining <=  0.0f) {
                // Pause the game
                paused = true;

                // Change the message shown to the player 
                messageText.setString("Out of time!!!!!!!");

                // Change the color to red (not in the book, added myself)
                messageText.setFillColor(sf::Color::Red);
                
                // Reposition the text based on its new size
                sf::FloatRect textRect = messageText.getLocalBounds();
                    messageText.setOrigin(textRect.left + 
                    textRect.width / 2.0f,
                    textRect.top + 
                    textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }
            // Setup the bee
            if (!beeActive) {
                // How fast the bee is
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high the bee is
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                beeSprite.setPosition(2000, height);
                beeActive = true;
            } else {
                // Move the bee
                beeSprite.setPosition(
                    beeSprite.getPosition().x -
                    (beeSpeed * dt.asSeconds()),
                    beeSprite.getPosition().y);

                // Has the bee reached the left-hand edge of the screen?
                if (beeSprite.getPosition().x < -100) {
                    // Set it ready to be a whole new different bee in the next frame
                    beeActive = false;
                }
            }
            // Manage the clouds
            // Cloud 1
            if (!cloud1Active) {

                // How fast is the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                // How high the cloud is
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                cloudSprite1.setPosition(-200, height);
                cloud1Active = true;
            } else { 
                cloudSprite1.setPosition(
                    cloudSprite1.getPosition().x + 
                    (cloud1Speed * dt.asSeconds()),
                    cloudSprite1.getPosition().y);

                // Has the cloud reached the right-hand edge of the screen?
                if (cloudSprite1.getPosition().x > 1920) {
                    // set it up ready for a whole new cloud next frame
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active) {

                // How fast is the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // How high the cloud is
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                cloudSprite2.setPosition(-200, height);
                cloud2Active = true;
            } else {
                cloudSprite2.setPosition(
                    cloudSprite2.getPosition().x + 
                    (cloud2Speed * dt.asSeconds()),
                    cloudSprite2.getPosition().y);

                // Has the cloud reached the right-hand edge of the screen?
                if (cloudSprite2.getPosition().x > 1920) {
                    // set it up ready for a whole new cloud next frame
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active) {

                // How fast is the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // How high the cloud is
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                cloudSprite3.setPosition(-200, height);
                cloud3Active = true;
            } else {
                cloudSprite3.setPosition(
                    cloudSprite3.getPosition().x + 
                    (cloud3Speed * dt.asSeconds()),
                    cloudSprite3.getPosition().y);

                // Has the cloud reached the right-hand edge of the screen?
                if (cloudSprite3.getPosition().x > 1920) {
                    // set it up ready for a whole new cloud next frame
                    cloud3Active = false;
                }
            } 
            // Update the score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());
            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT) {
                    // Move the sprite to the left side
                    branches[i].setPosition(610, height);
                        // Flip the sprite around the other way
                        branches[i].setRotation(180);
                } else if (branchPositions[i] == side::RIGHT) {
                    // Move the sprite to the right side
                    branches[i].setPosition(1300, height);
                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                } else
                {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }

            }
        } // End if (!paused)
        /*
        ********************
        Draw the scene
        ********************
        */

        window.clear();
        window.draw(bgSprite);
        window.draw(cloudSprite1);
        window.draw(cloudSprite2);
        window.draw(cloudSprite3);
        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }
        window.draw(treeSprite);
        window.draw(beeSprite);

        // Draw the player
        window.draw(playerSprite);

        // Draw the axe
        window.draw(axeSprite);

        // Draw the flying log
        window.draw(logSprite);
        
        // Draw the gravestone
        window.draw(RIPSprite);

        // Draw the score
        window.draw(scoreText);
        if (paused) {
            // Draw the message
            window.draw(messageText);
        }
        window.draw(timeBar);
        window.display();
    }
    return 0;
}