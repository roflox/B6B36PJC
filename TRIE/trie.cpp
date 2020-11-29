#include "trie.hpp"

#include <utility>
#include <algorithm>
#include <cassert>
#include <stack>
#include <set>
#include <iostream>

trie_node *currentNodePointer;
using namespace std;

//420 line just for fun

trie::trie(const std::vector<std::string> &strings) {
    for (const auto &word: strings) {
        insert(word);
    }
}

bool trie::erase(const std::string &str) {
    //nasetovaní
    if (!currentNodePointer && str.length() == 0) {
        if (m_root->is_terminal) {
            m_size--;
            m_root->is_terminal = false;
            return true;
        } else {
            return false;
        }
    } else if (!currentNodePointer) {
        currentNodePointer = m_root;
    }
    auto correspondingChild = (currentNodePointer)->children[int(str[0])];
    //když to nikam nevede
    if (!correspondingChild && str.length() != 0) {
        currentNodePointer = nullptr;
        return false;
    } else {
        //kdyz mi dosel string
        if (str.length() == 0) {
            if (!(currentNodePointer)->is_terminal) {
                currentNodePointer = nullptr;
                return false;
            } else {
                m_size--;
                (currentNodePointer)->is_terminal = false;
                bool doLoop = (currentNodePointer)->is_terminal;
                bool hasChildren;
                trie_node *parent;
                //deletion if branch is empty
                while (!doLoop) {
                    hasChildren = false;
                    int index = int(currentNodePointer->payload);
                    for (auto child: (currentNodePointer)->children) {
                        if (child != nullptr) {
                            hasChildren = true;
                            break;
                        }
                    }
                    if (!hasChildren && currentNodePointer != m_root) {
                        parent = (currentNodePointer)->parent;
                        delete currentNodePointer;
                        parent->children[index] = nullptr;
                        currentNodePointer = parent;
                        doLoop = (currentNodePointer)->is_terminal;
                    } else {
                        doLoop = true;
                    }
                }
                currentNodePointer = nullptr;
                return true;
            }
        } else {
            currentNodePointer = correspondingChild;
            return erase(str.substr(1, str.length() - 1));
        }
    }
}


