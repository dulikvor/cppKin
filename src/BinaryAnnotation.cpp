#include "BinaryAnnotation.h"
#include "core/Exception.h"
#include "boost/variant/get.hpp"

namespace cppkin
{
    
    BinaryAnnotation::BinaryAnnotation(const EndPoint& endPoint, const char* key, bool value)
            :Annotation(AnnotationType::Binary, endPoint), m_key(key),
             m_valueType(BinaryValueTypes::Boolean), m_value(value)
    {
    }
    
    BinaryAnnotation::BinaryAnnotation(const EndPoint& endPoint, const char* key, const char* value)
            :Annotation(AnnotationType::Binary, endPoint), m_key(key),
             m_valueType(BinaryValueTypes::String), m_value(std::string(value))
    {
    }
    
    BinaryAnnotation::BinaryAnnotation(const BinaryAnnotation& object)
            :Annotation(AnnotationType::Binary, object.m_endPoint), m_key(object.m_key),
             m_valueType(object.m_valueType), m_value(object.m_value)
    {
    }
    
    void BinaryAnnotation::GetValue(bool& value) const
    {
        if(m_valueType != BinaryValueTypes::Boolean)
            throw core::Exception(__CORE_SOURCE, "Requested type dosen't match the stored instance type");
        value = boost::get<bool>(m_value);
    }
    void BinaryAnnotation::GetValue(std::string& value) const
    {
        if(m_valueType != BinaryValueTypes::String)
            throw core::Exception(__CORE_SOURCE, "Requested type dosen't match the stored instance type");
        value = boost::get<std::string>(m_value);
    }
}