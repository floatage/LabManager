#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <memory>

class StorableObject{
    virtual QString toString()const = 0;
};

#define DataProviderPtr std::shared_ptr<DataProvider>

class DataProvider
{
public:
    virtual ~DataProvider(){}
    DataProvider(){}

    virtual bool add(const StorableObject& data)=0;
    virtual bool remove(const QString& selection, const std::vector<QString>& selectionArgs)=0;
    virtual bool update(const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs)=0;
    virtual bool query(const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order)=0;
    virtual QString getMarkString()const=0;
    virtual QString getStorageWay()const=0;

    bool operator <(const DataProvider& other){return getMarkString() == other.getMarkString() && getStorageWay() == other.getStorageWay();}

protected:
    virtual bool onCreate()=0;
};

class UserDataProvider: public DataProvider
{
public:
    virtual ~UserDataProvider();
    UserDataProvider();

    bool add(const StorableObject& data);
    bool remove(const QString& selection, const std::vector<QString>& selectionArgs);
    bool update(const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs);
    bool query(const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order);
    QString getMarkString()const;
    QString getStorageWay()const;
    static DataProviderPtr createDataProvider();

protected:
    bool onCreate();
};

class DataManager: public QObject
{
    Q_OBJECT
public:
    static DataManager* getInstance();

    bool registerDataProvider(const DataProviderPtr& provider);
    bool unRegisterDataProvider(const DataProviderPtr& provider);

    bool add(const QString& uri, const StorableObject& data);
    bool remove(const QString& uri, const QString& selection, const std::vector<QString>& selectionArgs);
    bool update(const QString& uri, const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs);
    bool query(const QString& uri, const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order);

signals:

public slots:

private:
    DataManager(QObject* parent);
    ~DataManager();
    DataProviderPtr getDataProvider(const QString& uri);

    static DataManager* instance;

    std::multimap<QString, DataProviderPtr> providerMap;
};

#endif // DATAMANAGER_H
