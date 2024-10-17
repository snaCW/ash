#ifndef ASH_EXPRESSION_H_INCLUDED
#define ASH_EXPRESSION_H_INCLUDED

#include <string>
#include <stdexcept>
#include <functional>
#include "stack.h"

namespace ash
{
    class expression;
}

class ash::expression
{
    public:
        enum type
        {
            infix, prefix, postfix
        };

    private:
        static const constexpr char operators[5] {'-', '+', '/', '*', '^'};
        std::string text;
        type expressionType;
    
    public:
    // Constructors
        expression()
        {
            text = "";
            expressionType = infix;
        }
        expression(const std::string& text, type expressionType)
        {
            this->text = text;
            this->expressionType = expressionType;
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
        type get_type() const
        {
            return expressionType;
        }
    
    // Converters - To do
        expression to_postfix() const
        {
            switch (expressionType)
            {
                case infix:
                    return infix_to_postfix(*this);
                
                case prefix:
                    return prefix_to_postfix(*this);

                case postfix:
                    return *this;
            }
            return *this;
        }
    
    // Static Methods
        static expression infix_to_postfix(const expression& infixExpression)
        {
            if (infixExpression.expressionType != infix)
            {
                std::invalid_argument("Expected infix expression as argument");
            }
            std::string postfixText = "";
            ash::stack<char> tempStack(infixExpression.text.size());

            for (int i = 0; i < infixExpression.text.size(); i++)
            {
                if (infixExpression[i] == '(')
                {
                    tempStack.push('(');
                    continue;
                }
                if (infixExpression[i] == ')')
                {
                    empty_stack_to_string(tempStack, postfixText, [](char top) { return true; });
                    continue;
                }

                int p = priority(infixExpression[i]);
                if (p == -1)
                {
                    postfixText += infixExpression[i];
                }
                else
                {
                    empty_stack_to_string(tempStack, postfixText, [p](char top){
                        return priority(top) >= p;
                    });
                    tempStack.push(infixExpression[i]);
                }
            }

            empty_stack_to_string(tempStack, postfixText, [](char top) { return true; });
            return expression(postfixText, postfix);
        }
        // To do
        static expression prefix_to_postfix(const expression& prefixExpression)
        {
            if (prefixExpression.expressionType != prefix)
            {
                std::invalid_argument("Expected prefix expression as argument");
            }
            return prefixExpression;
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
        static void empty_stack_to_string(stack<char>& stk, std::string& str, const std::function<bool (char top)>& func)
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

#endif