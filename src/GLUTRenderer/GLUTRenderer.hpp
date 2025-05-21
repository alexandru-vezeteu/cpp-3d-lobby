//
// Created by alex on 20/05/25.
//

#ifndef GLUTRENDERER_HPP
#define GLUTRENDERER_HPP

#include <iostream>

#include "../Interfaces/IRenderer.hpp"

class GLUTRenderer : public IRenderer {
    static DisplayFunc display_func;
    static KeyboardFunc keyboard_func;
    static ReshapeFunc reshape_func;
    static TimerFunc timer_func;
    static int ms;

public:
    GLUTRenderer() {
        std::cout<<"GLUTRenderer"<<std::endl;
    }


    void init(int argc, char** argv, const char* windowTitle) override;
    void startMainLoop() override;

    void setDisplayFunc(DisplayFunc func) override;
    void setKeyboardFunc(KeyboardFunc func) override;
    void setReshapeFunc(ReshapeFunc func) override;
    void setTimerFunction(TimerFunc) override;
    void setTimeout(int ms) override;

    void clear() override;
    void swapBuffers() override;
    void redisplay() override;
    void viewport(int x,int y,int w, int h) override;


    ~GLUTRenderer() override {

    }



private:
    static void displayWrapper();
    static void keyboardWrapper(unsigned char key, int x, int y);
    static void reshapeWrapper(int width, int height);
    static void timeoutWrapper(int ms);
};



#endif //GLUTRENDERER_HPP
