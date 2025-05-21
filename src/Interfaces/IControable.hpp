//
// Created by alex on 10/05/25.
//

#ifndef ICONTROABLE_HPP
#define ICONTROABLE_HPP


class IControable {
    public:
    virtual ~IControable() = default;
    virtual void control(unsigned char, int, int) = 0;
};

#endif //ICONTROABLE_HPP
