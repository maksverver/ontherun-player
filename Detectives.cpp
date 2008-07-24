#include "Detectives.h"
#include "common.h"
#include <algorithm>
#include <vector>

Detectives::Detectives()
{
}

const Node *Detectives::place()
{
    TRACESCOPE("Placing detectives");

    // Select train stations
    std::set<int> tmp; 
    FOREACH(i, map) if(i->mode == train) tmp.insert(i->src);
    std::vector<int> v(tmp.begin(), tmp.end());
    const int N = SZ(v);

    int best_max = 999999, best_sum = 999999;
    REP(a, N) FOR(b, a + 1, N) FOR(c, b + 1, N) FOR(d, c + 1, N) {
        int max = 0, sum = 0;
        REP(n, map.nodes()) {
            int dist = std::min(
                std::min(map.dist(n, v[a]), map.dist(n, v[b])),
                std::min(map.dist(n, v[c]), map.dist(n, v[d])) );
            max = std::max(max, dist);
            sum += dist;
        }
        if(max < best_max || (max == best_max && sum < best_sum))
        {
            best_max = max;
            best_sum = sum;
            cops[0] = v[a], cops[1] = v[b], cops[2] = v[c], cops[3] = v[d];
        }
    }
    TRACE << "Best max dist: " << best_max
              << "; avg dist: " << double(best_sum)/map.nodes() << std::endl;
    return cops;
}

void Detectives::place(Node crook)
{
    hot.insert(crook);
}

const Node *Detectives::move(Mode mode, Node dest)
{
    TRACESCOPE("Moving detectives");

    // Update hot sites
    if(dest >= 0)
    {
        hot.clear();
        hot.insert(dest);
    }
    else
    {
        hot = update_hot(hot, mode, cops);
    }

    Node new_cops[4];
    int best_sum = 999999;
    FORIN(i, map.begin(cops[0]), map.end(cops[0])) {
        FORIN(j, map.begin(cops[1]), map.end(cops[1])) {
            if(i->dst == j->dst)
                continue;
            FORIN(k, map.begin(cops[2]), map.end(cops[2])) {
                if(i->dst == k->dst || j->dst == k->dst)
                    continue;
                FORIN(l, map.begin(cops[3]), map.end(cops[3])) {
                    if(i->dst == l->dst || j->dst == l->dst || k->dst == l->dst)
                        continue;

                    Node cur[4] = { i->dst, j->dst, k->dst, l->dst };

                    int sum = weighted_summed_dist(hot, cur);
                    if(sum < best_sum)
                    {
                        best_sum = sum;
                        ASSIGN(new_cops, cur);
                    }
                }
            }
        }
    }

    TRACE << "Sum: " << best_sum
              << "; avg dist: " << double(best_sum)/SZ(hot)/4
              << "; hot size: " << SZ(hot) << std::endl;

    ASSIGN(cops, new_cops);
    return cops;
}
