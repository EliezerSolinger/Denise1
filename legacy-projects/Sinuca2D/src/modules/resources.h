#ifndef GROTA_RESOURCES_H
#define GROTA_RESOURCES_H
#include "core/list.h"
#include <stdbool.h>

typedef enum _ResourceType {
    TextureResource,
    AudioResource,
    ImageResource
} ResourceType;

typedef struct _GTResource {
    char filepath[64];
    ResourceType type;
    bool loaded;
    void* pointer;
} GTResource;

GTResource* resource_new(const char *filepath);

void resource_load(GTResource* this);

void resource_unload(GTResource* this);

void resource_destroy(GTResource *this);
GTResource* resource_get_from_list(List* list,const char * filepath);

void free_resource_list(List* list);
#endif