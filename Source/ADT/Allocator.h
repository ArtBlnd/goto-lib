#ifndef __GTFW_LIB_ADT_ALLOCATOR_H__
#define __GTFW_LIB_ADT_ALLOCATOR_H__

namespace GTFW
{
    namespace ADT
    {
        class IAllocator
        {
            size_t m_alAllocatedCnt   = 0;
            size_t m_alAllocatedBytes = 0;
            size_t m_alFreedCnt       = 0;
            size_t m_alFreedBytes     = 0;

        protected:
            // All allocators should record all stats when its allocating
            // this will be used for debugging, consistent checking, also for after allocating offsets
            // so make sure that you are recording all stats when you are allocating object.
            void RecordAllocatedBytes(size_t allocatedBytes);
            void RecordFreedCnt();

            // Check that all memory has been freed
            // because some of allocators will free all memory when its distroyed. so it might cause memory corruption.
            // that type of allocators will need to call this method to check that all memory has been freed.
            bool CheckAllocatorMemoryConsistent() const;

        public:
            virtual void* allocate(size_t size) = 0;
            virtual void free(void* pObject) = 0;

            size_t GetAllocatedAsBytes() const;
            size_t GetFreedCount() const;
        };

        class CommonAllocator
        {
            
        };

        template <size_t allocSize>
        class SmallAllocator : IAllocator
        {
            union
            {
                char  allocBuf[allocSize];
                void* allocPtr;
            };

        public:
            void* allocate(size_t size) override
            {
                if (GetAllocatedAsBytes() + size > allocSize)
                {
                    // TODO : exception
                }

                RecordAllocatedBytes(size);
                return &allocBuf[GetAllocatedAsBytes()];
            }

            void free(void* pObject) override
            {
                RecordFreedCnt();
            }
        };
    }
}


#endif