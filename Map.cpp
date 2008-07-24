#include "Map.h"
#include "common.h"
#include "debug.h"

#include <algorithm>
#include <fstream>
#include <sstream>

Map map("connect.txt");

Map::Map(const char *filepath)
{
    ASSERT(filepath);

    std::string line;
    std::ifstream ifs(filepath);
    ASSERT(ifs);

    // Read  number of nodes
    VERIFY(getline(ifs, line));
    std::istringstream iss(line);
    VERIFY(iss >> m_nodes);
    ASSERT(m_nodes >= min_nodes && m_nodes <= max_nodes);

    // Read edges
    while(true)
    {
        VERIFY(std::getline(ifs, line));
        if(line == "END")
            break;
        std::istringstream iss(line);
        char mode;
        Node from, to;
        VERIFY(iss >> mode >> from >> to);
        ASSERT(from >= 1 && from <= m_nodes);
        ASSERT(-to  >= 1 && -to  <= m_nodes);
        Edge e = { from - 1, -to - 1 }, f = { -to - 1, from - 1 };
        switch(mode)
        {
        case 'C': e.mode = f.mode = car; break;
        case 'T': e.mode = f.mode = train; break;
        case 'P': e.mode = f.mode = plane; break;
        default: ASSERT(0);
        }
        edges.push_back(e);
        edges.push_back(f);
    }

    std::sort(edges.begin(), edges.end());

    // Set-up position pointers
    REP(n, max_nodes + 1) {
        Edge e = { n };
        pos[n] = std::lower_bound(edges.begin(), edges.end(), e);
    }
    ASSERT(pos[max_nodes] == edges.end());

    // Pre-calculate...
    calcDist();
    calcCenter();
}

void Map::calcDist()
{
    TRACESCOPE("calculating distances");
    REP(i, max_nodes) REP(j, max_nodes) m_dist[i][j] = (i == j ? 0 : 999);
    FOREACH(i, edges) m_dist[i->src][i->dst] = 1;
    REP(k, max_nodes) REP(i, max_nodes) REP(j, max_nodes) {
        m_dist[i][j] = std::min(m_dist[i][j], m_dist[i][k] + m_dist[k][j]);
    }
}

void Map::calcCenter()
{
    TRACESCOPE("calculating center");
    int best_max = 999, best_sum = 999999;
    center = -1;
    REP(i, m_nodes) {
        int max = 0, sum = 0;
        REP(j, m_nodes) {
            max = std::max(max, m_dist[i][j]);
            sum += m_dist[i][j];
        }
        if(max < best_max || max == best_max && sum < best_sum)
        {
            best_max = max;
            best_sum = sum;
            center = i;
        }
    }
    ASSERT(center >= 0);
    TRACE << "Center: " << 1 + center
          << "; max. dist: " << best_max
          << "; avg. dist: " << double(best_sum)/m_nodes << std::endl;
}
