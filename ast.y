%{
	#include <stdio.h>
	#include <stdlib.h> 
	#include "ast.h"
	#include "ast.c"
	#include <string.h>
	int yylex(void);
%}

%token END WRITE ID NUM EQUALS READ EE IF THEN ELSE ENDIF RETURN WHILE DO ENDWHILE INT DECL ENDDECL LB RB BOOL TRUE FALSE MAIN LBR RBR NE DIV AND OR NOT

%left OR
%left AND
%left EE NE
%left '>' '<'
%left '+' '-'
%left '*' DIV
%left NOT

%union{
	struct Tnode* tnode;
	struct Gsymbol* gnode;
	struct Params* pnode;
}

%type<tnode> MAINBLOCK typename Lid slist stmt expr ID NUM FDefList FDefBlock Body RETSTMT Args

%type<gnode> Gid

%type<pnode> Paramlist Param

%%

program : GDeclBlock FDefList MAINBLOCK END     {  
							fullcodegen($2,$3);
							exit(1);
						}
	
	 | GDeclBlock MAINBLOCK END		{
							fullcodegen(NULL,$2);
							exit(1);
						}
		;

GDeclBlock : DECL GDeclList ENDDECL	
           ;

GDeclList  : GDeclList GDecl
           | GDecl
           ;

GDecl      : typename Gidlist ';'     
           ; 

Gidlist    : Gidlist ',' Gid	{ $3->type = $<tnode>0->TYPE;}
           |   Gid              { $1->type = $<tnode>0->TYPE;}      
           ;           

typename   : INT                {$$= TreeCreate(0,0,NULL,NULL,NULL,NULL,1,NULL);}
           | BOOL		{$$= TreeCreate(0,0,NULL,NULL,NULL,NULL,2,NULL);}
	   ;

Gid        :   ID               {$$ = GInstall($1, 1, NULL);}    
           |   ID LB NUM RB     {if($3->VALUE <= 1){
           						printf("Array declaration invalid. ComE");
		 					exit(1);
           					   }
           		         $$ = GInstall($1, $3->VALUE, NULL);
				} 
	   |   ID '(' Paramlist ')'    { $$ = GInstall($1, 0, $3); ahead = NULL;}    
	   ;

MAINBLOCK : INT MAIN '(' ')' LBR LDeclBlock Body RBR {$$=TreeCreate(18,0,NULL,$7,NULL,NULL,0,NULL);$$->LEntry=lhead;printf("main\n");}
	  ;

LDeclBlock : DECL LDeclList ENDDECL
           ;

LDeclList  : LDeclList LDecl
           | LDecl           
	   ;

LDecl : typename Lidlist ';' 
      ;

Lidlist : Lidlist ',' Lid    { LInstall($3,$<tnode>0->TYPE);}
        |   Lid              { LInstall($1,$<tnode>0->TYPE);}
	;

Lid 	   :   ID               {$1->VALUE=1; $$=$1;}    
           |   ID LB NUM RB     {$1->VALUE=$3->VALUE;$$=$1;}     
	   ;
		
FDefList  : FDefBlock			{ $$ = $1;}
          | FDefList FDefBlock  	{ $$ = funcadd($1, $2);}  
;

FDefBlock : typename ID '(' Paramlist ')' LBR LDeclBlock Body RBR  { 	
									$$ = funcmake($2->NAME, $1->TYPE, $4, lhead, $8); 										lhead = NULL; 
									ahead = NULL;
								   }
	   
	   | typename ID '(' Paramlist ')' LBR Body RBR           {
									printf("HEY\n");
									$$ = funcmake($2->NAME, $1->TYPE, $4, lhead, $7);
									printf("Ho\n");
 									lhead = NULL; 
									ahead = NULL;
								   }               
	   ;

Paramlist :  Paramlist ',' Param			   {$$ = paramadd($1,$3);} 
	   | Param					   {$$=$1;}
	   ;

Param : typename ID	{$$ = parammake($1->TYPE,$2->NAME);}
      ;

Body : slist RETSTMT     {$$ = TreeCreate(16,0,NULL,$1,$2,NULL,0,NULL);}
     ;

slist   : slist stmt              {$$ = TreeCreate(0,0,NULL,$1,$2,NULL,0,NULL);}
	| stmt			  {$$ = $1;}
	;

