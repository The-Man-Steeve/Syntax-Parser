/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages.
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <ctype.h>
#include <stdio.h>
#include <string.h>

//#include file from "front.h"
/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
//OPS
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LESSER_OP 26
#define GREATER_OP 27
#define EQUAL_OP 28
#define NEQUAL_OP 29
#define LEQUAL_OP 30
#define GEQUAL_OP 31
#define MOD_OP 32
//SEMICOLON
#define SEMICOLON 35
//PARENTHESESE AND BRACES
#define LEFT_PAREN 40
#define RIGHT_PAREN 41
#define LEFT_CBRACE 42
#define RIGHT_CBRACE 43
//KEYS
#define KEY_READ 50
#define KEY_PRINT 51
#define KEY_WHILE 53
#define KEY_DO 54



int lex();
//#include file from "parser.h"
void expr();
void term();
void factor();
void stmt();
//functions from "parser.c"
static void error();
void var();
void number();
void comp();


/* Global Variable */
int nextToken;
int exitCode = 0;

/* Local Variables */
static int charClass;
static char lexeme[100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static void secondLookup();
static void checkKeyword();
static char *getTokenCode();

/******************************************************/
/* main driver */
int main(int argc, char* argv[]) {
  printf("DanC Parser :: R1167656\n");
  //exit code to return
  /* Open the input data file and process its contents */
  if ((in_fp = fopen(argv[1], "r")) == NULL) {
    printf("ERROR - cannot open \n");
    exitCode = 3;
  } else {
  int argSize = strlen(argv[1]);
  if(!(argv[1][argSize -1] == 'c' && argv[1][argSize -2] == 'n' &&
    argv[1][argSize-3] == 'a' && argv[1][argSize - 4] == 'd' &&
    argv[1][argSize - 5] == '.')){
    printf("Error encounter: No file Found");
    exitCode = 2;
    }
    else{
    getChar();
    do {
      lex();
      stmt();
    } while (nextToken != EOF && exitCode == 0);
  }
  }
  if(exitCode == 0)
    printf("\nSyntax Validated \n");
	if(argc < 2)
		exitCode = 2;
  return exitCode;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the
 * token */
static int lookup(char ch) {
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
    nextToken = LEFT_CBRACE;
    break;
  case '}':
    addChar();
    nextToken = RIGHT_CBRACE;
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
  case ';':
    addChar();
    nextToken = SEMICOLON;
    break;
  case '=':
    addChar();
    nextToken = ASSIGN_OP;
    break;
  case '<':
    addChar();
    nextToken = LESSER_OP;
    break;
  case '>':
    addChar();
    nextToken = GREATER_OP;
    break;
  default:
    addChar();
    nextToken = UNKNOWN;
    break;
  }
  return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
  if (lexLen <= 98) {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  } else {
    printf("Error - lexeme is too long \n");
  }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its
 * character class */
static void getChar() {
  if ((nextChar = getc(in_fp)) != EOF) {
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else
      charClass = UNKNOWN;
  } else {
    charClass = EOF;
  }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace
 * character */
static void getNonBlank() {
  while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
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
    checkKeyword();
    break;

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
    secondLookup();
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
  if (nextToken != -1)
    printf("%s\t%s\n", lexeme, getTokenCode());
  return nextToken;
} /* End of function lex */

/* checks for two-character operators, and corrects the token */
static void secondLookup() {
  if (nextToken == LESSER_OP) { // checking for <=
    if (nextChar == '=') {
      addChar();
      nextToken = LEQUAL_OP;
      getChar();
    } else if (nextChar == '>') { // checking for <>
      addChar();
      nextToken = NEQUAL_OP;
      getChar();
    }
  } 
  else if (nextToken == GREATER_OP) { // checking for >=
    if(nextChar == '='){
      addChar();
      nextToken = GEQUAL_OP;
      getChar();
    }
  }
  else if (nextToken == ASSIGN_OP) {
    if (nextChar == '=') {
      addChar();
      nextToken = EQUAL_OP;
      getChar();
    }
  }
}

// compares identifiers to keywords and sets the token to that keyword if they
// are the same
static void checkKeyword() {
  if (strcmp(lexeme, "read") == 0) { // key read
    nextToken = KEY_READ;
  } else if (strcmp(lexeme, "print") == 0) { // key print
    nextToken = KEY_PRINT;
  } else if (strcmp(lexeme, "while") == 0) {
    nextToken = KEY_WHILE;
  } else if (strcmp(lexeme, "do") == 0) {
    nextToken = KEY_DO;
  }
  // this is probably not efficient, but there are only 4 keywords so suck it
}

static char *getTokenCode() {
  switch (nextToken) {
  case 10:
    return "INT_LIT";
    break;
  case 11:
    return "IDENT";
    break;
  case 20:
    return "ASSIGN_OP";
    break;
  case 21:
    return "ADD_OP";
    break;
  case 22:
    return "SUB_OP";
    break;
  case 23:
    return "MULT_OP";
    break;
  case 24:
    return "DIV_OP";
    break;
  case 26:
    return "LESSER_OP";
    break;
  case 27:
    return "GREATER_OP";
    break;
  case 28:
    return "EQUAL_OP";
    break;
  case 29:
    return "NEQUAL_OP";
    break;
  case 30:
    return "LEQUAL_OP";
    break;
  case 31:
    return "GEQUAL_OP";
    break;
  case 32:
    return "MOD_OP";
    break;
  case 35:
    return "SEMICOLON";
    break;
  case 40:
    return "LEFT_PAREN";
    break;
  case 41:
    return "RIGHT_PAREN";
    break;
  case 42:
    return "LEFT_CBRACE";
    break;
  case 43:
    return "RIGHT_CBRACE";
    break;
  case 50:
    return "KEY_READ";
    break;
  case 51:
    return "KEY_PRINT";
    break;
  case 53:
    return "KEY_WHILE";
    break;
  case 54:
    return "KEY_DO";
    break;
  case -1:
    return "EOF";
    break;
  default:
    return "UNKNOWN";
    break;
  }
}

/* stmt
Parses strings such that
ident = expr | key_read(var) | key_print(var) | key_do{stmt} key_while comp
| stmt; stmt
*/
void stmt(){
  printf("Enter <stmt>\n");
  if(nextToken == IDENT){ //V = E
    lex();
    if(nextToken == ASSIGN_OP){
      lex();
      expr();
    }
    else
      error();
  }
  else if(nextToken == KEY_READ || nextToken == KEY_PRINT){ //read(n) or print(n)
    lex();
    if(nextToken == LEFT_PAREN){
      lex();
      var();
      if(nextToken == RIGHT_PAREN){
        lex();
      }
      else
        error();
    }
    else
      error();
  }
  else if(nextToken == KEY_DO){  //<do> { <stmt> } while <comp>
    lex();
    if(nextToken == LEFT_CBRACE){
      while(nextToken != RIGHT_CBRACE){
        lex();
        stmt();
      }
      if(nextToken == RIGHT_CBRACE){
        lex();
        if(nextToken == KEY_WHILE){
          lex();
          comp();
        }
        else
          error();
      }
      else
        error();
    }
    else
      error();
  }
  else
    error();
  if(nextToken != SEMICOLON && nextToken != EOF && nextToken != RIGHT_CBRACE){
    error();
  }
  printf("Exit <stmt>\n");
}
/*
comp
parses comparison operators
used only
*/
void comp(){
  printf("Enter <comp>\n");
  expr();
  if(nextToken >= 26 && nextToken <= 31){  //token is a comparison operator
    lex();
    expr();
  } 
  else
    error();
 /* if(nextToken != SEMICOLON && nextToken != EOF && nextToken != RIGHT_CBRACE){
    error();
  }
*/
  printf("Exit <comp>\n");
}
/* expr
 * Parses strings in the language generated by the rule:
 * <expr> -> <term> {(+ | -) <term>}
 */
void expr() 
{
  if(nextToken != EOF && nextToken != UNKNOWN && nextToken != SEMICOLON){
    printf("Enter <expr>\n");

    /* Parse the first term */
    term();

    /* As long as the next token is + or -, get
    the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }

    printf("Exit <expr>\n");
    }
} /* End of function expr */

/* term
 * Parses strings in the language generated by the rule:
 * <term> -> <factor> {(* | /) <factor>)
 */
void term() 
{
    printf("Enter <term>\n");
    /* Parse the first factor */
    factor();
    /* As long as the next token is * or /, get the
    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
            lex();
            factor();
    }
    printf("Exit <term>\n");
} /* End of function term */

/* factor
 * Parses strings in the language generated by the rule:
 * <factor> -> id | int_constant | ( <expr )
 * */
void factor() 
{
    printf("Enter <factor>\n");
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex(); /* Get the next token */
    } else {
        /* If the RHS is (<expr>), call lex to pass over the 
        left parenthesis, call expr, and check for the right 
        parenthesis */
        if (nextToken == LEFT_PAREN) {
            lex(); 
            expr();

            if (nextToken == RIGHT_PAREN) {
                lex(); 
            } else { 
                error();
            }
        } /* End of if (nextToken == ... */
        /* It was not an id, an integer literal, or a left parenthesis */
        else 
        { 
            error(); 
        }
    } /* End of else */
    printf("Exit <factor>\n");
} /* End of function factor */

/*
var()
expects next Token to be IDENT
if not, it will throw an error
*/
void var(){
  if(nextToken == IDENT){
    lex();
  }
  else
    error();
}
/*
number()
expects nextToken to be INT_LIT
if not, throws an error
*/
void number(){
  if(nextToken == INT_LIT){
    lex();
  }
  else
    error();
}
//error message
static void error() 
{
  exitCode = 1;
    printf("\n\nError encounter: the next lexeme was %s and the next token was %s.\n", lexeme, getTokenCode());
}
