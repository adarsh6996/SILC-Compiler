struct Gsymbol *ghead;
struct Lsymbol *lhead;
struct Params  *ahead;
int i=0, sp=4096, j=0, k=0;//i- register, j- labels, k- functions

FILE *fptr;

struct Tnode *TreeCreate(int NODETYPE,int VALUE,char *name, struct Tnode *Ptr1, struct Tnode *Ptr2, struct Tnode *Ptr3,int TYPE,struct Tnode *args)
{
	struct Tnode *tmp;
	tmp = (struct Tnode*)malloc(sizeof(struct Tnode));
	if(name!=NULL)
	{
		tmp->NAME=(char*)malloc(10*sizeof(char));
		strcpy(tmp->NAME,name);
	}
	tmp->NODETYPE = NODETYPE;
	tmp->VALUE = VALUE;
	tmp->Ptr1 = Ptr1;    
	tmp->Ptr2 = Ptr2;
	tmp->Ptr3 = Ptr3;
	tmp->arglist=args;
	if(name != NULL)
	{
		tmp->GEntry = Lookup(name);
		tmp->LEntry = LLookup(name);
		tmp->AEntry = arglookup(name);
	}
	else
	{
		tmp->LEntry = NULL;
		tmp->GEntry = NULL;
		tmp->AEntry = NULL;
	}
	if(tmp->GEntry!=NULL)
	{
		tmp->TYPE=tmp->GEntry->type;
	}
	else if(tmp->LEntry!=NULL)
	{
		tmp->TYPE=tmp->LEntry->type;
	}
	else if(tmp->AEntry!=NULL)
	{
		tmp->TYPE=tmp->AEntry->type;
	}
	else
	{
		tmp->TYPE=TYPE;
	}
	switch(tmp->NODETYPE)
	{
		case 1:		
			if((Ptr1->TYPE)!=(Ptr2->TYPE))
			{
				printf("Invalid type mismatch. CompE\n");
				exit(1);
			}
			break;
	
		case 4:		
			if((Ptr1->TYPE)!=1)
			{
				printf("Invalid read type . CompE\n");
				exit(1);
			}
			break;

		case 5:
			if(tmp->Ptr1->TYPE!=1)
			{
				printf("\nCan only print int. CompE\n");
				exit(1);
			}
			break;

		case 6: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;

		case 7: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;

		case 8: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;
	
		case 9: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;

		case 10: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;
	
		case 11: if(tmp->Ptr1->TYPE!=2)
			{
				printf("\nInvalid type for if statement. CompE\n");
				exit(1);
			}
			break;

		case 12: if(tmp->Ptr1->TYPE!=2)
			{
				printf("\nInvalid type for while statement. CompE\n");
				exit(1);
			}
			break;

		case 13: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;
		case 19: if(tmp->Ptr1->TYPE!=1&&tmp->Ptr2->TYPE!=1)
			{
				printf("\nInvalid type mismatch. CompE\n");
				exit(1);
			}
			break;
	}
	return tmp;
}

void savestate()
{
	int a;
	for(a=0;a<i;a++)
	{
		fprintf(fptr,"PUSH R%d\n",a);
	}
}

void loadstate()
{
	int a;
	for(a =i-2;a >= 0; a--)
	{
		fprintf(fptr, "POP R%d\n", a);
	}
}


void memalloc()
{
	fprintf(fptr,"MOV SP, 4095\n");
	fprintf(fptr,"MOV BP, 4096\n");
	fprintf(fptr,"ADD SP, %d\n",sp-4096);
}

localmemalloc()
{
	int c;
	struct Lsymbol *t;
	c=0;
	t=lhead;
	while(t!=NULL)
	{
		c+=(t->size);
		t->Binding=c;
		t=t->next;
	}
	fprintf(fptr,"ADD SP, %d\n",c);
}

localmemdealloc()
{
	struct Lsymbol *tmp;
	int memdealloc = 0;
	tmp = lhead;
	while(tmp != NULL)
	{
		memdealloc++;
		tmp = tmp->next;
	}
	if(memdealloc != 0)
		fprintf(fptr, "SUB SP, %d\n", memdealloc);
}


