/* 
##############################################################################################################################
Name: Christopher Nguyen 10236311
CSC 340 - Prog. Lang & Translations
Assignment #2
##############################################################################################################################

The BNF is:
<ifstmt> -> if (<boolexpr>) '{'<assign>'}' | if (<boolexpr>) '{'<assign>'}' else '{'<assign>'}'
<boolexpr> -> <boolexpr> || <boolterm> | <boolterm>
<boolterm> -> <boolterm> && <boolfactor> | <boolfactor>
<boolfactor> -> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)
<assign> -> id = <expr>; | id = <expr>; 
<assign><expr> -> <term> + <expr> | <term> -<expr> | <term>
<term> -> <factor> * <term> | <factor>/<term> | <factor> %<term> | <factor>
<factor> -> id | int_constant | (<expr>)

The EBNF is:
<ifstmt> -> if (<boolexpr>) '{'<assign>'}' [else '{'<assign>'}']
<boolexpr> -> <boolterm> {|| <boolterm>}
<boolterm> -> <boolfactor> {&& <boolfactor>}
<boolfactor> -> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)
<assign> -> id = <expr>; | id = <expr>; <assign>
<expr> -> <term> {( +|-) <term>}
<term> -> <factor> {( *|/|% ) <factor>}
<factor>-> id | int_constant | (<expr>)

NOTE:
Operator/keyword: +, -, *, /, %, (, ), ;, TRUE, FALSE, ||, &&, !, if, else, {, }
id: (a+b+...+z+A+B+...Z)(a+b+...+z+A+B+...Z+0+1+2+...+9)*
int_constant: (0+1+2+...+9)(0+1+2+...+9)*

Sample syntax #1:
if (TRUE&&FALSE){
A=sum /(total+ 47);
B=(4+A)%3;
C=B-15;}

Sample syntax #2:
if (FALSE|| (!TRUE)){
A=sum *(total-47);
B=(4+A)%3;C=B-15;
}
else{
A=(sum + total) * 4;
B=B+1;C=B/2; 
}
*/

#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

/* Global declarations */
/* Variables */

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int counter;
ifstream in_fp("syntax.txt");

/* Function declarations */
void ifStmt();
void boolExpr();
void boolTerm();
void boolFactor();
void getChar();
void addChar();
void getNonBlank();
int lex();      // to get the nextToken
void expr();
void term();
void factor();
void assign();


/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define MOD_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define ASSIGN 27
#define SEMICOL 28
#define TRUE 29
#define FALSE 30
#define IF_STMNT 31
#define ELSE 32
#define AMP 33
#define NOT_SYM 34
#define RIGHT_CURL_BRACE 35
#define LEFT_CURL_BRACE 36
#define OR 37

/******************/
/* main driver */
int main()
{
	/* Open the input data file and process its contents */

	// If the file does not....
	if (in_fp.fail())
	{
		cout << "File could not be opened\n";
		cin.get();
		exit(1);
	}
	else {
		getChar();
		//do {
			lex();  // Getting the nextToken
			ifStmt();
		//} while (nextToken != EOF);
	}
	in_fp.close();



	// Mimics the command output when building the program
	cout << "\n";

	if (counter == 0) {
		cout << "========== Rebuild All: 1 succeeded, 0 failed ==========";
	}
	else
		cout << "========== Rebuild All : 0 succeeded, " << counter << "failed ==========";




	system("PAUSE");
}

/*******************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch)
{
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '{':
		addChar();
		nextToken = LEFT_CURL_BRACE;
		break;
	case '}':
		addChar();
		nextToken = RIGHT_CURL_BRACE;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;
	case '%':
		addChar();
		nextToken = MOD_OP;
		break;
	case '=':
		addChar();
		nextToken = ASSIGN;
		break;
	case '!':
		addChar();
		nextToken = NOT_SYM;
		break;
	case ';':
		addChar();
		nextToken = SEMICOL;
		break;
	case '&':
		addChar();
		getChar();
		addChar();
		if (lexeme[0] == '&' && lexeme[1] == '&' && lexeme[2] == 0) {
			nextToken = AMP;
		}
		break;
	case '|':
		addChar();
		getChar();
		addChar();
		if (lexeme[0] == '|' && lexeme[1] == '|' && lexeme[2] == 0) {
			nextToken = OR;
		}
		break;

	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}



/*******************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		cout << " Error - lexeme is too long \n";
}

/*******************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar()
{
	in_fp.get(nextChar);
	if (in_fp.eof())   // if no more character in the file
		nextChar = EOF;

	if (nextChar != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}

/*******************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank()
{
	while (isspace(nextChar))
		getChar();
}

/******************* /
/* lex - a lexical analyzer for arithmetic expressions */
int lex()
{
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;

		// true, false, else, keywords

		if (lexeme[0] == 'i' && lexeme[1] == 'f' && lexeme[2] == 0) {
			nextToken = IF_STMNT;
		}
	    if (lexeme[0] == 'T' && lexeme[1] == 'R' && lexeme[2] == 'U' && lexeme[3] == 'E' && lexeme[4] == 0) {
			nextToken = TRUE;
		}
		if (lexeme[0] == 'F' && lexeme[1] == 'A' && lexeme[2] == 'L' && lexeme[3] == 'S' && lexeme[4] == 'E' && lexeme[5] == 0) {
			nextToken = FALSE;
		}
		if (lexeme[0] == 'e' && lexeme[1] == 'l' && lexeme[2] == 's' && lexeme[3] == 'e' && lexeme[4] == 0) {
			nextToken = ELSE;
		}
		break; // Break for case
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */
	cout << "Next token is: " << nextToken
		<< "       Next lexeme is " << lexeme << "\n";
	return nextToken;
}/* End of function lex */

