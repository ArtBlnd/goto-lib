#ifndef __GTFW_LIB_ADT_ALLOCATOR_H__
#define __GTFW_LIB_ADT_ALLOCATOR_H__

#include <type_traits>
#include <Host/Memory.h>

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
        template <class T1, class T2>
        inline T1 OffsetPtr(T1 base, T2 offs)
        {
            return (T1)((size_t)(base) + (size_t)(offs));
        }

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

        struct SmallStorageBuffer
        {
            SmallStorageBuffer* m_ssbPriv = nullptr;
            char                m_ssbSmallBuf[0];
        };

        template <size_t SmallSize>
        class SmallAllocatorStroage : IAllocatorStorage
        {
            size_t m_sasSotrageBytesLeft = SmallSize;

            union
            {
                char                StorageBuf[SmallSize + sizeof(void*)];
                SmallStorageBuffer* StorageSSO;
            }
            

        protected:
            SmallAllocatorStroage() : IAllocatorStorage(SmallSize) { }

            void* AllocateMemoryImpl(size_t size)
            {
                void* newMem = nullptr;

                if (m_sasStorageBytesLeft >= size)
                {
                    m_sasStorageBytesLeft -= size;
                    newMem = OffsetPtr(uStorageSSO.m_ssbSmallBuf, m_sasStorageBytesLeft);
                }
                else
                {
                    SmallStorageBuffer* oldStorage = &uStorageSSO;
                    StorageSSO = (SmallStorageBuffer*) Host::AllocPage(1, HPF_COMMIT | HPF_READ | HPF_WRITE);
                    StorageSSO->m_ssbPriv = oldStorage;

                    m_sasStorageBytesLeft = Host::QueryPageSize();
                    return AllocateMemoryImpl(size);
                }

                return newMem;
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
            void* allocate(size_t size)
            {
                return AllocateMemoryImpl(size);
            }

            void* allocate(size_t size, size_t align)
            {
                return AllocateMemoryImpl(size);
            }

            template <class T>
            T* allocate()
            {
                return (T*) allocate(sizeof(T));
            }
        };

    }
}


#endif