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
    double a{};

    viewPoint() = default;
    viewPoint(const ray &l, const ray &r, double a) : l(l), r(r), a(a) {}

};

// Compute next secondary sources from the given secondary source
std::vector<viewPoint> getViewPoints(viewPoint point, const scene * const s);

/**
 * @param l - first ray
 * @param r - second ray
 * @return point of intersection of two lines obtained from these rays
 */
vec2 getCenter(const ray &l, const ray &r);

/**
 *
 * @param l - left ray
 * @param r - right ray
 * @param point  - point in coordinate of view point
 * @return if point in sector obtained rays
 */
bool inSector(const ray &l,const ray &r, const vec2 &point);

/**
 *
 * @param point
 * @param s
 * @return
 */
std::vector<vertex> getVisibilityPolygon(viewPoint point, const scene *const s);

#endif // WAVETRACING_VIEWPOINTS_H