//
// Created by alex on 20/05/25.
//

#ifndef GLUTRENDERER_HPP
#define GLUTRENDERER_HPP

#include "../Interfaces/IRenderer.hpp"

class GLUTRenderer : public IRenderer {
    static DisplayFunc display_func;
    static KeyboardFunc keyboard_func;
    static SpecialKeyboardFunc special_keyboard_func;
    static ReshapeFunc reshape_func;
    static TimeoutFunc timeout_func;
    static MouseFunc mouse_func;
    static int ms;
    static CloseFunc close_func;
    GLUTRenderer() = default;

public:
    static int width;
    static int height;

    static GLUTRenderer& getInstance();

    void init(int argc, char** argv, const char* windowTitle) override;
    void startMainLoop() override;

    void setDisplayFunc(DisplayFunc func) override;
    void setKeyboardFunc(KeyboardFunc func) override;
    void setReshapeFunc(ReshapeFunc func) override;
    void setTimeoutFunction(TimeoutFunc) override;
    void setMouseFunc(MouseFunc func) override;
    void setTimeout(int ms) override;
    void setCloseFunc(CloseFunc func) override;
    void setSpecialKeyboardFunc(SpecialKeyboardFunc func) override;


    void clear() override;
    void swapBuffers() override;
    void redisplay() override;
    void viewport(int x,int y,int w, int h) override;


    ~GLUTRenderer() override = default;



private:
    static void displayWrapper();
    static void keyboardWrapper(unsigned char key, int x, int y);
    static void reshapeWrapper(int width, int height);
    static void timeoutWrapper(int ms);
    static void mouseWrapper(int x,int y);
    static void closeWrapper();
    static void specialKeyboardWrapper(int key, int x, int y);
};



#endif //GLUTRENDERER_HPP
