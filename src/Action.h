#pragma once
#include <iostream>

class Action
{
public:
    std::string m_name = "default";
    std::string m_type = "default";

    Action(){}
    Action(const std::string& name, const std::string& type) : m_name(name), m_type(type){}
    const std::string& name() const;
    const std::string& type() const;
};
