#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>

trie::trie(const std::vector<std::string> &strings) {
    for (const auto &word: strings) {
        insert(word);
    }
}

bool trie::erase(const std::string &str) {
    return true;
}

bool trie::insert(const std::string &str) {
    if (!this->m_root) {
        m_root = new trie_node();
    }
    trie_node *current = m_root;
    for (auto i = 0; i < str.length(); i++) {
        int childIndex = int(str[i]);
        trie_node *tmpChild = current->children[childIndex];
        if (!tmpChild) {
            tmpChild = new trie_node();
            tmpChild->payload = str[i];
            tmpChild->parent = current;
            current->children[childIndex] = tmpChild;
        }
        if (i == str.length() - 1) {
            if(current->children[childIndex]->is_terminal){
                return false;
            } else{
                current->children[int(str[i])]->is_terminal = true;
                this->m_size++;
            }
        }
        current = tmpChild;
    }
    return true;
}

trie::trie() {
    m_root = new trie_node();
    m_size = 0;
}

trie::trie(const trie &rhs) {

}

trie::~trie() {

}

trie::trie(trie &&rhs) {

}

size_t trie::size() const {
    return m_size;
}

bool trie::empty() const {
    return this->m_size == 0;
}

bool trie::contains(const std::string &str) const {
    if(m_size==0 || str.size()==0){
        return false;
    }
    trie_node* tmpNode = m_root;
    for (auto i = 0; i < str.length(); i++) {
        int childIndex = int(str[i]);
        if(tmpNode->children[childIndex]){
            if(i==str.length()-1&&tmpNode->children[childIndex]->is_terminal){
                return true;
            } else{
                tmpNode = tmpNode->children[childIndex];
            }
        } else{
            break;
        }
    }
    return false;
}