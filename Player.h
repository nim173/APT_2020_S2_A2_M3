#ifndef PLAYER
#define PLAYER

#include <string>

using std::string;

class Player {
public:
    Player(string name);
    ~Player();

private:
    string name;

    // Player Mosaic
    // Keep track of score
    // ...
};

#endif // Player