bool trie::insert(const std::string &str) {
    if (!this->m_root) {
        m_root = new trie_node();
    }
    trie_node *currentNode = m_root;
    const char *sptr = str.c_str();
    if (str.length() == 0) {
        if (m_root->is_terminal) {
            return false;
        }
        m_root->is_terminal = true;
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

trie::trie(const trie &rhs) {
    m_root = new trie_node();
    vector<string> words = {rhs.begin(), rhs.end()};
    for (const string &word : words) {
        insert(word);
    }
}

trie::trie(trie &&rhs) {
    m_root = new trie_node();
    vector<string> words = {rhs.begin(), rhs.end()};
    for (const string &word : words) {
        insert(word);
    }
}

trie::trie() {
    m_root = new trie_node();
    m_size = 0;
}

trie::~trie() {
    if (m_root) {
        std::stack<trie_node *> nodes;
        nodes.push(m_root);
        while (!nodes.empty()) {
            auto currentNode = nodes.top();
            nodes.pop();
            for (auto child : currentNode->children) {
                if (child) {
                    nodes.push(child);
                }
            }
            delete currentNode;
        }
    }
}

size_t trie::size() const {
    return m_size;
}

bool trie::empty() const {
    return this->m_size == 0;
}

bool trie::contains(const std::string &str) const {
    const char *sptr = str.c_str();
    if (str.length() == 0 && m_root->is_terminal) {
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

trie::const_iterator trie::begin() const {
    if (m_size == 0) {
        return trie::const_iterator();
    }
    currentNodePointer = m_root;
    while (true) {
        if (currentNodePointer->is_terminal) {
            auto tmpNode = currentNodePointer;
            currentNodePointer = nullptr;
            return trie::const_iterator(tmpNode);
        }
        for (auto child: currentNodePointer->children) {
            if (child != nullptr) {
                currentNodePointer = child;
                break;
            }
        }
    }
}

trie::const_iterator trie::end() const {
    return nullptr;
}

trie &trie::operator=(const trie &rhs) {
    if (this->m_root != rhs.m_root) {
        this->~trie();
        this->m_root = new trie_node;
        this->m_size = 0;
        vector<string> words = {rhs.begin(), rhs.end()};
        for (const string &word : words) {
            insert(word);
        }
    }
    return *this;
}

trie &trie::operator=(trie &&rhs) {
    if (this->m_root != rhs.m_root) {
        this->~trie();
        this->m_root = new trie_node;
        this->m_size = 0;
        vector<string> words = {rhs.begin(), rhs.end()};
        for (const string &word : words) {
            insert(word);
        }
    }
    return *this;
}

void trie::swap(trie &rhs) {
    auto tmpNode = m_root;
    auto tmpSize = m_size;
    m_root = rhs.m_root;
    m_size = rhs.m_size;
    rhs.m_root = tmpNode;
    rhs.m_size = tmpSize;
}

bool trie::operator==(const trie &rhs) const {
    vector<string> wordsT1 = {this->begin(), this->end()};
    vector<string> wordsT2 = {rhs.begin(), rhs.end()};
    return wordsT1 == wordsT2;
}

bool trie::operator<(const trie &rhs) const {
    vector<string> wordsT1 = {this->begin(), this->end()};
    vector<string> wordsT2 = {rhs.begin(), rhs.end()};
    return !(wordsT1 == wordsT2) &&
           lexicographical_compare(wordsT1.begin(), wordsT1.end(), wordsT2.begin(), wordsT2.end());
}

//todo
trie trie::operator&(const trie &rhs) const {
    vector<string> words = {rhs.begin(), rhs.end()};
    vector<string> intersectingWords;
    for (auto word:words) {
        if (contains(word)) {
            intersectingWords.push_back(word);
        }
    }
    trie tmpTrie = trie(intersectingWords);
    return tmpTrie;
}

//todo
trie trie::operator|(const trie &rhs) const {
    vector<string> words = {rhs.begin(), rhs.end()};
    vector<string> words2 = {this->begin(), this->end()};
    words.insert(words.end(), make_move_iterator(words2.begin()), make_move_iterator(words2.end()));
    trie tmpTrie = trie(words);
    return tmpTrie;
}

std::vector<std::string> trie::search_by_prefix(const string &prefix) const {
    vector<string> words = {this->begin(), this->end()};
    vector<string> ret;
    for (const auto &word: words) {
        if (word.substr(0, prefix.length()) == prefix) {
            ret.push_back(word);
        }
    }
    return ret;
}

std::vector<std::string> trie::get_prefixes(const string &str) const {
    vector<string> words = {this->begin(), this->end()};
    vector<string> ret;
    for (auto const word:words) {
        if (!str.find(word)) {
            ret.push_back(word);
        }
    }
    return ret;
}

trie::const_iterator::const_iterator(const trie_node *node) {
    this->current_node = node;
}

//prefix
trie::const_iterator &trie::const_iterator::operator++() {
    bool goDown = true;
    bool goUp = false;
    while (goDown) {
        goDown = true;
        bool hasChildren = false;
        for (auto child: current_node->children) {
            if (child != nullptr) {
                current_node = child;
                hasChildren = true;
                if (current_node->is_terminal) {
                    goDown = false;
                    goUp = false;
                }
                break;
            }
        }
        if (!hasChildren) {
            goUp = true;
        }
        while (goUp) {
            if (current_node->parent == nullptr) {
                goUp = false;
                goDown = false;
                current_node = nullptr;
                break;
            }
            for (int i = int(current_node->payload); i++ < 127;) {
                auto child = current_node->parent->children[i];
                if (child != nullptr) {
                    current_node = current_node->parent->children[i];
                    if (current_node->is_terminal) {
                        goDown = false;
                    }
                    goUp = false;
                    break;
                }
            }
            if (goUp) {
                current_node = current_node->parent;
            }
        }
    }
    return *this;
}

//postfix
trie::const_iterator trie::const_iterator::operator++(int) {
    trie::const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

trie::const_iterator::reference trie::const_iterator::operator*() const {
    vector<char> chars;
    auto tmpNode = current_node;
    string ret;
    while (tmpNode->parent != nullptr) {
        ret.insert(ret.begin(), tmpNode->payload);
        tmpNode = tmpNode->parent;
    }
    return ret;
}

bool trie::const_iterator::operator==(const trie::const_iterator &rhs) const {
    return current_node == rhs.current_node;
}

bool trie::const_iterator::operator!=(const trie::const_iterator &rhs) const {
    return current_node != rhs.current_node;
}


//todo
//! 2 trie jsou si nerovné právě tehdy, když si nejsou rovné (viz operator==)
bool operator!=(const trie &lhs, const trie &rhs) {
    return !(lhs == rhs);
};

//! Lexicografické uspořádání, viz operator<
bool operator<=(const trie &lhs, const trie &rhs) {
    return !(rhs < lhs);
};

//! Lexicografické uspořádání, viz operator<
bool operator>(const trie &lhs, const trie &rhs) {
    return rhs < lhs;
};

//! Lexicografické uspořádání, viz operator<
bool operator>=(const trie &lhs, const trie &rhs) {
    return !(lhs < rhs);
};


/**
 * ADL customization point pro std::swap.
 * Výsledek `swap(lhs, rhs);` by měl být ekvivalentní s výsledkem
 * `lhs.swap(rhs);`
 */
void swap(trie &lhs, trie &rhs) {
    lhs.swap(rhs);
}


/**
 * Operátor výpisu do proudu.
 *
 * Tuto funkci netestujeme, ale pokud ji vhodně implementujete, budete mít
 * ve výstupech z testů užitěčně vypsaný obsah trie.
 */
std::ostream &operator<<(std::ostream &out, trie const &trie) {
    for(auto it = trie.begin(); it != trie.end(); ++it){
        out<<*it<<endl;
    }
    return out;
};
