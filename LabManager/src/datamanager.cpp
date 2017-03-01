#include "datamanager.h"

#define PROVIDER_ITEATOR_PAIR std::pair<std::multimap<QString,DataProviderPtr>::iterator, std::multimap<QString,DataProviderPtr>::iterator>

DataManager* DataManager::instance = nullptr;
DataManager::DataManager(QObject* parent):QObject(parent)
{
    registerDataProvider(UserDataProvider::createDataProvider());
}

DataManager::~DataManager()
{

}

DataManager* DataManager::getInstance()
{
    if (instance == nullptr){
        instance = new DataManager(nullptr);
    }

    return instance;
}

DataProviderPtr DataManager::getDataProvider(const QString& uri)
{
    PROVIDER_ITEATOR_PAIR it = providerMap.equal_range(uri);
    if (it.first != it.second){
        return (*it.first).second;
    }

    return DataProviderPtr(nullptr);
}

bool DataManager::registerDataProvider(const DataProviderPtr& provider)
{
    providerMap.insert(std::pair<QString, DataProviderPtr>(provider->getMarkString(), provider));
    return true;
}

bool DataManager::unRegisterDataProvider(const DataProviderPtr& provider)
{
    PROVIDER_ITEATOR_PAIR it = providerMap.equal_range(provider->getMarkString());

    for (std::multimap<QString, DataProviderPtr>::iterator begin = it.first; begin != it.second; ++begin){
        if ((begin->second)->getStorageWay() == provider->getStorageWay()){
            providerMap.erase(begin);
            return true;
        }
    }

    return false;
}

bool DataManager::add(const QString& uri, const StorableObject& data)
{
    DataProviderPtr provider = getDataProvider(uri);
    if (provider.get() != nullptr){
        return provider->add(data);
    }

    return false;
}

bool DataManager::remove(const QString& uri, const QString& selection, const std::vector<QString>& selectionArgs)
{
    DataProviderPtr provider = getDataProvider(uri);
    if (provider.get() != nullptr){
        return provider->remove(selection, selectionArgs);
    }

    return false;
}

bool DataManager::update(const QString& uri, const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs)
{
    DataProviderPtr provider = getDataProvider(uri);
    if (provider.get() != nullptr){
        return provider->update(data, selection, selectionArgs);
    }

    return false;
}

bool DataManager::query(const QString& uri, const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order)
{
    DataProviderPtr provider = getDataProvider(uri);
    if (provider.get() != nullptr){
        return provider->query(condition, selection, selectionArgs, order);
    }
    return false;
}


//UserDataProvider implement

UserDataProvider::UserDataProvider()
{

}

UserDataProvider::~UserDataProvider()
{

}

bool UserDataProvider::onCreate()
{
    return false;
}

bool UserDataProvider::add(const StorableObject& data)
{
    return false;
}

bool UserDataProvider::remove(const QString& selection, const std::vector<QString>& selectionArgs)
{
    return false;
}

bool UserDataProvider::update(const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs)
{
    return false;
}

bool UserDataProvider::query(const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order)
{
    return false;
}

QString UserDataProvider::getMarkString()const
{
    return "/labmanager/user";
}

QString UserDataProvider::getStorageWay()const
{
    return "database";
}

DataProviderPtr UserDataProvider::createDataProvider()
{
    return DataProviderPtr(new UserDataProvider());
}
