#ifndef FBXPROPERTY_H
#define FBXPROPERTY_H

#include <memory>
#include <iostream>
#include <vector>

namespace fbx {

// WARNING: (copied from fbxutil.h)
// this assumes that float is 32bit and double is 64bit
// both conforming to IEEE 754, it does not assume endianness
// it also assumes that signed integers are two's complement
union FBXPropertyValue {
    int16_t i16;  // Y
    bool boolean; // C, b
    int32_t i32;  // I, i
    float f32;    // F, f
    double f64;   // D, d
    int64_t i64;  // L, l

};

class FBXProperty
{
public:
    FBXProperty(std::ifstream &input);
    // primitive values
    FBXProperty(int16_t);
    FBXProperty(bool);
    FBXProperty(int32_t);
    FBXProperty(float);
    FBXProperty(double);
    FBXProperty(int64_t);
    // arrays
    FBXProperty(const std::vector<bool>);
    FBXProperty(const std::vector<int32_t>);
    FBXProperty(const std::vector<float>);
    FBXProperty(const std::vector<double>);
    FBXProperty(const std::vector<int64_t>);
    // raw / string
    FBXProperty(const std::vector<uint8_t>, uint8_t type);
    FBXProperty(const std::string);
    FBXProperty(const char *);

    void write(std::ofstream &output);

    std::string to_string();
    char getType();

    bool is_array();
    uint32_t getBytes();
    /**
     * Helper function for casting values to float. Undefined behaviour for large numbers.
     * @return
     */
    float cast_float() {
        return cast_value<float>(this->value, this->type);
    }

    float cast_float(int index) {
        return cast_value<float>(this->values[index], this->type);
    }

    std::string cast_string();

    /**
     * Helper function for casting values to 32 bit integer. Undefined behaviour for large numbers.
     * @return
     */
    int cast_int() {
        return cast_value<int>(this->value, this->type);
    }
    int cast_int(int index) {
        return cast_value<int>(this->values[index], this->type);
    }

    /**
     * Helper function for casting to bool.
     * @return
     */
    bool cast_bool(FBXPropertyValue value) {
        return cast_value<bool>(this->value, this->type);
    }

    bool cast_bool(int index) {
        return cast_value<bool>(this->values[index], this->type);
    }

    std::vector<FBXPropertyValue> values;

private:
    uint8_t type;
    FBXPropertyValue value;
    std::vector<uint8_t> raw;

    template<typename Type>
    static Type cast_value(FBXPropertyValue value, const uint8_t type) {
        if (type == 'Y') {
            return (Type)value.i16;
        } else if (type == 'C' || type == 'b') {
            return (Type)value.boolean;
        } else if (type == 'I' ||type == 'i') {
            return (Type)value.i32;
        } else if (type == 'F' || type == 'f') {
            return (Type)value.f32;
        } else if (type == 'D' || type == 'd') {
            return (Type)value.f64;
        } else if (type == 'L' || type == 'l') {
            return (Type)value.i64;
        }
        return Type();
    }
};

} // namespace fbx

#endif // FBXPROPERTY_H
