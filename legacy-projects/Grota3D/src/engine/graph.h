#ifndef GROTA_GRAPH_H
#define GROTA_GRAPH_H
#include "core/mathematics.h"
#include "core/list.h"

#define GRAPH_RENDERER_REFERENCE

typedef struct GraphComponent{
    int type;
    void *component;
}GraphComponent;

typedef struct GraphNode GraphNode;
struct GraphNode {
    Transform transform;
    List *childs;
    List *components;
};
#define GRAPHNODE_NEW  {TRANSFORM_NEW,NULL,NULL}
static const graphnode_new=GRAPHNODE_NEW;

GraphNode *graph_alloc();
GraphNode *graph_destroy(GraphNode *node);
#endif