#include "viewpoints.h"
#include <cmath>
#include <bits/stdc++.h>

typedef std::pair<coord_t,vertex> aVer;

const coord_t EPSILON = 0;

coord_t cross(const vec2 &a, const vec2 &b) {
    return a.x * b.y - a.y * b.x;
}

bool isParallel(const ray &l, const ray &r) {
    coord_t d = cross(l.dir, r.dir);
    return std::abs(d) <= EPSILON;
}

bool isZero(const vec2 &v) {
    return std::abs(v.x) <= EPSILON && std::abs(v.y) <= EPSILON;
}

tag_t getNext(int i, int size) {
    return i == size - 1 ? 0 : i + 1;
}

tag_t getPrev(int i, int size) {
    return i == 0 ? size - 1 : i - 1;
}

bool inSector(const ray &l, const ray &r, const vec2 &point) {
    coord_t c1 = cross(r.dir, point - r.pos);
    coord_t c2 = cross(point - l.pos, l.dir);
    if (c2 >= 0 && c1 >= 0) {
        vec2 v = l.pos - r.pos;
        if (isZero(v))return true;
        else return cross(point - r.pos, v) >= 0;
    } else return false;
}

vec2 intersect(const ray &r, vec2 p1, vec2 p2) {
    p1 -= r.pos;
    p2 -= r.pos;
    const coord_t k1 = cross(p1,r.dir);
    const coord_t k2 = cross(r.dir,p2);
    if (k2==0)return p2 + r.pos;
    const coord_t x = std::abs(k1/k2);
    vec2 v{p1.x + x*p2.x,p1.y + x*p2.y};
    v*=(1/(1+x));
    return v+r.pos;
}

vec2 getCenter(const ray &l, const ray &r) {
    const coord_t t = cross(r.pos - l.pos, r.dir) / cross(l.dir, r.dir);
    return l.pos + t * l.dir;
}

bool compare(const aVer& v1,const aVer & v2){
    if (v1.first == v2.first)
        return glm::dot(v1.second.pos,v1.second.pos)<glm::dot(v2.second.pos,v2.second.pos);
    return v1.first < v2.first;
}

void addLeft(const aVer v, std::vector<aVer>& leftVertices){
    if (leftVertices[v.second.tag].first == -100. || cross(leftVertices[v.second.tag].second.pos, v.second.pos) > 0){
        leftVertices[v.second.tag] = v;
    }
}

void addRight(const aVer v, std::vector<aVer>& leftVertices){
    if (leftVertices[v.second.tag].first == -100. || cross(leftVertices[v.second.tag].second.pos, v.second.pos) < 0){
        leftVertices[v.second.tag] = v;
    }
}

bool equalVertex(const vertex &v1,const vertex &v2){
    return v1.tag== v2.tag && v1.order == v2.order;
}

