#pragma once

#include <string>
#include <cinttypes>
#include "boost/variant.hpp"
#include "Annotation.h"
#include "Export.h"

namespace cppkin
{
    
    enum BinaryValueTypes
    {
        Boolean,
        String
    };
    
    class BinaryAnnotation : public Annotation
    {
    public:
        BinaryAnnotation(const EndPoint& endPoint, const char* key, bool value);
        BinaryAnnotation(const EndPoint& endPoint, const char* key, const char* value);
        BinaryAnnotation(const BinaryAnnotation& object);
        
        ~BinaryAnnotation() override = default;
        BinaryValueTypes GetValueType() const { return m_valueType; }
        const std::string& GetKey() const { return m_key; }
        
        void GetValue(bool& value) const;
        void GetValue(std::string& value) const;
        
    private:
        std::string m_key;
        BinaryValueTypes m_valueType;
        boost::variant<bool, std::string> m_value;
    };
}

