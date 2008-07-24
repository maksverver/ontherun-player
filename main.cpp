#include "debug.h"
#include "Detectives.h"
#include "Fugitive.h"
#include "Map.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

static unsigned seed;

std::string read_line()
{
    std::string line;
    VERIFY(std::getline(std::cin, line));
    if(line == "Quit")
    {
        TRACE << "Received \"Quit\"!" << std::endl;
        std::exit(0);
    }
    return line;
}

int read_int(int min, int max)
{
    std::istringstream iss(read_line());
    int i;
    VERIFY(iss >> i);
    ASSERT(i >= min && i <= max);
    return i;
}

void play_detectives()
{
    Detectives detectives;

    const Node *cops = detectives.place();
    std::cout << cops[0] + 1 << '\n' << cops[1] + 1 << '\n'
                << cops[2] + 1 << '\n' << cops[3] + 1 << std::endl;

    detectives.place(read_int(1, map.nodes()) - 1);

    for(int turn = 1; turn <= 50; ++turn)
    {
        std::istringstream iss(read_line());
        Node pos = 0;
        char c;
        VERIFY(iss >> c);
        if(turn%5 == 0)
            VERIFY(iss >> pos);
        ASSERT(c == 'C' || c == 'T' || c == 'P');
        cops = detectives.move(c == 'C' ? car : c == 'T' ? train : plane, pos - 1);

        std::cout << cops[0] + 1 << '\n' << cops[1] + 1 << '\n'
                  << cops[2] + 1 << '\n' << cops[3] + 1 << std::endl;
    }
}

void play_fugitive()
{
    Fugitive fugitive;

    for(int turn = 1; turn <= 50; ++turn)
    {
        Node cops[4];
        for(int n = 0; n < 4; ++n)
            cops[n] = read_int(1, map.nodes()) - 1;

        if(turn == 1)
            std::cout << fugitive.place(cops) + 1 << std::endl;

        static const char mode_str[] = "CTP";
        Edge e = fugitive.move(cops);
        std::cout << mode_str[e.mode] << ' ' << e.dst + 1 << std::endl;
    }
}

void parse_args(int argc, char *argv[])
{
    for(int n = 0; n < argc; ++n)
    {
        if(std::strncmp(argv[n], "seed=", 5) == 0)
            seed = (unsigned)std::atoll(argv[n] + 5);
        else
            TRACE << "Ignored argument: " << argv[n] << std::endl;
    }
}

int main(int argc, char *argv[])
{
    parse_args(argc - 1, argv + 1);

    // Seed random number generator
    if(seed == 0)
        seed = (unsigned)time(NULL);
    TRACE << "RNG seed: " << seed << std::endl;
    std::srand(seed);

    // Read role
    std::string line;
    VERIFY(getline(std::cin, line));
    ASSERT(line == "Detectives" || line == "Fugitive");

    if(line == "Detectives")
    {
        TRACE << "Playing detecives." << std::endl;
        play_detectives();
    }

    if(line == "Fugitive")
    {
        TRACE << "Playing fugitive." << std::endl;
        play_fugitive();
    }

    TRACE << "Quiting." << std::endl;

    return 0;
}
