#ifndef POSN_H
#define POSN_H

#include <vector>

class Posn {
    public:
        int x, y;
        Posn(int x, int y);
        Posn();
        std::vector<Posn> getNeighbours();
        bool operator==(const Posn& other) const;
};

#endif
