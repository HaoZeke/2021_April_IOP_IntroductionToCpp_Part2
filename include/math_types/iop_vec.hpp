#ifndef IOP_VEC_H
#define IOP_VEC_H

/**
 * @brief A basic class for componentwise addition
 */
class Vector3
{
    public:
    double x;
    double y;
    double z;
    /**
     * @brief Defaults to zero
     * @return A zero vector
     */
    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    /**
     * @brief Defaults to zero
     * @param _x is the x coordinate
     * @param _y is the y coordinate
     * @param _z is the z coordinate
     * @return An initialized vector
     */
    Vector3(double _x, double _y, double _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    /**
     * @brief Defines the operation +=
     * @param rhs a vector which is added component-wise
     * @return Modified vector post component-wise addition
     */
    Vector3& operator+=(const Vector3& rhs)
    {
        x+=rhs.x;
        y+=rhs.y;
        z+=rhs.z;
        return *this;
    }
};



#endif /* IOP_VEC_H */
