#include "lexer.h"
#include<fstream>
void Lexer::load_from_str(std::string source)
{
	this->source = source;
}

void Lexer::load_from_file(std::string path)
{
	std::ifstream file(path, std::ios::in);
}

std::string Lexer::get_next_token()
{
	if (!is_go_next)
	{
		is_go_next = true;
		return curr_token;
	}

	curr_token = "";
	char c;
	int status = 0;
	while (c=get_char())
	{
		bool is_finish = false;
		bool is_add = true;
		switch (status)
		{
		case 0:
			if (c == ' '||c=='\t')
			{
				status = 0;
				is_add = false;
			}
			else if (c == '('||c==')')
			{
				is_finish = true;
				break;
			}
			else
			{
				status = 1;
			}
			break;
		case 1:
			if (c == ' '||c=='\n' ||c=='\t'|| c == '('||c==')')
			{
				is_add = false;
				is_finish = true;
				put_char();
			}
			break;
		}
		if (is_add)
			curr_token += c;
		if (is_finish)
			break;
	}
	if (c == 0)
		is_end = true;
	return curr_token;
}