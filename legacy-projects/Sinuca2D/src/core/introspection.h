#ifndef GROTA_INTROSPECTION_H
#define GROTA_INTROSPECTION_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef offsetof
    #define offsetof(st, m) \
        ((size_t) ( (char *)&((st *)(0))->m - (char *)0 ))
#endif
#define BOOLSTR(b) (b ? "true" : "false")
#define COUNT(arr)	(sizeof (arr) / sizeof ((arr)[0]))


typedef enum _iType {
    iNotFound,
    iPointer,
    iFloat,
    iEnum,
    iStringPointer,
    iString,
    iInt,
    iUint,
    iBool,
    iStruct,
    iFixed
} iType;

typedef struct _iEnumValue {
    char *name;
    unsigned int value;
} iEnumValue;
typedef struct _iStructReg iStructReg;
typedef struct _iEnumReg iEnumReg;
typedef struct _iProperty iProperty;
struct _iProperty{
    char *name;
    iType type;
    size_t size;
    size_t offset;
    iStructReg *struct_template;
    iEnumReg *enum_template;
    bool is_pointer;
};

struct _iStructReg {
    char *name;
    iProperty *properties;
    size_t prop_count;
    size_t total_size;
};
struct _iEnumReg {
    char *name;
    iEnumValue *values;
    size_t values_count;
    size_t option_size;
};

typedef struct _iReturnValue {
    void* pointer;
    iProperty *property;
} iReturnValue;


#define ISTRUCTREG(type) static iStructReg type ## _reg ={#type,type ## _i,COUNT(type ## _i),sizeof(type)}
#define IENUMREG(type) static iEnumReg type ## _reg ={ #type ,type ## _i,COUNT(type ## _i),sizeof(type)}

#define IPROPP(member,itype,size,is_pointer) {#member,itype,size,offsetof(ITYPE,member),NULL,NULL,is_pointer}
#define IPROP(member,itype,size) IPROPP(member,itype,size,false)
#define ISTRUCTP(member,struct_type,is_pointer) {#member,iStruct,sizeof(struct_type ## _i),offsetof(ITYPE,member),&(struct_type ## _reg),NULL,is_pointer}
#define ISTRUCT(member,struct_type) ISTRUCTP(member,struct_type,false)
#define ISTRUCTPOINTER(member,struct_type) ISTRUCTP(member,struct_type,true)
#define IENUMVALUE(value) {#value,value}
#define IENUM(member,enum_type) {#member,iEnum,sizeof(enum_type),offsetof(ITYPE,member),NULL,&(enum_type ## _reg),false}

#define IFLOAT(member) IPROP(member,iFloat,sizeof(float))
#define IINT(member) IPROP(member,iInt,sizeof(int))
#define IUINT(member) IPROP(member,iUint,sizeof(unsigned int))
#define IBOOL(member) IPROP(member,iBool,sizeof(bool))
#define ISTRINGPOINTER(member) IPROP(member,iStringPointer,sizeof(char*))
#define ISTRING(member) IPROP(member,iString,sizeof(((ITYPE *)0)->member))
#define IFIXED(member,size) IPROP(member,iFixed,size)
#define IPOINTER(member,type) IPROPP(member,iPointer,sizeof(type),true)
#define IFIXEDTYPE(member,type) IFIXED(member,sizeof(type))

#define STRPOS(string,char) ((int)(strchr(string, char)-string))


static iReturnValue i_relative_address(iStructReg *template,const char* propname,void* offset) {
    iReturnValue out={iNotFound,NULL};
    int eos=STRPOS(propname, '.');
    if(!eos) eos=strlen(propname);
    iProperty * properties;
    for(int c=0;c<template->prop_count;c++) {
        if(strncmp(properties[c].name,propname,eos)==0) {
            if(properties[c].type==iStruct) {
                void* xoffset=offset+properties[c].offset;

                if(properties[c].is_pointer) xoffset=*(void **)offset;

                out=i_relative_address(properties[c].struct_template,propname+eos+1,xoffset);
                return out;
            } else {
                out.property=&(properties[c]);
                out.pointer=offset+properties[c].offset;
                return out;
            }
        }
    }
    return out; // nao encontrou
}

#define IGETPOINTER(struct_type,propname,pointer) (i_relative_address(struct_type ## _reg,propname,pointer))


static char* i_str_not_found="NOT FOUND";
static inline char* i_enum_name(iEnumReg *reg,int value) {
    if(!reg) return NULL;
    iEnumValue *names=reg->values;
    for(int c=0;c<reg->values_count;c++) {
        if(names[c].value==value) return names[c].name;
    }
    return i_str_not_found;
}
#define IENUM_NOT_FOUND -23423
static inline int i_enum_value(iEnumReg *reg,const char *name) {
    if(!reg) return IENUM_NOT_FOUND;
    iEnumValue *values=reg->values;
    for(int c=0;c<reg->values_count;c++) {
        if(strcmp(values[c].name,name)==0) return values[c].value;
    }
    return IENUM_NOT_FOUND;
}
static inline char* i_enum_name_from_property(iProperty *p,int value) {
    if(!p) return NULL;
    if(!(p->enum_template)) return NULL;
    return i_enum_name(p->enum_template,value);
}
#define IENUM_GETNAME(enum_type,value) (i_enum_name(&(enum_type ## _reg),value))
#define IENUM_GETVALUE(enum_type,name) (i_enum_value(&(enum_type ## _reg),name))

static iEnumValue iType_i[]={
    IENUMVALUE(iNotFound),
    IENUMVALUE(iPointer),
    IENUMVALUE(iFloat),
    IENUMVALUE(iEnum),
    IENUMVALUE(iStringPointer),
    IENUMVALUE(iString),
    IENUMVALUE(iInt),
    IENUMVALUE(iUint),
    IENUMVALUE(iBool),
    IENUMVALUE(iStruct),
    IENUMVALUE(iFixed),
};
IENUMREG(iType);



static void i_printf(iStructReg *template,int recursion_level) {
    printf("(\n");
    iProperty *props=template->properties;
    for(int c=0;c<template->prop_count;c++) {
        if(props[c].type==iStruct) {
            for(int n=0;n<recursion_level+1;n++) {printf("  ");}
            printf(" %s",props[c].struct_template->name);
            
            if(props[c].is_pointer) printf(" -> ");
            i_printf(props[c].struct_template,recursion_level+1);
        } else {
           for(int n=0;n<recursion_level+1;n++) {printf("  ");}
           printf("%s %s ",IENUM_GETNAME(iType,props[c].type),props[c].name);
           if(props[c].is_pointer) printf(" POINTER ");
           printf("\n");
        }
    }
    for(int n=0;n<recursion_level;n++) {printf("  ");} printf(")\n");
}

#define IPRINT(struct_type) i_printf(&(struct_type ## _reg),0)


#endif
