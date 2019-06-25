#ifndef __GTFW_LIB_ADT_ALLOCATOR_H__
#define __GTFW_LIB_ADT_ALLOCATOR_H__

#include <type_traits>

#ifndef NO_VTABLE
#ifdef _MSC_VER
// novtable description does only exists in msvc compilers
// gcc, clang or another compilers will remove virtual table automacally if its available.
#define NO_VTABLE __declspec(novtable)
#else
#define NO_VTABLE
#endif
#endif

namespace Goto
{
    namespace ADT
    {
        NO_VTABLE class IAllocatorStorage
        {
            size_t m_asCapacity;

            size_t m_asAllocatedCount = 0;
            size_t m_asFreedCount = 0;
            size_t m_asAllocatedBytes = 0;
            size_t m_asFreedBytes = 0;

        protected:
            IAllocatorStorage(size_t initialCapacity)
            {
                m_asCapacity = initialCapacity;
            }

            void RecordStorageAllocation(size_t sizeBytes);

            virtual void* AllocateMemoryImpl(size_t size) = 0;
            virtual void FreeMemoryImpl(void* object, size_t size) = 0;

        public:
            size_t GetAllocatedBytes() const;
            size_t GetAllocatedCount() const;
        };

        template <size_t SmallSize, size_t PtrSize = sizeof(void*)>
        struct SmallStorageBuffer
        {
            SmallStorageBuffer* m_ssbPriv = nullptr;
        };

        template <size_t SmallSize>
        class SmallAllocatorStroage : IAllocatorStorage
        {
            size_t StorageLeftBytes = SmallSize;

            union 
            {
                char               uStorageBuf[SmallSize];
                SmallStorageBuffer uStorageSSO;
            }

        protected:
            SmallAllocatorStroage() : IAllocatorStorage(SmallSize) { }

            void* AllocateMemoryImpl(size_t size)
            {
                if (StorageLeftBytes >= size)
                {
                    
                }
            }
        };

        template <class Ty>
        class ObjectAllocatorStorage : IAllocatorStorage
        {
            
        };

        class CommonAllocatorStorage : IAllocatorStorage
        {

        };

        template <class StorageT>
        class Allocator : public StorageT
        {
            static_assert(std::is_base_of<IAllocatorStorage, StorageT>::value, "Storage type must be IAllocatorStorage");

        public:
            void* allocate(size_t size);
            void* allocate(size_t size, size_t align);

            template <class T>
            T* allocate()
            {
                return (T*) allocate(sizeof(T));
            }


        };

    }
}


#endif