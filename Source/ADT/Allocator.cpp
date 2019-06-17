#include "ADT/Allocator.h"


namespace GTFW
{
    void ADT::IAllocator::RecordAllocatedBytes(size_t allocatedBytes)
    {
        m_alAllocatedCnt++;
        m_alAllocatedBytes += allocatedBytes;
    }

    void ADT::IAllocator::RecordFreedBytes(size_t freedBytes)
    {
        m_alFreedCnt++;
        m_alFreedBytes += freedBytes;
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

    size_t ADT::IAllocator::GetFreedAsBytes() const
    {
        return m_alFreedBytes;
    }
}