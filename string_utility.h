#ifndef ASH_STRING_UTILITY_H_INCLUDED
#define ASH_STRING_UTILITY_H_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <set>
#include <sstream>

namespace ash
{
// Char Methods
    int digit(char c);
    bool is_digit(char c);
    bool is_letter(char c);
    bool is_lower(char c);
    bool is_upper(char c);
    char lower(char c);
    bool try_convert_to_digit(char c, int &out);
    char upper(char c);

// Split
    std::vector<std::string> split(const std::string &, char);
    std::vector<std::string> split(const std::string &, const std::vector<char> &);
    std::vector<std::string> split(const std::string &, std::string);
    void quick_split(const std::string &, const std::vector<char> &, std::vector<std::string> &, char, char);

// String Methods From Python
    std::string capitalize(const std::string &str);
    std::string center(const std::string &str, size_t final_size, char fill_char);
    size_t count(const std::string &str, const std::string &sub, size_t begin = 0, size_t end = 0);
    bool ends_with(const std::string &str, const std::string &suffix, size_t begin = 0, size_t end = 0);
    size_t find(const std::string &str, const std::string &sub, size_t begin = 0, size_t end = 0);
    size_t index(const std::string &str, const std::string &sub, size_t begin = 0, size_t end = 0);
    bool is_alnum(const std::string &str);
    bool is_alpha(const std::string &str);
    bool is_ascii(const std::string &str);
    bool is_decimal(const std::string &str);
    
    bool is_keyword(const std::string &str); // It should become compiler dependant
    std::string join(const std::vector<std::string> &input, const std::string &delimiter);
}

int ash::digit(char c)
{
    if (!is_digit(c))
        throw std::invalid_argument("Cannot convert non-digit character to int");
    
    return c - '0';
}

bool ash::is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

bool ash::is_letter(char c)
{
    if (c >= 'A' && c <= 'Z')
        return true;
    else if (c >= 'a' && c <= 'z')
        return true;
    
    return false;
}

bool ash::is_lower(char c)
{
    return (c >= 'a' && c <= 'z');
}

bool ash::is_upper(char c)
{
    return (c >= 'A' && c <= 'Z');
}

char ash::lower(char c)
{
    if (is_lower(c))
        return c;
    return c + 'a' - 'A';
}

bool ash::try_convert_to_digit(char c, int &out)
{
    if (is_digit(c))
    {
        out = c - '0';
        return true;
    }
    return false;
}

char ash::upper(char c)
{
    if (is_upper(c))
        return c;
    return c - 'a' + 'A';
}

/** Split method to split the given string using the specified delimiter.
 */
std::vector<std::string> ash::split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    if (str.size() == 0)
        return result;

    std::string current;
    current.reserve(str.size());

    for (char c : str)
    {
        if (delimiter == c)
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);

    return result;
}

/** Split method to split the given string using the specified delimiters.
 * 
 * This method uses 'binary search' and 'set' structure, so it is suitable when the delimiters are long distant from each other.
 * If the delimiters are close to each other in ASCII table, please use `quick_split()`.
 */
std::vector<std::string> ash::split(const std::string &str, const std::vector<char> &delimiters)
{
    std::vector<std::string> result;
    if (str.size() == 0)
        return result;
    if (delimiters.empty())
        return std::vector<std::string> {str};

    std::set<char> delimiters_set(delimiters.begin(), delimiters.end());
    
    std::string current;
    current.reserve(str.size());

    for (char c : str)
    {
        if (delimiters_set.count(c) == 1)
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);

    return result;
}

/** Split method to split the given string using the specified delimiters.
 * Each character in the `delimiters` argument is considered an delimiter.
 * 
 * This method uses 'binary search' and 'set' structure, so it is suitable when the delimiters are long distant from each other.
 * If the delimiters are close to each other in ASCII table, please use `quick_split()`.
 */
std::vector<std::string> ash::split(const std::string &str, std::string delimiters)
{
    return split(str, std::vector<char>(delimiters.begin(), delimiters.end()));
}

/** Split method to split the given string using the specified delimiters.
 * Returns the result using a reference argument: `result`.
 * 
 * This method builds an boolean array for all possible delimiters in the range of [begin, end).
 * If the delimiters are far from each other in ASCII table, please use `split()` to avoid high memory usage.
 */
void ash::quick_split(const std::string &str, const std::vector<char> &delimiters, std::vector<std::string> &result, char begin, char end)
{
    if (str.size() == 0)
        return;
    if (delimiters.empty())
    {
        result.push_back(str);
        return;
    }
    
    // Using std::vector<char> instead of std::vector<bool> because std::vector<bool>
    // takes more time to access memory as it stores the values as "bits" rather than "bytes".
    std::vector<char> is_delimiter(end - begin, false);
    for (char delimiter : delimiters)
    {
        if (delimiter < begin || delimiter >= end)
            throw std::invalid_argument("Delimiter was out of range:\nDelimiter: " + std::string{delimiter});
        is_delimiter[delimiter - begin] = true;
    }
    
    std::string current;
    current.reserve(str.size());
    for (char c : str)
    {
        if (c >= begin && c < end && is_delimiter[c - begin])
        {
            result.push_back(current);
            current.clear();
        }
        else
            current += c;
    }
    result.push_back(current);
}

