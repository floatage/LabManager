#include "datamanager.h"

DataManager* DataManager::instance = nullptr;
DataManager::DataManager(QObject* parent):QObject(parent)
{

}

DataManager::~DataManager()
{

}

DataManager* DataManager::getInstance()
{
    if (instance == nullptr){
        instance = new DataManager();
    }

    return instance;
}

const DataManager::DataProvider* getDataProvider(const QString& uri)
{
    std::pair<multimap<QString,DataProvider>::iterator, multimap<QString,int>::DataProvider> it = providerMap.equal_range(uri);
    if (it.first != it.second){
        return *(it.first);
    }

    return nullptr;
}

bool DataManager::registerDataProvider(const DataProvider& provider)
{
    providerMap[provider.getMarkString() = provider.clone();
    return true;
}

bool DataManager::unRegisterDataProvider(const DataProvider& provider)
{
    std::pair<multimap<QString,DataProvider>::iterator, multimap<QString,int>::DataProvider> it = providerMap.equal_range(provider.getMarkString());

    for (std::multimap<QString, DataProvider>::iterator begin = it.first; begin != it.second; ++begin){
        if (begin->getStorageWay() == provider.getStorageWay()){
            providerMap.erase(begin);
            return true;
        }
    }

    return false;
}

bool DataManager::add(const QString& uri, const StorableObject& data)
{
    return false;
}

bool DataManager::remove(const QString& uri, const QString& selection, const std::vector<QString>& selectionArgs)
{
    return false;
}

bool DataManager::update(const QString& uri, const StorableObject& data, const QString& selection, const std::vector<QString>& selectionArgs)
{
    return false;
}

bool DataManager::query(const QString& uri, const QString& condition, const QString& selection, const std::vector<QString>& selectionArgs, QString order)
{
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

const DataProvider& clone()const
{
    return DataProvider(*this);
}
