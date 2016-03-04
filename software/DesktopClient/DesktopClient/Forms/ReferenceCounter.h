#ifndef REFERENCECOUNTER_H
#define REFERENCECOUNTER_H

template <class T>
class ReferenceCounter
{
public:
    ReferenceCounter()
    {
        _ref_counter++;
    }

    virtual ~ReferenceCounter()
    {
        _ref_counter--;
    }

    static int GetRefCount()
    {
        return _ref_counter;
    }

private:
    static int _ref_counter;
};


template <class T>
int ReferenceCounter<T>::_ref_counter = 0;

#endif // REFERENCECOUNTER_H
