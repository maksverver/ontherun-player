#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

// Common header files
#include <iostream>
#include "debug.h"
#include "Map.h"

// Useful macro's
#define FOR(i,a,b) for(int i = (a); i < int(b); ++i)
#define REP(i,n) FOR(i, 0, n)
#define FORIN(i,a,b) for(__typeof(a) i = (a), _##i##_end = (b); i != _##i##_end; ++i)
#define FOREACH(i,v) FORIN(i, (v).begin(), (v).end())
#define SZ(x) (int((x).size()))
#define ASSIGN(dst,src) do { REP(n,sizeof(dst)/sizeof(*dst)) dst[n] = src[n]; } while(0);

// Common functions
#include <set>
std::set<Node> update_hot(const std::set<Node> &hot, Mode mode, const Node *cops);
int summed_dist(const std::set<Node> &hot, const Node *cops);
int weighted_summed_dist(const std::set<Node> &hot, const Node *cops);

#endif /* ndef COMMON_H_INCLUDED */