stmt : ID EQUALS expr	';'	{$$ = TreeCreate(1,0,NULL,$1,$3,NULL,0,NULL);}
       | ID LB expr RB EQUALS expr ';'    {
						if($3->TYPE!=1){printf("Invalid index type. CompE\n");exit(0);}
						$1->Ptr1=$3; 
						$$ = TreeCreate(1,0,NULL,$1,$6,NULL,0,NULL);
					  }	
       |WRITE '(' expr ')' ';' {$$ = TreeCreate(5,0,NULL,$3,NULL,NULL,0,NULL);}
       |READ '(' ID ')' ';'   {$$ = TreeCreate(4,0,NULL,$3,NULL,NULL,0,NULL);}
       |READ '(' ID LB expr RB ')' ';'  {
						if($5->TYPE!=1){printf("Invalid index type. CompE\n");exit(0);}
						$3->Ptr1=$5; 
						$$ = TreeCreate(4,0,NULL,$3,NULL,NULL,0,NULL);
					}
       |IF '(' expr ')' THEN slist ENDIF ';'  {$$ = TreeCreate(11,0,NULL,$3,$6,NULL,0,NULL);}
       |IF '(' expr ')' THEN slist ELSE slist ENDIF ';'  {$$ = TreeCreate(11,0,NULL,$3,$6,$8,0,NULL);}
       |WHILE '(' expr ')' DO slist ENDWHILE ';'  {$$ = TreeCreate(12,0,NULL,$3,$6,NULL,0,NULL);}
	;

expr :  '(' expr ')'              {$$ = $2;}
	 |expr '+' expr           {$$ = TreeCreate(6,0,NULL,$1,$3,NULL,1,NULL);}
	 |expr '-' expr           {$$ = TreeCreate(13,0,NULL,$1,$3,NULL,1,NULL);}
	 |expr '*' expr           {$$ = TreeCreate(7,0,NULL,$1,$3,NULL,1,NULL);}
	 |expr '<' expr           {$$ = TreeCreate(8,0,NULL,$1,$3,NULL,2,NULL);}
	 |expr '>' expr           {$$ = TreeCreate(9,0,NULL,$1,$3,NULL,2,NULL);}
	 |expr DIV expr           {$$ = TreeCreate(20,0,NULL,$1,$3,NULL,1,NULL);}
	 |expr EE  expr           {$$ = TreeCreate(10,0,NULL,$1,$3,NULL,2,NULL);}
	 |expr NE  expr           {$$ = TreeCreate(19,0,NULL,$1,$3,NULL,2,NULL);}
	 |expr AND expr           {$$ = TreeCreate(21,0,NULL,$1,$3,NULL,2,NULL);}
	 |expr OR expr            {$$ = TreeCreate(22,0,NULL,$1,$3,NULL,2,NULL);}
	 |NOT expr                {$$ = TreeCreate(23,0,NULL,$2,NULL,NULL,2,NULL);}
	 | ID 			  {$$ = $1;}
	 | ID LB expr RB          {if($3->TYPE!=1){printf("Invalid index type. CompE\n");exit(0);}$1->Ptr1=$3; $$ = $1;}
         | NUM                    {$$ = $1;}
	 | FALSE                  {$$ = TreeCreate(14,1,NULL,NULL,NULL,NULL,2,NULL);}
	 | TRUE                   {$$ = TreeCreate(14,0,NULL,NULL,NULL,NULL,2,NULL);}
	 | ID '(' Args ')'	  {$$ = TreeCreate(15,0,$1->NAME,NULL,NULL,NULL,0,$3);}
	;

Args :  Args ',' expr	{$$ = arglistadd($1,$3);}
	| expr		{$$=$1;}
	;	 

RETSTMT : RETURN expr ';'   {$$ = TreeCreate(17,0,NULL,$2,NULL,NULL,0,NULL);}
	;

%%

yyerror(char const *s)
{
    printf("yyerror %s \n",s);
}


int main(int argc, char *argv[])
{
	ghead=NULL;
	lhead=NULL;
	ahead=NULL;
	if(argc<2)
	{
		yyin=stdin;
	}
	else
	{
		yyin=fopen(argv[1],"r");
	}	
	yyparse();
	return 0;

}
