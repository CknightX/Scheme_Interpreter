#ifndef _ENV_H
#define _ENV_H
#include<map>
#include<string>
#include <vector>

/*环境*/


class Type;

typedef std::map<std::string, Type*> name_type;
typedef std::vector<std::string> parms_name_list;
typedef std::vector<Type*> parms_value_list;

class Env
{
public:
	Env():outer(nullptr){}
	Env(parms_name_list& parms_name, parms_value_list& parms_value, Env* outer)
		:outer(outer)
	{
		auto iter1 = parms_name.begin();
		auto iter2 = parms_value.begin();
		for (; iter1 != parms_name.end() && iter2 != parms_value.end(); ++iter1, ++iter2)
			env[*iter1] = *iter2;
	}
	~Env(){}
public:
	Type* find(std::string name)
	{
		if (env.count(name) > 0)
			return env[name];
		else if (outer == nullptr)
			return nullptr;
		else
			return outer->find(name);
	}

	name_type env;
	Env* outer; //外部环境
};
#endif