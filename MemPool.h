/*
MemPool: helper class to have access to memory blocks from the heap during 
audio processing (where standard allocation is strictly forbidden)
Allocate/Resize a mem pool at startup (prepare audio) and clear at the end
*/
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <cassert>
#include <cstring>

class MemPool
{
    public:
    MemPool(size_t maxSizeBytes = 8192)
    :m_maxSize(0), m_dataOffset(0),m_data(nullptr)
    {
        setMemSize(maxSizeBytes);
    };

    void setMemSize(size_t maxSizeBytes)
    {
        if (maxSizeBytes < m_maxSize)
            return;
        
        m_maxSize = maxSizeBytes;
        m_dataOffset = 0;
        auto *const data = static_cast<uint8_t *>(std::realloc(m_data,m_maxSize));
        if (!data)
           throw std::bad_alloc();

         m_data = data;
         wipeMem();
    }


    ~MemPool()
    {
        std::free(m_data);
    };
    
    // explicitly forbid any other copy operation
    MemPool(const MemPool &) = delete;
    MemPool(MemPool &&) = delete;
    MemPool &operator=(const MemPool &) = delete;
    MemPool &operator=(MemPool &&) = delete;

    // 
    void * allocateMem(size_t NrOfBytes, size_t byteAlignment = 8)
    {
        assert(m_dataOffset < m_maxSize); // if not ==> no mem left
        size_t remainingMem = m_maxSize - m_dataOffset;
        void *data = m_data + m_dataOffset;

        if (!std::align(byteAlignment, NrOfBytes, data, remainingMem))
            return nullptr;

        auto AlignmentOffset = static_cast<uint8_t *>(data) - m_data;
        m_dataOffset = AlignmentOffset + NrOfBytes;
        std::memset(data, 0, NrOfBytes);
        return data;
    };
    void clearMem()
    {
        m_dataOffset = 0;
    };
    void wipeMem()
    {
        std::memset(m_data, 0, m_maxSize);
    };
    void *getDirectAccessAt(size_t bytepos)
    {
        return m_data + bytepos;
    };


    private:
    size_t m_maxSize;
    uint8_t *m_data;
    size_t m_dataOffset;


};