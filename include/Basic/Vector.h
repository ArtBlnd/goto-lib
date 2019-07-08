#ifndef __GOTO_LIB_STL_VECTOR_H__
#define __GOTO_LIB_STL_VECTOR_H__

namespace Goto
{
    namespace ADT
    {
        template <class T>
        class VectorBase
        {
            T*     m_vbElement;
            size_t m_vbSize;
            size_t m_vbCapacity;

        public:
            size_t GetCapacity() const;
            size_t GetSize() const;

            const T& LookUp(size_t index) const
            {
                return m_vbElement[index];
            }

            T& LookUp(size_t index)
            {
                return m_vbElement[index];
            }
        };

        // ADT class Vector
        //
        // Template Types :
        //      T : Type of vector
        //      N : Small size of vector (See about Small-String Optimization)
        template <class T, size_t N = 8>
        class Vector : VectorBase<T>
        {

        };
    }
}

#endif