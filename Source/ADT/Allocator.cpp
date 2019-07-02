#include "ADT/Allocator.h"


namespace Goto
{
    namespace ADT
    {
        void IAllocatorStorage::RecordStorageAllocation(size_t size)
        {
            m_asAllocatedCount++;
            m_asAllocatedBytes += size;
        }

        void IAllocatorStorage::RecordStorageDeallocation(size_t size)
        {
            m_asFreedCount++;
            m_asAllocatedBytes += size;
        }

        size_t IAllocatorStorage::GetAllocatedBytes() const
        {
            return m_asAllocatedBytes;
        }

        size_t IAllocatorStorage::GetAllocatedCount() const
        {
            return m_asAllocatedCount;
        }

        size_t IAllocatorStorage::GetFloatingMemoryCount() const
        {
            return m_asAllocatedCount - m_asFreedCount;
        }
    }
}