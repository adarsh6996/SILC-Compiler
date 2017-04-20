%{
	#include <stdio.h>
	#include "y.tab.h"
	#include <stdlib.h>
	#include<string.h>

%}

%%

[0-9]+  {yylval.tnode = TreeCreate(3,atoi(yytext),NULL,NULL,NULL,NULL,1,NULL); return NUM;}
"=="    {return EE;}
"!="    {return NE;}
"="     {return EQUALS;}
"/"     {return DIV;}
"main"  {return MAIN;}
"int"   {return INT;}
"bool"  {return BOOL;}
"write" {return WRITE;}
"read"  {return READ;}
"if"    {return IF;}
"decl"  {return DECL;}
"enddecl" {return ENDDECL;}
"then"  {return THEN;}
"else"  {return ELSE;}
"endif" {return ENDIF;}
"while" {return WHILE;}
"do"    {return DO;}
"endwhile" {return ENDWHILE;}
"return" {return RETURN;}
"["     {return LB;}
"]"     {return RB;}
"end"   {return END;}
"true"  {return TRUE;}
"false" {return FALSE;} 
"and"   {return AND;}
"or"    {return OR;}
"not"   {return NOT;}
"{"     {return LBR;}
"}"     {return RBR;}
[();+*<>,-]    {return *yytext;}
[a-zA-Z][a-zA-Z0-9]*   { yylval.tnode = TreeCreate(2,0,yytext,NULL,NULL,NULL,1,NULL); return ID;}
[ \t\n]	{}
.	{yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {

	return 1;
}
