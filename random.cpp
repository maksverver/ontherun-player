#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Types
enum Type { car, train, plane };
typedef short Node;

struct Edge {
    Node src, dst;
    Type type;

    bool operator==(const Edge &e) const {
        return src == e.src && dst == e.dst && type == e.type;
    }
    bool operator<(const Edge &e) const {
        return src < e.src || ( src == e.src && dst < e.dst ||
                                (dst == e.dst && type < e.type) );
    }
};

typedef std::vector<Edge> EdgeList;


// Globals
unsigned seed = 0;
const char types[] = "CTP";
static int nodes;
static EdgeList edges;
static Node cops[4], crook;


bool read_graph(const char *filepath = "connect.txt")
{
    std::ifstream ifs(filepath);
    std::string line;

    // Read  number of nodes
    if(!getline(ifs, line))
        return false;
    std::istringstream iss(line);
    if(!(iss >> nodes))
        return false;

    // Read edges
    edges.clear();
    while(std::getline(ifs, line))
    {
        if(line == "END")
            return true;
        std::istringstream iss(line);
        char type;
        Node from, to;
        if(!(iss >> type >> from >> to))
            return false;
        Edge e = { from, -to }, f = { -to, from };
        switch(type)
        {
        case 'C': e.type = f.type = car; break;
        case 'T': e.type = f.type = train; break;
        case 'P': e.type = f.type = plane; break;
        default: return false;
        }
        edges.push_back(e);
        edges.push_back(f);
    }
    std::sort(edges.begin(), edges.end());
    return false;
}

std::string read_line()
{
    std::string line;
    if(!std::getline(std::cin, line))
    {
        std::cerr << "Unexpected end of input!" << std::endl;
        std::exit(1);
    }
    if(line == "Quit")
        std::exit(0);
    return line;
}

int read_int()
{
    std::istringstream iss(read_line());
    int i;
    if(!(iss >> i))
    {
        std::cerr << "Integer expected; read: " << iss.str() << " !" << std::endl;
        std::exit(1);
    }
    return i;
}


bool place_detectives()
{
    if(nodes < 4)
    {
        std::cerr << "Not enough starting positions!" << std::endl;
        return false;
    }

    std::vector<Node> v;
    v.reserve(nodes);
    for(int n = 1; n < nodes; ++n)
        v.push_back(n);
    std::random_shuffle(v.begin(), v.end());
    for(int n = 0; n < 4; ++n)
        cops[n] = v[n];

    return true;
}

bool play_detectives()
{
    if(!place_detectives())
        return false;

    std::cout << cops[0] << '\n' << cops[1] << '\n'
              << cops[2] << '\n' << cops[3] << std::endl;

    crook = read_int();

    for(int turn = 1; turn <= 50; ++turn)
    {
        read_line(); // ignored

        std::vector<Node> moves[4];
        for(int n = 0; n < 4; ++n)
            for(EdgeList::const_iterator i = edges.begin(); i != edges.end(); ++i)
                if(i->src == cops[n])
                    moves[n].push_back(i->dst);
        do {
            for(int n = 0; n < 4; ++n)
                cops[n] = moves[n][rand()%moves[n].size()];
        } while( cops[0] == cops[1] || cops[0] == cops[2] ||
                 cops[0] == cops[3] || cops[1] == cops[2] ||
                 cops[1] == cops[3] || cops[2] == cops[3] );

        std::cout << cops[0] << '\n' << cops[1] << '\n'
                  << cops[2] << '\n' << cops[3] << std::endl;
    }
    return true;
}

bool play_fugitive()
{
    // Read cops
    for(int n = 0; n < 4; ++n)
        cops[n] = read_int();

    // Place crook, not on a cop
    do {
        crook = 1 + std::rand()%nodes;
    } while( ( crook == cops[0] || crook == cops[1] ||
            crook == cops[2] || crook == cops[3] ) && nodes > 4 );
    std::cout << crook << std::endl;

    for(int turn = 1; turn <= 50; ++turn)
    {
        // Move to a random spot, preferably not to a cop
        std::vector<Edge> v;
        for(EdgeList::const_iterator i = edges.begin(); i != edges.end(); ++i)
            if( i->src == crook && i->dst != cops[0] && i->dst != cops[1]
                                && i->dst != cops[2] && i->dst != cops[3] )
                v.push_back(*i);
        if(v.empty())
            for(EdgeList::const_iterator i = edges.begin(); i != edges.end(); ++i)
                if(i->src == crook)
                    v.push_back(*i);
        Edge e = v[rand()%v.size()];
        crook = e.dst;
        std::cout << types[e.type] << ' ' << e.dst << std::endl;

        if(turn < 50)
        {
            for(int n = 0; n < 4; ++n)
                cops[n] = read_int();
        }
    }

    return true;
}

void parse_args(int argc, char *argv[])
{
    for(int n = 0; n < argc; ++n)
    {
        if(std::strncmp(argv[n], "seed=", 5) == 0)
            seed = (unsigned)std::atoll(argv[n] + 5);
        else
            std::cerr << "Ignored argument: " << argv[n] << std::endl;
    }
}

int main(int argc, char *argv[])
{
    parse_args(argc - 1, argv + 1);

    // Seed random number generator
    if(seed == 0)
        seed = (unsigned)time(NULL);
    std::cerr << "RNG seed: " << seed << std::endl;
    std::srand(seed);

    // Read graph description
    if(!read_graph())
    {
        std::cerr << "Incorrectly formatted graph data file!" << std::endl;
        return 1;
    }

    // Read role
    std::string line;
    std::getline(std::cin, line);
    if(line == "Detectives")
    {
        return play_detectives() ? 0 : 1;
    }
    else
    if(line == "Fugitive")
    {
        return play_fugitive() ? 0 : 1;
    }
    else
    {
        std::cerr << "Expected 'Fugitive' or 'Detectives'!" << std::endl;
        return 1;
    }

    return 0;
}
