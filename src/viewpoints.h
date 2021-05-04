#ifndef WAVETRACING_VIEWPOINTS_H
#define WAVETRACING_VIEWPOINTS_H

#include "def.h"


// a secondary source of waves
struct viewPoint{
    // left boundary of the angle this source covers
    ray l;
    // right boundary of the angle this source covers
    ray r;
    // the amplitude this source has
    double a;
};

// Compute next secondary sources from the given secondary source
std::vector<viewPoint> getViewPoints(viewPoint point, const scene * const s);


#endif // WAVETRACING_VIEWPOINTS_H