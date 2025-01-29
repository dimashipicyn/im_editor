#pragma once

#include <vector>
#include <string_view>

template <class T>
T CalcNumberOfDigits(T num)
{
    T result = 0;
    do
    {
        result++;
    } while ((num /= 10));

    return result;
}

inline std::vector<std::string_view> Split(std::string_view str, char delim)
{
    std::vector<std::string_view> result;
    size_t start = 0;
    size_t end = str.find(delim);

    while (end != std::string_view::npos) {
        result.emplace_back(str.substr(start, end - start));
        start = end + 1; // Move past the delimiter
        end = str.find(delim, start);
    }

    // Add the last substring
    result.emplace_back(str.substr(start, end));

    return result;
}