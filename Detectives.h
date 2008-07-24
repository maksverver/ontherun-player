#ifndef DETECTIVES_H_INCLUDED
#define DETECTIVES_H_INCLUDED

#include <set>
#include "Map.h"

class Detectives
{
public:
    Detectives();

    const Node *place();
    void place(Node crook);
    const Node *move(Mode mode, Node dest = -1);

private:
    Node cops[4];
    std::set<Node> hot;
};

#endif /* ndef DETECTIVES_H_INCLUDED */
