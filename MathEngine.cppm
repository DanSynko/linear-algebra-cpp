export module MathEngine;
import <cmath>;

export namespace MathEngine {

    struct Vector3 {
        using component = float;
        component x, y, z;

        float get_length() const {
            return sqrt((x * x) + (y * y) + (z * z));
        }


        Vector3 operator+(const Vector3& vec) const noexcept {
            return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
        }
        Vector3 operator-(const Vector3& vec) const noexcept {
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
    };
};