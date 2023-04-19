#pragma once


struct Star {
    int x;
    int y;
    double depth;
    double speed;
    double angle;
};
std::vector<Star> stars;

#define SCROLLING_STARFIELD 1

const int NUM_STARS = 200;
const int STAR_SIZE = 2;
const int MAX_STAR_SPEED = 2;
const int MAX_STAR_ANGLE = 360;
const double MAX_STAR_DEPTH = 3.0;
const double MAX_STAR_DEPTH_SPEED = 0.01;



// Generate random stars
void generateStars();

// Update star positions
void updateStars(float deltaTime);


// Generate random stars
void generateStars() {
#if SCROLLING_STARFIELD
    stars.clear();
    std::srand((unsigned int)std::time(0));
    for (int i = 0; i < NUM_STARS; ++i) {
        Star star;
        star.x = std::rand() % game_screen_width;
        star.y = std::rand() % game_screen_height;
        star.speed = std::rand() % MAX_STAR_SPEED + 1;
        stars.push_back(star);
    }
#else if
    stars.clear();
    std::srand((unsigned int)std::time(0));
    for (int i = 0; i < NUM_STARS; ++i) {
        Star star;
        star.x = std::rand() % game_screen_width;
        star.y = std::rand() % game_screen_height;
        star.depth = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX) * MAX_STAR_DEPTH;
        star.speed = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX) * MAX_STAR_SPEED + 1;
        star.angle = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX) * MAX_STAR_ANGLE;
        stars.push_back(star);
    }
#endif
}

// Update star positions
void updateStars(float deltaTime) {
#if SCROLLING_STARFIELD
    for (Star& star : stars) {
        star.y += star.speed;
        if (star.y >= game_screen_height) {
            star.y = 0;
        }
    }
#else if
    for (Star& star : stars) {
        // Update star position based on velocity and deltaTime
        star.x += star.velocity.x * deltaTime;
        star.y += star.velocity.y * deltaTime;
        star.z += star.velocity.z * deltaTime;

        // Wrap stars around screen edges
        if (star.x < 0) {
            star.x = game_screen_width;
        }
        if (star.y < 0) {
            star.y = game_screen_height;
        }

        // Update star size based on depth
        int starSize = static_cast<int>(STAR_SIZE * (MAX_STAR_DEPTH - star.z + 1));
        star.size = starSize;
    }
#endif
}

