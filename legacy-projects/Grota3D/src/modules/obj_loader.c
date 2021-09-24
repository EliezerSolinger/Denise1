#include "obj_loader.h"
#include "core/mathematics.h"
#include "graphics/3d/renderer.h"
#include "graphics/graphics.h"
#include <stdio.h>
#include <stdbool.h>

/*
Ka color_ambient
Kd color_diffuse
Ks color_specular
Ke color_emmissive
d alpha_value 	

Ns shineness
Ni index of refraction

m* texture
map_kd diffuse texture
map_ka ambient texture
map_ks specular texture
map_ke emissive texture
map_bump bump texture
map_ns glossiness texture

n* new material name
i* ilumination model

*/
static void get_file_relative_to(const char *relative_file,const char *file_path,char *out_file_dir) {
    unsigned int last_separator_i=0;
    for(int c=0;c<10000;c++) {
        if(file_path[c]=='\n' || file_path[c]=='\0') break;
        if(file_path[c]=='/' || file_path[c]=='\\') last_separator_i=c;
    }
    memcpy(out_file_dir,file_path,last_separator_i+1);
    strcpy(out_file_dir+last_separator_i+1,relative_file);
}

void mtlmaterial_freecontent(MTLMaterial *mat) {
    if(mat->name) free(mat->diffuse_texture_path);
    mat->diffuse_texture_path=NULL;
    mat->diffuse_texture_path_length=0;
    if(mat->name) free(mat->name);
    mat->name=NULL;
    mat->name_length=0;
}
void mtlfile_freecontent(MTLFileData *mtldata,bool trash_materials) {
        if(trash_materials)
            for(int c=0;c<mtldata->materials_count;c++) {
                mtlmaterial_freecontent(&(mtldata->materials[c]));
            }
        if(mtldata->materials)free(mtldata->materials);
        mtldata->materials_count=0;
}
void mtlfile_destroy(MTLFileData *mtldata,bool trash_materials) {
    mtlfile_freecontent(mtldata,trash_materials);
    free(mtldata);
}
MTLFileData * mtl_loader(const char *filename) {
    MTLFileData *mtldata=calloc(1,sizeof(MTLFileData));
    printf("loading MTL\nfile: %s \n",filename);
    bool success=true;
    FILE *file=fopen(filename, "r");
    if( file == NULL ){
		printf("Impossible to open the MTL file. NULL POINTER !\n");
		return NULL;
	}
    mtldata->materials=calloc(MTL_MAX_MATERIALS,sizeof(MTLMaterial));
    mtldata->materials_count=0;

    bool loading_first_material=true;
    bool readed_all_file=false;
    char *new_material_name=NULL;
    int new_material_name_length=0;
    while (!readed_all_file) {// para cada mesh
        MTLMaterial material;
        memset(&material,0,sizeof(MTLMaterial));
        material.diffuse=color4f_create(1,1,1,1);
        material.ambient=color4f_create(1,1,1,1);
        material.specular=color4f_create(1,1,1,1);
        material.name=new_material_name;
        material.name_length=new_material_name_length;
        new_material_name=NULL;
        new_material_name_length=0;

        while(true) { // para cada linha
            char lineHeader[128];
            // read the first word of the line
            if (fscanf(file, "%s", lineHeader) == EOF) {
                readed_all_file=true;
                break;
            } 
            if ( strcmp( lineHeader, "illum" ) == 0 ){ // color ambient
                fscanf(file, "%d\n",&material.illum);
            }else if ( strcmp( lineHeader, "Ns" ) == 0 ){ // color ambient
                fscanf(file, "%f\n",&material.shineness);
                
            }else if ( strcmp( lineHeader, "Ka" ) == 0 ){ // color ambient
                fscanf(file, "%f %f %f\n",
                 &(material.ambient.r), &(material.ambient.g), &(material.ambient.b) );
                
            }else if ( strcmp( lineHeader, "Kd" ) == 0 ){ // color diffuse
                fscanf(file, "%f %f %f\n",
                 &(material.diffuse.r), &(material.diffuse.g), &(material.diffuse.b) );
            }else if ( strcmp( lineHeader, "Ks" ) == 0 ){ // color specular
                fscanf(file, "%f %f %f\n",
                 &(material.specular.r), (&material.specular.g), &(material.specular.b) );
            } else if ( strcmp( lineHeader, "newmtl" ) == 0 ){
                char buffer[1000];
                fscanf(file, "%s", buffer);
                new_material_name_length=strlen(buffer)+1;
                new_material_name=malloc(new_material_name_length*sizeof(char));
                strcpy_s(new_material_name,new_material_name_length*sizeof(char),buffer);

                if(loading_first_material) {
                    material.name=new_material_name;
                    material.name_length=new_material_name_length;
                    loading_first_material=false;
                } else {
                    break;
                }
            } else if ( strcmp( lineHeader, "map_Kd" ) == 0 ){
               char texture_path[1000];
                fscanf(file, "%s", texture_path);
                material.diffuse_texture_path_length=strlen(texture_path)+1;
                material.diffuse_texture_path=malloc(material.diffuse_texture_path_length*sizeof(char));
                strcpy_s(material.diffuse_texture_path,material.diffuse_texture_path_length,texture_path);
                printf("MTL TEXTURE FILE: %s\n",material.diffuse_texture_path);
            }  else {
                // Probably a comment, eat up the rest of the line
                char stupidBuffer[1000];
                fgets(stupidBuffer, 1000, file);
            }
            
        }
        mtldata->materials[mtldata->materials_count]=material;
        mtldata->materials_count++;
    }
    if(success) {
        //mtldata->materials=realloc(mtldata->materials,mtldata->materials_count*sizeof(MTLMaterial))
        printf("MTL carregado %d materiais\n",mtldata->materials_count);
    } else {
        // clean pointers if occur an error
        mtlfile_freecontent(mtldata,true);
    }
    fclose(file);
    return mtldata;
}

