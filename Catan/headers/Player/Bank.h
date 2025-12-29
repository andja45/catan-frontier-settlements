//
// Created by w on 23/12/2025.
//

#ifndef CATAN_BANK_H
#define CATAN_BANK_H
#include <map>
#include <string>

#include "ResourceHolder.h"

enum class ResourceType;

class Bank: public ResourceHolder{
private:
	static const int m_standardNumOfResources= 20;
	static const std::map<DevType,int> m_numberOfStandardDevCardsByType;
public:
    Bank(const std::string &name) : ResourceHolder() {}
	void initializeStandardBank();
	DevType takeRandomDev();
};


#endif //CATAN_BANK_H