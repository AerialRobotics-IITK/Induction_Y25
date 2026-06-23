#include <iostream>
#include <string>

class Transaction
{
    std::string m_ttype{};
    double m_amount{};

public:
    std::string getTType() const
    {
        return m_ttype;
    }
    
    double getAmount() const
    {
        return m_amount;
    }
    Transaction(std::string type, double amount)
        : m_ttype{type}
        , m_amount{amount}
        {}
};
