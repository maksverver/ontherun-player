#ifndef FUGITIVE_H_INCLUDED
#define FUGITIVE_H_INCLUDED

#include "Map.h"
#include <set>

class Fugitive
{
public:
    Fugitive();

    int place(const Node *detectives);
    Edge move(const Node *detectives);

private:
    int invisible;
    Node crook;
    std::set<Node> hot;
};

#endif /* ndef FUGITIVE_H_INCLUDED */
