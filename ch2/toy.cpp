#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>
#include <cctype>
#include <iostream>

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
using namespace std;
//Use enumeration for token types
enum Token_Type
{
	EOF_TOKEN = 0,
	NUMERIC_TOKEN,
	IDENTIFIER_TOKEN,
	PARAN_TOKEN,
	DEF_TOKEN
};

static int Numeric_val;

static std::string Identifier_string;

static int get_token(FILE *file)
{
	static int LastChar = ' ';
	while (isspace(LastChar))
		LastChar = fgetc(file);
	if (isalpha(LastChar))
	{
		Identifier_string = LastChar;

		while (isalnum((LastChar = fgetc(file))))
			Identifier_string += LastChar;

		if (Identifier_string == "def")
			return DEF_TOKEN;

		return IDENTIFIER_TOKEN;
	}
	if (isdigit(LastChar))
	{
		std::string NumStr;
		do
		{
			NumStr += LastChar;
			LastChar = fgetc(file);
		} while (isdigit(LastChar));

		Numeric_val = strtod(NumStr.c_str(), 0);

		return NUMERIC_TOKEN;
	}
	if (LastChar == '#')
	{
		do
			LastChar = fgetc(file);
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return get_token(file);
	}
	if (LastChar == EOF)
		return EOF_TOKEN;
	int ThisChar = LastChar;
	LastChar = fgetc(file);

	return ThisChar;
}

class BaseAST
{
public:
	virtual ~BaseAST();
};

class VariableAST : public BaseAST
{
	std::string Var_Name;
	// string object to store name of  the variable.
public:
	VariableAST(std::string &name) : Var_Name(name) {} // parameterized constructor of variable AST class to be initialized with the string passed to the constructor.
};

class NumericAST : public BaseAST
{
	int numeric_val;

public:
	NumericAST(intval) : numeric_val(val)
};

class BinaryAST : public BaseAST
{
	std::string Bin_Operator; // string object to store binary operator
	BaseAST *LHS, *RHS;		  // Objects used to store LHS and
							  // RHS of a binary Expression. The LHS and RHS binary
							  // operation can be of any type, hence a BaseAST object
							  // is used to store them.

public:
	BinaryAST(std::string op, BaseAST *lhs, BaseAST *rhs) : Bin_Operator(op), LHS(lhs), RHS(rhs) {} // Constructor
																									// to initialize binary operator, lhs and rhs of the binary
																									// expression.
};

class FunctionDeclAST
{
	std::string Func_Name;
	std::vector<std::string> Arguments;

public:
	FunctionDeclAST(const std::string &name, const std::vector<std::string> &args) : Func_Name(name), Arguments(args){};
};

class FunctionDefnAST
{
	FunctionDeclAST *Func_Decl;
	BaseAST *Body;

public:
	FunctionDefnAST(FunctionDeclAST *proto, BaseAST *body) : Func_Decl(proto), Body(body) {}
};

class FunctionCallAST : public BaseAST
{
	std::string Function_Callee;
	std::vector<BaseAST *> Function_Arguments;

public:
	FunctionCallAST(const std::string &callee, std::vector<BaseAST *> &args) : Function_Callee(callee), Function_Arguments(args) {}
};

static int Current_token;

static void next_token(FILE *file)
{
	Current_token = get_token(file);
}

static BaseAST *Base_Parser()
{
	switch (Current_token)
	{
	default:
		return 0;
	case IDENTIFIER_TOKEN:
		return identifier_parser();
	case NUMERIC_TOKEN:
		return numeric_parser();
	case '(':
		return paran_parser();
	}
}

static BaseAST *numeric_parser()
{
	BaseAST *Result = new NumericAST(Numeric_val);
	next_token();

	return Result;
}

static BaseAST *identifier_parser()
{
	std::string IdName = Identifier_string;
	next_token();
	if (Current_token != '(')
		return new VariableAST(IdName);
	next_token();
	std::vector<BaseAST *> Args;

	if (Current_token != ')')
	{
		while (1)
		{
			BaseAST *Arg = expression_parser();

			if (!Arg)
				return 0;
			Args.push_back(Arg);

			if (Current_token == ')')
				break;

			if (Current_token != ',')
				return 0;
			next_token();
		}
	}

	next_token();

	return new FunctionCallAST(IdName, Args);
}

static FunctionDeclAST *func_decl_parser()
{
	if (Current_token != IDENTIFIER_TOKEN)
		return 0;
	std::string FnName = Identifier_string;
	next_token();
	if (Current_token != '(')
		return 0;
	std::vector<std::string> Function_Argument_Names;
	while (next_token() == IDENTIFIER_TOKEN)
		Function_Argument_Names.push_back(Identifier_string);
	if (Current_token != ')')
		return 0;
	next_token();
	return new FunctionDeclAST(FnName, Function_Argument_Names);
}

static FunctionDefnAST *func_defn_parser()
{
	next_token();
	FunctionDeclAST *Decl = func_decl_parser();
	if (Decl == 0)
		return 0;
	if (BaseAST *Body = expression_parser())
		return new FunctionDefnAST(Decl, Body);
	return 0;
}

static BaseAST *expression_parser()
{
	BaseAST *LHS = Base_Parser();
	if (!LHS)
		return 0;
	return binary_op_parser(0, LHS);
}

static std::map<char, int> Operator_Precedence;

static void init_precedence()
{
	Operator_Precedence['-'] = 1;
	Operator_Precedence['+'] = 2;
	Operator_Precedence['/'] = 3;
	Operator_Precedence['*'] = 4;
}

static int getBinOpPrecedence()
{
	if (!isascii(Current_token))
		return -1;
	int TokPrec = Operator_Precedence[Current_token];
	if (TokPrec <= 0)
		return -1;
	return TokPrec;
}

static BaseAST *binary_op_parser(int Old_Prec, BaseAST *LHS)
{
	while (1)
	{
		int Operator_Prec = getBinOpPrecedence();
		if (Operator_Prec < Old_Prec)
			return LHS;
		int BinOp = Current_token;
		next_token();
		BaseAST *RHS = Base_Parser();
		if (!RHS)
			return 0;
		int Next_Prec = getBinOpPrecedence();
		if (Operator_Prec < Next_Prec)
		{
			RHS = binary_op_parser(Operator_Prec + 1, RHS);
			if (RHS == 0)
				return 0;
		}
		LHS = new BinaryAST(std::to_string(BinOp), LHS, RHS);
	}
}

static BaseAST *paran_parser()
{
	next_token();
	BaseAST *V = expression_parser();
	if (!V)
		return 0;
	if (Current_token != ')')
		return 0;
	return V;
}

static void HandleDefn()
{
	if (FunctionDefnAST *F = func_defn_parser())
	{
		if (Function *LF = F->Codegen())
		{
		}
	}
	else
	{
		next_token();
	}
}
static void HandleTopExpression()
{
	if (FunctionDefnAST *F = top_level_parser())
	{
		if (Function *LF = F->Codegen())
		{
		}
	}
	else
	{
		next_token();
	}
}

static void Driver()
{
	while (1)
	{
		switch (Current_token)
		{
		case EOF_TOKEN:
			return;
		case ';':
			next_token();
			break;
		case DEF_TOKEN:
			HandleDefn();
			break;
		default:
			HandleTopExpression();
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	LLVMContext &Context = getGlobalContext();
	init_precedence();
	FILE *file = fopen(argv[1], "r");
	if (file == 0)
	{
		printf("Could not open file\n");
	}
	next_token(file);
	Module_Ob = new Module("my compiler", Context);
	Driver();
	Module_Ob->dump();
	return 0;
}