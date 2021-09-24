#ifndef GROTA_HEIGHTMAP_H
#define GROTA_HEIGHTMAP_H
#include "core/mathematics.h"
#include "graphics/3d/renderer.h"

typedef struct HeightMap {
    int width;
    int height;
    unsigned char *data;
} HeightMap; 


MeshData *htmap_generate_mesh(HeightMap *map,Rect portion,int resolution);
HeightMap *htmap_load(const char *filename);
void htmap_destroy(HeightMap *htmap);

#endif