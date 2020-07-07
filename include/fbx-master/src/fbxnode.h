#ifndef FBXNODE_H
#define FBXNODE_H

#include "fbxproperty.h"
#include <unordered_map>
//#include <hash_map>

namespace fbx {

class FBXNode
{
public:
    FBXNode();
    FBXNode(std::string name);

    std::uint32_t read(std::ifstream &input, uint32_t start_offset);
    std::uint32_t write(std::ofstream &output, uint32_t start_offset);
    void print(std::string prefix="");
	void printGeometry(std::string prefix = "");

    bool isNull();

    void addProperty(int16_t);
    void addProperty(bool);
    void addProperty(int32_t);
    void addProperty(float);
    void addProperty(double);
    void addProperty(int64_t);
    void addProperty(const std::vector<bool>);
    void addProperty(const std::vector<int32_t>);
    void addProperty(const std::vector<float>);
    void addProperty(const std::vector<double>);
    void addProperty(const std::vector<int64_t>);
    void addProperty(const std::vector<uint8_t>, uint8_t type);
    void addProperty(const std::string);
    void addProperty(const char*);
    void addProperty(FBXProperty);

    void addPropertyNode(const std::string name, int16_t);
    void addPropertyNode(const std::string name, bool);
    void addPropertyNode(const std::string name, int32_t);
    void addPropertyNode(const std::string name, float);
    void addPropertyNode(const std::string name, double);
    void addPropertyNode(const std::string name, int64_t);
    void addPropertyNode(const std::string name, const std::vector<bool>);
    void addPropertyNode(const std::string name, const std::vector<int32_t>);
    void addPropertyNode(const std::string name, const std::vector<float>);
    void addPropertyNode(const std::string name, const std::vector<double>);
    void addPropertyNode(const std::string name, const std::vector<int64_t>);
    void addPropertyNode(const std::string name, const std::vector<uint8_t>, uint8_t type);
    void addPropertyNode(const std::string name, const std::string);
    void addPropertyNode(const std::string name, const char*);
    std::vector<FBXProperty> getProperties() const {
        return this->properties;
    }

    void addChild(FBXNode child);
    bool hasChild(const char* child) {
        return this->m_children.contains(child);
    }
    uint32_t getBytes() const;

    std::vector<FBXNode>& getChildren(const char* nodename);
    const std::string getName();
    std::vector<FBXProperty> properties;
private:
    std::unordered_map<std::string, std::vector<FBXNode>> m_children;
//    std::vector<FBXNode> children;
    std::string name;
};

} // namespace fbx

#endif // FBXNODE_H
