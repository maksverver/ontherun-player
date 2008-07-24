#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <vector>

enum Mode { car, train, plane };
typedef short Node;

struct Edge {
    Node src, dst;
    Mode mode;

    bool operator==(const Edge &e) const {
        return src == e.src && dst == e.dst && mode == e.mode;
    }
    bool operator<(const Edge &e) const {
        return src < e.src || ( src == e.src && dst < e.dst ||
                                (dst == e.dst && mode < e.mode) );
    }
};

typedef std::vector<Edge> EdgeList;

class Map
{
public:
    static const int min_nodes = 5, max_nodes = 200;

    Map(const char *filepath);

    inline int nodes() { return m_nodes; }
    inline EdgeList::const_iterator &begin(int node = 0) { return pos[node]; }
    inline EdgeList::const_iterator &end(int node = max_nodes - 1) { return pos[node + 1]; }

    inline int dist (int i, int j) { return m_dist[i][j]; }
    inline int layer(int i) { return m_dist[i][center]; }

private:
    void calcDist();
    void calcCenter();

    EdgeList edges;
    EdgeList::const_iterator pos[max_nodes + 1];

    int m_nodes;
    int m_dist[max_nodes][max_nodes];
    int center;
};

extern Map map;

#endif /* ndef MAP_H_INCLUDED */
