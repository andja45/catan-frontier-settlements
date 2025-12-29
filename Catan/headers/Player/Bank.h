//
// Created by w on 23/12/2025.
//

#ifndef CATAN_BANK_H
#define CATAN_BANK_H
#include <map>
#include <string>

#include "../Types/DevType.hpp"
#include "ResourceHolder.h"
#include "../../headers/Types/TypeAliases.hpp"

enum class ResourceType;

class Bank: public ResourceHolder{
private:
	static const int m_standardNumOfResources= 20;
	static const std::map<DevType,int> m_numberOfStandardDevCardsByType;
public:
    Bank(const std::string &name) : ResourceHolder(name) {}
	void initializeStandardBank();
};


#endif //CATAN_BANK_H