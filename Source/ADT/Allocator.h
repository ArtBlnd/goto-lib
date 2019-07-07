#ifndef __GTFW_LIB_ADT_ALLOCATOR_H__
#define __GTFW_LIB_ADT_ALLOCATOR_H__

#include <type_traits>
#include <vector>
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

#ifdef USE_LARGE_PAGE
#define DEFAULT_PAGE_COUNT 4
#else
#define DEFAULT_PAGE_COUNT 1
#endif

namespace Goto
{
    namespace ADT
    {
        template <class BaseTy, class OffsTy, class RetnTy = BaseTy>
        inline BaseTy OffsetPtr(BaseTy base, OffsTy offs)
        {
            return (RetnTy)((size_t)(base) + (size_t)(offs));
        }

        NO_VTABLE class IAllocatorStorage
        {
            size_t m_asAllocatedCount = 0;
            size_t m_asFreedCount     = 0;
            size_t m_asAllocatedBytes = 0;
            size_t m_asFreedBytes     = 0;

        protected:
            size_t m_asCapacity;

            // We do check allocating consistent for safty issues such as leaks, curruptions
            // But some storage like SmallAllocatorStorage does not has deallocation, allocation counters
            // So make sure that consistent check is disabled for some allocators.
            bool isCheckBytesConsistent = true;
            bool isCheckCountConsistent = true;

            IAllocatorStorage(size_t initialCapacity)
            {
                m_asCapacity = initialCapacity;
            }
            virtual ~IAllocatorStorage()
            {
                if (isCheckBytesConsistent)
                {
                    if (m_asAllocatedBytes != m_asFreedBytes)
                    {
                        // TODO : Bad consistent assertion.
                    }
                }

                if (isCheckCountConsistent)
                {
                    if (m_asAllocatedCount != m_asFreedCount)
                    {
                        // TODO : Bad consistent assertion.
                    }
                }
            }

            void RecordStorageAllocation(size_t sizeBytes);
            void RecordStorageDeallocation(size_t sizeBytes = 0 /* Can empty */);

            virtual void* AllocateMemoryImpl(size_t size) = 0;
            virtual void FreeMemoryImpl(void* object, size_t size) = 0;

        public:
            size_t GetAllocatedBytes() const;
            size_t GetAllocatedCount() const;

            size_t GetFloatingMemoryCount() const;

            size_t capacity() const;
        };

        struct SmallStorageBuffer
        {
            SmallStorageBuffer* m_ssbPriv = nullptr;
            char                m_ssbSmallBuf[0];
        };

        // class SmallAllocatorStorage 
        //
        // This allocation storage will use stack, which is size of SmallSize in template operand
        // and this will not perform common page allocation before use all of these stack allocations
        // so it will be useful when you need to allocate small, fast, and temporal storge.
        //
        // Template Parameter
        //      size_t SmallSize    : Size that uses storage of object its own.
        //      size_t PageAllocCnt : Count of pages that allocating new pages when its needed
        template <size_t SmallSize, size_t PageAllocCnt = DEFAULT_PAGE_COUNT>
        class SmallAllocatorStorage : IAllocatorStorage
        {
            size_t m_asSotrageBytesLeft = SmallSize;

            union
            {
                char                BufStorage[SmallSize + sizeof(void*)];
                SmallStorageBuffer* SmlStorage;
            };
            

        protected:
            SmallAllocatorStorage() : IAllocatorStorage(SmallSize) 
            {
                isCheckBytesConsistent = false;
            }

            virtual ~SmallAllocatorStorage()
            {
                // If we allocated pages from OS, deallocating it before destructing
                // and also before deallocating, we must check that this is a stack storage (which is class storage).
                if (SmlStorage->m_ssbPriv != nullptr)
                {
                    for (SmallStorageBuffer* buffer = SmlStorage;; buffer = buffer->SmlStorage)
                    {
                        if (buffer->SmlStorage == nullptr)
                        {
                            break;
                        }

                        Host::FreePage(buffer);
                    }
                }
            }

            void* AllocateMemoryImpl(size_t size) override
            {
                void* newMem = nullptr;

                if (m_asSotrageBytesLeft >= size)
                {
                    m_asSotrageBytesLeft -= size;
                    newMem = OffsetPtr(SmlStorage.m_ssbSmallBuf, m_asSotrageBytesLeft);
                }
                else
                {
                    // We do not have space lefts for current storage
                    // allocating new storage from OS (Page)

                    SmallStorageBuffer* oldStorage = &SmlStorage;
                    SmlStorage = (SmallStorageBuffer*) Host::AllocPage(1, HPF_COMMIT | HPF_READ | HPF_WRITE);
                    SmlStorage->m_ssbPriv = oldStorage;

                    m_asSotrageBytesLeft = Host::QueryPageSize();
                    return AllocateMemoryImpl(size);
                }

                RecordStorageAllocation(size);
                return newMem;
            }

            void FreeMemoryImpl(void* object, size_t size) override
            {
                RecordStorageDeallocation(size);
                // Nothing will be happened. we are not freeing object because its small allocator
                // so make sure that you will use this in temporal situations.
            }
        };