void expr() {

	term();

	while (nextToken == ADD_OP || nextToken == SUB_OP || nextToken == MOD_OP) {
		lex();
		term();
	}
}

void term() {

	factor();
	while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MULT_OP || nextToken == MOD_OP) {
		lex();
		factor();
	}

}



void factor() {

	if (nextToken == IDENT || nextToken == INT_LIT) {
		lex();
	}
	else if (nextToken == LEFT_PAREN) {

		lex();
		expr();
		if (nextToken == RIGHT_PAREN) {
			lex();
		}
		else {
			cout << " Error #1 - missing a RIGHT_PAREN\n";
			counter++;
		}
	}

	else {
		cout << " Error #2 - missing a LEFT_PAREN \n";
		counter++;
	}


}




void assign() {


	if (nextToken == IDENT) {
		lex();
		if (nextToken == ASSIGN) {
			lex();
			expr();
			if (nextToken != SEMICOL) {
				cout << "ERROR #5 - statement must end with semicoln ';' \n";
				counter++;
			}
		}
		else
		{
			cout << "ERROR #4 - missing assignment ';' \n";
			counter++;
		}
	}
	else
	{
		cout << "ERROR #3 - Must start with an ID \n";
		counter++;
	}

	lex();
	if (nextToken != EOF && nextToken != RIGHT_CURL_BRACE){
		assign();
	}
}


void boolFactor() {

	if (nextToken == TRUE || nextToken == FALSE) {
		lex();
	}

	else if (nextToken == NOT_SYM) {
		lex();
		boolFactor();
	}

	else if(nextToken == LEFT_PAREN) {

		lex();
		boolExpr();
		if (nextToken == RIGHT_PAREN) {
			lex();
		}
	}
	else { 
		cout << "ERROR: Missing a right paren";
		counter++;
	}
}

void boolTerm() {

	boolFactor();

	while (nextToken == AMP) {
		lex();
		boolFactor();
	}
}


void boolExpr() {

	boolTerm();



	while (nextToken == OR) {
		lex();
		boolTerm();
	}
}


void ifStmt() {

	if (nextToken != EOF) {
		if (nextToken == IF_STMNT) {
			lex();
			if (nextToken == LEFT_PAREN) {
				lex();
				boolExpr();
				if (nextToken == RIGHT_PAREN) {
					lex();
					if (nextToken == LEFT_CURL_BRACE) {
						lex();
						assign();
						if (nextToken == RIGHT_CURL_BRACE) {
							lex();
							if (nextToken == ELSE) {
								lex();
								if (nextToken == LEFT_CURL_BRACE) {
									lex();
									assign();
									if (nextToken == RIGHT_CURL_BRACE)
									{	
										lex();
										// THE ELSE STATEMENT (IF THERE IS ONE)

									}
									else {
										cout << "IF ERROR 7: Missing a right curly brace \n";
										counter++;
									}
								}
								else
								{
									cout << "IF ERROR 6: Missing a left curly brace \n";
									counter++;
								}
							}
						}
						else {
							cout << "IF ERROR 5: Missing right curly brace";
							counter++;
						}
					}
					else {
						cout << "IF ERROR 4: Missing left paren \n";
						counter++;
					}
				}
				else { 
					cout << "IF ERROR 3: Missing right paren \n"; 
					counter++;
				}
			}
			else {
				cout << "IF ERROR 2: Missing Left paren \n";
				counter++;
			}
		}
		else {
			cout << "IF ERROR 1: Missing the IF Token \n";
		}
	}
}




