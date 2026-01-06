//
// Created by w on 23/12/2025.
//

#ifndef CATAN_BANK_H
#define CATAN_BANK_H
#include <map>
#include <string>

#include <player/ResourceHolder.h>

enum class ResourceType;

class Bank: public ResourceHolder{
private:
	static constexpr int m_standardNumOfResources= 20;
	static const std::map<DevType,int> m_numberOfStandardDevCardsByType;
public:
    Bank();
	Bank(const ResourcePack &resources, const DevPack &devCards);
	void initializeBank(const ResourcePack &res, const DevPack &devCards);
	DevType takeRandomDev();
};


#endif //CATAN_BANK_H