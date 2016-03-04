#ifndef REFERENCECOUNTER_H
#define REFERENCECOUNTER_H


class ReferenceCounter
{
public:
    ReferenceCounter();
    virtual ~ReferenceCounter();

    static int GetRefCount();
private:
    static int _ref_counter;
};

#endif // REFERENCECOUNTER_H
