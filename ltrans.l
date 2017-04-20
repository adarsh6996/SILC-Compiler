%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	int i=2048,f=0;
	char s[3],str[5];
	FILE *fptr;
	int mainloc;

	struct LabelTable{
			   int a;
			   char *l;
			   struct LabelTable *next;
			 };
	
	struct FTable{
			   int a;
			   char *l;
			   struct FTable *next;
			 };
	
	struct LabelTable *head,*tmp;	
	struct FTable *head1,*tmp1;	

%}

%%

"\n" 	{
		if(f==0)
		{
			if(i<2056)
				{i++;}
			 else
				{i+=2;}
		}
		else
		{
			fprintf(fptr,"%c",yytext[0]);	
		}
	}

L[0-9]+ {
		if(f==1)
		{
			tmp=head;
			while(tmp!=NULL)
			{
				strcpy(str,yytext);
				strcat(str,":\n");
				if(strcmp(str,tmp->l)==0)
				{
					fprintf(fptr,"%d",tmp->a);
					break;
				}
				tmp=tmp->next;	
			}
		}
	}

F[0-9]+ {
		if(f==1)
		{
			tmp1=head1;
			while(tmp!=NULL)
			{
				strcpy(str,yytext);
				strcat(str,":\n");
				if(strcmp(str,tmp1->l)==0)
				{
					fprintf(fptr,"%d",tmp1->a);
					break;
				}
				tmp1=tmp1->next;	
			}
		}
	}

L[0-9]+[:][\n]	{
			if(f==0)
			{
				tmp = (struct LabelTable*)malloc(sizeof(struct LabelTable));		
				tmp->l = strdup(yytext);
				tmp->a = i;
				tmp->next = head;
				head = tmp;
			}
			if(f==1){}
					
		}

F[0-9]+[:][\n]	{
			if(f==0)
			{
				tmp1 = (struct FTable*)malloc(sizeof(struct FTable));		
				tmp1->l = strdup(yytext);
				tmp1->a = i;
				tmp1->next = head1;
				head1 = tmp1;
			}
			if(f==1){}
					
		}

"MAIN"[:][\n]   {
			if(f==0)
			{
				mainloc=i;
			}
		}

"MAIN"		{
			if(f==1)
			{
				fprintf(fptr,"%d",mainloc);
			}
		}


.       {	
		if(f==1)
		{
			fprintf(fptr,"%c",yytext[0]);
		}		
	}

%%

int yywrap()
{
	if(f==0)
	{
		yyin = fopen("opfile","r");  	
		f=1;
		fptr = fopen("opfile1.xsm","w");
		return 0;
	}
	else 
	{
		return 1;
	}
}

int main()
{
	head=NULL;
	yyin = fopen("opfile","r");
  	yylex();
  	return 1;
}
