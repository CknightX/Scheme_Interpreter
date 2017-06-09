#include "eval.h"
#include "type.h"
#include<iostream>
#include<sstream>
Type* Eval::eval(Type* type, Env* env)
{
	Type* result = nullptr;
	if (!type)
		return result;
	switch (type->type_info)
	{
	case APPLY:
		result = eval_apply(type,env);
		break;
	case DEFINE:
		result = eval_define(type, env);
		break;
	case PROCEDURE:
		result = eval_procedure(type, env);
		break;
	case BEGIN:
		result = eval_begin(type, env);
		break;
	case VARIABLE:
		result = eval_variable(type, env);
		break;
	case IF:
		result = eval_if(type, env);
		break;
	case BASE_TYPE_DOUBLE:
	case BASE_TYPE_CONS:
	case BASE_TYPE_TRUE:
	case BASE_TYPE_FALSE:
		result = eval_base_type(type, env);
		break;
	case BASE_PROCEDURE_ADD:
	case BASE_PROCEDURE_SUB:
	case BASE_PROCEDURE_MUL:
	case BASE_PROCEDURE_DIV:
	case BASE_PROCEDURE_G:
	case BASE_PROCEDURE_L:
	case BASE_PROCEDURE_E:
	case BASE_PROCEDURE_GE:
	case BASE_PROCEDURE_LE:
	case BASE_PROCEDURE_AND:
	case BASE_PROCEDURE_OR:
	case BASE_PROCEDURE_NOT:
	case BASE_PROCEDURE_CONS:
	case BASE_PROCEDURE_LIST:
	case BASE_PROCEDURE_BEGIN:
	case BASE_PROCEDURE_CAR:
	case BASE_PROCEDURE_CDR:
	case BASE_PROCEDURE_ISNULL:
	case BASE_PROCEDURE_ISPAIR:
		result = eval_base_procedure(type, env);
		break;
	}
	return result;
}



Type* Eval::eval_procedure(Type* type,Env* env) //过程定义
{
	auto exp_procedure = static_cast<Type_Procedure*>(type);
	exp_procedure->outer_env = env; //设置过程体的外部环境
	return exp_procedure;
}


Type* Eval::eval_apply(Type* type, Env* env)
{
	

	auto exp_apply = static_cast<Type_Apply*>(type);
	std::vector<Type*> value_parms;
	for (auto i : exp_apply->parms)
		value_parms.push_back(eval(i, env));  //对参数求值

	auto apply_body = static_cast<Type_Apply*>(type);
	auto procedure = static_cast<Type_Procedure*>(eval(apply_body->procedure,env));

	switch (procedure->type_info)
	{
	case BASE_PROCEDURE_ADD:
	case BASE_PROCEDURE_SUB:
	case BASE_PROCEDURE_MUL:
	case BASE_PROCEDURE_DIV:
	case BASE_PROCEDURE_G:
	case BASE_PROCEDURE_L:
	case BASE_PROCEDURE_E:
	case BASE_PROCEDURE_GE:
	case BASE_PROCEDURE_LE:
	case BASE_PROCEDURE_AND:
	case BASE_PROCEDURE_OR:
	case BASE_PROCEDURE_NOT:
	case BASE_PROCEDURE_CONS:
	case BASE_PROCEDURE_LIST:
	case BASE_PROCEDURE_BEGIN:
	case BASE_PROCEDURE_CAR:
	case BASE_PROCEDURE_CDR:
	case BASE_PROCEDURE_ISNULL:
	case BASE_PROCEDURE_ISPAIR:
		Type* _procedure = procedure;
		auto base_apply = static_cast<Type_BaseProcedureApply*>(_procedure);
		base_apply->parms = value_parms;
		return eval(base_apply, env);
		break;
	}
	auto curr_env = new Env(procedure->parms, value_parms, procedure->outer_env);
	return eval(procedure->body, curr_env);
}


