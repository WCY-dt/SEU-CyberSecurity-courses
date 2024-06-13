#include <Windows.h>
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

#define EPSILON -1
#define CLOSURE '*'
#define OPEN_PAREN '('
#define CLOSE_PAREN ')'
#define OR '+'
#define CONCAT '.'

struct Token
{
	string value;
	string type;
};

vector<Token> Tokens;

class Tree;
class TreeNode;
class LinkedList;
class node;
class Parser;

void color(int number)
{
	HANDLE color;
	color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, number);
}

class State
{
public:
	set<State *> NFAStates;

	typedef vector<State *> Table;
	typedef set<State *>::iterator StateIterator;
	multimap<char, State *> Transition;
	int StateID;
	bool Accept;
	bool Marked;
	int GroupID;

	State() : StateID(-1), Accept(false){};
	State(int nID) : StateID(nID), Accept(false), GroupID(0){};
	State(std::set<State *> NFAState, int nID)
	{
		NFAStates = NFAState;
		StateID = nID;
		Accept = false;
		GroupID = 0;
		StateIterator iter;
		for (iter = NFAState.begin(); iter != NFAState.end(); ++iter)
		{
			if ((*iter)->Accept)
			{
				Accept = true;
			}
		}
	}
	State(const State &other)
	{
		*this = other;
	}

	~State()
	{
		NFAStates.clear();
		Transition.clear();
	}

	void AddTransition(char inputCh, State *pState)
	{
		Transition.insert(std::make_pair(inputCh, pState));
	}

	void GetTransition(char inputCh, Table &States)
	{
		States.clear();
		multimap<char, State *>::iterator iter;
		for (iter = Transition.lower_bound(inputCh); iter != Transition.upper_bound(inputCh); ++iter)
		{
			State *pState = iter->second;
			States.push_back(pState);
		}
	}

	State &operator=(const State &other)
	{
		this->Transition = other.Transition;
		this->StateID = other.StateID;
		this->NFAStates = other.NFAStates;
		return *this;
	}

	bool operator==(const State &other)
	{
		if (NFAStates.empty())
			return (StateID == other.StateID);
		else
			return (NFAStates == other.NFAStates);
	}

	string getStringID()
	{
		stringstream out;
		out << StateID;
		return out.str();
	}

	set<State *> &GetNFAState()
	{
		return NFAStates;
	}
};

typedef vector<State *> Table;
typedef Table::reverse_iterator TableReverseIterator;
typedef Table::iterator TableIterator;
typedef set<State *>::iterator StateIterator;

Table NFATable;
Table DFATable;

class LexAnalyser
{
public:
	stack<Table> ThompsonStack;
	stack<char> ExpressionStack;
	set<char> InputSet;
	string strText;
	string PostfixRE;
	char CurPreProcChar;
	int NextStateID;
	vector<string> RegularExpressions;
	string InfixRE;
	string Buffer;

	void InputFileInfixRE();
	void PreProcessInfixRE();
	string RemoveBracket(string);
	void ConvertInfixREToPostfixRE();
	bool TakesPrecedence(char, char);
	void OutputFilePostfixRE();

	bool ConvertPostfixREToNFA();

	bool Match(string);

	void CreateOneCharNFA(char);

	bool PopTable(Table &NFATable);

	bool Concat();
	bool Closure();
	bool Or();

	bool IsOperator(char inputCh);

	void EpsilonClosure(set<State *> startSet, set<State *> &result);

	void Move(char chInput, set<State *> NFAState, set<State *> &Result);

	void ConvertNFAtoDFA();

	void OutputFileNFATable();

	void InputFileProgram();

	void Scanner();

	string TypeDetection(string);

	void OutputFileTokens();
};

bool LexAnalyser::Match(string S)
{
	State *pState = DFATable[0];
	vector<State *> Transition;
	for (int i = 0; i < S.size(); i++)
	{
		char CurrChar = S[i];
		pState->GetTransition(CurrChar, Transition);
		if (Transition.empty())
			return false;
		pState = Transition[0];
	}
	if (pState->Accept)
		return true;

	return false;
}

bool LexAnalyser::ConvertPostfixREToNFA()
{
	for (int i = 0; i < (int)PostfixRE.size(); i++)
	{
		char Ch = PostfixRE[i];

		if (!IsOperator(Ch))
		{
			if (Ch == '~')
			{
				i++;
				Ch = PostfixRE[i];
			}
			CreateOneCharNFA(Ch);
		}
		else if (IsOperator(Ch))
		{
			if (Ch == '.')
				Concat();
			else if (Ch == '*')
				Closure();
			else if (Ch == '+')
				Or();
		}
	}
	PopTable(NFATable);
	NFATable[NFATable.size() - 1]->Accept = true;
	return true;
}

void LexAnalyser::CreateOneCharNFA(char ch)
{
	State *s0 = new State(NextStateID++);
	State *s1 = new State(NextStateID++);

	s0->AddTransition(ch, s1);

	Table NewSubTable;
	NewSubTable.push_back(s0);
	NewSubTable.push_back(s1);
	ThompsonStack.push(NewSubTable);

	InputSet.insert(ch);
}

bool LexAnalyser::PopTable(Table &NFATable)
{
	if (ThompsonStack.size() > 0)
	{
		NFATable = ThompsonStack.top();
		ThompsonStack.pop();
		return true;
	}
	return false;
}

bool LexAnalyser::Concat()
{
	Table LeftTable, RightTable;

	if (!PopTable(RightTable) || !PopTable(LeftTable))
		return false;

	(*(LeftTable.rbegin()))->AddTransition(EPSILON, *(RightTable.begin()));
	LeftTable.insert(LeftTable.end(), RightTable.begin(), RightTable.end());
	ThompsonStack.push(LeftTable);

	return true;
}

bool LexAnalyser::Closure()
{
	Table PrevTable;

	if (!PopTable(PrevTable))
		return false;

	State *LeftTable = new State(NextStateID++);
	State *RightTable = new State(NextStateID++);

	LeftTable->AddTransition(EPSILON, RightTable);
	LeftTable->AddTransition(EPSILON, *(PrevTable.begin()));
	(*(PrevTable.rbegin()))->AddTransition(EPSILON, RightTable);
	(*(PrevTable.rbegin()))->AddTransition(EPSILON, *(PrevTable.begin()));

	PrevTable.insert(PrevTable.begin(), LeftTable);
	PrevTable.push_back(RightTable);

	ThompsonStack.push(PrevTable);

	return true;
}