        // class ObjectAllocatorStorage
        //
        // Template Parameter
        //      Ty                  : Type that will allocate
        //      size_t PageAllocCnt : Count of pages that allocating new pages when its needed
        template <class Ty, size_t PageAllocCnt = DEFAULT_PAGE_COUNT>
        class ObjectAllocatorStorage : IAllocatorStorage
        {
            std::vector<void*> m_asAllocatedPages;

            size_t m_asStorageIndex  = 0;
            size_t m_asStorageLength = 0;
            Ty**   m_asStorage       = nullptr;

            void ResizeStorage()
            {
                size_t newSize = m_asStorageLength + Host::QueryPageSize() / sizeof(Ty);

                delete[] m_asStorage;
                m_asStorage       = new Ty*[newSize];
                m_asStorageLength = newSize;
            }

            void RefillObjects()
            {
                void* newPage = Host::AllocPage(1, HPF_COMMIT | HPF_READ | HPF_WRITE);

                for (unsigned int i = 0; i < Host::QueryPageSize() / sizeof(Ty); ++i)
                {
                    m_asStorage[i] = OffsetPtr(newPage, i * sizeof(Ty));
                }

                m_asStorageIndex  = Host::QueryPageSize() / sizeof(Ty);
            }

        protected:
            ObjectAllocatorStorage() : IAllocatorStorage(0) 
            {
                ResizeStorage();
                RefillObjects();
            }

            virtual ~ObjectAllocatorStorage()
            {
                for (void* page : m_asAllocatedPages)
                {
                    Host::FreePage(page);
                }

                delete[] m_asStorage;
            }

            void* AllocateMemoryImpl(size_t size) override
            {
                if (sizeof(Ty) != size)
                {
                    // TODO : Assertion.
                }

                if (m_asStorageIndex == 0)
                {
                    ResizeStorage();
                    RefillObjects();

                    return AllocateMemoryImpl(size);
                }

                Ty* newObject = m_asStorage[--m_asStorageIndex];
                RecordStorageAllocation(size);

                return newObject;
            }

            void FreeMemoryImpl(void* object, size_t size) override
            {
                m_asStorage[m_asStorageIndex++] = (Ty*)object;
                RecordStorageDeallocation(size);
            }
        };

        // class CommonAllocatorStorage
        // 
        // Template Parameter
        //      size_t PageAllocCnt : Count of pages that allocating new pages when its needed
        template <size_t PageAllocCnt = DEFAULT_PAGE_COUNT>
        class CommonAllocatorStorage : IAllocatorStorage
        {
            std::vector<void*> m_asAllocatedPages;
            
            void*  m_asCurrentPage      = nullptr;
            size_t m_asCurrentPageIndex = 0;

            void ReallocateCurrentPage()
            {
                void* newPage = Host::AllocPage(1, HPF_COMMIT | HPF_READ | HPF_WRITE);

                m_asAllocatedPages.push_back(newPage);
                m_asCurrentPage      = newPage;
                m_asCurrentPageIndex = 0;

                m_asCapacity += Host::QueryPageSize();
            }

        protected:
            CommonAllocatorStorage() : IAllocatorStorage(0) 
            {
                ReallocateCurrentPage();
                isCheckBytesConsistent = false;
            }

            virtual ~CommonAllocatorStorage()
            {
                for (void* page : m_asAllocatedPages)
                {
                    Host::FreePage(page);
                }
            }
            
            void* AllocateMemoryImpl(size_t size) override
            {
                if (Host::QueryPageSize() >= m_asCurrentPageIndex + size)
                {
                    ReallocateCurrentPage();
                    return AllocateMemoryImpl(size);
                }

                void* pObject = OffsetPtr(m_asCurrentPage, m_asCurrentPageIndex);
                m_asCurrentPageIndex += size;

                RecordStorageAllocation(size);
                return pObject;
            }

            void FreeMemoryImpl(void* object, size_t size) override
            {
                // We are freeing memory at once.
                RecordStorageDeallocation(size);
            }
        };

        template <size_t PageAllocCnt = DEFAULT_PAGE_COUNT>
        class ReusableAllocatorStorage : IAllocatorStorage
        {
            
        protected:
            void* AllocateMemoryImpl(size_t size) override
            {

            }

            void FreeMemoryImpl(void* object, size_t size) override
            {
                
            }
        };

        // This is based on common STL-allocator (STL allocator traits.)
        // you can use it with default std ADT(Abstract Data Types). but not in multithreading situations.
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
                return allocate(size);
            }

            template <class Ty>
            Ty* allocate()
            {
                return (Ty*) allocate(sizeof(T));
            }

            void free(void* object, size_t size)
            {
                FreeMemoryImpl(object, size);
            }

            void free(void* object)
            {
                free(object, 0);
            }

            template <class Ty>
            void free(Ty* object)
            {
                free(object, sizeof(Ty));
            }
        };

    }
}


#endif