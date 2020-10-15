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
    const char *sptr = str.c_str();

    while (*sptr) {
        auto childp = &(current->children[int(*sptr)]);
        if (!*childp) {
            *childp = new trie_node();
            (*childp)->parent = current;
            (*childp)->payload = *sptr;
        }
        if(!*(sptr+1)){
            if((*childp)->is_terminal){
                return false;
            }
            (*childp)->is_terminal = true;
            m_size++;
            return true;
        }
        current = *childp;
        sptr++;
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
    const char *sptr = str.c_str();
    trie_node *current = m_root;
    while (*sptr) {
        auto childp = &(current->children[int(*sptr)]);
        if (!*childp) {
            break;
        } else {
            if(!*(sptr+1)&&(*childp)->is_terminal){
                return true;
            }
            current = *childp;
        }
        sptr++;
    }
    return false;
}