#include "parser.h"
#include<cmath>
double string2double(std::string num)
{
	int status = 0;
	double sum = 0;
	int floor = 1;
	for (auto c : num)
	{
		if (c == '.')
		{
			status++;
		}
		else if (status == 0)
		{
			sum = 10 * sum + (c - '0');
		}
		else if (status==1)
		{
			sum += (c - '0')*pow(10, -(floor++));
		}
		else
		{
			throw "unexpected \".\"";
		}
	}
	return sum;
}


Type* Parser::deal_expression()
{
	Type* result = nullptr;
	std::string str = lexer.get_next_token();
	if (lexer.is_end)
		return result;
	if (str[0] == '(')
	{
		str = lexer.get_next_token();
		if (str == "define")
		{
			result = deal_define();
		}
		else if (str == "lambda")
		{
			result = deal_lambda();
		}
		else if (str == "if")
		{
			result = deal_if();
		}
		else if (str == "cond")
		{
			result = deal_cond();
		}
		else //apply
		{
			lexer.put_formal_token();
			/*
			if (str == "+")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_ADD);
				tmp->parms = deal_parms_value();
				result = tmp;
			} 
			else if (str == "-")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_SUB);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "*")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_MUL);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "/")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_DIV);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == ">")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_G);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "<")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_L);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "=")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_E);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == ">=")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_GE);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "<=")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_LE);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "cons")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_CONS);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "begin")
			{
				auto tmp = new Type_BaseProcedureApply(BASE_PROCEDURE_BEGIN);
				tmp->parms = deal_parms_value();
				result = tmp;
			}
			else if (str == "append")
			{

			}
			else if (str == "map")
			{

			}
			*/
			auto exp = deal_expression();
			auto parms = deal_parms_value();
			result = new Type_Apply(exp, parms);
		}
		lexer.get_next_token(); //)
	}
	else
	{
		if (str[0] >= '0'&&str[0] <= '9')  //num
		{
			Type_BaseType* type_num = new Type_BaseType(BASE_TYPE_DOUBLE);
			type_num->u.value_double = string2double(str);
			result = type_num;
		}
		else  //variable
		{
			return new Type_Variable(str);
		}
	}
	return result;
}


Type* Parser::deal_lambda()
{
	auto parms = deal_parms_name();
	auto exp = deal_expression();
	return new Type_Procedure(parms, exp);
}

Type* Parser::deal_define()
{
	std::string name = lexer.get_next_token();
	if (name == "(")
	{
		name = lexer.get_next_token();
		lexer.put_formal_token();
		return new Type_Define(name, deal_lambda());
	}
	return new Type_Define(name, deal_expression());
}
Type* Parser::deal_if()
{
	auto condition = deal_expression();
	auto conseq = deal_expression();
	auto alter = deal_expression();
	return new Type_If(condition,conseq,alter);
}
std::pair<Type*, Type*> Parser::deal_cond_part()
{
	lexer.get_next_token(); // (
	auto condition = deal_expression(); 
	auto conseq = deal_expression();
	lexer.get_next_token(); // )
	return std::make_pair(condition, conseq);
}

Type* Parser::deal_cond()
{
	if (lexer.get_next_token() != "(")
	{
		lexer.put_formal_token();
		return nullptr;
	}

	lexer.put_formal_token();

	auto part = deal_cond_part();
	if (part.first->type_info == VARIABLE) // else
	{
		auto var = static_cast<Type_Variable*>(part.first);
		if (var->name == "else")
			return part.second;
	}
	return new Type_If(part.first, part.second, deal_cond());
}
std::vector<std::string> Parser::deal_parms_name()
{
	std::vector<std::string> result;
	lexer.get_next_token(); //(
	std::string str;
	while ((str = lexer.get_next_token()) != ")")
		result.push_back(str);
	return result;
}
Type* Parser::deal_parm_value()
{
	return (deal_expression());
}
std::vector<Type*> Parser::deal_parms_value()
{
	std::vector<Type*> parms;
	while ((lexer.get_next_token()) != ")")
	{
		lexer.put_formal_token();
		parms.push_back(deal_expression());
	}
	lexer.put_formal_token();
	return parms;
}
std::vector<Type*> Parser::get_all_ast()
{
	std::vector<Type*> result;
	Type* exp = nullptr;
	while (exp=deal_expression())
		result.push_back(exp);
	return result;
}
