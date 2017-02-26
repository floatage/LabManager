#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

class StorableObject{
    virtual QString toString()const = 0;
};

class DataManager: public QObject
{
    Q_OBJECT
public:
    static DataManager* getInstance();

    bool registerDataProvider(const DataProvider& provider);
    bool unRegisterDataProvider(const DataProvider& provider);

    bool add(const QString& uri, const StorableObject& data);
    bool remove(const QString& uri, const QString& selection, const std::vector<QString>& selectionArgs);
    bool update(const QString& uri, const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs);
    bool query(const QString& uri, const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order);

signals:

public slots:

private:
    DataManager(QObject* parent);
    ~DataManager();
    const DataProvider* getDataProvider(const QString& uri);

    static DataManager* instance;

    std::multimap<QString, DataProvider> providerMap;
};

class DataProvider
{
public:
    bool add(const StorableObject& data)=0;
    bool remove(const QString& selection, const std::vector<QString>& selectionArgs)=0;
    bool update(const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs)=0;
    bool query(const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order)=0;
    QString getMarkString()const=0;
    QString getStorageWay()const=0;
    const DataManager& clone()const=0;

    bool operator <(const DataProvider& other){return getMarkString() == other.getMarkString() && getStorageWay() == other.getStorageWay();}

protected:
    bool onCreate()=0;
    virtual ~DataProvider(){}
    DataProvider(){}
};

class UserDataProvider: public DataProvider
{
public:
    bool add(const StorableObject& data);
    bool remove(const QString& selection, const std::vector<QString>& selectionArgs);
    bool update(const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs);
    bool query(const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order);
    QString getMarkString()const;
    QString getStorageWay()const;
    const DataManager& clone()const;

protected:
    bool onCreate();
    virtual ~UserDataProvider();
    UserDataProvider();
};

#endif // DATAMANAGER_H
