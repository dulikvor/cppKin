#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "Core/src/Assert.h"
#include "Core/src/Exception.h"
#include "Param.h"

using namespace core;

namespace cppkin
{
    typedef Param<GeneralTypesCollection> GeneralParam;

    class GeneralParams
    {
    public:
        GeneralParams(){}
        ~GeneralParams(){}

        template<typename X>
        void AddParam(const char* key, const X& value)
        {
            typedef std::pair<std::string, GeneralParam> ParamPair;
            auto comparator = [&key](const ParamPair& pair) -> bool {return pair.first == key;};
            if(std::find_if(m_values.begin(), m_values.end(), comparator) == m_values.end())
            {
                GeneralParam param;
                param.Set(value);
                m_values.push_back({key, std::move(param)});
            }
            else
            {
                throw core::Exception(SOURCE, "A requested key was already existed - %s", key.c_str());
            }
        }

        GeneralParam GetValue(const char* key) const
        {
            typedef std::pair<std::string, GeneralParam> ParamPair;
            auto comparator = [&key](const ParamPair& pair) -> bool {return pair.first == key;}; //redundancy from above, but never mind :)
            std::list<ParamPair>::const_iterator it = std::find_if(m_values.begin(), m_values.end(), comparator);
            if(it == m_values.end())
                throw Exception(SOURCE, "Non existing parameter was requested %s", key.c_str());
            return it->second;
        }

    private:
        std::vector<std::pair<const char*, GeneralParam>> m_values;
    };

    class StringConverter
    {
    public:
        static std::string Convert(const std::string str) { return str; }
    };
}
