#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_set>

class BookService {
    using page = int;
    using number_of_users_read_till = int;
public:
    BookService() = default;

    void ParseCommand(std::istream &input) {
        std::string command;
        input >> command;
        if (command == "READ") {
            std::string user_id, page;
            input >> user_id >> page;
            AddReadEvent(user_id, page);
        } else if (command == "CHEER") {
            std::string user_id;
            input >> user_id;
            ProceedCheerEvent(user_id);
        }
    }

private:
    std::vector<page> users_read_till_;
    std::vector<number_of_users_read_till> number_of_users_read_till_;
    std::unordered_set<int> users_;

    void ReshapeVector(std::vector<int> &vector, int new_size) {
        if (new_size >= vector.size()) {
            vector.resize(new_size + 1, 0);
        }
    }

    std::vector<std::string> SplitCommand(std::string &command) {
        std::vector<std::string> result;
        std::string word;
        for (char c: command) {
            if (c == ' ') {
                result.push_back(word);
                word.clear();
            } else {
                word += c;
            }
        }
        result.push_back(word);
        return result;
    }

    void AddReadEvent(std::string &user_id, std::string &page) {
        auto user_id_int = std::stoi(user_id);
        auto page_int = std::stoi(page);
        users_.insert(user_id_int);
        ReshapeVector(users_read_till_, user_id_int);
        ReshapeVector(number_of_users_read_till_, page_int);
        auto old_page = users_read_till_[user_id_int];
        users_read_till_[user_id_int] = page_int;
        if (old_page < page_int) {
            for (int i = old_page + 1; i <= page_int; ++i) {
                number_of_users_read_till_[i]++;
            }
        }
    }

    void ProceedCheerEvent(std::string &user_id) {
        double result = 0;
        auto user_id_int = std::stoi(user_id);
        if (user_id_int > users_read_till_.size() || users_read_till_[user_id_int] == 0) {
            result = 0;
        } else if (users_.size() == 1) {
            result = 1;
        } else {
            auto less = number_of_users_read_till_[1] - number_of_users_read_till_[users_read_till_[user_id_int]];
            result =
                    static_cast<double>(less) / (users_.size() - 1);
        }
        std::cout << std::setprecision(6) << result << std::endl;
    }
};


int main() {
    BookService book_service;
    int number_of_commands;
    std::cin >> number_of_commands;
    for (int i = 0; i < number_of_commands; ++i) {
        book_service.ParseCommand(std::cin);
    }
    return 0;
}
