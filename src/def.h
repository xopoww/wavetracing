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
};


// all data about the experiment
struct scene {
    // all vertices of the scene
    std::vector<vertex> vertices;

    // acoustic densities of the obstacles (densities[i] is the density of the obstacle with the tag, densities[0] is the density of the outer medium)
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

#endif // WAVETRACING_DEF_H