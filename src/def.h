#ifndef WAVETRACING_DEF_H
#define WAVETRACING_DEF_H

//
// Typedefs and useful struct definitions for the project
//

#include <glm/glm.hpp>  // glm::vec
#include <vector>       // c++ arrays
#include <iostream>     // std::istream for reading scene from file 


// coordinates, angles, etc
typedef long double coord_t;

// tags of vertices, etc
typedef unsigned short tag_t;

// 2d vector
typedef glm::vec<2, coord_t, glm::defaultp> vec2;


// ray (vector with fixed origin)
struct ray {
    vec2 pos;
    vec2 dir;

    ray():pos{0,0},dir{0,0}{}
    ray(const vec2 &pos, const vec2 &dir) : pos(pos), dir(dir) {}

};


// single vertex of an obstacle
struct vertex {

    // position
    vec2 pos;

    // normal to the surface of the obstacle (facing outwards)
    vec2 nor;

    // unique tag of the obstacle this vertex belongs to
    tag_t tag;
    
    // unique (among other vertices of the obstacle) ordinal number of the vertex
    tag_t order;

    vertex():pos{0,0}, nor{0,0},tag(0),order(0) {}
    vertex(const vec2 &pos, const vec2 &nor, tag_t tag, tag_t order) : pos(pos), nor(nor), tag(tag), order(order) {}

};


// all data about the experiment
struct scene {
    //vector of polygons, polygon - vector of vertices
    std::vector<std::vector<vertex>> vertices;

    //total num of vertices in scene
    tag_t numVertices;

    //vector of polygons with all their normal vectors for segments between points
    //normals[0][0] is normal for first poly for segment between 0 and 1 vertex
    std::vector<std::vector<vec2>> normals;

    // acoustic densities of the obstacles
    //(densities[i] is the density of the obstacle with the tag, densities[0] is the density of the outer medium)
    std::vector<coord_t> densities;

    // the location of the source of the waves
    vec2 source;

    // the location of the sensor (a line beween sensorA and sensorB)
    vec2 sensorA;
    vec2 sensorB;

    // the original amplitude of the waves
    coord_t start_amp;

    // the waves with the amplitude less than min_amp are discarded from further simulation
    coord_t min_amp;
};

// Load scene from input stream
// TODO: figure out serialization method
scene * load_scene(std::istream &is);


std::ostream &operator<<(std::ostream &os, const vec2 &v);

std::ostream &operator<<(std::ostream &os, const ray &r);

#endif // WAVETRACING_DEF_H