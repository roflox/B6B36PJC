#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <stack>

trie_node** currentNodePointer;

trie::trie(const std::vector<std::string> &strings) {
    for (const auto &word: strings) {
        insert(word);
    }
}

bool trie::erase(const std::string &str) {
    //nasetovaní
    if(!currentNodePointer&&str.length()==0){
        if(m_root->is_terminal){
            m_size--;
            m_root->is_terminal= false;
            return true;
        } else{
            return false;
        }
    }else if(!currentNodePointer){
        currentNodePointer = &m_root;
    }
    auto correspondingChild = (*currentNodePointer)->children[int(str[0])];
    //když to nikam nevede
    if(!correspondingChild&&str.length()!=0) {
        currentNodePointer = nullptr;
        return false;
    } else {
        //kdyz mi dosel string
        if(str.length()==0){
            if(!(*currentNodePointer)->is_terminal){
                currentNodePointer = nullptr;
                return false;
            } else {
                (*currentNodePointer)->is_terminal = false;
                m_size--;
                currentNodePointer = nullptr;
                return true;
            }
        } else {
            currentNodePointer = &correspondingChild;
            return erase(str.substr(1,str.length()-1));
        }
    }
}


bool trie::insert(const std::string &str) {
    if (!this->m_root) {
        m_root = new trie_node();
    }
    trie_node *currentNode = m_root;
    const char *sptr = str.c_str();
    if(str.length()==0){
        if(m_root->is_terminal){
            return false;
        }
        m_root->is_terminal= true;
        m_size++;
    }
    while (*sptr) {
        auto childPointer = &(currentNode->children[int(*sptr)]);
        if (!*childPointer) {
            *childPointer = new trie_node();
            (*childPointer)->parent = currentNode;
            (*childPointer)->payload = *sptr;
        }
        if (!*(sptr + 1)) {
            if ((*childPointer)->is_terminal) {
                return false;
            }
            (*childPointer)->is_terminal = true;
            m_size++;
            return true;
        }
        currentNode = *childPointer;
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
    if(m_root){
        std::stack<trie_node *> nodes;
        nodes.push(m_root);
        while (!nodes.empty()){
            auto currentNode = nodes.top();
            nodes.pop();
            for(auto child : currentNode->children){
                if(child){
                    nodes.push(child);
                }
            }
            delete currentNode;
        }
    }
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
    if(str.length()==0&&m_root->is_terminal){
        return true;
    }
    trie_node *current = m_root;
    while (*sptr) {
        auto childp = &(current->children[int(*sptr)]);
        if (!*childp) {
            break;
        } else {
            if (!*(sptr + 1) && (*childp)->is_terminal) {
                return true;
            }
            current = *childp;
        }
        sptr++;
    }
    return false;
}