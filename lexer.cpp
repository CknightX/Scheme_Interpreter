#include "lexer.h"
#include<fstream>
#include<sstream>
void Lexer::load_from_str(std::string source)
{
	this->source = source;
}

void Lexer::load_from_file(std::string path)
{
	std::ifstream file(path, std::ios::in);
	std::stringstream sstr;
	sstr << file.rdbuf();
	file.close();
	source = sstr.str();
}
std::string Lexer::get_next_token()
{
	if (!is_go_next)
	{
		is_go_next = true;
		return curr_token;
	}

	status = START;
	curr_token = "";
	char c=get_char();
	if (c == 0)
	{
		is_end = true;
		return "";
	}
	put_char();
	while (c=get_char())
	{
		bool is_finish = false;
		bool is_add = true;
		switch (status)
		{
		case START:
			if (c == ' '||c=='\t'||c=='\n')
			{
				status = START;
				is_add = false;
			}
			else if (c == '(')
			{
				status = LEFT_BRACKET;
				is_finish = true;
				break;
			}
			else if (c == ')')
			{
				status = RIGHT_BRACKET;
				is_finish = true;
			}
			else if (c == '\'')
			{
				is_add = false;
				status = SYMBOL;
			}
			else if (c == '"')
			{
				is_add = false;
				status = STRING;
			}
			else
			{
				status = KEY;
			}
			break;
		case KEY:
			if (c == ' '||c=='\n' ||c=='\t'|| c == '('||c==')')
			{
				is_add = false;
				is_finish = true;
				put_char();
			}
			break;
		case SYMBOL:
			if (c == ' ' || c == '\n' || c == '\t'|| c == ')')
			{
				is_add = false;
				is_finish = true;
				put_char();
			}
			break;
		case STRING:
			if (c == '"')
			{
				is_add = false;
				is_finish = true;
			}
			break;
		}
		if (is_add)
			curr_token += c;
		if (is_finish)
			break;
	}
	return curr_token;
}