struct Gsymbol *GInstall(struct Tnode *Ptr1, int size, struct Params *params)
{
	struct Gsymbol *tmp,*t;
	t = Lookup(Ptr1->NAME);
	if(t!=NULL){printf("Error Redeclaration of Variable\n"); exit(0);}
	tmp=(struct Gsymbol *)malloc(sizeof(struct Gsymbol));
	tmp->name=Ptr1->NAME;
	tmp->params=params;
	tmp->Binding=sp;
	tmp->size=size;
	sp=sp+(tmp->size);
	if(tmp->size==0)
	{
		tmp->flabel = k;
		k++;
	}
	tmp->next=ghead;
	ghead=tmp;
	return tmp;
}


struct Gsymbol *Lookup(char *n)
{
	struct Gsymbol *tmp;
	tmp=ghead;
	while(tmp!=NULL)
	{
		if(strcmp(tmp->name,n)==0)
		{	
			return tmp;
		}
		tmp=tmp->next;
	}	
	return NULL;
}

void LInstall(struct Tnode *Ptr1, int type)
{
	struct Lsymbol *tmp,*t;
	t = LLookup(Ptr1->NAME);
	if(t!=NULL){printf("Error Redeclaration of Variable. CompE\n"); exit(0);}
	tmp=(struct Lsymbol *)malloc(sizeof(struct Lsymbol));
	tmp->name=Ptr1->NAME;
	tmp->size=Ptr1->VALUE;
	tmp->Binding=-1;
	tmp->type=type;
	tmp->next=lhead;
	lhead=tmp;
}

struct Lsymbol *LLookup(char *n)
{
	struct Lsymbol *tmp;
	tmp=lhead;
	while(tmp!=NULL)
	{
		if(strcmp(tmp->name,n)==0)
		{	
			return tmp;
		}
		tmp=tmp->next;
	}	
	return NULL;
}

getreg()
{
	if(i>20)
	{
		printf("Reg Overload\n");
		exit(0);
	}
	i=i+1;
	return i-1;
}

freereg()
{
	if(i>0)
		i=i-1;
	else
	{
		printf("Reg Underload\n");
		exit(0);
	}	
}

getlabel()
{
	j=j+1;
	return j-1;
}

int getIDLoc(struct Tnode *t)
{
	int x,p,q,r;	
	p=getreg();
	r=getreg();
	if(t->LEntry!=NULL)
	{
		fprintf(fptr, "MOV R%d, BP\n", p);
		fprintf(fptr, "MOV R%d, %d\n", r, t->LEntry->Binding);
		fprintf(fptr,"ADD R%d, R%d\n",p,r);
	}
	else if(t->GEntry!=NULL)
	{
		x=t->GEntry->Binding;
		fprintf(fptr,"MOV R%d, %d\n",p,x);
	}
	else if(t->AEntry!=NULL)
	{
		fprintf(fptr, "MOV R%d, BP\n", p);
		fprintf(fptr, "MOV R%d, 2\n", r);
		fprintf(fptr, "SUB R%d, R%d\n", p, r);
		fprintf(fptr, "SUB R%d, %d\n", p, t->AEntry->binding);	
	}
	freereg();
	if(t->Ptr1!=NULL)
	{
		q = codegen(t->Ptr1);
		fprintf(fptr,"ADD R%d, R%d\n",p,q);
		freereg();
	}
	return p;
}	

struct Tnode *funcmake(char *name,int type,struct Params *plist,struct Lsymbol *llist,struct Tnode *body)
{
	struct Tnode *tmp;
	struct GSymbol *t;
	tmp = (struct Tnode*)malloc(sizeof(struct Tnode));
	tmp->NAME=name;
	tmp->TYPE=type;
	tmp->AEntry=plist;
	tmp->GEntry=Lookup(name);
	tmp->LEntry=llist;
	tmp->Ptr1=NULL;
	tmp->Ptr2=body;
	return tmp;
}	

struct Tnode *funcadd(struct Tnode *t1, struct Tnode *t2)
{
	struct Tnode *tmp;
	tmp=t1;
	while(tmp->Ptr1!=NULL)
	{
		tmp=tmp->Ptr1;
	}
	tmp->Ptr1=t2;
	return t1;
}

struct Params *parammake(int type,char *name)
{
	struct Params *tmp;
	tmp = (struct Params*)malloc(sizeof(struct Params));
	tmp->type=type;
	tmp->name=name;
	tmp->binding=0;
	tmp->next=NULL;
	ahead=tmp;
	return tmp;
}
	
