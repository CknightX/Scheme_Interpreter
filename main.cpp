#include<iostream>
#include<vector>
#include<string>
#include "parser.h"
#include "eval.h"
#include "test.h"
int main()
{  
	std::string code = "(define (append list1 list2)\
		(if (null? list1)\
		list2\
		(cons(car list1) (append(cdr list1) list2))))\
						(define (fringe items)\
						(cond((null? items) null)\
						((not(pair? items)) (list items))\
						(else (append(fringe(car items)) (fringe(cdr items))))))\
												(fringe (list (cons 1 2) (cons 3 4) (list 5 6)))\
		";
	std::string code2 = "(define (x y) (lambda (a b) (+ a b y))) ((x 2) 3 4)";
	std::string code3 = "(define (x y) (+ 10 y)) (x 10)";
	Parser a(code);
	Env* env = new Env;
	Eval b(env);
	b.create_base_env();	//创建基础环境
	auto source = a.get_all_ast(); 
	Type* result = nullptr;
	try
	{
		for (auto code_line : source)
		{
			std::string result = b.convert_scheme2str(b.eval(code_line));
			if (result != "")
				std::cout << result << std::endl;
			//result = b.eval(code_line);
		}
	}
	catch (const char* str)
	{
		std::cerr << str;
	}
	std::cin.get();
}