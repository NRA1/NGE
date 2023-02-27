#ifndef NGE_ROTATION_HPP
#define NGE_ROTATION_HPP


class Rotation
{
public:
    Rotation();
    Rotation(float roll, float pitch, float yaw);

    float roll() const;
    void setRoll(float roll);
    float pitch() const;
    void setPitch(float pitch);
    float yaw() const;
    void setYaw(float yaw);

    Rotation operator+(const Rotation &x) const;
    Rotation operator*(float x) const;

private:
    float roll_;
    float pitch_;
    float yaw_;
};


#endif //NGE_ROTATION_HPP
