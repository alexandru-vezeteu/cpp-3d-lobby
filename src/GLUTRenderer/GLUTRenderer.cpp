//
// Created by alex on 20/05/25.
//

#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>
#include <iostream>

#include "GLUTRenderer.hpp"

IRenderer::DisplayFunc GLUTRenderer::display_func;
IRenderer::KeyboardFunc GLUTRenderer::keyboard_func;
IRenderer::ReshapeFunc GLUTRenderer::reshape_func;
IRenderer::TimeoutFunc GLUTRenderer::timeout_func;
IRenderer::MouseFunc GLUTRenderer::mouse_func;
IRenderer::CloseFunc GLUTRenderer::close_func;
IRenderer::SpecialKeyboardFunc GLUTRenderer::special_keyboard_func;
int GLUTRenderer::ms = 0;
int GLUTRenderer::width = 800;
int GLUTRenderer::height = 600;


void GLUTRenderer::init(int argc, char** argv, const char* windowTitle) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(windowTitle);
    glutSetOption(GLUT_MULTISAMPLE, 4);
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr<<"Error: "<<glewGetErrorString(err)<<std::endl;
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);

    //pt corectie gamma
    //glEnable(GL_FRAMEBUFFER_SRGB);

    //anti alias
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glewInit();
}
void GLUTRenderer::startMainLoop() {
    glutDisplayFunc(GLUTRenderer::displayWrapper);
    glutReshapeFunc(GLUTRenderer::reshapeWrapper);
    glutKeyboardFunc(GLUTRenderer::keyboardWrapper);
    glutSpecialFunc(GLUTRenderer::specialKeyboardWrapper);
    glutPassiveMotionFunc(GLUTRenderer::mouseWrapper);
    glutTimerFunc(0, GLUTRenderer::timeoutWrapper, ms);
    glutCloseFunc(GLUTRenderer::closeWrapper);
    glutMainLoop();
};

void GLUTRenderer::setDisplayFunc(DisplayFunc func) {
    display_func = func;
}
void GLUTRenderer::setKeyboardFunc(KeyboardFunc func) {
    keyboard_func = func;
}
void GLUTRenderer::setReshapeFunc(ReshapeFunc func) {
    reshape_func = func;
}
void GLUTRenderer::setCloseFunc(CloseFunc func) {
    close_func = func;
}


void GLUTRenderer::setMouseFunc(MouseFunc func) {
    mouse_func = func;
}


void GLUTRenderer::mouseWrapper( int x, int y) {
    if (mouse_func) {
        mouse_func( x, y);
    }
}


void GLUTRenderer::displayWrapper() {
    if (display_func) {

        display_func();
    }
}
void GLUTRenderer::keyboardWrapper(unsigned char key, int x, int y) {
    if (keyboard_func) {
        keyboard_func(key, x, y);
    }
}
void GLUTRenderer::closeWrapper() {
    if (close_func) {
        close_func();
    }
}


void GLUTRenderer::reshapeWrapper(int width, int height) {
    if (reshape_func) {
        reshape_func(width, height);
    }
}

void GLUTRenderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLUTRenderer::swapBuffers() {
    glutSwapBuffers();
}

void GLUTRenderer::redisplay() {
    glutPostRedisplay();
}

void GLUTRenderer::viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void GLUTRenderer::setTimeoutFunction(const TimeoutFunc f) {
    timeout_func = f;
}

void GLUTRenderer::timeoutWrapper(const int ms) {
    if (timeout_func) {
        timeout_func(ms);
        glutTimerFunc(ms, GLUTRenderer::timeoutWrapper, ms);
    }
}

void GLUTRenderer::setTimeout(const int ms) {
    GLUTRenderer::ms = ms;
}
void GLUTRenderer::setSpecialKeyboardFunc(const SpecialKeyboardFunc func) {
    special_keyboard_func = func;
}

void GLUTRenderer::specialKeyboardWrapper(const int key, const int x, const int y) {
    if (special_keyboard_func) {
        special_keyboard_func(key,x,y);
    }
}


GLUTRenderer& GLUTRenderer::getInstance()
{
    static GLUTRenderer instance;
    return instance;
}

