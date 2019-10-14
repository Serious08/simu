#pragma once

#include <new>
#include <iostream>
#include <cstdlib>

#include "TypeInfo.h"
#include "Memory.h"
#include "Mutex.h"

namespace serious
{

template<typename ITEM, typename HEADER>
struct ArrayData
{
    ArrayData(size_t nbItems, size_t headerSize)
        : m_headerSize(headerSize)
        , m_nbItems(nbItems)
        , m_nbItemsUsed(0)
        , m_mutex(Mutex::SimpleMutex)
    {
        HEADER* h = header();
        if (h)
        {
            new (h) HEADER();
        }
        constructItemsObj();
    }

    ~ArrayData()
    {
        HEADER* h = header();
        if (h)
        {
            header()->~HEADER();
        }
        destructItemsObj();
    }

    HEADER* header()
    {
        return m_headerSize ? reinterpret_cast<HEADER*>(reinterpret_cast<char*>(this) + sizeof(ArrayData)) : 0;
    }

    ITEM* items()
    {
        return reinterpret_cast<ITEM*>(reinterpret_cast<char*>(this) + sizeof(ArrayData) + m_headerSize);
    }

private:
    void constructItemsObj()
    {
        ITEM* pItems = items();
        if (!is_primitive<ITEM>::value)
        {
            for (int i = 0; i < m_nbItems; ++i)
            {
                new (&pItems[i]) ITEM();
            }
        }
        else
        {
            ::bzero(static_cast<void*>(pItems), m_nbItems);
        }
    }

    void destructItemsObj()
    {
        ITEM* pItems = items();
        if (! is_primitive<ITEM>::value)
        {
            for (int i = 0; i < m_nbItems; ++i)
            {
                pItems[i].~ITEM();
            }
        }
    }

public:
    size_t m_headerSize;  //!< size of the header
    size_t m_nbItems;     //!< number of items
    size_t m_nbItemsUsed; //!< number of items used
    Mutex  m_mutex;       //!< mutex for synchronisation
};

struct NoHeader
{};

template<typename ITEM, typename HEADER = NoHeader>
class Array
{
public:
    typedef ArrayData<ITEM, HEADER> ArrData;

    Array(const char* name, size_t nbItems, size_t hSizeSup = 0);
    ~Array();

    HEADER& header() { return *m_pData->header(); }
    int size() const { return m_pData->m_nbItems; }
    ITEM& operator[](int index) { return m_pData->items()[index]; }

private:
    Array() {}

    const char* m_name;
    ArrData* m_pData;

private:
    friend class UT_Array;
};

template<typename ITEM, typename HEADER>
Array<ITEM, HEADER>::Array(const char* name, size_t nbItems, size_t hSizeSup)
    : m_name(name)
{
    int headerSize = hSizeSup + (!is_empty<HEADER>::value ? sizeof(HEADER) : 0);
    int bytesToAlloc = sizeof(ArrData) + headerSize + sizeof(ITEM)*nbItems;

    m_pData = reinterpret_cast<ArrData*>(theMemoryMgr.allocate(m_name, bytesToAlloc));
    new (m_pData) ArrData(nbItems, headerSize);
}

template<typename ITEM, typename HEADER>
Array<ITEM, HEADER>::~Array()
{
    m_pData->~ArrData();
    theMemoryMgr.deallocate(m_pData);
}


}
