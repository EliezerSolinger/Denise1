#include "resources.h"
#include "graphics/graphics.h"
#include <stdlib.h>
#include <string.h>

GTResource* resource_new(const char *filepath) {
    GTResource* res=calloc(1,sizeof(GTResource));
    strcpy_s(res->filepath,64,filepath);
    return res;
}

void resource_load(GTResource* this) {
    if(this->loaded) return;
    if(this->type==TextureResource) {
       // printf("cagando %s\n",this->filepath);
        this->pointer=load_texture(this->filepath,false);
        this->loaded=this->pointer!=NULL;
    }
}

void resource_unload(GTResource* this) {
    if(!this->loaded)return;
    if(this->type==TextureResource) {
       texture_destroy(this->pointer);
    }
    this->pointer=NULL;
    this->loaded=false;
}

void resource_destroy(GTResource *this) {
    resource_unload(this);
    free(this);
}

GTResource* resource_get_from_list(List* list,const char * filepath) {
    List *i=list;
    while(i) {
        char* res_filepath=((GTResource*)i->pointer)->filepath;
        if(strcmp(filepath,res_filepath)==0) {
            return (GTResource*)i->pointer;
        }
        i=list_iterate(i);
    }
    return NULL;
}

void free_resource_list(List* list) {
    List *i=list;
    while(i) {
        resource_unload(i->pointer);
        resource_destroy(i->pointer);
        i=list_iterate(i);
    }
    list_destroy(list);
}