void objmesh_freecontent(OBJMesh *mesh) {
    if(mesh->vertices) free(mesh->vertices);
    mesh->vertices=NULL;
    mesh->vertices_count=0;
    if(mesh->normals) free(mesh->normals);
    mesh->normals=NULL;
    mesh->normals_count=0;
    if(mesh->uvs) free(mesh->uvs);
    mesh->uvs=NULL;
    mesh->uvs_count=0;
    if(mesh->triangles) free(mesh->triangles);
    mesh->triangles=NULL;
    mesh->triangles_count=0;
}
void objfile_freecontent(OBJFileData *objdata,bool trash_meshes) {
        if(trash_meshes)
            for(int c=0;c<objdata->objects_count;c++) {
                objmesh_freecontent(&(objdata->objects[c]));
            }
        if(objdata->objects)free(objdata->objects);
        objdata->objects=NULL;
        objdata->objects_count=0;
}
void objmesh_to_meshdata(OBJMesh mesh,MeshData *result) {
    static bool print_c=false;

    result->vertices_count=mesh.triangles_count*3;
    result->vertices=malloc(sizeof(VertexData)*(mesh.triangles_count*3));

    if(print_c) printf("VertexData data[%d]={\n",result->vertices_count);
    for(int tri_index=0;tri_index<mesh.triangles_count;tri_index++) {
        OBJTriangle triangle=mesh.triangles[tri_index];
        for(int vi=0;vi<3;vi++) {
            VertexData vt;
            vt.vertex=mesh.vertices[triangle.vertexIndices[vi]-1];
            vt.normal=mesh.normals[triangle.normalIndices[vi]-1];
            vt.texture_coordinates=mesh.uvs[triangle.uvIndices[vi]-1];
            result->vertices[(tri_index*3)+vi]=vt;
             if(print_c)printf("\t{%f,%f,%f},{%f,%f,%f},{%f,%f},\n",
            vt.vertex.x,vt.vertex.y,vt.vertex.z,
            vt.normal.x,vt.normal.y,vt.normal.z,
            vt.texture_coordinates.x,vt.texture_coordinates.x);
        }
    }
    if(print_c) printf("}\n");
}

