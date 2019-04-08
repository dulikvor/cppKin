#pragma once

#include <string>
#include <cinttypes>
#include <memory>
#include "Annotation.h"
#include "Export.h"

namespace cppkin
{
    union BinaryValue
    {
        BinaryValue(){}
        ~BinaryValue(){}
        
        bool         m_bool_val;
        std::string* m_str_val;
    };
    
    enum BinaryValueTypes
    {
        Boolean,
        String
    };
    
    class CPPKIN_EXPORT BinaryAnnotation : public Annotation
    {
    public:
        BinaryAnnotation(const EndPoint& endPoint, const char* key, bool value);
        BinaryAnnotation(const EndPoint& endPoint, const char* key, const std::string& value);
        BinaryAnnotation(const EndPoint& endPoint, const char* key, const char* value);
        BinaryAnnotation(const BinaryAnnotation& object);
        
        ~BinaryAnnotation() override;
        BinaryValueTypes GetValueType() const { return m_valueType; }
        const std::string& GetKey() const { return m_key; }
        
        void GetValue(bool& value) const;
        void GetValue(std::string& value) const;
        
    private:
        std::string m_key;
        BinaryValueTypes m_valueType;
        BinaryValue m_value;
    };
}

