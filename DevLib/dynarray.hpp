#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP
#include <cstdlib>
#include "Exceptions.hpp"
namespace dev
{
    template<typename VecType> class vector
    {
    public:
        VecType* ptr;
        size_t ptr_sz;
        vector() : ptr_sz(0)
        {
            ptr = nullptr;
        }
        vector(size_t preallocate) : ptr_sz(0)
        {
            ptr = new VecType[preallocate];
        }
        vector(dev::vector<VecType>& vec)
        {
            delete[] ptr;
            ptr = vec.ptr;
            ptr_sz = vec.ptr_sz;
        }
        VecType& operator[](size_t index)
        {
            if(index > ptr_sz) { throw dev::VectorException("OUT OF BOUNDS"); }
            return ptr[index];
        }

        void push_back(VecType obj)
        {
            if(ptr_sz == 0)
            {
                ptr = new VecType[1];
            }
            else
            {
                VecType* nptr = new VecType[ptr_sz + 1];
                for(size_t i = 0; i < ptr_sz; i++)
                {
                    nptr[i] = ptr[i];
                }
                delete[] ptr;
                nptr[ptr_sz + 1] = obj;
                ptr = nptr;
                ptr_sz++;
            }
        }

        void erase(size_t index)
        {
            if(index > ptr_sz) { throw dev::VectorException("OUT OF BOUNDS"); }
            VecType* nptr = new VecType[ptr_sz - 1];
            bool yea;
            for(size_t i = 0; i < ptr_sz; i++)
            {
                if(i == index)
                {
                    yea = true;
                }
                if(!yea) { nptr[i] = ptr[i]; }
                if(yea) { nptr[i - 1] = ptr[i]; }
            }
        }
    };
}
#endif // DYNARRAY_HPP
