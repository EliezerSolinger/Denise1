#include "stb_image/stb_image.h"
#include "core/mathematics.h"
#include "graphics/3d/renderer.h"
#include "heightmap.h"


HeightMap *htmap_load(const char *filename) {
    printf("carregando imagem heightmap %s\n",filename);
    HeightMap *map=malloc(sizeof(HeightMap));
    int channels_in_file=0;
    map->data=stbi_load(filename,&(map->width),&(map->height),&channels_in_file,1);
    printf("imagem carregada!\n");
    return map;
}
static float get_height(HeightMap *map,Vec2 pos) {
    
    int x=(int)(pos.x*((float)(map->width)));
    int y=(int)((1.0f-pos.y)*((float)(map->height)));
    if(x<0) x*=-1;
    if(y<0) x*=-1;
    if(x>map->width) x=x%map->width;
    if(y>map->height) y=y%map->height;
  
    return (1.0f/256.0f)* (float) (map->data[(y*map->width)+(x)]);
}
static Vec3 generate_ht_vertex(HeightMap *map,Vec2 vt) {
    return vec3_create(vt.x,get_height(map,vt),vt.y);
}
MeshData *htmap_generate_mesh(HeightMap *map,Rect portion,int resolution) {
    if(!map) {
        printf("impossivel gerar mesh de um mapa nulo!\n");
        return NULL;
    }
    if(resolution==0 || resolution%2!=0) {
        printf("Impossivel obter mesh com a resolucao impar ou zero\n");
        return NULL;
    }
    printf("alocando mesh!\n");
    MeshData *mesh=malloc(sizeof(MeshData));
    mesh->vertices_count=resolution*resolution*6;
    mesh->vertices=malloc(mesh->vertices_count*sizeof(VertexData));

    printf("calculando fraction.\n");
    float fraction=(1.0f/(float)resolution);
    float fx=fraction*portion.size.x;
    float fy=fraction*portion.size.y;

    printf("iniciando varredura geometrica.\n");
    for(int x=0;x<resolution;x++) {
        for(int y=0;y<resolution;y++) {
            Vec2 t=vec2_add(portion.position,vec2_create(x*fraction,y*fraction));
            //printf("desenhando triangulo (%d,%d)=(%f,%f)\n",x,y,t.x,t.y);
            VertexData *vertex=&(mesh->vertices[(y*resolution+x)*6]);
            vertex[5].normal=vec3_create(0,1,0);
            vertex[5].texture_coordinates=vec2_add(portion.position,vec2_create(x*fraction,y*fraction));
            vertex[5].vertex=generate_ht_vertex(map,vertex[5].texture_coordinates);
            
            vertex[4].normal=vec3_create(0,1,0);
            vertex[4].texture_coordinates=vec2_add(portion.position,vec2_create((x+1)*fraction,y*fraction));
            vertex[4].vertex=generate_ht_vertex(map,vertex[4].texture_coordinates);


            vertex[3].normal=vec3_create(0,1,0);
            vertex[3].texture_coordinates=vec2_add(portion.position,vec2_create((x+1)*fraction,(y+1)*fraction));
            vertex[3].vertex=generate_ht_vertex(map,vertex[3].texture_coordinates);

            /////////////////////

            vertex[2].normal=vec3_create(0,1,0);
            vertex[2].texture_coordinates=vec2_add(portion.position,vec2_create(x*fraction,y*fraction));
            vertex[2].vertex=generate_ht_vertex(map,vertex[2].texture_coordinates);

            vertex[1].normal=vec3_create(0,1,0);
            vertex[1].texture_coordinates=vec2_add(portion.position,vec2_create((x+1)*fraction,(y+1)*fraction));
            vertex[1].vertex=generate_ht_vertex(map,vertex[1].texture_coordinates);

            vertex[0].normal=vec3_create(0,1,0);
            vertex[0].texture_coordinates=vec2_add(portion.position,vec2_create(x*fraction,(y+1)*fraction));
            vertex[0].vertex=generate_ht_vertex(map,vertex[0].texture_coordinates);

        }   
    }
    return mesh;
}
void htmap_destroy(HeightMap *htmap) {
    free(htmap->data);
    free(htmap);
}