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
    DataManager(QObject* parent);
    const DataProvider* getDataProvider();

signals:

public slots:

private:
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

protected:
    bool onCreate()=0;
};


class DataProviderProxy
{
public:
    bool add(const QString& uri, const StorableObject& data);
    bool remove(const QString& uri, const QString& selection, const std::vector<QString>& selectionArgs);
    bool update(const QString& uri, const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs);
    bool query(const QString& uri, const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order);
};

#endif // DATAMANAGER_H
