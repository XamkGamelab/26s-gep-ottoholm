#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "game.hpp"

int main(int argc, char** argv)
{
    gep::game g{};
    if (g.init())
    {
        g.run();
    }

    g.shutdown();
    return 0;
}
