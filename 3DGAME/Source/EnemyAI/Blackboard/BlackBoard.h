#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <stdexcept>

class BlackBoard
{
public:
    BlackBoard();
    ~BlackBoard();

    bool HasKey(const std::string& key) const;

    template<typename T>
    void SetValue(const std::string& key, const T& value)
    {
        m_Data[key] = value;
    }

    template<typename T>
    T GetValue(const std::string& key) const
    {
        auto it = m_Data.find(key);

        if (it != m_Data.end())
        {
            return std::any_cast<T>(it->second);
        }

        throw std::runtime_error("BlackBoard key not found: " + key);
    }

private:
    std::unordered_map<std::string, std::any> m_Data;
};