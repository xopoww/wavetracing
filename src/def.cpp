#include "def.h"

std::ostream &operator<<(std::ostream &os, const vec2 &v) {
    os<<'{'<<v.x<<','<<v.y<<"} ";
    return os;
}

std::ostream &operator<<(std::ostream &os, const ray &r) {
    os<<"pos:"<<r.pos<<" dir:"<<r.dir<<' ';
    return os;
}