bool obj_loader(const char * filename,OBJFileData *objdata) {
    printf("loading OBJ\nfile: %s \n",filename);
    bool success=true;
    
    FILE *file=fopen(filename, "r");
    if( file == NULL ){
		printf("Impossible to open the OBJ file. NULL POINTER !\n");
		return NULL;
	}
    objdata->objects=calloc(OBJ_MAX_MESHES,sizeof(OBJMesh));
    objdata->objects_count=0;
    objdata->mtl_path=NULL;
    objdata->mtl_path_length=0;
    GLuint vertex_index_offset=0;
    GLuint uv_index_offset=0;
    GLuint normal_index_offset=0;
    bool loading_first_mesh=true;
    bool readed_all_file=false;
    
    char *new_object_name=NULL;
    int new_object_name_length=0;
    while (!readed_all_file) {// para cada mesh
        OBJMesh mesh;
        memset(&mesh,0,sizeof(OBJMesh));
        mesh.vertices=malloc(OBJ_MAX_VERTICES*sizeof(Vec3));
        mesh.vertices_count=0;
        mesh.normals=malloc(OBJ_MAX_VERTICES*sizeof(Vec3));
        mesh.normals_count=0;
        mesh.uvs=malloc(OBJ_MAX_VERTICES*sizeof(Vec2));
        mesh.uvs_count=0;
        mesh.triangles=malloc(OBJ_MAX_TRIANGLES*sizeof(OBJTriangle));
        mesh.triangles_count=0;
        
        mesh.name=new_object_name;
        mesh.name_length=new_object_name_length;
        new_object_name=NULL;
        new_object_name_length=0;

    
        while(true) { // para cada linha
            char lineHeader[128];
            // read the first word of the line
            if (fscanf(file, "%s", lineHeader) == EOF) {
                readed_all_file=true;
                break;
            }
           if (strcmp( lineHeader, "s" ) == 0 ){
                GLuint group_id;
                fscanf(file, "%d\n", &group_id);
                mesh.group_id=group_id;
            } else if (strcmp( lineHeader, "v" ) == 0 ){
                Vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
    
                mesh.vertices[mesh.vertices_count]=vertex;
                mesh.vertices_count++;
            } else if ( strcmp( lineHeader, "vt" ) == 0 ){
                Vec2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y );
                mesh.uvs[mesh.uvs_count]=uv;
                mesh.uvs_count++;
            } else if ( strcmp( lineHeader, "vn" ) == 0 ){
                Vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                mesh.normals[mesh.normals_count]=normal;
                mesh.normals_count++;
            } else if ( strcmp( lineHeader, "f" ) == 0 ){
                OBJTriangle face;
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                        &(face.vertexIndices[0]), &(face.uvIndices[0]), &(face.normalIndices[0]),
                        &(face.vertexIndices[1]), &(face.uvIndices[1]), &(face.normalIndices[1]),
                        &(face.vertexIndices[2]), &(face.uvIndices[2]), &(face.normalIndices[2])
                );
                for(int i=0;i<3;i++){
                    face.vertexIndices[i]-=vertex_index_offset;
                    face.normalIndices[i]-=normal_index_offset;
                    face.uvIndices[i]-=uv_index_offset;
                }
                mesh.triangles[mesh.triangles_count]=face;
                mesh.triangles_count++;  
                if (matches == 9){
                    face.with_error=false;
                    //printf("mathes %d OBJ file can't be read by our simple parser :-( Try exporting with other options\n",matches);
                    //success=false;
                    //break;
                } else {
                    face.with_error=true;
                        printf("mathes %d OBJ file can't be read by our simple parser :-( Try exporting with other options\n",matches);
                        //success=false;
                        //break;
                }
              
            }else if (!(objdata->mtl_path) && strcmp( lineHeader, "mtllib" ) == 0 ){
                char buffer[1000];
                fscanf(file, "%s\n", buffer);
                objdata->mtl_path_length=strlen(buffer)+1;
                objdata->mtl_path=malloc(objdata->mtl_path_length*sizeof(char));
                strcpy_s(objdata->mtl_path,objdata->mtl_path_length*sizeof(char),buffer);
                printf("usando arquivo mtl %s\n",objdata->mtl_path);
            } else if ( strcmp( lineHeader, "usemtl" ) == 0 ){
                char buffer[1000];
                fscanf(file, "%s\n", buffer);
                mesh.material_name_length=strlen(buffer)+1;
                mesh.material_name=malloc(mesh.material_name_length*sizeof(char));
                strcpy_s(mesh.material_name,mesh.material_name_length*sizeof(char),buffer);
                //printf("usando material %s\n",mesh.material_name);
            }  else if ( strcmp( lineHeader, "o" ) == 0 ){
                char buffer[1000];
                fscanf(file, "%s\n", buffer);
                new_object_name_length=strlen(buffer)+1;
                new_object_name=malloc(new_object_name_length*sizeof(char));
                strcpy_s(new_object_name,new_object_name_length*sizeof(char),buffer);

                if(loading_first_mesh) {
                    mesh.name=new_object_name;
                    mesh.name_length=new_object_name_length;
                    loading_first_mesh=false;
                } else {
                    break;
                }
            } else {
                // Probably a comment, eat up the rest of the line
                char stupidBuffer[1000];
                fgets(stupidBuffer, 1000, file);
            }
            
        }

  
        vertex_index_offset+=mesh.vertices_count;
        uv_index_offset+=mesh.uvs_count;
        normal_index_offset+=mesh.normals_count;

        objdata->objects[objdata->objects_count]=mesh;
        objdata->objects_count++;
        printf("mesh %s carregada %d tris %d verts %d normals %d uvs\n",mesh.name,
        mesh.triangles_count,mesh.vertices_count,mesh.normals_count,mesh.uvs_count);
         // realloca todo espaco nao usado
        // nao sei se eh realmente necessario
        //mesh.vertices=realloc(mesh.vertices,mesh.vertices_count*sizeof(Vec3));
        //mesh.normals=realloc(mesh.normals,mesh.normals_count*sizeof(Vec3));
        //mesh.uvs=realloc(mesh.normals,mesh.uvs_count*sizeof(Vec2));
        //mesh.triangles=realloc(mesh.triangles,mesh.triangles_count*sizeof(OBJTriangle));
        
    }
    if(success) {
       
        printf("OBJ carregado %d meshes\n",objdata->objects_count);
    } else {
        // clean pointers if occur an error
        objfile_freecontent(objdata,true);
    }
       fclose(file);
    return success;
}


