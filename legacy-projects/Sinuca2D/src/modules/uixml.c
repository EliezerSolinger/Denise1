#include "resources.h"
#include "uixml.h"
#include "ezxml/ezxml.h"
#include "stdlib.h"
#include "graphics/ui/ui.h"

static inline float parsevalue(const char *str) {
    return atof(str)/100.0f;
}




UIRect *get_rect_from_xmlnode(ezxml_t node,UIXml *this) {
    UIRect *new_rect=ui_rect_alloc();
    const char *value;
    
    if(value= ezxml_attr(node, "id")) {
        UIXmlIdentifier* identifier=calloc(1,sizeof(UIXmlIdentifier));
        strcpy_s(identifier->id,sizeof(identifier->id),value);
        identifier->pointer=new_rect;
        this->identifiers=list_add(this->identifiers,identifier);
        value=0;
    }
 
    if(value= ezxml_attr(node, "vertical")) {
        if(strcmp(value,"top")==0) {
            new_rect->corner_y=Ytop;
        }else  if(strcmp(value,"bottom")==0) {
            new_rect->corner_y=Ybottom;
        }
    }
    
    if(value= ezxml_attr(node, "horizontal")) {
        if(strcmp(value,"left")==0) {
            new_rect->corner_x=Xleft;
        }else  if(strcmp(value,"right")==0) {
            new_rect->corner_y=Xright;
        }
    }

    if(value= ezxml_attr(node, "bgimage")) {
        GTResource * res=NULL;
        // se nao achar cria uma resource nova
        if(!(res=resource_get_from_list(this->resources,value))) {
            res=resource_new(value);
            res->type=TextureResource;
            
            resource_load(res);
            this->resources=list_add(this->resources,res);
        }
        new_rect->texture_id=res->pointer;
        value=0;
    }
    ezxml_t txt=ezxml_child(node, "text");
    if(txt) {
        printf("criando txt: %s\n",txt->txt);
        UITextBox* textbox=ui_textbox_alloc();

        if(value= ezxml_attr(txt, "color")) {
            textbox->color=color4f_hexstr(value);value=0;
        }
        if(value= ezxml_attr(txt, "font")) {
            GTResource * res=NULL;
            // se nao achar cria uma resource nova
            if(!(res=resource_get_from_list(this->resources,value))) {
                res=resource_new(value);
                res->type=TextureResource;
                
                resource_load(res);
                this->resources=list_add(this->resources,res);
            }
            textbox->texture_font_id=(res->pointer);
            value=0;
        }

        if(value= ezxml_attr(txt, "size")) {
            textbox->font_size=vec2_scalar_mult(textbox->font_size,parsevalue(value));
            value=0;
        }
        ui_textbox_setText(textbox,txt->txt);
        new_rect->textbox=textbox;
    }
    for (ezxml_t chnode = ezxml_child(node, "rect"); chnode!=NULL; chnode = chnode->next) {
        new_rect->child=list_add(new_rect->child,get_rect_from_xmlnode(chnode,this));
    }
    return new_rect;
}

UIXml* uixml_load(const char* filename) {
    UIXml* this=calloc(1,sizeof(UIXml));

    ezxml_t root = ezxml_parse_file(filename);

    const char *attr;
    for (ezxml_t node = root->child; node!=NULL; node = node->ordered) {
        if(strcasecmp(node->name,"rect")==0) {
            this->DOM=list_add(this->DOM,get_rect_from_xmlnode(node,this));
        }
        //;
    }

    ezxml_free(root);


    return this;
}

void uixml_draw(UIXml* this) {
    for(List *i=this->DOM;i;i=list_iterate(i)) {
        ui_rect_draw(i->pointer,NULL);
    }
}

void *uixml_getbyid(UIXml* this,const char* id) {
    for(List *i=this->identifiers;i;i=list_iterate(i)) {
        UIXmlIdentifier* identifier=i->pointer;
        if(strncmp(identifier->id,id,sizeof(identifier->id))==0) {
            return identifier->pointer;
        }
    }
    return NULL;
}

void uixml_destroy(UIXml* this){
    for(List *i=this->identifiers;i;i=list_iterate(i)) {
        free(i->pointer);
    }
    list_destroy(this->identifiers);

    for(List *i=this->resources;i;i=list_iterate(i)) {
        resource_unload(i->pointer);
        resource_destroy(i->pointer);
    }
    list_destroy(this->resources);
    for(List *i=this->DOM;i;i=list_iterate(i)) {
        ui_rect_destroy(i->pointer);
    }
    list_destroy(this->DOM);       
}
