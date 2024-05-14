#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameEngine.h"

int main()
{
    GameEngine g("Assets.txt");
    g.run();

    return 0;
}
