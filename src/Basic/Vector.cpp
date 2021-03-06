#include <ADT/Vector.h>

namespace Goto
{
    namespace ADT
    {
        size_t VectorBase::size() const
        {
            return m_vecSize;
        }

        size_t VectorBase::capacity() const
        {
            return m_vecCapacity;
        }
    }
}