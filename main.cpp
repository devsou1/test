#include <iostream>
#include <vector>
#include <filesystem>
#include <functional>

namespace fs = std::filesystem;

struct node;

struct node {
    std::string name;

    /// дочерние узлы (листья)
    /// листья могут быть как директориями так и файлами
    std::vector<node> leafs;
};


static void remove(
        node& node_,
        const std::function<bool(node&)>& checker_){

    if (node_.leafs.empty())
        return;

    auto& leafs = node_.leafs;
    // удалим листья, которые не подходят по условиях указанным вот методе checker_
    const auto first = std::remove_if(leafs.begin(), leafs.end(), checker_);
    // теперь удалим оставщиеся узлы
    leafs.erase(first, leafs.end());

    // теперь для каждого оставшегося узла, зайдем внутрь,
    for (auto leaf: leafs) {
        remove(leaf, checker_);
    }
}


int main() {
    node tree;
    // создадим первые листья
    tree.leafs.push_back(node{.name="1"});
    tree.leafs.push_back(node{.name="2.delete"});
    tree.leafs.push_back(node{.name="3"});

    // добавим пару листьев, для листьев
    tree.leafs[0].leafs.push_back(node{.name="1"});
    tree.leafs[0].leafs.push_back(node{.name="2.delete"});
    tree.leafs[0].leafs.push_back(node{.name="3"});

    auto rm = [](node& node_){
        return node_.name.find(".delete") != std::string::npos;
    };

    remove(tree, rm);

    auto stop = 1;

    return 0;
}