bool load_obj_first_mesh(const char *filename,MeshData *out_mesh) {
    OBJFileData objfile;
    obj_loader(filename,&objfile);
    if(objfile.objects_count==0) return false;
    objmesh_to_meshdata(objfile.objects[0],out_mesh);
    objfile_freecontent(&objfile,true);

    /*for(int c=0;c<20000;c++) {
        obj_loader(filename,&objfile);
        objfile_freecontent(&objfile,true);
    }*/

    return true;
}

Material mtlmaterial_to_material(MTLMaterial *mtl_material) {
    Material result;
    result.ambient=mtl_material->ambient;
    result.diffuse=mtl_material->diffuse;
    result.specular=mtl_material->specular;
    result.shineness=mtl_material->shineness;
    result.texture_id=0;
    return result;
}

bool load_obj_renderers(const char *obj_filename,Renderer **renderers,int *renderers_count) {
    //printf(" obj %s mtl %s calc mtl: %s \n x");

    //printf("x1\n");
    OBJFileData objfile;
    if(!obj_loader(obj_filename,&objfile)) return false;
    if(objfile.objects_count==0) return false;
    MTLFileData *mtlfile=NULL;
    if(objfile.mtl_path) {
        char buffer[512];
        get_file_relative_to(objfile.mtl_path,obj_filename,buffer);
        mtlfile=mtl_loader(buffer);
    }   

    //printf("x2\n");

    *renderers=calloc(objfile.objects_count,sizeof(Renderer));
    *renderers_count=objfile.objects_count;
    
    //printf("x3\n");
    printf("Gerando %d renderers\n",objfile.objects_count);
    for(int c=0;c<objfile.objects_count;c++) {
        //////////////////////////////////////////////////
      //  printf("x4\n");
        MeshData *mesh=malloc(sizeof(MeshData));
        
        objmesh_to_meshdata(objfile.objects[c],mesh);
        //printf("4.1\n");
        
        /*printf("DADOS NO OBJMESH\n");
        for(int x=0;x<objfile.objects[c].vertices_count;x++) {
            Vec3 vec=objfile.objects[c].vertices[x];
            printf("%f %f %f\n",vec.x,vec.y,vec.z);
        }
        printf("DADOS NA MESHDATA\n");
        for(int x=0;x<mesh->vertices_count;x++) {
            Vec3 vec=mesh->vertices[x].vertex;
            printf("%f %f %f\n",vec.x,vec.y,vec.z);
        }
        */
          if(mtlfile) {
            for(int m=0;m<mtlfile->materials_count;m++) {
          //      printf("4.2 - %d\n",m);
                if(strcmp(mtlfile->materials[m].name,objfile.objects[c].material_name) == 0) {
                    MTLMaterial *mtl_material=&(mtlfile->materials[m]);
                    Renderer *renderer=&((*renderers)[c]);
                    renderer->material=mtlmaterial_to_material(mtl_material);
                    
                    if(mtl_material->diffuse_texture_path) {
                        char buffer[512];
                        get_file_relative_to(mtl_material->diffuse_texture_path,obj_filename,buffer);
                        renderer->material.texture_id=load_texture(buffer,false);
                    }
                }
            }
        }
        ///////////////////////////////////////////////////////////
        //printf("x5\n");
        (*renderers)[c].matrix=mat4_create(1.0f);
        RenderObject *rendobj=calloc(1,sizeof(RenderObject));
        *rendobj=mesh_load(mesh);
        
        (*renderers)[c].render_object=rendobj;
        (*renderers)[c].material.diffuse=color4f_create(1,1,1,1);
        
        //printf("x6\n");
        //mesh_freecontent(mesh); 
    }
    
    printf("x7\n");
    objfile_freecontent(&objfile,true);
    
    printf("x8\n");
    return true;
}