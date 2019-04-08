#include "BinaryAnnotation.h"
#include "core/Exception.h"

namespace cppkin
{
    
    BinaryAnnotation::BinaryAnnotation(const EndPoint& endPoint, const char* key, bool value)
            :Annotation(AnnotationType::Binary, endPoint), m_key(key),
             m_valueType(BinaryValueTypes::Boolean)
    {
        m_value.m_bool_val = value;
    }
    
    BinaryAnnotation::BinaryAnnotation(const EndPoint& endPoint, const char* key, const std::string& value)
            :Annotation(AnnotationType::Binary, endPoint), m_key(key),
             m_valueType(BinaryValueTypes::String)
    {
        m_value.m_str_val = new std::string(value);
    }
    
    BinaryAnnotation::BinaryAnnotation(const EndPoint& endPoint, const char* key, const char* value)
            :Annotation(AnnotationType::Binary, endPoint), m_key(key),
             m_valueType(BinaryValueTypes::String)
    {
        m_value.m_str_val = new std::string(value);
    }
    
    BinaryAnnotation::BinaryAnnotation(const BinaryAnnotation& object)
            :Annotation(AnnotationType::Binary, object.m_endPoint), m_key(object.m_key),
             m_valueType(object.m_valueType)
    {
        switch(m_valueType)
        {
            case BinaryValueTypes::Boolean:
                m_value.m_bool_val = object.m_value.m_bool_val;
                break;
            case BinaryValueTypes::String:
                m_value.m_str_val = new std::string(*object.m_value.m_str_val);
                break;
        }
    }
    
    BinaryAnnotation::~BinaryAnnotation()
    {
        if(m_valueType == BinaryValueTypes::String)
			delete m_value.m_str_val;
    }

    void BinaryAnnotation::GetValue(bool& value) const
    {
        if(m_valueType != BinaryValueTypes::Boolean)
            throw core::Exception(__CORE_SOURCE, "Requested type dosen't match the stored instance type");
        value = m_value.m_bool_val;
    }
    void BinaryAnnotation::GetValue(std::string& value) const
    {
        if(m_valueType != BinaryValueTypes::String)
            throw core::Exception(__CORE_SOURCE, "Requested type dosen't match the stored instance type");
        value = *m_value.m_str_val;
    }
}