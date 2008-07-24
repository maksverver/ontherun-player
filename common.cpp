#include "common.h"

std::set<Node> update_hot(const std::set<Node> &hot, Mode mode, const Node *cops)
{
    std::set<Node> new_hot;
    bool cop[200] = { };
    cop[cops[0]] = cop[cops[1]] = cop[cops[2]] = cop[cops[3]] = true;
    FOREACH(i, map) {
        if( i->mode == mode && !(cop[i->src] || cop[i->dst]) &&
            hot.find(i->src) != hot.end() )
        {
            new_hot.insert(i->dst);
        }
    }
    return new_hot;
}

int summed_dist(const std::set<Node> &hot, const Node *cops)
{
    int sum = 0;
    FOREACH(i, hot) {
        sum += map.dist(*i, cops[0]);
        sum += map.dist(*i, cops[1]);
        sum += map.dist(*i, cops[2]);
        sum += map.dist(*i, cops[3]);
    }
    return sum;
}

int weighted_summed_dist(const std::set<Node> &hot, const Node *cops)
{
    int sum = 0;
    FOREACH(i, hot) {
        int w = map.dist(*i, cops[0]) + map.dist(*i, cops[1])
              + map.dist(*i, cops[2]) + map.dist(*i, cops[3]);
        sum += (10 - map.layer(*i))*w;
    }
    return sum;
}
