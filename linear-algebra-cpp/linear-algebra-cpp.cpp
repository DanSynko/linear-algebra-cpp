#include <iostream>


struct Vector3 {
    using component = float;
    component x, y, z;

    float get_length() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }


    const Vector3 operator+(const Vector3& vec) const {
        return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
    }
    const Vector3 operator-(const Vector3& vec) const {
        return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
    }


    float dot_product(const Vector3& other_vec) const {
        return (this->x * other_vec.x) + (this->y * other_vec.y) + (this->z * other_vec.z);
    }
    Vector3 cross_product(const Vector3& vec) {
        return Vector3(
            this->y * vec.z - this->z * vec.y,
            this->z * vec.x - this->x * vec.z,
            this->x * vec.y - this->y * vec.x
        );
    }
    void normalize() {
        float length = get_length();
        x /= length;
        y /= length;
        z /= length;
    }



    
    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
        os << "{ " << vec.x << ", " << vec.y << ", " << vec.z << " }";
        return os;
    }
};

int main()
{
    Vector3 vec = { 1, 5, 3 };
    Vector3 vec2 = { 4, 2, 5 };

    
    std::cout << "sum of vectors:" << std::endl;
    std::cout << vec + vec2 << std::endl;
    std::cout << "\n" << std::endl;

    std::cout << "subtraction of vectors:" << std::endl;
    std::cout << vec - vec2 << std::endl;
    std::cout << "\n" << std::endl;

    std::cout << "length of vec:" << std::endl;
    std::cout << vec.get_length() << std::endl;
    std::cout << "\n" << std::endl;

    std::cout << "dot product of vec:" << std::endl;
    std::cout << vec.dot_product(vec2) << std::endl;
    std::cout << "\n" << std::endl;

    std::cout << "cross product:" << std::endl;
    std::cout << vec.cross_product(vec2) << std::endl;

    return 0;
}