// Написать калькулятор, который решает выражения, типа 23*(456+98)/2-567
// 
// Для простоты будем считать, что:
// унарный минус не допускается;
// пробелы не допускаются;
// цепочки из цифр представляют целые числа, представимые в компьютере
// Вызов: calc "23*(456+98)/2-567",
// т. е. строка для вычисления берется из командной строки.
//

#include <iostream>
#include <string>
#include <optional>

class Calculator
{
    enum tokenType
    {
        number, // sequence of digits, such as "456", value is respective integer value
        lPar,   // '('
        rPar,   // ')'
        plus,   // '+'
        minus,  // '-'
        mul,    // '*'
        divide,    // '/'
        other,  // all other characters, syntax error
        end     // end of the sequence
    };

    struct Token
    {
        // value is integer number if the type is number, otherwise, value is 0 (not used)
        tokenType type;
        int value;
    };

    std::string::iterator m_first;
    std::string::iterator m_last;
    Token m_token;
public:
    Calculator(std::string::iterator first, std::string::iterator last);
    // return value contains result of calculation if the string is correct, and is empty otherwise
    std::optional<int> run();
private:
    void NextToken();
    void ReadNumber();
    /*
    Grammar:
    E->TE'
    E'->+TE'|-TE'|null
    T-> FT'
    T'->*FT'|/FT'|null
    F-> num|(E)
    */
    // every sub-parser will throw an exception if the input is incorrect; otherwise, it will return the value of the parsed expression
    int E();
    int T();
    int Ed(int n);
    int F();
    int Td(int n);
    int Error();
};

Calculator::Calculator(std::string::iterator first, std::string::iterator last)
    : m_token({ other,0 })
    , m_first(first)
    , m_last(last)

{
    //std::copy(first, last, std::back_inserter(m_src));
}

int Calculator::Error()
{
    throw std::exception("");
    return 0;
}

int Calculator::E()
{
    //std::cout << "E->TE'" << "\n";
    int n = T();    // value of the T part
    return Ed(n);   // Ed is + or - followed by another TE'; calculate that TE' and combine it with n by apprpriate operation (+ or -)
}

int Calculator::T()
{
    //std::cout << "T->FT'" << "\n";
    int n = F();    // value of the F part
    return Td(n);   // Td is * or / followed by another FT'; calculate that FT' and combine it with n by apprpriate operation (* or /)
}

// Expected input is +TE'|-TE'|null
// n is the number preceding plus/minus; the next item is calculated and combined with n
int Calculator::Ed(int n)
{
    if (m_token.type == plus)
    {
        NextToken();
        //std::cout << "E'->+TE'" << "\n";
        return Ed(n + T());
    }
    else if(m_token.type == minus)
    {
        NextToken();
        //std::cout << "E'->-TE'" << "\n";
        return Ed(n - T());
    }
    else
    {
        //std::cout << "E'->null" << "\n";
        return n;
    }
}

// Expected input is *FT'|/FT'|null
// n is the number preceding mul/div; the next item is calculated and combined with n
int Calculator::Td(int n)
{
    if (m_token.type == mul)
    {
        NextToken();
        //std::cout << "T'->*FT'" << "\n";
        return Td(n * F());
    }
    else if (m_token.type == divide)
    {
        NextToken();
        //std::cout << "T'->/FT'" << "\n";
        return Td(n / F());
    }
    else
    {
        //std::cout << "T'->null" << "\n";
        return n;
    }
}

// Expected input is num|(E)
// that is, number or expression in parentheses
int Calculator::F()
{
    if (m_token.type == number)
    {
        int n = m_token.value;
        NextToken();
        //std::cout << "F->num" << "\n";
        return n;
    }
    else if (m_token.type == lPar)
    {
        // consume left parenthesis
        NextToken();
        //std::cout << "F->(E)" << "\n";
        // parse expression
        int n = E();
        // it must be followed by right parenthesis
        if (m_token.type == rPar)
        {
            NextToken();
            return n;
        }
        else
        {
            return Error();
        }
    }
    else
    {
        return Error();
    }
}

// invoked only if the m_src string begins with digits. Reads all the characters that are digits and stores the respective number
void Calculator::ReadNumber()
{
    std::string result;
    decltype(m_first) it;
    for (it = m_first; it != m_last; it++)
    {
        if (isdigit(*it))
        {
            result.push_back(*it);
        }
        else
        {
            break;
        }
    }
    // here, the result is the prefix of the m_src consisting of digits. Remove it from the m_src
    m_first = it;
    m_token.type = number;
    m_token.value = atol(result.c_str());
}

// updates m_token value according to the first characters of the string
// updates the string by dropping those characters
void Calculator::NextToken()
{
    m_token.value = 0;
    if (m_first == m_last)
    {
        m_token.type = end;
        return;
    }
    if (isdigit(*m_first))
    {
        ReadNumber();
        return;
    }
    switch (*m_first)
    {
    case '(':
        m_token.type = lPar; break;
    case ')':
        m_token.type = rPar; break;
    case '+':
        m_token.type = plus; break;
    case '-':
        m_token.type = minus; break;
    case '*':
        m_token.type = mul; break;
    case '/':
        m_token.type = divide; break;
    default:
        m_token.type = other; break;
    }
    m_first++;
}

std::optional<int> Calculator::run()
{
    try
    {
        NextToken();
        int n = E();
        if ((m_first == m_last) && (m_token.type == end))
            return n;
        else
            return Error();
    }
    catch (const std::exception&)
    {
        return std::optional<int>{};
    }
}

int main(int ac, char *av[])
{
    if (ac < 2)
    {
        std::cout << "Usage: calc <string to calculate>\n";
        return 0;
    }
    std::string src = av[1];
    std::cout << "Calculating: " << src << "\n";
    Calculator c(src.begin(), src.end());
    auto r = c.run();
    if (r.has_value())
    {
        std::cout << r.value() << "\n";
    }
    else
    {
        std::cout << "syntax error" << "\n";
    }
}
