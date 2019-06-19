#include "ADT/Allocator.h"


namespace GTFW
{
    void ADT::IAllocator::RecordAllocatedBytes(size_t allocatedBytes)
    {
        m_alAllocatedCnt++;
        m_alAllocatedBytes += allocatedBytes;
    }

    void ADT::IAllocator::RecordFreedCnt()
    {
        m_alFreedCnt++;
    }

    bool ADT::IAllocator::CheckAllocatorMemoryConsistent() const
    {
        if (m_alAllocatedCnt   != m_alFreedCnt || 
            m_alAllocatedBytes != m_alFreedBytes)
        {
            return false;
        }

        return true;
    }

    size_t ADT::IAllocator::GetAllocatedAsBytes() const
    {
        return m_alAllocatedBytes;
    }

    size_t ADT::IAllocator::GetFreedCount() const
    {
        return m_alFreedCnt;
    }
}