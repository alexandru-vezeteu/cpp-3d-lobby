//
// Created by alex on 20/05/25.
//

#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include <functional>

class IRenderer {
public:
    using DisplayFunc = std::function<void()>;
    using KeyboardFunc = std::function<void(unsigned char, int, int)>;
    using ReshapeFunc = std::function<void(int, int)>;
    using TimeoutFunc = std::function<void(int)>;
    using MouseFunc = std::function<void(int, int)>;
    using CloseFunc = std::function<void()>;
    using SpecialKeyboardFunc = std::function<void(int, int, int)>;

    virtual ~IRenderer() = default;

    virtual void clear() = 0;
    virtual void swapBuffers() = 0;
    virtual void redisplay() = 0;
    virtual void viewport(int,int,int,int) = 0;

    virtual void init(int argc, char** argv, const char* windowTitle) = 0;
    virtual void startMainLoop() = 0;

    virtual void setTimeoutFunction(TimeoutFunc) = 0;
    virtual void setTimeout(int ms) = 0;
    virtual void setDisplayFunc(DisplayFunc func) = 0;
    virtual void setKeyboardFunc(KeyboardFunc func) = 0;
    virtual void setReshapeFunc(ReshapeFunc func) = 0;
    virtual void setMouseFunc(MouseFunc func) = 0;
    virtual void setSpecialKeyboardFunc(SpecialKeyboardFunc func) = 0;
    virtual void setCloseFunc(CloseFunc func) = 0;




};


#endif //IRENDERER_HPP