struct Params *paramadd(struct Params *p1,struct Params *p2)
{
	struct Params *t;
	t=p1;
	while(t->next!=NULL)
	{
		t=t->next;
	}
	t->next=p2;
	ahead=p1;
	return p1;
}

struct Tnode *arglistadd(struct Tnode *t1, struct Tnode *t2)
{
	struct Tnode *tmp;
	tmp=t1;
	while(tmp->Ptr3!=NULL)
	{
		tmp=tmp->Ptr3;
	}
	tmp->Ptr3=t2;
	return t1;
}

struct Params *arglookup(char *name)
{
	struct Params *tmp;
	tmp = ahead;
	while(tmp != NULL)
	{
		if(strcmp(name,tmp->name) == 0)
		{
			break;
		}
		tmp = tmp->next;
	}
	return tmp;
}   

void argpush(struct Tnode *args)
{
	int p;
	if(args != NULL){
		argpush(args->Ptr3);
		p = codegen(args);
		fprintf(fptr, "PUSH R%d\n", p);
		freereg();
	}
}

void argpop(struct Tnode *args)
{
	int p;
	p = getreg();
	while(args != NULL)
	{
		fprintf(fptr, "POP R%d\n", p);
		args = args->Ptr3;
	}
	freereg();
}

void argbinding(){
	struct Params* tmp;
	int bind = 1;
	tmp = ahead;
	while(tmp != NULL){
		tmp->binding = bind;
		bind++;
		tmp = tmp->next;
	}
}


