#include "Fugitive.h"
#include "common.h"
#include <algorithm>
#include <iostream>

Fugitive::Fugitive()
{
}

int Fugitive::place(const Node *cops)
{
    TRACESCOPE("Placing fugitive");

    invisible = 4;
    crook = -1;

    // Pick starting position
    int best_min = -1, best_degree = -1;
    REP(n, map.nodes())
    {
        int min = 999;
        REP(c, 4) min = std::min(min, map.dist(cops[c], n));

        int degree = 0;
        FORIN(i, map.begin(n), map.end(n)) if(i->mode == car) ++degree;

        if(min > best_min || (min == best_min && degree > best_degree))
        {
            best_min    = min;
            best_degree = degree;
            crook       = n; 
        }
    }
    ASSERT(crook >= 0);

    TRACE << "Best min: " << best_min << "; car degree: " << best_degree << std::endl;

    hot.insert(crook);
    return crook;
}

Edge Fugitive::move(const Node *cops)
{
    TRACESCOPE("Moving fugitive");

    Edge move = { -1 };
    std::set<Node> new_hot[3];
    int new_sum[3];

    REP(mode, 3) {
        new_hot[Mode(mode)] = update_hot(hot, Mode(mode), cops);
        new_sum[Mode(mode)] = summed_dist(new_hot[Mode(mode)], cops);
    }

    int best_min = -1, best_sum = -1;
    FORIN(i, map.begin(crook), map.end(crook))
    {
        int min = 999;
        REP(c, 4) {
            int dist = map.dist(cops[c], i->dst);
            min = std::min(min, dist);
        }

        int sum;
        if(invisible)
            sum = new_sum[i->mode];
        else
        {
            sum = 0;
            REP(c, 4) sum += map.dist(cops[c], i->dst);
        }

        if(min > best_min || (min == best_min && sum > best_sum))
        {
            best_min = min;
            best_sum = sum;
            move = *i;
        }
    }

    TRACE << "Best min: " << best_min << "; best sum: " << best_sum << std::endl;

    // Update state & return selected move
    ASSERT(move.src == crook);
    crook     = move.dst;
    hot       = new_hot[move.mode];
    invisible = (invisible ? invisible - 1 : 4);
    return move;
}
