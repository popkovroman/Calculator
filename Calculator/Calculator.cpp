﻿#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>
#include <string>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

struct OpeningBracket {};
struct ClosingBracket {};
struct Number {
    int value;
};
struct UnknownToken {
    std::string value;
};
struct MinToken {};
struct AbsToken {};
struct Plus {};
struct Minus {};
struct Multiply {};
struct Divide {};
struct Modulo {};
struct MaxToken {};
struct SqrToken {};

using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, Plus, Minus, Multiply, Divide, Modulo, MaxToken, SqrToken>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token{
    {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}},  {'(', OpeningBracket{}}, {')', ClosingBracket{}}
};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size()) {
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{ value };
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string name;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isalpha(symbol)) {
        name.push_back(symbol);
        if (pos == input.size()) {
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    if (name == "min") {
        return MinToken{};
    }
    else if (name == "abs") {
        return AbsToken{};
    }
    else if (name == "max") {
        return MaxToken{};
    }
    else if (name == "sqr") {
        return SqrToken{};
    }

    return UnknownToken{ name };
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;
    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
            ++pos;
        }
        else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        }
        else if (std::isalpha(symbol)) {
            tokens.emplace_back(ParseName(input, pos));
        }
        else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        }
    }
    return tokens;
}

int main() {
    std::string input = "()12312412 next min abs + - * / % sqr max 321";
    std::vector<Token> tokens = Tokenize(input);
    std::cout << "123";
    return 0;
}