int codegen(struct Tnode *t)
{
	int p,q,r,x,y,z;
	if(t!=NULL)
	{
		switch(t->NODETYPE)
		{
			case 0: codegen(t->Ptr1);
				codegen(t->Ptr2);
				break;

			case 1: p = getIDLoc(t->Ptr1);
				q = codegen(t->Ptr2);
				fprintf(fptr,"MOV [R%d], R%d\n",p,q);
				freereg();
				freereg();					
				return -1;

			case 2: p = getIDLoc(t);
				fprintf(fptr,"MOV R%d, [R%d]\n",p,p);
				return p;

			case 3: p = getreg();
				fprintf(fptr,"MOV R%d, %d\n",p,t->VALUE);
				return p;
			
			case 4: p = getreg();
				q = getIDLoc(t->Ptr1);
				fprintf(fptr,"MOV R%d, \"Read\"\n",p);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"MOV R%d, %d\n",p,-1);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"PUSH R%d\n",q);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"CALL 0\n");
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				freereg();
				freereg();
				return -1;

			case 5:	p = getreg();
				q = codegen(t->Ptr1);
				fprintf(fptr,"PUSH R%d\n",q);
				fprintf(fptr,"MOV R%d, SP\n",q);
				fprintf(fptr,"MOV R%d, %d\n",p,5);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"MOV R%d, %d\n",p,-2);
				fprintf(fptr,"PUSH R%d\n",p);	
				fprintf(fptr,"PUSH R%d\n",q);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"PUSH R%d\n",p);
				fprintf(fptr,"CALL 0\n");
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				fprintf(fptr,"POP R%d\n",q);
				freereg();
				freereg();
				return -1;					

			case 6: p = codegen(t->Ptr1);
				q = codegen(t->Ptr2);
				fprintf(fptr,"ADD R%d, R%d\n",p,q);
				freereg();
				return p;

			case 7: p = codegen(t->Ptr1);
				q = codegen(t->Ptr2);
				fprintf(fptr,"MUL R%d, R%d\n",p,q);
				freereg();
				return p;

			case 8: p = codegen(t->Ptr1);
				q = codegen(t->Ptr2);
				fprintf(fptr,"LT R%d, R%d\n",p,q);
				freereg();
				return p;

			case 9: p = codegen(t->Ptr1);
				q = codegen(t->Ptr2);
				fprintf(fptr,"GT R%d, R%d\n",p,q);
				freereg();
				return p;
			
			case 10: p = codegen(t->Ptr1);
				 q = codegen(t->Ptr2);
				 fprintf(fptr,"EQ R%d, R%d\n",p,q);
				 freereg();
				 return p;

			case 11: p = codegen(t->Ptr1);
                                x = getlabel();
				y = getlabel();
				fprintf(fptr,"JZ R%d, L%d\n",p,x);
				freereg();
				codegen(t->Ptr2);
				fprintf(fptr,"JMP L%d\n",y);	
				fprintf(fptr,"L%d:\n",x);		
				if(t->Ptr3!=NULL)
				{
					codegen(t->Ptr3);
				}
				fprintf(fptr,"L%d:\n",y);
				return -1;

			case 12: x = getlabel(); 
				y = getlabel();
				fprintf(fptr,"L%d:\n",x);
				p = codegen(t->Ptr1);
				fprintf(fptr,"JZ R%d, L%d\n",p,y);
				freereg();
				codegen(t->Ptr2);
				fprintf(fptr,"JMP L%d\n",x);
				fprintf(fptr,"L%d:\n",y);
				return -1;					

			case 13: p = codegen(t->Ptr1);
				q = codegen(t->Ptr2);
				fprintf(fptr,"SUB R%d, R%d\n",p,q);
				freereg();
				return p;

			case 15: savestate();
				z=i;
				i=0;
				argpush(t->arglist);
				fprintf(fptr, "PUSH R0\n");
				fprintf(fptr, "CALL F%d\n", t->GEntry->flabel); 
				i=z;
				p=getreg();
				fprintf(fptr, "POP R%d\n", p);
				argpop(t->arglist);
				loadstate();
				return p;

			case 16: fprintf(fptr,"PUSH BP\n");
				 fprintf(fptr,"MOV BP, SP\n");
				 localmemalloc();
				 codegen(t->Ptr1);
				 codegen(t->Ptr2);
				 return -1;
			
			case 17: p=codegen(t->Ptr1);
				 q=getreg();
				 r=getreg();
				 localmemdealloc();
				 fprintf(fptr, "MOV R%d, BP\n", q);
				 fprintf(fptr, "MOV R%d, 2\n", r);
				 fprintf(fptr, "SUB R%d, R%d\n", q, r);	
				 fprintf(fptr, "MOV [R%d], R%d\n",q,p); 
				 fprintf(fptr, "MOV BP, [SP]\n");
				 fprintf(fptr, "POP R%d\n",r);
				 fprintf(fptr, "RET\n");
				 return -1;	

			case 19: p = codegen(t->Ptr1);
				 q = codegen(t->Ptr2);
				 fprintf(fptr,"NE R%d, R%d\n",p,q);
				 freereg();
				 return p;

			case 20: p = codegen(t->Ptr1);
				 q = codegen(t->Ptr2);
				 fprintf(fptr,"DIV R%d, R%d\n",p,q);
				 freereg();
				 return p;

			case 21: p = codegen(t->Ptr1);
				 q = codegen(t->Ptr2);
				 fprintf(fptr, "ADD R%d, R%d\n", p, q);
				 fprintf(fptr, "MOV R%d, 1\n", q);
				 fprintf(fptr, "LE R%d, R%d\n", p, q);
				 freereg();
				 return p;

			case 22: p = codegen(t->Ptr1);
				 q = codegen(t->Ptr2);
				 fprintf(fptr, "ADD R%d, R%d\n", p, q);
				 fprintf(fptr, "MOV R%d, 0\n", q);
				 fprintf(fptr, "LE R%d, R%d\n", p, q);
				 freereg();
				 return p;
		
			case 23: p = codegen(t->Ptr1);
				 q = getreg();
				 fprintf(fptr, "MOV R%d, 1\n", q);
				 fprintf(fptr, "LT R%d, R%d\n", p, q);
				 freereg();
				 return p;
		}
	}
}

int fullcodegen(struct Tnode *fns, struct Tnode *mainf)
{
	fptr=fopen("opfile","w");
	fprintf(fptr, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\nBRKP\n",0,2056,0,0,0,0,0,0);
	memalloc();
	fprintf(fptr, "PUSH R0\nCALL MAIN\n");
	fprintf(fptr,"INT 10\n");
	while(fns!=NULL)
	{
		printf("SUP\n");
		ahead = fns->AEntry;
		argbinding();
		printf("SUPPER\n");
		lhead = fns->LEntry;
		printf("HEY\n");
		fprintf(fptr, "F%d:\n", fns->GEntry->flabel);
		printf("HEYYY\n");
		codegen(fns->Ptr2);
		printf("SUPPERER\n");
		fns = fns->Ptr1;
	}
	lhead=mainf->LEntry;
	fprintf(fptr,"MAIN:\n");
	printf("Almost done\n");
	codegen(mainf->Ptr1); 
	fclose(fptr);						
	printf("\n");
}







	





















