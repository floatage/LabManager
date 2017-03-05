#ifndef OBJECTDECLARE
#define OBJECTDECLARE

class StorableObject{
    virtual QByteArray toString()const = 0;
    virtual void toObject(const QByteArray& str)const=0;
};

class UnCopyable
{
protected:
    UnCopyable(){}
    ~UnCopyable(){}

private:
    UnCopyable(const UnCopyable&);
    UnCopyable& operator =(const UnCopyable&);
};

#endif // OBJECTDECLARE

