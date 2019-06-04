#ifndef __GTFW_LIB_ADT_VECTOR_H__
#define __GTFW_LIB_ADT_VECTOR_H__

namespace GTFW
{
    namespace ADT
    {
        // ADT class VectorBase
        class VectorBase
        {
            void*  m_vecElements = nullptr;
            size_t m_vecSize     = 0;
            size_t m_vecCapacity = 0;

        protected:
            VectorBase() = delete;
            VectorBase(void* Elements, size_t Capacity);

        public:
            size_t size() const;
            size_t capacity() const;
        };

        template <class T, size_t N>
        class VectorTSSO
        {
            T             m_vecSSOBuffer[N];
            unsigned char m_vecSSOSize;
        };

        // ADT class Vector
        //
        // Template Types :
        //      T : Type of vector
        //      N : Small size of vector (See about Small-String Optimization)
        template <class T, size_t N>
        class Vector
        {
            enum { TSSO_MASK = 0x01 };
            enum { BASE_MASK = 0x01ul };

            union
            {
                VectorTSSO bufTSSO;
                VectorBase bufBase;
            } m_vecData;
        };
    }
}

#endif