#ifndef PLAYER_H_EXCEPTION_H
#define PLAYER_H_EXCEPTION_H

#include <exception>
#include <string>

class DeckFileNotFound : public std::exception {

public:
    const char *what() const noexcept override {
            return "Deck File Error: File not found\n";
    }
};

class DeckFileFormatError : public std::exception {

    std::string lineNumberString;

public:

    DeckFileFormatError(int lineNumber) {
        lineNumberString = "Deck File Error: File format error in line ";
        std::string add = std::to_string(lineNumber);
        lineNumberString.append(add);
    };

    const char *what() const noexcept override {
            return lineNumberString.c_str();
    }
};

class DeckFileInvalidSize : public std::exception {

public:
    const char *what() const noexcept override {
            return "Deck File Error: Deck size is invalid";
    }
};

#endif //PLAYER_H_EXCEPTION_H
