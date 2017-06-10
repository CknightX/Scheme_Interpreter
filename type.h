#ifndef _TYPE_H
#define _TYPE_H
#include<vector>
#include<string>
#include "env.h"

/*  基本类型
	包含：变量，定义，lambda，begin，if，cond，

*/
class Type;
class Env;


enum TYPE
{
	APPLY,
	DEFINE,
	PROCEDURE,
	BEGIN,
	VARIABLE,
	IF,

	BASE_TYPE_TRUE,
	BASE_TYPE_FALSE,
	BASE_TYPE_DOUBLE,
	BASE_TYPE_STRING,
	BASE_TYPE_SYMBOL,
	BASE_TYPE_CONS,

	BASE_PROCEDURE_ADD,
	BASE_PROCEDURE_SUB,
	BASE_PROCEDURE_MUL,
	BASE_PROCEDURE_DIV,
	BASE_PROCEDURE_G,
	BASE_PROCEDURE_L,
	BASE_PROCEDURE_E,
	BASE_PROCEDURE_GE,
	BASE_PROCEDURE_LE,
	BASE_PROCEDURE_AND,
	BASE_PROCEDURE_OR,
	BASE_PROCEDURE_NOT,
	BASE_PROCEDURE_CONS,
	BASE_PROCEDURE_LIST,
	BASE_PROCEDURE_BEGIN,
	BASE_PROCEDURE_CAR,
	BASE_PROCEDURE_CDR,
	BASE_PROCEDURE_ISNULL,
	BASE_PROCEDURE_ISPAIR,
	BASE_PROCEDURE_ISSYMBOL,
	BASE_PROCEDURE_ISEQ,

};


/*
	可求值类型
*/
class Type //类型基类
{
public:
	Type(TYPE type) :type_info(type){}
	//virtual Type* eval(Env* outer_env) = 0;
	virtual ~Type()=0 {}
	TYPE type_info;

};

class Type_True :public Type //True类型
{
public:
	Type_True() :Type(BASE_TYPE_TRUE){}

	
};
class Type_False :public Type //False类型
{
public:
	Type_False() :Type(BASE_TYPE_FALSE){}

};

class Type_Define :public Type  //Define类型 --> (define ... ...)
{
public:
	Type_Define(const std::string& name,Type* value) :Type(DEFINE),name(name),value(value){}

	std::string name;
	Type* value;

};
class Type_BaseType :public Type //基础类型 --> double string
{
public:
	Type_BaseType(TYPE type) :Type(type){}
	union _u
	{
		int value_int;
		double value_double;
	}u;

};

class Type_Symbol :public Type
{
public:
	Type_Symbol(std::string symbol) :Type(BASE_TYPE_SYMBOL),symbol(symbol){}
	std::string symbol;
};

class Type_String :public Type
{
public:
	Type_String(std::string str) :Type(BASE_TYPE_STRING),str(str){}
	std::string str;
};

class Type_BaseProcedureApply :public Type //基本过程apply --> + - * / ...
{
public:
	Type_BaseProcedureApply(TYPE type) :Type(type){}
	parms_value_list parms;
};

class Type_Variable :public Type //变量类型 
{
public:
	Type_Variable(const std::string& name) :Type(VARIABLE),name(name),value(value){}
	std::string name;
	Type* value;
};

class Type_Procedure :public Type //过程类型 --> (lambda <parms> <procedure> )
{
public:
	Type_Procedure(parms_name_list& parms, Type* body)
		:Type(PROCEDURE), parms(parms), body(body){}


public:
	parms_name_list parms;
	Type* body;
	Env* outer_env;
};


class Type_Apply:public Type //apply类型 --> (<procedure> <parms>)
{
public:
	Type_Apply(Type* procedure, parms_value_list& parms)
		:procedure(procedure),Type(APPLY),parms(parms){}
	Type* procedure;
	parms_value_list parms;

};

class Type_If :public Type //if类型 --> (if condition conseq alter)
{
public:
	Type_If(Type* condition,Type* conseq,Type* alter) 
		:Type(IF),condition(condition),conseq(conseq),alter(alter){}

	Type* condition;
	Type* conseq;
	Type* alter;

};

class Type_Cons :public Type
{
public:
	Type_Cons(Type* left, Type* right)
		:Type(BASE_TYPE_CONS), left(left), right(right){}
		Type *left, *right;
};





#endif