Type* Eval::eval_define(Type* type, Env* env)
{
	auto type_define = static_cast<Type_Define*>(type);
	(*env).env[type_define->name] = eval(type_define->value, env);
	return nullptr;
}
Type* Eval::eval_base_procedure(Type* type, Env* env)
{
	auto base_procedure = static_cast<Type_BaseProcedureApply*>(type);
	switch (base_procedure->type_info)
	{
	case BASE_PROCEDURE_ADD:
		return eval_base_procedure_add(base_procedure, env);
		break;
	case BASE_PROCEDURE_SUB:
		return eval_base_procedure_sub(base_procedure, env);
		break;
	case BASE_PROCEDURE_MUL:
		return eval_base_procedure_mul(base_procedure, env);
		break;
	case BASE_PROCEDURE_DIV:
		return eval_base_procedure_div(base_procedure, env);
		break;
	case BASE_PROCEDURE_G:
		return eval_base_procedure_greater(base_procedure, env);
		break;
	case BASE_PROCEDURE_L:
		return eval_base_procedure_less(base_procedure, env);
		break;
	case BASE_PROCEDURE_E:
		return eval_base_procedure_equal(base_procedure, env);
		break;
	case BASE_PROCEDURE_GE:
		return eval_base_procedure_greater_equal(base_procedure, env);
		break;
	case BASE_PROCEDURE_LE:
		return eval_base_procedure_less_equal(base_procedure, env);
		break;
	case BASE_PROCEDURE_AND:
		return eval_base_procedure_and(base_procedure, env);
		break;
	case BASE_PROCEDURE_NOT:
		return eval_base_procedure_not(base_procedure, env);
		break;
	case BASE_PROCEDURE_OR:
		return eval_base_procedure_or(base_procedure, env);
		break;
	case BASE_PROCEDURE_CONS:
		return eval_base_procedure_cons(base_procedure, env);
		break;
	case BASE_PROCEDURE_BEGIN:
		return eval_base_procedure_begin(base_procedure, env);
		break;
	case BASE_PROCEDURE_CAR:
		return eval_base_procedure_car(base_procedure, env);
		break;
	case BASE_PROCEDURE_CDR:
		return eval_base_procedure_cdr(base_procedure, env);
		break;
	case BASE_PROCEDURE_LIST:
		return eval_base_procedure_list(base_procedure, env);
		break;
	case BASE_PROCEDURE_ISNULL:
		return eval_base_procedure_is_null(base_procedure, env);
		break;
	case BASE_PROCEDURE_ISPAIR:
		return eval_base_procedure_is_pair(base_procedure, env);
		break;
	}
}



Type* Eval::eval_base_type(Type* type, Env* env)
{
	return type;
}



Type* Eval::eval_if(Type* type, Env* env)
{
	auto exp_if = static_cast<Type_If*>(type);
	if (eval(exp_if->condition, env)->type_info == BASE_TYPE_TRUE)
	{
		return eval(exp_if->conseq, env);
	}
	else
	{
		return eval(exp_if->alter, env);
	}
}

Type* Eval::eval_lambda(Type* type, Env* env)
{
	return type;
}

Type* Eval::eval_variable(Type* type, Env* env)
{
	auto exp_variable = static_cast<Type_Variable*>(type);
	std::string name = exp_variable->name;
	return env->find(name);
}

//------------------eval_base_procedure------------------

