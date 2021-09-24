#ifndef GROTA_UIXML_H
#define GROTA_UIXML_H

#include "core/list.h"
#define LISTNODE 0
#define RECTNODE 1

typedef struct _UIXmlIdentifier {
    void *pointer;
    char id[64];
} UIXmlIdentifier;

typedef struct _UIXml {
    List *identifiers;
    List *resources;
    List *DOM;
} UIXml;

UIXml* uixml_load(const char* filename);

void uixml_draw(UIXml* this);

void *uixml_getbyid(UIXml* this,const char* id);

void uixml_destroy(UIXml* this);

#endif // UIXML_H