#ifndef OBJECTDECLARE
#define OBJECTDECLARE

class StorableObject{
public:
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

class CommunicationObject:public StorableObject
{
public:
    CommunicationObject(){}
    virtual ~CommunicationObject(){}

    virtual QByteArray toString()const{return QByteArray();}
    virtual void toObject(const QByteArray&)const{}

    virtual QString getType()const {return QString();}
    virtual uint getId()const {return 0;}
};

#endif // OBJECTDECLARE