bool LexAnalyser::Or()
{
	Table UpperTable, LowerTable;

	if (!PopTable(LowerTable) || !PopTable(UpperTable))
		return false;

	State *LeftTable = new State(NextStateID++);
	State *RightTable = new State(NextStateID++);

	LeftTable->AddTransition(EPSILON, *(UpperTable.begin()));
	LeftTable->AddTransition(EPSILON, *(LowerTable.begin()));
	(*(UpperTable.rbegin()))->AddTransition(EPSILON, RightTable);
	(*(LowerTable.rbegin()))->AddTransition(EPSILON, RightTable);

	LowerTable.push_back(RightTable);
	UpperTable.insert(UpperTable.begin(), LeftTable);
	UpperTable.insert(UpperTable.end(), LowerTable.begin(), LowerTable.end());

	ThompsonStack.push(UpperTable);
	return true;
}

bool LexAnalyser::IsOperator(char Ch)
{
	if (Ch == CLOSURE || Ch == OR || Ch == OPEN_PAREN || Ch == CLOSE_PAREN || Ch == CONCAT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void LexAnalyser::EpsilonClosure(set<State *> startSet, set<State *> &result)
{
	stack<State *> UnVisitedStates;
	result = startSet;
	StateIterator iter;
	for (iter = startSet.begin(); iter != startSet.end(); ++iter)
	{
		UnVisitedStates.push(*iter);
	}

	while (!UnVisitedStates.empty())
	{
		State *curState = UnVisitedStates.top();
		UnVisitedStates.pop();

		Table epsilonStates;
		curState->GetTransition(EPSILON, epsilonStates);

		TableIterator epsilonItr;

		for (epsilonItr = epsilonStates.begin(); epsilonItr != epsilonStates.end(); ++epsilonItr)
		{
			if (result.find(*epsilonItr) == result.end())
			{
				result.insert(*epsilonItr);
				UnVisitedStates.push(*epsilonItr);
			}
		}
	}
}

void LexAnalyser::Move(char chInput, set<State *> NFAState, set<State *> &Result)
{
	Result.clear();
	StateIterator iter;
	for (iter = NFAState.begin(); iter != NFAState.end(); ++iter)
	{
		Table States;
		(*iter)->GetTransition(chInput, States);
		for (int index = 0; index < (int)States.size(); ++index)
		{
			Result.insert(States[index]);
		}
	}
}

void LexAnalyser::ConvertNFAtoDFA()
{
	set<State *> NFAStartStateSet;
	NFAStartStateSet.insert(NFATable[0]);

	set<State *> DFAStartStateSet;
	EpsilonClosure(NFAStartStateSet, DFAStartStateSet);

	NextStateID = 0;
	State *DFAStartState = new State(DFAStartStateSet, NextStateID++);

	DFATable.push_back(DFAStartState);

	Table UnVisitedStates;
	UnVisitedStates.push_back(DFAStartState);
	while (!UnVisitedStates.empty())
	{

		State *CurDFAState = UnVisitedStates[UnVisitedStates.size() - 1];
		UnVisitedStates.pop_back();

		std::set<char>::iterator iter;
		for (iter = InputSet.begin(); iter != InputSet.end(); ++iter)
		{
			std::set<State *> MoveRes, EpsilonClosureRes;

			Move(*iter, CurDFAState->GetNFAState(), MoveRes);
			EpsilonClosure(MoveRes, EpsilonClosureRes);

			StateIterator MoveResItr;
			StateIterator EpsilonClosureResItr;

			bool bFound = false;
			State *s = NULL;
			for (int i = 0; i < (int)DFATable.size(); ++i)
			{
				s = DFATable[i];
				if (s->GetNFAState() == EpsilonClosureRes)
				{
					bFound = true;
					break;
				}
			}
			if (!bFound)
			{
				State *U = new State(EpsilonClosureRes, NextStateID++);
				UnVisitedStates.push_back(U);
				DFATable.push_back(U);
				CurDFAState->AddTransition(*iter, U);
			}
			else
			{
				CurDFAState->AddTransition(*iter, s);
			}
		}
	}
}

void LexAnalyser::ConvertInfixREToPostfixRE()
{
	stack<char> OperatorStack;
	char TopSymbol, Symbol;
	int k;

	for (k = 0; k < InfixRE.size(); k++)
	{
		Symbol = InfixRE[k];

		if (Symbol == '~')
		{
			PostfixRE = PostfixRE + Symbol;
			k++;
			Symbol = InfixRE[k];
			PostfixRE = PostfixRE + Symbol;
		}
		else if (Symbol == '(')
		{
			OperatorStack.push(Symbol);
		}
		else if (Symbol == ')')
		{
			while (OperatorStack.top() != '(')
			{
				TopSymbol = OperatorStack.top();
				OperatorStack.pop();
				PostfixRE = PostfixRE + TopSymbol;
			}
			OperatorStack.pop();
		}
		else if (IsOperator(Symbol))
		{
			if (OperatorStack.empty() || (!(OperatorStack.empty()) && TakesPrecedence(OperatorStack.top(), Symbol)))
			{
				OperatorStack.push(Symbol);
			}
			else
			{
				while (!(OperatorStack.empty()) && !TakesPrecedence(OperatorStack.top(), Symbol))
				{
					TopSymbol = OperatorStack.top();
					OperatorStack.pop();
					PostfixRE = PostfixRE + TopSymbol;
				}
				OperatorStack.push(Symbol);
			}
		}
		else
		{
			PostfixRE = PostfixRE + Symbol;
		}
	}
	while (!OperatorStack.empty())
	{
		TopSymbol = OperatorStack.top();
		OperatorStack.pop();
		PostfixRE = PostfixRE + TopSymbol;
	}
	PostfixRE += "\0";
}

void LexAnalyser::OutputFilePostfixRE()
{
	fstream f;
	f.open("postfixRE.txt", ios::out);
	f << PostfixRE;
	f.close();
}

string LexAnalyser::RemoveBracket(string s)
{
	string::size_type startPos, endPos, separatorPos;
	string ReplacedStr;

	startPos = s.find_first_of("[");
	endPos = s.find_first_of("]");
	separatorPos = s.find_first_of("-");

	if (startPos == string::npos || endPos == string::npos)
		return s;

	ReplacedStr += s.substr(0, startPos);
	ReplacedStr.push_back('(');
	string result = s.substr(startPos + 1, endPos - startPos - 1);
	char first = result[0];
	char last = result[result.size() - 1];

	if (separatorPos != string::npos)
	{
		while (first < last)
		{
			ReplacedStr.push_back(first);
			ReplacedStr += "+";
			first++;
		}
		ReplacedStr.push_back(first);
	}
	else
	{
		startPos++;
		while (startPos != endPos - 1)
		{
			ReplacedStr.push_back(s[startPos]);
			ReplacedStr += "+";
			startPos++;
		}
		ReplacedStr.push_back(s[endPos - 1]);
	}
	ReplacedStr.push_back(')');
	ReplacedStr += s.substr(endPos + 1, s.size() - endPos);

	return ReplacedStr;
}

void LexAnalyser::InputFileInfixRE()
{
	fstream file;
	string temp;

	file.open("RegularExpression.txt", ios::in);

	if (!file)
	{
		cout << "no RE file available\n";
		system("pause");
		exit(0);
	}

	while (!file.eof())
	{
		file >> temp;
		this->RegularExpressions.push_back(temp);
	}
}

void LexAnalyser::PreProcessInfixRE()
{
	int i;
	for (i = 0; i < RegularExpressions.size() - 1; i++)
	{
		InfixRE += "(" + RegularExpressions[i] + ")" + "+";
	}
	InfixRE += "(" + RegularExpressions[i] + ")";

	while (InfixRE.find("[") != string::npos)
	{
		InfixRE = RemoveBracket(InfixRE);
	}
}

void LexAnalyser::OutputFileNFATable()
{
	fstream f;

	f.open("NFA.dat", ios::out);

	if (!f)
	{
		cout << "no dat file available\n";
		system("pause");
		exit(0);
	}
	for (int i = 0; i < (int)NFATable.size(); ++i)
	{
		State *pState = NFATable[i];
		if (pState->Accept)
		{
			f << pState->getStringID() << "\n";
		}
	}
	for (int i = 0; i < (int)NFATable.size(); ++i)
	{
		State *pState = NFATable[i];
		vector<State *> State;
		pState->GetTransition(EPSILON, State);
		for (int j = 0; j < (int)State.size(); ++j)
		{
			f << pState->getStringID() << "  " << State[j]->getStringID() << "  epsilon\n";
		}
		set<char>::iterator iter;
		for (iter = InputSet.begin(); iter != InputSet.end(); ++iter)
		{
			pState->GetTransition(*iter, State);
			for (int j = 0; j < (int)State.size(); ++j)
			{
				f << pState->getStringID() + "  " << State[j]->getStringID();
				stringstream out;
				out << *iter;
				f << "  " << out.str() << "\n";
			}
		}
	}
	f.close();
}

void LexAnalyser::InputFileProgram()
{
	fstream file;
	string temp;

	file.open("input.pas", ios::in);

	if (!file)
	{
		cout << "no program file available\n";
		system("pause");
		exit(0);
	}

	while (!file.eof())
	{
		Buffer += file.get();
	}
}

void LexAnalyser::Scanner()
{
	int first = 0;
	int follow = 0;
	int save = 0;

	while (first < this->Buffer.size() - 1)
	{
		follow = first;

		while (follow < Buffer.size())
		{
			if (Match(Buffer.substr(first, follow - first + 1)))
				save = follow;

			follow++;
		}

		follow = save;

		Token temp;
		temp.value = Buffer.substr(first, follow - first + 1);
		temp.type = TypeDetection(temp.value);

		Tokens.push_back(temp);

		first = follow + 1;

		while ((Buffer[first] == ' ' || Buffer[first] == '\n' || Buffer[first] == '\t') && Buffer[first] != '\0')
			first++;
	}
}

bool LexAnalyser::TakesPrecedence(char topStack, char ch)
{
	if (topStack == '(')
	{
		return true;
	}
	char operators[6] = {'+', '.', '*', '('};

	int i, j;
	for (i = 0; i < strlen(operators); i++)
	{
		if (operators[i] == topStack)
			break;
	}
	for (j = 0; j < strlen(operators); j++)
	{
		if (operators[j] == ch)
			break;
	}

	if (i >= j)
		return false;
	else
		return true;
}

string LexAnalyser::TypeDetection(string s)
{
	if (s == "main" || s == "if" || s == "then" || s == "else" || s == "while" || s == "do" || s == "begin" || s == "end" || s == "var" || s == "integer" || s == "real" || s == "for" || s == "function" || s == "array" || s == "procedure" || s == "result" || s == "program")
	{
		return "KEYWORD";
	}
	else if (s == "=" || s == "<>" || s == "<=" || s == ">=" || s == ">" || s == "<")
	{
		return "RELOP";
	}
	else if (s == "+" || s == "-" || s == "or")
	{
		return "ADDOP";
	}
	else if (s == "*" || s == "/" || s == "div" || s == "mod" || s == "and")
	{
		return "MULOP";
	}
	else if (s == ":=")
	{
		return "ASSIGNOP";
	}
	else if (s == "(" || s == ")" || s == "," || s == ":" || s == ";")
	{
		return "SPECIALSYMBOL";
	}
	else if (s[0] <= '9' && s[0] >= '0')
	{
		return "CONSTANT";
	}
	else
	{
		return "IDENTIFIER";
	}
}

void LexAnalyser::OutputFileTokens()
{
	fstream f;
	f.open("output.txt", ios::out);
	for (int i = 0; i < Tokens.size(); i++)
	{
		f << Tokens[i].value << "\t" << Tokens[i].type << "\n";
	}
	f.close();
}

class LinkedListNode
{
public:
	TreeNode *data;
	LinkedListNode *next;
	LinkedListNode()
	{
		next = NULL;
	}
	LinkedListNode(TreeNode *x)
	{
		data = x;
		next = NULL;
	}
};

class LinkedList
{
public:
	LinkedListNode *first;
	LinkedList()
	{
		first = NULL;
	}

	bool insert(TreeNode *x)
	{
		LinkedListNode *temp = new LinkedListNode(x);
		if (!temp)
			return false;
		if (!first)
		{
			first = temp;
			return true;
		}
		LinkedListNode *help = first;
		while (help->next)
			help = help->next;
		help->next = temp;
		return true;
	}
};

class TreeNode
{
public:
	TreeNode(string label)
	{
		data = label;
	}

	TreeNode()
	{
		data = "";
	}

	string data;
	LinkedList childs;
};

class Parser
{
public:
	Parser();

	TreeNode *program();
	TreeNode *identifier_list();
	TreeNode *identifier_list_prime();
	TreeNode *declarations();
	TreeNode *declaration_list();
	TreeNode *declaration_list_prime();
	TreeNode *type();
	TreeNode *standard_type();
	TreeNode *array_type();
	TreeNode *argumenrs();
	TreeNode *parameter_list();
	TreeNode *parameter_list_prime();
	TreeNode *statement_list();
	TreeNode *statement_list_prime();
	TreeNode *statement();
	TreeNode *statement_prime();
	TreeNode *restricted_statement();
	TreeNode *elementary_statement();
	TreeNode *elementary_statement_prime();
	TreeNode *variable();
	TreeNode *variable_prime();
	TreeNode *subprogram_declarations();
	TreeNode *subprogram_declarations_prime();
	TreeNode *subprogram_declaration();
	TreeNode *subprogram_head();
	TreeNode *compound_statement();
	TreeNode *expression();
	TreeNode *expression_prime();
	TreeNode *expression_list();
	TreeNode *expression_list_prime();
	TreeNode *simple_expression();
	TreeNode *simple_expression_prime();
	TreeNode *term();
	TreeNode *term_prime();
	TreeNode *factor();

	void Error();
	Token NextToken();
	void CreateTree();

	void OutputFileHTMLTree();
	void CreateHTMLTree(TreeNode *);
	void PrintTree(TreeNode *);

	TreeNode *root;
	int TokenIndex;
	Token token;
	string nexttoken;
	bool GoodProgram;
	string HTMLTree;
	bool See_Epsilon;
};

void Parser::CreateTree()
{
	token = NextToken();
	root = program();
}

Parser::Parser()
{
	TokenIndex = 0;
	GoodProgram = true;
	See_Epsilon = false;
}

void Parser::Error()
{
	cout << "Syntax Error near " << token.value << "\n";
	TokenIndex++;
	GoodProgram = false;
}

Token Parser::NextToken()
{
	if (See_Epsilon == true)
	{
		See_Epsilon = false;
		return token;
	}
	return Tokens[TokenIndex++];
}

TreeNode *Parser::factor()
{
	if (token.value == "not")
	{
		TreeNode *not_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *factor_node = factor();

		if (factor_node != NULL)
		{
			TreeNode *myroot = new TreeNode("factor");

			myroot->childs.insert(not_node);
			myroot->childs.insert(factor_node);

			return myroot;
		}
	}
	else if (token.value == "(")
	{
		TreeNode *parantez_open_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_list_node = expression_list();

		if (expression_list_node != NULL)
		{
			token = NextToken();

			if (token.value == ")")
			{
				TreeNode *parantez_close_node = new TreeNode(token.value);

				TreeNode *myroot = new TreeNode("factor");

				myroot->childs.insert(parantez_open_node);
				myroot->childs.insert(expression_list_node);
				myroot->childs.insert(parantez_close_node);

				return myroot;
			}
		}
	}
	else if (token.type == "CONSTANT")
	{
		TreeNode *CONSTANT_node = new TreeNode(token.value);

		TreeNode *myroot = new TreeNode("factor");

		myroot->childs.insert(CONSTANT_node);

		return myroot;
	}
	else
	{
		TreeNode *variable_node = variable();

		if (variable_node != NULL)
		{
			TreeNode *myroot = new TreeNode("factor");

			myroot->childs.insert(variable_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::term_prime()
{
	if (token.type == "MULOP")
	{
		TreeNode *MULOP_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *factor_node = factor();

		if (factor_node != NULL)
		{
			token = NextToken();

			TreeNode *term_prime_node = term_prime();

			if (term_prime_node != NULL)
			{
				TreeNode *myroot = new TreeNode("term_prime");

				myroot->childs.insert(MULOP_node);
				myroot->childs.insert(factor_node);
				myroot->childs.insert(term_prime_node);

				return myroot;
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::term()
{
	TreeNode *factor_node = factor();

	if (factor_node != NULL)
	{
		token = NextToken();

		TreeNode *term_node = term_prime();

		if (term_node != NULL)
		{
			TreeNode *myroot = new TreeNode("term");

			myroot->childs.insert(factor_node);
			myroot->childs.insert(term_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::simple_expression_prime()
{
	if (token.type == "ADDOP")
	{
		TreeNode *ADDOP_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *term_node = term();

		if (term_node != NULL)
		{
			token = NextToken();

			TreeNode *simple_expression_prime_node = simple_expression_prime();

			if (simple_expression_prime_node != NULL)
			{
				TreeNode *myroot = new TreeNode("simple_expression_prime");

				myroot->childs.insert(ADDOP_node);
				myroot->childs.insert(term_node);
				myroot->childs.insert(simple_expression_prime_node);

				return myroot;
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::simple_expression()
{
	if (token.value == "+" || token.value == "-")
	{
		TreeNode *sign_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *term_node = term();

		if (term_node != NULL)
		{
			token = NextToken();

			TreeNode *simple_expression_prime_node = simple_expression_prime();

			if (simple_expression_prime_node != NULL)
			{
				TreeNode *myroot = new TreeNode("simple_expression");

				myroot->childs.insert(sign_node);
				myroot->childs.insert(term_node);
				myroot->childs.insert(simple_expression_prime_node);

				return myroot;
			}
		}
	}
	else
	{
		TreeNode *term_node = term();

		if (term_node != NULL)
		{
			token = NextToken();

			TreeNode *simple_expression_prime_node = simple_expression_prime();

			if (simple_expression_prime_node != NULL)
			{
				TreeNode *myroot = new TreeNode("simple_expression");

				myroot->childs.insert(term_node);
				myroot->childs.insert(simple_expression_prime_node);

				return myroot;
			}
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::expression_prime()
{
	if (token.type == "RELOP")
	{
		TreeNode *RELOP_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *simple_expression_node = simple_expression();

		if (simple_expression_node != NULL)
		{
			TreeNode *myroot = new TreeNode("expression_prime");

			myroot->childs.insert(RELOP_node);
			myroot->childs.insert(simple_expression_node);

			return myroot;
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::expression()
{
	TreeNode *simple_expression_node = simple_expression();

	if (simple_expression_node != NULL)
	{
		token = NextToken();

		TreeNode *expression_prime_node = expression_prime();

		if (expression_prime_node != NULL)
		{
			TreeNode *myroot = new TreeNode("expression");

			myroot->childs.insert(simple_expression_node);
			myroot->childs.insert(expression_prime_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::expression_list_prime()
{
	if (token.value == ",")
	{
		TreeNode *comma_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			token = NextToken();

			TreeNode *expression_list_prime_node = expression_list_prime();

			if (expression_list_prime_node != NULL)
			{
				TreeNode *myroot = new TreeNode("expression_list_prime");

				myroot->childs.insert(comma_node);
				myroot->childs.insert(expression_node);
				myroot->childs.insert(expression_list_prime_node);

				return myroot;
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::expression_list()
{
	TreeNode *expression_node = expression();

	if (expression_node != NULL)
	{
		token = NextToken();

		TreeNode *expression_list_prime_node = expression_list_prime();

		if (expression_list_prime_node != NULL)
		{
			TreeNode *myroot = new TreeNode("expression_list");

			myroot->childs.insert(expression_node);
			myroot->childs.insert(expression_list_prime_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::variable_prime()
{
	if (token.value == "(")
	{
		TreeNode *parantez_open_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_list_node = expression_list();

		if (expression_list_node != NULL)
		{
			token = NextToken();

			if (token.value == ")")
			{
				TreeNode *parantez_close_node = new TreeNode(token.value);

				TreeNode *myroot = new TreeNode("variable_prime");

				myroot->childs.insert(parantez_open_node);
				myroot->childs.insert(expression_list_node);
				myroot->childs.insert(parantez_close_node);

				return myroot;
			}
		}
	}
	else if (token.value == "[")
	{
		TreeNode *bracket_open_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			token = NextToken();

			if (token.value == "]")
			{
				TreeNode *bracket_close_node = new TreeNode(token.value);

				TreeNode *myroot = new TreeNode("variable_prime");

				myroot->childs.insert(bracket_open_node);
				myroot->childs.insert(expression_node);
				myroot->childs.insert(bracket_close_node);

				return myroot;
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::variable()
{
	if (token.type == "IDENTIFIER")
	{
		TreeNode *IDENTIFIER_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *variable_prime_node = variable_prime();

		TreeNode *myroot = new TreeNode("variable");

		myroot->childs.insert(IDENTIFIER_node);
		myroot->childs.insert(variable_prime_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::elementary_statement_prime()
{
	if (token.type == "ASSIGNOP")
	{
		TreeNode *ASSIGNOP_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			TreeNode *myroot = new TreeNode("elementary_statement_prime");

			myroot->childs.insert(ASSIGNOP_node);
			myroot->childs.insert(expression_node);

			return myroot;
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode("elementary_statement_prime");

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::elementary_statement()
{
	TreeNode *variable_node = variable();

	if (variable_node != NULL)
	{
		token = NextToken();

		TreeNode *elementary_statement_prime_node = elementary_statement_prime();

		if (elementary_statement_prime_node != NULL)
		{
			TreeNode *myroot = new TreeNode("elementary_statement");

			myroot->childs.insert(variable_node);
			myroot->childs.insert(elementary_statement_prime_node);

			return myroot;
		}
	}
	else
	{
		TreeNode *compound_statement_node = compound_statement();

		if (compound_statement_node != NULL)
		{
			TreeNode *myroot = new TreeNode("elementary_statement");

			myroot->childs.insert(compound_statement_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::restricted_statement()
{
	if (token.value == "if")
	{
		TreeNode *if_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			token = NextToken();

			if (token.value == "then")
			{
				TreeNode *then_node = new TreeNode(token.value);

				token = NextToken();

				TreeNode *restricted_statement_node = restricted_statement();

				if (restricted_statement_node != NULL)
				{
					token = NextToken();

					if (token.value == "else")
					{
						TreeNode *else_node = new TreeNode(token.value);

						token = NextToken();

						TreeNode *restricted_statement2_node = restricted_statement();

						if (restricted_statement2_node != NULL)
						{
							TreeNode *myroot = new TreeNode("restricted_statement");

							myroot->childs.insert(if_node);
							myroot->childs.insert(expression_node);
							myroot->childs.insert(then_node);
							myroot->childs.insert(restricted_statement_node);
							myroot->childs.insert(else_node);
							myroot->childs.insert(restricted_statement2_node);

							return myroot;
						}
					}
				}
			}
		}
	}
	else if (token.value == "while")
	{
		TreeNode *while_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			token = NextToken();

			if (token.value == "do")
			{
				TreeNode *do_node = new TreeNode(token.value);

				token = NextToken();

				TreeNode *restricted_statement_node = restricted_statement();

				if (restricted_statement_node != NULL)
				{
					TreeNode *myroot = new TreeNode("restricted_statement");

					myroot->childs.insert(while_node);
					myroot->childs.insert(expression_node);
					myroot->childs.insert(do_node);
					myroot->childs.insert(restricted_statement_node);

					return myroot;
				}
			}
		}
	}
	else
	{
		TreeNode *elementary_statement_node = elementary_statement();

		if (elementary_statement_node != NULL)
		{
			TreeNode *myroot = new TreeNode("restricted_statement");

			myroot->childs.insert(elementary_statement_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::statement_prime()
{
	TreeNode *restricted_statement_node = restricted_statement();

	if (restricted_statement_node != NULL)
	{
		token = NextToken();

		if (token.value == "else")
		{
			TreeNode *else_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *statement_node = statement();

			if (statement_node != NULL)
			{
				TreeNode *myroot = new TreeNode("statement_prime");

				myroot->childs.insert(restricted_statement_node);
				myroot->childs.insert(else_node);
				myroot->childs.insert(statement_node);

				return myroot;
			}
		}
	}
	else
	{
		TreeNode *statement_node = statement();

		if (statement_node != NULL)
		{
			TreeNode *myroot = new TreeNode();

			myroot->childs.insert(statement_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::statement()
{
	if (token.value == "if")
	{
		TreeNode *if_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			token = NextToken();

			if (token.value == "then")
			{
				TreeNode *then_node = new TreeNode(token.value);

				token = NextToken();

				TreeNode *statement_prime_node = statement_prime();

				if (statement_prime_node != NULL)
				{
					TreeNode *myroot = new TreeNode("statement");

					myroot->childs.insert(if_node);
					myroot->childs.insert(expression_node);
					myroot->childs.insert(then_node);
					myroot->childs.insert(statement_prime_node);

					return myroot;
				}
			}
		}
	}
	else if (token.value == "while")
	{
		TreeNode *while_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *expression_node = expression();

		if (expression_node != NULL)
		{
			token = NextToken();

			if (token.value == "do")
			{
				TreeNode *do_node = new TreeNode(token.value);

				token = NextToken();

				TreeNode *statement_node = statement();

				if (statement_node != NULL)
				{
					TreeNode *myroot = new TreeNode("statement");

					myroot->childs.insert(while_node);
					myroot->childs.insert(expression_node);
					myroot->childs.insert(do_node);
					myroot->childs.insert(statement_node);

					return myroot;
				}
			}
		}
	}
	else
	{
		TreeNode *elementary_statement_node = elementary_statement();

		if (elementary_statement_node != NULL)
		{
			TreeNode *myroot = new TreeNode("statement");

			myroot->childs.insert(elementary_statement_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::statement_list_prime()
{
	if (token.value == ";")
	{
		TreeNode *semicolon_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *statement_node = statement();

		if (statement_node != NULL)
		{
			TreeNode *myroot = new TreeNode("statement_list_prime");

			myroot->childs.insert(semicolon_node);
			myroot->childs.insert(statement_node);

			return myroot;
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::statement_list()
{
	TreeNode *statement_node = statement();

	if (statement_node != NULL)
	{
		token = NextToken();

		TreeNode *statement_list_prime_node = statement_list_prime();

		if (statement_list_prime_node != NULL)
		{
			TreeNode *myroot = new TreeNode("statement_list");

			myroot->childs.insert(statement_node);
			myroot->childs.insert(statement_list_prime_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::compound_statement()
{
	if (token.value == "begin")
	{
		TreeNode *begin_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *statement_list_node = statement_list();

		if (statement_list_node != NULL)
		{
			token = NextToken();

			if (token.value == "end")
			{
				TreeNode *end_node = new TreeNode(token.value);

				TreeNode *myroot = new TreeNode("compound_statement");

				myroot->childs.insert(begin_node);
				myroot->childs.insert(statement_list_node);
				myroot->childs.insert(end_node);

				return myroot;
			}
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::parameter_list_prime()
{
	if (token.value == ";")
	{
		TreeNode *semicolon_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *identifier_list_node = identifier_list();

		if (identifier_list_node != NULL)
		{
			token = NextToken();

			if (token.value == ":")
			{
				TreeNode *colon_node = new TreeNode(token.value);

				token = NextToken();

				TreeNode *type_node = type();

				if (type_node != NULL)
				{
					token = NextToken();

					TreeNode *parameter_list_prime_node = parameter_list_prime();

					if (parameter_list_prime_node != NULL)
					{
						TreeNode *myroot = new TreeNode("parameter_list_prime");

						myroot->childs.insert(semicolon_node);
						myroot->childs.insert(identifier_list_node);
						myroot->childs.insert(colon_node);
						myroot->childs.insert(type_node);
						myroot->childs.insert(parameter_list_prime_node);

						return myroot;
					}
				}
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::parameter_list()
{
	TreeNode *identifier_list_node = identifier_list();

	if (identifier_list_node != NULL)
	{
		token = NextToken();

		if (token.value == ":")
		{
			TreeNode *colon_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *type_node = type();

			if (type_node != NULL)
			{
				token = NextToken();

				TreeNode *parameter_list_prime_node = parameter_list_prime();

				if (parameter_list_prime_node != NULL)
				{
					TreeNode *myroot = new TreeNode("parameter_list");

					myroot->childs.insert(identifier_list_node);
					myroot->childs.insert(colon_node);
					myroot->childs.insert(type_node);
					myroot->childs.insert(parameter_list_prime_node);

					return myroot;
				}
			}
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::argumenrs()
{
	if (token.value == "(")
	{
		TreeNode *parantez_open_node = new TreeNode(token.value);

		token = NextToken();
		TreeNode *parameter_list_node = parameter_list();

		if (parameter_list_node != NULL)
		{
			token = NextToken();

			if (token.value == ")")
			{
				TreeNode *parantez_close_node = new TreeNode(token.value);

				TreeNode *myroot = new TreeNode("argumenrs");

				myroot->childs.insert(parantez_open_node);
				myroot->childs.insert(parameter_list_node);
				myroot->childs.insert(parantez_close_node);

				return myroot;
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::subprogram_head()
{
	if (token.value == "function")
	{
		TreeNode *function_node = new TreeNode(token.value);

		token = NextToken();

		if (token.type == "IDENTIFIER")
		{
			TreeNode *IDENTIFIER_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *argumenrs_node = argumenrs();

			if (argumenrs_node != NULL)
			{
				token = NextToken();

				if (token.value == ":")
				{
					TreeNode *colon_node = new TreeNode(token.value);

					token = NextToken();

					if (token.value == "result")
					{
						TreeNode *result_node = new TreeNode(token.value);

						token = NextToken();

						TreeNode *standard_type_node = standard_type();

						if (standard_type_node != NULL)
						{
							token = NextToken();

							if (token.value == ";")
							{
								TreeNode *semicolon_node = new TreeNode(token.value);

								TreeNode *myroot = new TreeNode("subprogram_head");

								myroot->childs.insert(function_node);
								myroot->childs.insert(IDENTIFIER_node);
								myroot->childs.insert(argumenrs_node);
								myroot->childs.insert(colon_node);
								myroot->childs.insert(result_node);
								myroot->childs.insert(standard_type_node);
								myroot->childs.insert(semicolon_node);

								return myroot;
							}
						}
					}
				}
			}
		}
	}
	else if (token.value == "procedure")
	{
		TreeNode *procedure_node = new TreeNode(token.value);

		token = NextToken();

		if (token.type == "IDENTIFIER")
		{
			TreeNode *IDENTIFIER_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *argumenrs_node = argumenrs();

			if (argumenrs_node != NULL)
			{
				token = NextToken();

				if (token.value == ";")
				{
					TreeNode *semicolon_node = new TreeNode(token.value);

					TreeNode *myroot = new TreeNode("subprogram_head");

					myroot->childs.insert(procedure_node);
					myroot->childs.insert(IDENTIFIER_node);
					myroot->childs.insert(argumenrs_node);
					myroot->childs.insert(semicolon_node);

					return myroot;
				}
			}
		}
	}
	else
	{
		return NULL;
	}
	Error();
	return NULL;
}

TreeNode *Parser::subprogram_declaration()
{
	TreeNode *subprogram_head_node = subprogram_head();

	if (subprogram_head_node != NULL)
	{
		token = NextToken();

		TreeNode *declarations_node = declarations();

		if (declarations_node != NULL)
		{
			token = NextToken();

			TreeNode *compound_statement_node = compound_statement();

			if (compound_statement_node != NULL)
			{
				TreeNode *myroot = new TreeNode("subprogram_declaration");

				myroot->childs.insert(subprogram_head_node);
				myroot->childs.insert(declarations_node);
				myroot->childs.insert(compound_statement_node);

				return myroot;
			}
		}
	}
	else
	{
		return NULL;
	}
	Error();
	return NULL;
}

TreeNode *Parser::subprogram_declarations()
{
	TreeNode *subprogram_declaration_node = subprogram_declaration();

	if (subprogram_declaration_node != NULL)
	{
		token = NextToken();

		TreeNode *subprogram_declarations_node = subprogram_declarations();

		if (subprogram_declarations_node != NULL)
		{
			TreeNode *myroot = new TreeNode("subprogram_declarations");

			myroot->childs.insert(subprogram_declaration_node);
			myroot->childs.insert(subprogram_declarations_node);

			return myroot;
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::array_type()
{
	if (token.value == "array")
	{
		TreeNode *array_node = new TreeNode(token.value);

		token = NextToken();

		if (token.value == "[")
		{
			TreeNode *bracket_open_node = new TreeNode(token.value);

			token = NextToken();

			if (token.type == "CONSTANT")
			{
				TreeNode *CONSTANT_node = new TreeNode(token.value);

				token = NextToken();

				if (token.value == "..")
				{
					TreeNode *dot_dot_node = new TreeNode(token.value);

					token = NextToken();

					if (token.type == "CONSTANT")
					{
						TreeNode *CONSTANT2_node = new TreeNode(token.value);

						token = NextToken();

						if (token.value == "]")
						{
							TreeNode *bracket_close_node = new TreeNode(token.value);

							token = NextToken();

							if (token.value == "of")
							{
								TreeNode *of_node = new TreeNode(token.value);

								token = NextToken();

								TreeNode *standard_type_node = standard_type();

								if (standard_type_node != NULL)
								{
									TreeNode *myroot = new TreeNode("array_type");

									myroot->childs.insert(array_node);
									myroot->childs.insert(bracket_open_node);
									myroot->childs.insert(CONSTANT_node);
									myroot->childs.insert(dot_dot_node);
									myroot->childs.insert(CONSTANT2_node);
									myroot->childs.insert(of_node);
									myroot->childs.insert(standard_type_node);

									return myroot;
								}
							}
						}
					}
				}
			}
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::standard_type()
{
	if (token.value == "integer")
	{
		TreeNode *integer_node = new TreeNode(token.value);

		TreeNode *myroot = new TreeNode("standard_type");

		myroot->childs.insert(integer_node);

		return myroot;
	}
	else if (token.value == "real")
	{
		TreeNode *real_node = new TreeNode(token.value);

		TreeNode *myroot = new TreeNode("standard_type");

		myroot->childs.insert(real_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::type()
{
	TreeNode *standard_type_node = standard_type();

	if (standard_type_node != NULL)
	{
		TreeNode *myroot = new TreeNode("type");

		myroot->childs.insert(standard_type_node);

		return myroot;
	}
	else
	{
		TreeNode *array_type_node = array_type();

		if (array_type_node != NULL)
		{
			TreeNode *myroot = new TreeNode("type");

			myroot->childs.insert(array_type_node);

			return myroot;
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::declaration_list_prime()
{
	TreeNode *identifier_list_node = identifier_list();

	if (identifier_list_node != NULL)
	{
		if (token.value == ":")
		{
			TreeNode *colon_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *type_node = type();

			if (type_node != NULL)
			{
				token = NextToken();

				if (token.value == ";")
				{
					TreeNode *semicolon_node = new TreeNode(token.value);

					token = NextToken();

					TreeNode *declaration_list_prime_node = declaration_list_prime();

					if (declaration_list_prime_node != NULL)
					{
						TreeNode *myroot = new TreeNode("declaration_list_prime");

						myroot->childs.insert(identifier_list_node);
						myroot->childs.insert(colon_node);
						myroot->childs.insert(type_node);
						myroot->childs.insert(semicolon_node);
						myroot->childs.insert(declaration_list_prime_node);

						return myroot;
					}
				}
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::declaration_list()
{
	TreeNode *identifier_list_node = identifier_list();

	if (identifier_list_node != NULL)
	{
		token = NextToken();

		if (token.value == ":")
		{
			TreeNode *colon_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *type_node = type();

			if (type_node != NULL)
			{
				token = NextToken();

				if (token.value == ";")
				{
					TreeNode *semicolon_node = new TreeNode(token.value);

					token = NextToken();

					TreeNode *declaration_list_prime_node = declaration_list_prime();

					if (declaration_list_prime_node != NULL)
					{
						TreeNode *myroot = new TreeNode("declaration_list");

						myroot->childs.insert(identifier_list_node);
						myroot->childs.insert(colon_node);
						myroot->childs.insert(type_node);
						myroot->childs.insert(semicolon_node);
						myroot->childs.insert(declaration_list_prime_node);

						return myroot;
					}
				}
			}
		}
	}
	Error();
	return NULL;
}

TreeNode *Parser::declarations()
{
	if (token.value == "var")
	{
		TreeNode *var_node = new TreeNode(token.value);

		token = NextToken();
		TreeNode *declaration_list_node = declaration_list();

		if (declaration_list_node != NULL)
		{
			TreeNode *myroot = new TreeNode("declarations");

			myroot->childs.insert(var_node);
			myroot->childs.insert(declaration_list_node);

			return myroot;
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::identifier_list_prime()
{
	if (token.value == ",")
	{
		TreeNode *comma_node = new TreeNode(token.value);

		token = NextToken();

		if (token.type == "IDENTIFIER")
		{
			TreeNode *IDENTIFIER_node = new TreeNode(token.value);

			token = NextToken();

			TreeNode *identifier_list_prime_node = identifier_list_prime();

			if (identifier_list_prime_node != NULL)
			{
				TreeNode *myroot = new TreeNode("identifier_list_prime");

				myroot->childs.insert(comma_node);
				myroot->childs.insert(IDENTIFIER_node);
				myroot->childs.insert(identifier_list_prime_node);

				return myroot;
			}
		}
	}
	else
	{
		See_Epsilon = true;

		TreeNode *epsilon_node = new TreeNode(EPSILON + "");

		TreeNode *myroot = new TreeNode();

		myroot->childs.insert(epsilon_node);

		return myroot;
	}
	Error();
	return NULL;
}

TreeNode *Parser::identifier_list()
{
	if (token.type == "IDENTIFIER")
	{
		TreeNode *IDENTIFIER_node = new TreeNode(token.value);

		token = NextToken();

		TreeNode *identifier_list_prime_node = identifier_list_prime();

		if (identifier_list_prime_node != NULL)
		{
			TreeNode *myroot = new TreeNode("identifier_list");

			myroot->childs.insert(IDENTIFIER_node);
			myroot->childs.insert(identifier_list_prime_node);

			return myroot;
		}
	}
	else
	{
		return NULL;
	}
	Error();
	TreeNode *myroot = new TreeNode("identifier_list");
	return myroot;
}

TreeNode *Parser::program()
{
	if (token.value == "program")
	{
		TreeNode *program_node = new TreeNode(token.value);

		token = NextToken();

		if (token.type == "IDENTIFIER")
		{
			TreeNode *IDENTIFIER_node = new TreeNode(token.value);

			token = NextToken();

			if (token.value == "(")
			{
				TreeNode *parantez_open_node = new TreeNode(token.value);

				token = NextToken();

				TreeNode *identifier_list_node = identifier_list();

				if (identifier_list_node != NULL)
				{
					token = NextToken();

					if (token.value == ")")
					{
						TreeNode *parantez_close_node = new TreeNode(token.value);

						token = NextToken();

						if (token.value == ";")
						{
							TreeNode *semicolon_node = new TreeNode(token.value);

							token = NextToken();

							TreeNode *declarations_node = declarations();

							if (declarations_node != NULL)
							{
								token = NextToken();
								TreeNode *subprogram_declarations_node = subprogram_declarations();

								if (subprogram_declarations_node != NULL)
								{
									token = NextToken();
									TreeNode *compound_statement_node = compound_statement();

									if (compound_statement_node != NULL)
									{
										TreeNode *myroot = new TreeNode("program");

										myroot->childs.insert(program_node);
										myroot->childs.insert(IDENTIFIER_node);
										myroot->childs.insert(parantez_open_node);
										myroot->childs.insert(identifier_list_node);
										myroot->childs.insert(parantez_close_node);
										myroot->childs.insert(semicolon_node);
										myroot->childs.insert(declarations_node);
										myroot->childs.insert(subprogram_declarations_node);
										myroot->childs.insert(compound_statement_node);

										return myroot;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	Error();
	return NULL;
}

void Parser::CreateHTMLTree(TreeNode *x)
{
	HTMLTree += "<table border=\"1px\"><tr><td colspan=\"100%\">";

	HTMLTree += x->data;

	HTMLTree += "</td></tr>";

	HTMLTree += "<tr>";

	LinkedListNode *help = x->childs.first;

	while (help)
	{
		HTMLTree += "<td>";

		CreateHTMLTree(help->data);
		help = help->next;

		HTMLTree += "</td>";
	}
	HTMLTree += "</tr></table>";
}

void Parser::OutputFileHTMLTree()
{
	fstream f;

	f.open("output.html", ios::out);

	if (!f)
	{
		cout << "no html file available\n";
		system("pause");
		exit(0);
	}

	HTMLTree = "<html><head></head><body>";

	CreateHTMLTree(root);

	HTMLTree += "</body></html>";

	for (int i = 0; i < HTMLTree.size(); i++)
	{
		f << HTMLTree[i];
	}

	f.close();
}

char depth[2056];
int di = 0;

void Parser::PrintTree(TreeNode *x)
{
	LinkedListNode *help = x->childs.first;

	if (help)
		color(11);
	else
		color(12);

	cout << x->data << "\n";

	color(10);

	while (help)
	{
		cout << depth << " +--";

		if (help->next)
		{
			depth[di++] = ' ';
			depth[di++] = '|';
			depth[di++] = ' ';
			depth[di] = 0;
		}
		else
		{
			depth[di++] = ' ';
			depth[di++] = ' ';
			depth[di++] = ' ';
			depth[di] = 0;
		}

		PrintTree(help->data);

		depth[di -= 3] = 0;

		help = help->next;
	}
}

int main()
{
	LexAnalyser LA;
	Parser PA;

	LA.InputFileInfixRE();
	LA.PreProcessInfixRE();
	LA.ConvertInfixREToPostfixRE();
	LA.OutputFilePostfixRE();
	LA.ConvertPostfixREToNFA();
	LA.OutputFileNFATable();
	LA.ConvertNFAtoDFA();
	LA.InputFileProgram();
	LA.Scanner();
	LA.OutputFileTokens();

	PA.CreateTree();

	if (PA.GoodProgram == true)
	{
		PA.OutputFileHTMLTree();
		system("output.html");

		PA.PrintTree(PA.root);
	}

	system("pause");
}