//
// Created by alex on 10/05/25.
//

#ifndef ICONTROL_HPP
#define ICONTROL_HPP


class IControl {
    public:
    virtual ~IControl() = default;
    virtual bool control(unsigned char, int, int) = 0;
    virtual bool mouse(int,int) = 0;
    virtual bool specialKey(int, int, int) = 0;
};

#endif //ICONTROL_HPP
