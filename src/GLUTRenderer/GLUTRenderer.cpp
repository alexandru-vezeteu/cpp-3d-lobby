//
// Created by alex on 20/05/25.
//

#include "GLUTRenderer.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut_std.h>
IRenderer::DisplayFunc GLUTRenderer::display_func;
IRenderer::KeyboardFunc GLUTRenderer::keyboard_func;
IRenderer::ReshapeFunc GLUTRenderer::reshape_func;
IRenderer::TimerFunc GLUTRenderer::timer_func;
int GLUTRenderer::ms = -1;
void GLUTRenderer::init(int argc, char** argv, const char* windowTitle) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(windowTitle);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr<<"Error: "<<glewGetErrorString(err)<<std::endl;
        exit(EXIT_FAILURE);
    }
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glewInit();
}
void GLUTRenderer::startMainLoop() {
    glutDisplayFunc(GLUTRenderer::displayWrapper);
    glutReshapeFunc(GLUTRenderer::reshapeWrapper);
    glutKeyboardFunc(GLUTRenderer::keyboardWrapper);
    glutTimerFunc(ms, GLUTRenderer::timeoutWrapper, 0);

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

void GLUTRenderer::setTimerFunction(TimerFunc f) {
    timer_func = f;
}

void GLUTRenderer::timeoutWrapper(int ms) {
    if (timer_func) {
        timer_func(ms);
        glutTimerFunc(100, GLUTRenderer::timeoutWrapper, 0);
    }
}

void GLUTRenderer::setTimeout(int ms) {
    GLUTRenderer::ms = ms;
}


