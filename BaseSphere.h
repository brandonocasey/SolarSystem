#pragma once
#include "Utils.h"
#include "Camera.h"
class BaseSphere
{
public:
    virtual void InitRotation(bool rotation, int rotation_speed, double radius) = 0;
    virtual void Update(float delta_time, Camera* camera) = 0;
    virtual void Draw() = 0;
    void GetLocation(int *x, int *y, int *z)
    {
        *x = m_iPositionx;
        *y = m_iPositiony;
        *z = m_iPositionz;
    }
protected:
    bool m_bRotation;
    int m_iRotationSpeed;
    double m_dRadius;
    int m_iPositionx;
    int m_iPositiony;
    int m_iPositionz;

    void SolidSphere()
    {
        #define PI 3.14159265358979323846f
        int width = 32;
        int height = 16;

        float theta, phi, *dat;
        int i, j, t, ntri, nvec, *idx;

        nvec = (height-2)* width+2;
        ntri = (height-2)*(width-1)*2;

        dat = (float*) malloc( nvec * 3*sizeof(float) );
        idx =   (int*) malloc( ntri * 3*sizeof(int)   );

        for( t=0, j=1; j<height-1; j++ )
            for(      i=0; i<width; i++ )
            {
                theta = float(j)/(height-1) * PI;
                phi   = float(i)/(width-1 ) * PI*2;
                dat[t++] =  sinf(theta) * cosf(phi);
                dat[t++] =  cosf(theta);
                dat[t++] = -sinf(theta) * sinf(phi);
            }
            dat[t++]=0; dat[t++]= 1; dat[t++]=0;
            dat[t++]=0; dat[t++]=-1; dat[t++]=0;

            for( t=0, j=0; j<height-3; j++ )
                for(      i=0; i<width-1; i++ )
                {
                    idx[t++] = (j  )*width + i  ;
                    idx[t++] = (j+1)*width + i+1;
                    idx[t++] = (j  )*width + i+1;
                    idx[t++] = (j  )*width + i  ;
                    idx[t++] = (j+1)*width + i  ;
                    idx[t++] = (j+1)*width + i+1;
                }
                for( i=0; i<width-1; i++ )
                {
                    idx[t++] = (height-2)*width;
                    idx[t++] = i;
                    idx[t++] = i+1;
                    idx[t++] = (height-2)*width+1;
                    idx[t++] = (height-3)*width + i+1;
                    idx[t++] = (height-3)*width + i;
                }

                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_NORMAL_ARRAY);
                glVertexPointer(3,GL_FLOAT,0,dat);
                glNormalPointer(GL_FLOAT,0,dat);
                glDrawElements(GL_TRIANGLES, ntri*3, GL_UNSIGNED_INT, idx );
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_NORMAL_ARRAY);

                free(idx);
                free(dat);
    }
    bool OpenGLStatus()
    {
        printf("        GL Status: ");

        GLenum status = (GLenum)glGetError();
        switch(status)
        {
        case GL_NO_ERROR:
            printf("OK\n");
            return true;
        case GL_INVALID_ENUM:
            printf("GL_INVALID_ENUM\n");
            break;
        case GL_INVALID_VALUE:
            printf("GL_INVALID_VALUE\n");
            break;
        case GL_INVALID_OPERATION:
            printf("GL_INVALID_OPERATION\n");
            break;
        case GL_STACK_OVERFLOW:
            printf("GL_STACK_OVERFLOW\n");
            break;
        case GL_STACK_UNDERFLOW:
            printf("GL_STACK_UNDERFLOW\n");
            break;
        case GL_OUT_OF_MEMORY:
            printf("GL_OUT_OF_MEMORY\n");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
            printf("GL_INVALID_FRAMEBUFFER_OPERATION_EXT\n");
            break;
        default:
            printf("UNKNWON\n");
        }
        return false;
    }
    // Texture Here
    BaseSphere() { }

};

/* Vertex data for a sphere
int vertexCount = 30;
float radius = 1.0f;
float center_x = 0.0f;
float center_y = 0.0f;

//create a buffer for vertex data
float buffer[] = new float[vertexCount*2]; // (x,y) for each vertex
int idx = 0;

//center vertex for triangle fan
buffer[idx++] = center_x;
buffer[idx++] = center_y;

//outer vertices of the circle
int outerVertexCount = vertexCount-1;

for (int i = 0; i < outerVertexCount; ++i){
float percent = (i / (float) (outerVertexCount-1));
float rad = percent * 2*Math.PI;

//vertex position
float outer_x = center_x + radius * cos(rad);
float outer_y = center_y + radius * sin(rad);

buffer[idx++] = outer_x;    
buffer[idx++] = outer_y;    
}
*/