void getSortedPolygon(viewPoint point, const scene *const s, std::vector<aVer>& sortedPoly,std::vector<aVer>& leftVertices,std::vector<aVer>& rightVertices, const vec2 &center){
    sortedPoly.reserve(s->numVertices / 2);
    leftVertices.resize(s->vertices.size());
    rightVertices.resize(s->vertices.size());
    for (int i=0;i<leftVertices.size();i++){
        leftVertices[i].first = -100;
        rightVertices[i].first = -100;
    }

    if (isParallel(point.l, point.r)) {
        ///TODO if rays are parallel
    }

    //add in visibility poly all "visible" vertices
    //visible vertex - vertex in sector and one or two segments with this point are rotated to viewPoint
    for (int i = 0; i < s->vertices.size(); i++) {
        const auto &polygon = s->vertices[i];
        const auto size = polygon.size();
        for (int j = 0; j < size; j++) {
            tag_t next = getNext(j,size);
            aVer nextVer = {0,polygon[next]};
            vec2 z = polygon[next].pos - center;
            if (glm::dot(s->normals[i][j],z) <= 0){
                //is visible segment
                bool visible1 = inSector(point.l, point.r, polygon[j].pos);
                bool visible2 = inSector(point.l, point.r, polygon[next].pos);

                if (!visible1 && visible2){
                    vec2 tmp = intersect(point.l,polygon[j].pos,polygon[next].pos) - center;
                    aVer v {0,vertex{tmp, s->normals[i][j], nextVer.second.tag, polygon[j].order}};
                    v.first= atan2(tmp.y, tmp.x);
                    sortedPoly.push_back(v);
                    addLeft(v, leftVertices);
                    addRight(v,rightVertices);

                    nextVer.second.pos-=center;
                    nextVer.first = atan2(nextVer.second.pos.y, nextVer.second.pos.x);
                    sortedPoly.push_back(nextVer);
                    addLeft(nextVer, leftVertices);
                    addRight(v,rightVertices);
                }else if (visible1 && visible2){
                    nextVer.second.pos-=center;
                    nextVer.first = atan2(nextVer.second.pos.y, nextVer.second.pos.x);
                    sortedPoly.push_back(nextVer);
                    addLeft(nextVer, leftVertices);

                    addRight(nextVer,rightVertices);
                }else if (visible1){
                    vec2 tmp = intersect(point.r,polygon[j].pos,polygon[next].pos)-center;
                    aVer v {0,vertex{tmp, s->normals[i][j], nextVer.second.tag, nextVer.second.order}};
                    v.first = atan2(tmp.y, tmp.x);
                    sortedPoly.push_back(v);
                    addLeft(v, leftVertices);

                    addRight(v,rightVertices);
                }
            }else if (glm::dot(s->normals[i][next],z) <= 0 && inSector(point.l, point.r, polygon[next].pos)) {
                nextVer.second.pos-=center;
                nextVer.first = atan2(nextVer.second.pos.y, nextVer.second.pos.x);
                sortedPoly.push_back(nextVer);
                addLeft(nextVer, leftVertices);

                addRight(nextVer,rightVertices);

            }
        }
    }
//    auto begin = sortedPoly.begin();
//    while (true){
//        auto n = begin+1;
//        if(n==sortedPoly.end())
//            break;
//        while ((n+1)!= sortedPoly.end() && (n+1)->second.tag == begin->second.tag){
//            n++;
//        }
//        std::sort(begin, n+1, compare);
//        if((n+1)==sortedPoly.end())
//            break;
//        begin = n+1;
//    }
    //sorting all vertices by angle
    ///TODO fast method of sort without calculate angles and delete "angle" from vertex struct
    std::sort(sortedPoly.begin(), sortedPoly.end(), compare);
}

//все полигоны выпуклые!!!
std::vector<vertex> getVisibilityPolygon(viewPoint point, const scene *const s) {
    std::vector<aVer> sortedPoly;
    std::vector<aVer> leftVertices;
    std::vector<aVer> rightVertices;

    vec2 center = getCenter(point.l, point.r);

    getSortedPolygon(point,s,sortedPoly,leftVertices,rightVertices,center);

    const auto size = sortedPoly.size();

    std::vector<vertex> res;

    res.reserve(sortedPoly.size());
    //res.push_back(sortedPoly.front().second);

    for (auto v : sortedPoly){
        v.second.pos+=center;
        res.push_back(v.second);
    }

//    for (int i = 0;i<sortedPoly.size();i++){
//        int start = i;
//        int end = i;
//        while(sortedPoly[getNext(end,sortedPoly.size())].second.tag == sortedPoly[end].second.tag){
//            end = getNext(end,sortedPoly.size());
//        }
//        //для каждой вершины
//        for (int j = start;j< end;j++){
//            coord_t r = glm::length(sortedPoly[j].second.pos - center);
//            //coord_t angle = atan2();
//        }
//    }


    return res;
}

std::vector<viewPoint> getViewPoints(viewPoint point, const scene *const s) {
    ///step 1 - visibility polygon
    std::vector<vertex> vPolygon = getVisibilityPolygon(point, s);

    ///step 2 - view points
    std::vector<viewPoint> viewPoints;
    viewPoints.reserve(vPolygon.size() / 2);

    for (int i = 0; i < vPolygon.size()-1; i++) {
        tag_t next = getNext(i, vPolygon.size());
        if (vPolygon[next].tag == vPolygon[i].tag) {
            coord_t c = cross(vPolygon[i].nor, vPolygon[next].nor);
            ray r;
            ray l;
            if (c < 0) {

                l.dir = vPolygon[i].nor;
                l.pos = vPolygon[i].pos;

                r.dir = vPolygon[next].nor;
                r.pos = vPolygon[next].pos;
            } else if (c > 0) {

                r.dir = vPolygon[i].nor;
                r.pos = vPolygon[i].pos;

                l.dir = vPolygon[next].nor;
                l.pos = vPolygon[next].pos;
            } else {
                ///TODO it's impossible but check it
            }
            viewPoints.emplace_back(l, r, 1.);
        }
    }
    return viewPoints;
}