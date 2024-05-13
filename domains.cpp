#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(std::string_view domain) : inverse_domain_(domain) {
        std::reverse(inverse_domain_.begin(), inverse_domain_.end());
        length_ = inverse_domain_.size();
    }
    
    bool operator==(const Domain &other) const {
        return inverse_domain_ == other.inverse_domain_;
    }

    bool IsSubdomain(const Domain &other) const {
        return length_ >= other.length_ && inverse_domain_.find(other.inverse_domain_) == 0;
    }

    std::string_view GetInverseDomain() const {
        return inverse_domain_;
    }

private:
    std::string inverse_domain_;
    int length_;
};

int GetCharIndex(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    } else if (c == '.') {
        return 26;
    } else {
        return -1;
    }
}

class DomainChecker {
public:
    template<typename It>
    DomainChecker(It begin, It end) {
        for (auto it = begin; it != end; ++it) {
            AddDomain(*it);
        }
    }

    [[nodiscard]] bool IsForbidden(const Domain &domain) const {
        const Node *current = &root_;
        for (char c: domain.GetInverseDomain()) {
            int index = GetCharIndex(c);
            if (index == -1) {
                continue;
            }
            if (current->children[index] != nullptr) {
                current = current->children[index];
                if (current->symbol == '.' and current->is_last) {
                    return true;
                }
            } else {
                return false;
            }
        }
        return current->is_last;
    }

private:
    class Node {
    public:
        Node() : symbol(' '), is_last(false) {
        }

        explicit Node(char symbol) : symbol(symbol), is_last(false) {
        }

        char symbol;
        bool is_last;
        // массив на 27 элементов, чтобы уместить все латинские буквы и точку
        std::array<Node*, 27> children = {};
    };



    void AddDomain(const Domain &domain) {
        Node *current = &root_;
        for (char c: domain.GetInverseDomain()) {
            int index = GetCharIndex(c);
            if (index == -1) {
                continue;
            }
            if (current->children[index] == nullptr) {
                current->children[index] = new Node(c);
                current = current->children[index];
            } else {
                current = current->children[index];
            }

        }
        current->is_last = true;
        if (current->children[26] == nullptr) {
            current->children[26] = new Node('.');
        }
        current = current->children[26];
        current->is_last = true;
    }

    Node root_ = Node();
};

std::vector<Domain> ReadDomains(istream &input, size_t count) {
    std::vector<Domain> domains;
    for (size_t i = 0; i < count; ++i) {
        string domain;
        getline(input, domain);
        domains.emplace_back(domain);
    }
    return domains;
}

template<typename Number>
Number ReadNumberOnLine(istream &input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain &domain: test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
