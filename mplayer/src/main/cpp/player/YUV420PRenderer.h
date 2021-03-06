/*
 
  YUV420P Player
  --------------
 
  This class implements a simple YUV420P renderer. This means that you 
  need to feed planar YUV420 data to the `setYPixels()`, `setUPixels()`
  and `setVPixels()`. 
 
  First make sure to call setup() with the video width and height. We use
  these dimensions to allocate the Y, U and V textures. After calling setup
  you call the zset{Y,U,V}Pixels()` everytime you have a new frame that
  you want to render. With the `draw()` function you draw the current 
  frame to the screen.
 
  If you resize your viewport, make sure to  call `resize()` so we can 
  adjust the projection matrix.
 
 */
#ifndef ROXLU_YUV420P_PLAYER_H
#define ROXLU_YUV420P_PLAYER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const char gFragmentShader[] =
        "varying lowp vec2 TexCoordOut;                        \n"
                "uniform sampler2D SamplerY;                           \n"
                "uniform sampler2D SamplerU;                           \n"
                "uniform sampler2D SamplerV;                           \n"
                "void main(void)                                       \n"
                "{                                                     \n"
                "mediump vec3 yuv;                                 \n"
                "lowp vec3 rgb;                                    \n"
                "yuv.x = texture2D(SamplerY, TexCoordOut).r;       \n"
                "yuv.y = texture2D(SamplerU, TexCoordOut).r - 0.5; \n"
                "yuv.z = texture2D(SamplerV, TexCoordOut).r - 0.5; \n"
                "rgb = mat3( 1,       1,         1,                \n"
                "0,       -0.39465,  2.03211,                   \n"
                "1.13983, -0.58060,  0) * yuv;                  \n"
                "gl_FragColor = vec4(rgb, 1);                      \n"
                "}                                                     \n";

static const char gVertexShader[] =
        "attribute vec4 position;      \n"
                "attribute vec2 TexCoordIn;    \n"
                "varying vec2 TexCoordOut;     \n"
                "void main(void)               \n"
                "{                             \n"
                "gl_Position = position;   \n"
                "TexCoordOut = TexCoordIn; \n"
                "}                             \n";

static const GLfloat squareVertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
                                         1.0f, 1.0f, 1.0f,};

static const GLfloat coordVertices[] = {0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                                        1.0f, 0.0f,};

enum AttribEnum {
    ATTRIB_VERTEX,
    ATTRIB_TEXTURE,
    ATTRIB_COLOR,
};

enum TextureType {
    TEXY = 0,
    TEXU,
    TEXV,
    TEXC
};

class YUV420PRenderer {

public:
    YUV420PRenderer();

    ~YUV420PRenderer();


    //设置视频帧大小
    void setFrameSize(int frameWidth, int frameHeight);

    //清除渲染
    void clearFrame();

    //渲染旧视频帧
    void renderFrame();

    //渲染新视频帧
    void renderFrame(uint8_t **pixels);

    //重置窗口大小
    //winWidth  窗体宽度（即显示宽度）
    //winHeght  窗体高度
    void resetWindow(int winWidth, int winHeight);

private:
    uint8_t *y_pixels;
    uint8_t *u_pixels;
    uint8_t *v_pixels;
private:

    //初始化设置
    bool setup();

    GLuint compileShader(const char *pSource, GLenum shaderType);

    void setupYUVTexture();

    void loadShader();


public:
    //实际窗体大小
    int iDrawWidth;
    int iDrawHeight;
    //视频帧大小
    int iFrameWidth;
    int iFrameHeight;
    GLuint _textureYUV[3];

    uint8_t *frameData[8];


    GLuint gProgram;
    /* GLuint gvPositionHandle;

     GLuint gvYTexCoordHandle;
     GLuint gvUTexCoordHandle;
     GLuint gvVTexCoordHandle;

     GLuint guTexYHandle;
     GLuint guTexUHandle;
     GLuint guTexVHandle;*/
};

#endif
