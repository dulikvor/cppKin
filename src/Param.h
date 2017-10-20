#pragma once

#include <list>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Core/src/Assert.h"
#include "Core/src/Exception.h"

template<typename... Arg>
struct TypesCollection{
};

typedef TypesCollection<short, int, long, float, double, bool, void*, std::string, std::list<std::string>> GeneralTypesCollection;

template<typename... Arg>
struct VariantParam
{
};

template<typename X>
struct VariantParam<X>
{
    const static int typeLocation = -1;
};

template<typename X, typename... Arg>
struct VariantParam<X, X, Arg...>
{
    const static int typeLocation = 0;
};

template<typename X, typename T, typename... Arg>
struct VariantParam<X, T, Arg...>
{
    const static int typeLocation = (VariantParam<X, Arg...>::typeLocation >= 0 ? VariantParam<X, Arg...>::typeLocation + 1 : VariantParam<X, Arg...>::typeLocation);
};

template<typename... Arg>
struct VariantHelper
{
};

template<typename... Arg>
struct VariantHelper<TypesCollection<Arg...>>
{
	template<typename X>
	static int GetTypeID()
	{
		return VariantParam<X, Arg...>::typeLocation;
	}
};

template<typename... Arg>
struct ParamHelper{};

template<typename X>
struct ParamHelper<X>
{
	static void Copy(const int& typeID, char*& myBuffer, char* const objBuffer)
	{
		if(typeID == VariantHelper<GeneralTypesCollection>::GetTypeID<X>())
		{
			char* temp = (char*)malloc(sizeof(char)*sizeof(X));
			new (temp) X(*reinterpret_cast<X*>(objBuffer));
			std::swap(myBuffer, temp);
			if(temp != nullptr) {
				reinterpret_cast<X*>(temp)->~X();
				delete[] temp;
			}
		}
	}
	static void Destroy(const int& typeID, char* rawBuffer)
	{
		if(typeID == VariantHelper<GeneralTypesCollection>::GetTypeID<X>())
		{
			if(rawBuffer != nullptr) {
				reinterpret_cast<X *>(rawBuffer)->~X();
				delete[] rawBuffer;
			}
		}
	}
};

template<typename X, typename... Arg>
struct ParamHelper<X, Arg...>
{
	static void Copy(const int& typeID, char*& myBuffer, char* const objBuffer)
	{
		if(typeID == VariantHelper<GeneralTypesCollection>::GetTypeID<X>())
		{
			char* temp = (char*)malloc(sizeof(char)*sizeof(X));
			new (temp) X(*reinterpret_cast<X*>(objBuffer));
			std::swap(myBuffer, temp);
			if(temp != nullptr) {
				reinterpret_cast<X*>(temp)->~X();
				delete[] temp;
			}
		}
		else
			ParamHelper<Arg...>::Copy(typeID, myBuffer, objBuffer);
	}
	static void Destroy(const int& typeID, char* rawBuffer)
	{
		if(typeID == VariantHelper<GeneralTypesCollection>::GetTypeID<X>())
		{
			if(rawBuffer != nullptr) {
				reinterpret_cast<X*>(rawBuffer)->~X();
				delete[] rawBuffer;
			}
		}
		else
			ParamHelper<Arg...>::Destroy(typeID, rawBuffer);
	}
};

template<typename ...Arg>
class Param{};

template<typename ...Arg>
class Param<TypesCollection<Arg...>>
{
public:
    Param():m_rawBuffer(nullptr){}
    ~Param(){ ParamHelper<Arg...>::Destroy(
			m_typeID, m_rawBuffer); }

    void operator = (const Param& rhs) = delete;

	Param(const Param& obj):m_rawBuffer(nullptr), m_typeID(-1)
	{
		m_typeID = obj.GetTypeID();
		ParamHelper<Arg...>::Copy(
				m_typeID, m_rawBuffer, obj.GetBuffer());
	}

    Param(Param&& obj):m_rawBuffer(nullptr), m_typeID(-1)
    {
        m_typeID = obj.GetTypeID();
		char* newBuffer = obj.ReleaseBuffer();
		std::swap(m_rawBuffer, newBuffer);
    }

    template<typename X>
    void Set(const X& value)
    {
        m_typeID = VariantParam<X, Arg...>::typeLocation;
        m_rawBuffer = (char*)malloc(sizeof(char)*sizeof(X));
        new (m_rawBuffer) X(value);
    }
	
	template <typename X>
	void Set(const std::list<X>& value)
	{
		m_typeID = VariantParam<std::list<X>, Arg...>::typeLocation;
		m_rawBuffer = (char*)malloc(sizeof(std::list<X>)*sizeof(char));
		new (m_rawBuffer) std::list<X>(value);
	}
    template<typename X>
    operator X() const
    {
        int neededLocation = VariantParam<X, Arg...>::typeLocation;
        ASSERT(neededLocation == m_typeID);
        return *(reinterpret_cast<X*>(m_rawBuffer));
    }

	char* ReleaseBuffer()
	{
		char* oldBuffer = m_rawBuffer;
		m_rawBuffer = nullptr;
		return oldBuffer;
	}
    //Accessor
    int GetTypeID() const {return m_typeID;}
	char* const GetBuffer() const { return m_rawBuffer; }

private:
	char* m_rawBuffer;
    int m_typeID;
};


