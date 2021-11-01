
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "../core/dmath.h"


int width=20, height=20;

char buffer[20][20]={0};
float *zbuffer;

float vertices[] = {
    // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f,  // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
}; 


// function for line generation
void bresenham_buffer(int x1, int y1, int x2, int y2) {
    printf("bresenham_buffer: %d %d %d %d\n", x1, y1, x2, y2);
   int m_new = 2 * (y2 - y1);
   int slope_error_new = m_new - (x2 - x1);
   for (int x = x1, y = y1; x <= x2; x++)
   {
      buffer[x][y] = 'x';
 
      // Add slope to increment angle formed
      slope_error_new += m_new;
 
      // Slope error reached limit, time to
      // increment y and update slope error.
      if (slope_error_new >= 0)
      {
         y++;
         slope_error_new  -= 2 * (x2 - x1);
      }
   }
}
void draw_line(float x1, float y1,float z1, float x2, float y2,float z2) {
 bresenham_buffer(round((x1+0.5)*(float)width), round((y1+0.5)*(float)height), round((x2+0.5)*(float)width), round((y2+0.5)*(float)height));
}
void draw_triangles(float *vertices, int vertices_count) {
    if(vertices_count%3 != 0) {
        printf("vertices_count must be a multiple of 3\n");
        exit(1);
    }
    for(int index=0;index<vertices_count;index+=3) {
        float x1 = vertices[index+0];
        float y1 = vertices[index+1];
        float z1 = vertices[index+2];
        float x2 = vertices[index+3];
        float y2 = vertices[index+4];
        float z2 = vertices[index+5];
        float x3 = vertices[index+6];
        float y3 = vertices[index+7];
        float z3 = vertices[index+8];
        draw_line(x1,y1,z1,x2,y2,z2);
        draw_line(x2,y2,z2,x3,y3,z3);
        draw_line(x3,y3,z3,x1,y1,z1);
    }
}

int main() {
    memset(buffer, 0, sizeof(buffer));
   // bresenham_buffer(0,0,width-1,height-1);
    draw_triangles(vertices, sizeof(vertices)/sizeof(float));
    
        for(int y=0;y<height;y++) {
            for(int x=0;x<width;x++) {
                    printf("%c  ", buffer[x][y]);
            }
                printf("\n");
        }

    return EXIT_SUCCESS;
}