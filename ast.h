#include <string.h>

/*
	0 - START
	1 - EQUALS
	2 - ID
	3 - NUM
	4 - READ
	5 - WRITE
	6 - PLUS
        7 - MULT
        8 - LT
        9 - GT
	10 - EE
	11 - IF
	12 - WHILE
	13 - MINUS
	14 - BoolVal
	15 - FUNCTION CALL
	16 - BODY
	17 - Retstmt
	18 - MAIN
	19 - NE
        20 - DIV
	21 - AND
	22 - OR
	23 - NOT
	

	0 - VOID
	1 - INT
	2 - BOOL

*/

extern FILE *yyin;

struct Tnode {

int NODETYPE;

int TYPE;

char* NAME;

int VALUE;

struct Gsymbol *GEntry;

struct Lsymbol *LEntry;

struct Params *AEntry;

struct Tnode *Ptr1, *Ptr2, *Ptr3, *arglist;

};

struct Gsymbol {
	
	char *name;
	int size;
	int Binding;
	int type;
	int flabel;
	struct Params *params;
	struct Gsymbol *next;
};

struct Lsymbol {

	char *name;
	int size;
	int Binding;
	int type;
	struct Lsymbol *next;
};

struct Params{
	char *name;	        
	int type;  
	int binding;   
	struct Params *next;  
};

struct Tnode *TreeCreate(int NODETYPE,int VALUE,char *name, struct Tnode *Ptr1, struct Tnode *Ptr2, struct Tnode *Ptr3,int TYPE,struct Tnode *arglist);

void printtree(struct Tnode *t);

int evaltree(struct Tnode *t);

struct Gsymbol *Lookup(char *n);

struct Lsymbol *LLookup(char *n);

struct Params *arglookup(char *name);
