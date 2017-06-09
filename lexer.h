#ifndef _LEXER_H
#define _LEXER_H
#include<string>
#include<vector>

/*´Ê·¨·ÖÎöÆ÷*/

class Lexer
{
public:
	Lexer() :index(0),is_end(false),is_go_next(true){}
	Lexer(const std::string& source) :source(source), is_go_next(true){}
	~Lexer(){}
public:
	void load_from_file(std::string path);
	void load_from_str(std::string source);
	std::string get_next_token();
	void put_formal_token(){ is_go_next = false; }

	bool is_end;
private:
	size_t index;
	std::string curr_token;
	bool is_go_next;
	char get_char(){ if (index >= source.size())return 0; return source[index++]; }
	void put_char(){ --index; }
	std::string source;
	std::vector<std::string> pattern;

};
#endif