Type* Eval::eval_base_procedure_add(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 0;
	for (auto i : base_procedure->parms)
	{
		result->u.value_double += static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_sub(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 0;
	bool is_first = true;
	for (auto i : base_procedure->parms)
	{
		if (is_first)
		{
			result->u.value_double += static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
			is_first = false;
		}
		else
			result->u.value_double -= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_mul(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 1;
	bool is_first = true;
	for (auto i : base_procedure->parms)
	{
		result->u.value_double *= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_div(Type_BaseProcedureApply* base_procedure, Env* env)
{
	Type_BaseType* result = new Type_BaseType(BASE_TYPE_DOUBLE);
	result->u.value_double = 1;
	bool is_first = true;
	for (auto i : base_procedure->parms)
	{
		if (is_first)
		{
			result->u.value_double *= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
			is_first = false;
		}
		else
			result->u.value_double /= static_cast<Type_BaseType*>(eval(i, env))->u.value_double;
	}
	return result;
}
Type* Eval::eval_base_procedure_greater(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for >";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for >";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm > second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_less(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for <";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for <";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm < second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_equal(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for =";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for =";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm == second_parm)
			return new Type_True;
		else
			return new Type_False;
	}

}
Type* Eval::eval_base_procedure_greater_equal(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parm for >=";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parm for >=";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm >= second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_less_equal(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() > 2)
	{
		throw "too many parms for <=";
	}
	else if (base_procedure->parms.size() < 2)
	{
		throw "too less parms for <=";
	}
	else
	{
		auto first_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[0], env))->u.value_double;
		auto second_parm = static_cast<Type_BaseType*>(eval(base_procedure->parms[1], env))->u.value_double;
		if (first_parm <= second_parm)
			return new Type_True;
		else
			return new Type_False;
	}
}
Type* Eval::eval_base_procedure_and(Type_BaseProcedureApply* base_procedure, Env* env)
{
	for (auto parm : base_procedure->parms)
	{
		auto result = eval(parm, env);
		if (result->type_info == BASE_TYPE_FALSE)
			return new Type_False;
	}
	return new Type_True;
}
Type* Eval::eval_base_procedure_or(Type_BaseProcedureApply* base_procedure, Env* env)
{
	for (auto parm : base_procedure->parms)
	{
		auto result = eval(parm, env);
		if (result->type_info != BASE_TYPE_FALSE)
			return new Type_True;
	}
	return new Type_False;
}
Type* Eval::eval_base_procedure_not(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "not need 1 parm!";
	auto result = eval(base_procedure->parms[0],env);

	if (result->type_info != BASE_TYPE_FALSE)
		return new Type_False;
	else
		return new Type_True;
}
Type* Eval::eval_base_procedure_cons(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 2)
		throw "cons need 2 parms!";
	return new Type_Cons(base_procedure->parms[0], base_procedure->parms[1]);
}

Type* list2cons(parms_value_list& parms, size_t index)
{
	if (index == parms.size())
		return nullptr;
	else
	{
		return new Type_Cons(parms[index], list2cons(parms, index + 1));
	}
}


Type* Eval::eval_base_procedure_list(Type_BaseProcedureApply* base_procedure, Env* env)
{
	return list2cons(base_procedure->parms,0);
}

Type* Eval::eval_base_procedure_begin(Type_BaseProcedureApply* base_procedure, Env* env)
{
	for (auto iter = base_procedure->parms.begin(); iter != base_procedure->parms.end() - 1; ++iter)
		eval(*iter,env);
	return eval(*(base_procedure->parms.rbegin()), env);
}

Type* Eval::eval_base_procedure_car(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "car need 1 parms!";
	auto cons_type = static_cast<Type_Cons*>(base_procedure->parms[0]);
	return eval(cons_type->left);
}
Type* Eval::eval_base_procedure_cdr(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "cdr need 1 parms!";
	auto cons_type = static_cast<Type_Cons*>(base_procedure->parms[0]);
	return eval(cons_type->right);
}
Type* Eval::eval_base_procedure_is_null(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "null? need 1 parms!";
	auto cons_type = static_cast<Type_Cons*>(base_procedure->parms[0]);
	if (cons_type == nullptr)
		return new Type_True;
	else
		return new Type_False;
}
Type* Eval::eval_base_procedure_is_pair(Type_BaseProcedureApply* base_procedure, Env* env)
{
	if (base_procedure->parms.size() != 1)
		throw "pair? need 1 parms!";
	if (base_procedure->parms[0]->type_info == BASE_TYPE_CONS)
		return new Type_True;
	else
		return new Type_False;
}

/*输出基本类型*/
std::string Eval::convert_cons(Type* _type) //原始cons结构输出
{
	if (_type == nullptr)
		return "";
	else if (_type->type_info != BASE_TYPE_CONS)
		return convert_scheme2str(_type);
	else
	{
		auto cons_type = static_cast<Type_Cons*>(_type);
		std::string tmp = "(" + convert_cons(cons_type->left) + " " + convert_cons(cons_type->right)+")";
		return tmp;
	}

	return "";
}
std::string Eval::convert_cons_left(Type* _type)
{
	if (_type == nullptr)
		return "";
	else if (_type->type_info != BASE_TYPE_CONS)
		return convert_scheme2str(_type);
	else
	{
		auto cons_type = static_cast<Type_Cons*>(_type);
		std::string tmp = convert_cons_right(cons_type->right);
		if (tmp == "")
			return "(" + convert_cons_left(cons_type->left)  + ")";
		else
			return "(" + convert_cons_left(cons_type->left) + " " + tmp + ")";
	}

}
std::string Eval::convert_cons_right(Type* _type)
{
	if (_type == nullptr)
		return "";
	else if (_type->type_info != BASE_TYPE_CONS)
		return ". " + convert_scheme2str(_type);
	else
	{
		auto cons_type = static_cast<Type_Cons*>(_type);
		std::string tmp = convert_cons_right(cons_type->right);
		if (tmp == "")
			return  convert_cons_left(cons_type->left);
		else
			return  convert_cons_left(cons_type->left) + " " + tmp;
	}
}
std::string Eval::convert_cons_scheme(Type* _type) //scheme格式的cons输出
{
	if (_type == nullptr || _type->type_info != BASE_TYPE_CONS)
		return "";
	auto cons_type = static_cast<Type_Cons*>(_type);
	std::string tmp = convert_cons_right(cons_type->right);
	if (tmp == "")
		return "(" + convert_cons_left(cons_type->left) + ")";
	else
		return "(" + convert_cons_left(cons_type->left) + " " + tmp + ")";
}

std::string Eval::convert_base_type(Type* _type)
{
	auto type = static_cast<Type_BaseType*>(_type);
	std::stringstream ss;
	std::string tmp;
	switch (type->type_info)
	{
	case BASE_TYPE_DOUBLE:
		ss << type->u.value_double;
		ss >> tmp;
		return tmp;
		break;
	case BASE_TYPE_TRUE:
		return "#t";
		break;
	case BASE_TYPE_FALSE:
		return "#f";
		break;
	}
}


std::string Eval::convert_scheme2str(Type* type)
{
	if (!type)
		return "";
	switch (type->type_info)
	{
	case BASE_TYPE_DOUBLE:
	case BASE_TYPE_TRUE:
	case BASE_TYPE_FALSE:
		return convert_base_type(type);
		break;
	case BASE_TYPE_CONS:
		return convert_cons_scheme(type);
		break;

	default:
		return "";
		break;
	}
}
void Eval::create_base_env()
{
	//初始化基本环境
	base_env->env["+"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ADD);
	base_env->env["-"] = new Type_BaseProcedureApply(BASE_PROCEDURE_SUB);
	base_env->env["*"] = new Type_BaseProcedureApply(BASE_PROCEDURE_MUL);
	base_env->env["/"] = new Type_BaseProcedureApply(BASE_PROCEDURE_DIV);
	base_env->env["="] = new Type_BaseProcedureApply(BASE_PROCEDURE_E);
	base_env->env[">"] = new Type_BaseProcedureApply(BASE_PROCEDURE_G);
	base_env->env["<"] = new Type_BaseProcedureApply(BASE_PROCEDURE_L);
	base_env->env[">="] = new Type_BaseProcedureApply(BASE_PROCEDURE_GE);
	base_env->env["<="] = new Type_BaseProcedureApply(BASE_PROCEDURE_LE);
	base_env->env["and"] = new Type_BaseProcedureApply(BASE_PROCEDURE_AND);
	base_env->env["not"] = new Type_BaseProcedureApply(BASE_PROCEDURE_NOT);
	base_env->env["or"] = new Type_BaseProcedureApply(BASE_PROCEDURE_OR);
	base_env->env["cons"] = new Type_BaseProcedureApply(BASE_PROCEDURE_CONS);
	base_env->env["list"] = new Type_BaseProcedureApply(BASE_PROCEDURE_LIST);
	base_env->env["begin"] = new Type_BaseProcedureApply(BASE_PROCEDURE_BEGIN);
	base_env->env["car"] = new Type_BaseProcedureApply(BASE_PROCEDURE_CAR);
	base_env->env["cdr"] = new Type_BaseProcedureApply(BASE_PROCEDURE_CDR);
	base_env->env["print"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ISNULL);
	base_env->env["null"] = nullptr;

	base_env->env["null?"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ISNULL);
	base_env->env["pair?"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ISPAIR);
	//base_env->env["eq?"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ISEQ);
	//base_env->env["symbol?"] = new Type_BaseProcedureApply(BASE_PROCEDURE_ISNULL);
}
