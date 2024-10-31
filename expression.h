#ifndef ASH_EXPRESSION_H_INCLUDED
#define ASH_EXPRESSION_H_INCLUDED

#include <string>
#include <stdexcept>
#include <functional>
#include <math.h>
#include <unordered_map>
#include "stack.h"

#define INFIX 0
#define POSTFIX 1
#define PREFIX 2

namespace ash
{
    enum expressionType
    {
        infix,
        prefix,
        postfix
    };

    template <expressionType type>
    class expression;
}

typedef ash::expression<ash::infix> infix_expression;
typedef ash::expression<ash::postfix> postfix_expression;
typedef ash::expression<ash::prefix> prefix_expression;

template <ash::expressionType type>
class ash::expression
{
private:
    static const constexpr char operators[5]{'-', '+', '/', '*', '^'};
    std::string text;

public:
    // Constructors
    expression()
    {
        text = "";
    }
    expression(const std::string &text)
    {
        this->text = text;
    }
    expression(const expression &other)
    {
        this->text = other.text;
    }

    // Element Access
    char operator[](int index) const
    {
        if (index >= text.size())
        {
            throw std::out_of_range("Access out of range.");
        }
        return text[index];
    }
    std::string to_string() const
    {
        return text;
    }
    expressionType get_type() const
    {
        return type;
    }

    // Converters - To do
    expression<postfix> to_postfix() const
    {
#if type == INFIX
        return infix_to_postfix(*this);
#endif

#if type == PREFIX
        return prefix_to_postfix(*this);
#endif

#if type == POSTFIX
        return *this;
#endif
    }

    // Static Methods
    static bool validate(const expression<infix> &infixExp)
    {
        stack<char> temp;
        for (char c : infixExp.text)
        {
            switch (c)
            {
            case '(':
            case '{':
            case '[':
                temp.push(c);
                break;

            case ')':
            case '}':
            case ']':
                if (temp.pop() != c)
                    throw std::invalid_argument("Unexpected end of scope.");
                break;

            default:
                break;
            }
        }
    }
    static expression<postfix> infix_to_postfix(const expression<infix> &infixExpression)
    {
        std::string postfixText = "";
        stack<char> tempStack(infixExpression.text.size());

        for (int i = 0; i < infixExpression.text.size(); i++)
        {
            if (infixExpression[i] == '(')
            {
                tempStack.push('(');
                continue;
            }
            if (infixExpression[i] == ')')
            {
                empty_stack_to_string(tempStack, postfixText, [](char top)
                                      { return top != '('; });
                tempStack.pop(); // pop the remaining left parantheses
                continue;
            }

            int p = priority(infixExpression[i]);
            if (p == -1)
            {
                postfixText += infixExpression[i];
            }
            else
            {
                empty_stack_to_string(tempStack, postfixText, [p](char top)
                                      { return priority(top) >= p; });
                tempStack.push(infixExpression[i]);
            }
        }

        empty_stack_to_string(tempStack, postfixText, [](char top)
                              { return true; });
        return expression<postfix>(postfixText);
    }
    // To do
    static expression<postfix> prefix_to_postfix(const expression<prefix> &prefixExpression)
    {
        return expression<postfix>(prefixExpression.text);
    }

private:
    // Private Static Methods
    static int priority(char c)
    {
        for (int i = 0; i < 5; i++)
        {
            if (c == operators[i])
            {
                return i / 2;
            }
        }
        return -1;
    }
    static void empty_stack_to_string(stack<char> &stk, std::string &str, const std::function<bool(char top)> &func)
    {
        while (stk.size() != 0 && func(stk.top()))
        {
            char top = stk.pop();
            if (top != '(' && top != ')')
            {
                str += top;
            }
        }
    }
    static bool is_letter(char c)
    {
        return (c - 'a' < 26 || c - 'A' < 26);
    }
};

static void make_null_in_range(std::string &str, char c, int start, int count)
{
    str[start] = c;
    for (int i = 1; i < count; i++)
    {
        str[start + i] = '\0';
    }
}
bool is_digit(char c)
{
    return (c - '0' < 10 && c - '0' >= 0);
}
float calculate(const postfix_expression &expression, std::unordered_map<char, float> &variables)
{
    ash::stack<float> values(variables.size());
    for (char c : expression.to_string())
    {
        if (c == '\0')
            continue;

        if (variables.find(c) != variables.end())
        {
            values.push(variables[c]);
            continue;
        }

        float y = values.pop();
        float x = values.pop();
        float result;
        switch (c)
        {
        case '+':
            result = x + y;
            break;
        case '-':
            result = x - y;
            break;

        case '*':
            result = x * y;
            break;
        case '/':
            result = x / y;
            break;

        case '^':
            result = std::pow(x, y);
            break;
        
        default:
            throw std::invalid_argument("Unexpected end of expression partition.");
        }

        values.push(result);
    }

    return values.top();
}
float calculate(const infix_expression &expression)
{
    std::string text = expression.to_string();
    std::unordered_map<char, float> variables;
    char tempVariable = 0300U;

    std::string number = "";
    for (int i = 0; i < text.size(); i++)
    {
        char c = text[i];

        if (is_digit(c) || c == '.')
            number += c;
        else if (number.size() != 0)
        {
            variables[tempVariable] = std::stof(number);

            make_null_in_range(text, tempVariable++, i - number.size(), number.size());
            i -= number.size();

            number = "";
        }
    }

    if (number.size() != 0)
    {
        variables[tempVariable] = std::stof(number);

        make_null_in_range(text, tempVariable, text.size() - number.size(), number.size());
    }

    std::string postfixText = infix_expression(text).to_postfix().to_string();
    postfix_expression postfixExp(postfixText);

    return calculate(postfixExp, variables);
}

// typedef ash::expression<ash::infix> infix_expression;
// typedef ash::expression<ash::postfix> postfix_expression;
// typedef ash::expression<ash::prefix> prefix_expression;

#endif