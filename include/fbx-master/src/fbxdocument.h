#ifndef FBXDOCUMENT_H
#define FBXDOCUMENT_H

#include "fbxnode.h"
#include <unordered_map>
#include <vector>
namespace fbx {

class FBXDocument
{
public:
    FBXDocument();
    void read(std::ifstream &input);
    void read(std::string fname);
    void write(std::string fname);
    void write(std::ofstream &output);

    void createBasicStructure();

    std::vector<FBXNode> nodes;

    std::uint32_t getVersion();
    void print();
	void printGeometry();

	static std::unordered_map<std::string, std::vector<std::string>> extract_links(const FBXDocument& document) {
        std::unordered_map<std::string,std::vector<std::string>> links;
        for (fbx::FBXNode node : document.nodes) {
            if (node.getName() == "Connections") {
                for (fbx::FBXNode& child : node.getChildren("C")) {
                    if (!links.contains(child.getProperties()[2].to_string())) {
                        links[child.getProperties()[2].to_string()] = std::vector<std::string>();
                    }
                    links[child.getProperties()[2].to_string()].push_back(child.getProperties()[1].to_string());
                }
            }
        }
        return links;
	}

private:
    std::uint32_t version;
};

} // namespace fbx

#endif // FBXDOCUMENT_H