std::string ash::capitalize(const std::string &str)
{
    if (str.size() == 0)
        return str;
    
    std::string result = str;
    result[0] = upper(result[0]);
    
    for (size_t i = 1; i < str.size(); ++i)
        result[i] = lower(result[i]);
    
    return result;
}

std::string ash::center(const std::string &str, size_t final_size, char fill_char = ' ')
{
    if (final_size <= str.size())
        return str;

    std::ostringstream oss;

    const size_t fill_char_count = final_size - str.size();
    const size_t fill_char_left = fill_char_count / 2;
    const size_t fill_char_right = (fill_char_count + 1) / 2;

    for (size_t i = 0; i < fill_char_left; ++i)
        oss << fill_char;
    
    oss << str;
    
    for (size_t i = 0; i < fill_char_right; ++i)
        oss << fill_char;
    
    return oss.str();
}

size_t ash::count(const std::string &str, const std::string &sub, size_t begin, size_t end)
{
    if (end == 0)
        end = str.size();
    
    if (end > str.size() || end < begin)
        throw std::invalid_argument("Invalid Range.");
    if (sub.size() == 0 || str.size() == 0)
        return 0;
    if (end < sub.size())
        return 0;


    size_t count = 0;
    for (size_t i = begin; i < end - sub.size(); ++i)
    {
        bool found = true;
        for (size_t j = 0; j < sub.size(); ++j)
            if (str[i + j] != sub[j])
            {
                found = false;
                break;
            }
        
        if (found)
        {
            ++count;
            i += sub.size() - 1;
        }
    }

    return count;
}

bool ash::ends_with(const std::string &str, const std::string &suffix, size_t begin, size_t end)
{
    if (end == 0)
    end = str.size();

    if (end > str.size() || end < begin)
        throw std::invalid_argument("Invalid Range.");
    if (str.size() == 0)
        return false;
    if (suffix.size() == 0)
        return true;
    if (suffix.size() > end - begin)
        return false;

    size_t suffix_index = suffix.size() - 1;
    for (size_t i = end - 1; i >= begin; --i)
    {
        if (str[i] != suffix[suffix_index])
            return false;
        
        --suffix_index;
        if (i == 0)
            break;
    }

    return true;
}

size_t ash::find(const std::string &str, const std::string &sub, size_t begin, size_t end)
{
    if (end == 0)
        end = str.size();

    if (end > str.size() || end < begin)
        throw std::invalid_argument("Invalid Range.");
    if (sub.size() == 0 || str.size() == 0)
        return 0;
    if (end < sub.size())
        return 0;

    for (size_t i = begin; i < end - sub.size(); ++i)
    {
        bool found = true;
        for (size_t j = 0; j < sub.size(); ++j)
            if (str[i + j] != sub[j])
            {
                found = false;
                break;
            }
        
        if (found)
        {
            return i;
            i += sub.size() - 1;
        }
    }

    return end;
}

size_t ash::index(const std::string &str, const std::string &sub, size_t begin, size_t end)
{
    if (end == 0)
        end = str.size();

    size_t result = find(str, sub, begin, end);
    if (result == end)
        return result;
    else
        throw std::invalid_argument("Expected to find the substring.");
}

bool ash::is_alnum(const std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i)
        if (!is_letter(str[i]) || !is_digit(str[i]))
        return false;

    return true;
}

bool ash::is_alpha(const std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i)
        if (!is_letter(str[i]))
            return false;
    
    return true;
}

bool ash::is_ascii(const std::string &str)
{
    for (unsigned char c : str)
        if (c > 127)
            return false;
    
    return true;
}

bool ash::is_decimal(const std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i)
        if (!is_digit(str[i]))
            return false;
    
    return true;
}

bool ash::is_keyword(const std::string &str)
{
    const static std::string keywords[] // Sorted
    {
        "alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto",
        "bitand", "bitor", "bool", "break",
        "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval",
            "constexpr", "constinit", "const_cast", "continue", "contract_assert", "co_await", "co_return", "co_yield",
        "decltype", "default", "delete", "do", "double", "dynamic_cast",
        "else", "enum", "explicit", "export", "extern",
        "false", "float", "for", "friend",
        "goto",
        "if",
        "inline", "int", "long",
        "mutable",
        "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
        "operator", "or", "or_eq",
        "private", "protected", "public",
        "reflexpr", "register", "reinterpret_cast", "requires", "return",
        "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized",
        "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename",
        "union", "unsigned", "using", "virtual", "void", "volatile",
        "wchar_t", "while", "xor", "xor_eq"
    };

    size_t size = sizeof(keywords) / sizeof(std::string);
    return std::binary_search(keywords, keywords + size, str);
}

std::string ash::join(const std::vector<std::string> &input, const std::string &delimiter)
{
    if (input.empty())
        return "";

    size_t end_size = delimiter.size() * (input.size() - 1);
    for (const std::string &element : input)
        end_size += element.size();

    std::ostringstream oss;
    oss << input[0];

    for (size_t i = 1; i < input.size(); ++i)
        oss << delimiter << input[i];

    return oss.str();
}

#endif