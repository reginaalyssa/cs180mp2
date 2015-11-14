#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define LENGTH 100

typedef struct attnode attnode;
typedef struct att att;

struct attnode
{
	char attname[LENGTH];
	int x;
	int y;
	int equivalent;
	int used;
	struct attnode * next;
};

struct att
{
	attnode * head;
	attnode * tail;
};


char * removeNewline(char * str);
void init(att * foo);
void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent);
void showList(att * foo);
int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);
int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test);

int main()
{
	int i, j, k, l; // Iterators
	int valuesize;
	char strInput[LENGTH]; // Buffer for scanning string inputs
	int values[LENGTH][LENGTH]; // 2D Array to contain integer value equivalents
	int nattributes; // Number of attributes
	int nchoices; // Number of choices per attribute
	int nvalues; // Number of values in test data
	FILE * file;
	FILE * file2;
	att test;
	init(&test);

	/*
		Format of input.txt:
			Outlook				// 1st Attribute
			3					// Number of values for 1st attribute
			Sunny				// 1st value of 1st attribute
			Overcast			// 2nd value of 1st attribute
			Rain				// 3rd value of 1st attribute
			Temperature			// 2nd Attribute
			3 					// Number of values for 2nd attribute
			Hot					// 1st value of 2nd attribute
			Mild				// 2nd value of 2nd attribute
			Cool 				// 3rd value of 2nd attribute
	*/

	file=fopen("input.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
	}

	fscanf(file, "%d\n", &nattributes);
	valuesize=1;
	j=-1;
	for(k=0; k<nattributes; k++)
	{
		fgets(strInput, LENGTH, file);
		removeNewline(strInput);
		addNode(&test, strInput, 0, k, j);
		j--;

		fscanf(file, "%d\n", &nchoices);
		for(i=0; i<nchoices; i++)
		{
			fgets(strInput, LENGTH, file);
			removeNewline(strInput);
			addNode(&test, strInput, i+1, k, valuesize);
			valuesize++;
		}
	}

	showList(&test);

	/*
		Format of inputvalues.txt:
		5								// Number of values
		Sunny Hot High Light No 		// 1st input value
		Sunny Hot High Strong No 		// 2nd input value
		Overcast Hot High Light Yes 	// 3rd input value
		Rain Mild High Light Yes 		// 4th input value
		Rain Cool Normal Light Yes 		// 5th input value
	*/

	file2=fopen("inputvalues.txt", "r");
	if(file==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
	}

	fscanf(file2, "%d\n", &nvalues);
	struct attnode * curr;
	for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			fscanf(file2, "%s ", strInput);
			curr=test.head;
			while(curr!=NULL)
			{
				if(strcmp(strInput, curr->attname)==0)
				{
					break;
				}
				curr=curr->next;
			}
			values[i][j]=curr->equivalent;

		}
	}

	printf("\n");	

	for(i=0; i<nvalues; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			/*curr=test.head;
			while(curr!=NULL)
			{
				if(values[i][j]==curr->equivalent)
				{
					break;
				}
				curr=curr->next;
			}
			printf("%s ", curr->attname);*/
			printf("%d ", values[i][j]);
		}
		printf("\n");
	}

	positiveValues(1, 5, values, nvalues, nattributes, &test);
	negativeValues(1, 5, values, nvalues, nattributes, &test);
}

char * removeNewline(char * str)
{
	char * cptr=str;

	while(* cptr!='\0')
	{
		if(* cptr=='\n')
		{
			* cptr='\0';
		}
		cptr++;
	}
	return str;
}

void init(att * foo)
{
	foo->head=NULL;
	foo->tail=NULL;
}


void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent)
{
	struct attnode * node;
	struct attnode * curr;

	node=(attnode*)malloc(sizeof(attnode));
	strcpy(node->attname, attname);
	node->x=x;
	node->y=y;
	node->equivalent=equivalent;
	node->next=NULL;

	if(foo->head==NULL)
	{
		foo->head=node;
		foo->tail=node;
		foo->head->next=NULL;
	}
	else
	{
		curr=foo->head;
		while(curr->next!=NULL)
		{
			curr=curr->next;
		}
		curr->next=node;
		foo->tail=curr->next;
		foo->tail->next=NULL;
	}
}

void showList(att * foo)
{
	struct attnode * curr;
	curr=foo->head;
	if(foo->head!=NULL)
	{
		while(curr!=NULL)
		{
			printf("%s %d %d %d\n", curr->attname, curr->x, curr->y, curr->equivalent);
			curr=curr->next;
		}
	}
}

int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	struct attnode * curr;
	struct attnode * yes;
	int i, j, k;
	int positivectr=0;
	// With all S palang 'to
	if(S==0)
	{
		yes=test->head;
		while(yes!=NULL)
		{
			if(strcmp(yes->attname, "Yes")==0)
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<nvalues; i++)
		{
			if(attribute==0)
			{
				if(values[i][nattributes-1]==yes->equivalent)
				{
					positivectr++;
				}
			}
			else
			{
				for(j=0; j<nattributes; j++)
				{
					if(values[i][j]==attribute && values[i][nattributes-1]==yes->equivalent)
					{
						positivectr++;
					}
				}
			}
		}
	}
	else
	{
		curr=test->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		yes=test->head;
		while(yes!=NULL)
		{
			if(strcmp(yes->attname, "Yes")==0)
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<nvalues; i++)
		{
			for(j=0; j<nattributes; j++)
			{
				for(k=0; k<nattributes; k++)
				{
					if(values[i][k]==S && values[i][j]==attribute && values[i][nattributes-1]==yes->equivalent)
					{
						positivectr++;
					}
				}
			}
		}
	}
	printf("Positive: %d\n", positivectr);
	return positivectr;

}

int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test)
{
	struct attnode * curr;
	struct attnode * no;
	int i, j, k;
	int negativectr=0;
	if(S==0)
	{
		/*curr=test->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		} Use this if may value na attribute and S na hindi 0*/

		no=test->head;
		while(no!=NULL)
		{
			if(strcmp(no->attname, "No")==0)
			{
				break;
			}
			no=no->next;
		}

		for(i=0; i<nvalues; i++)
		{
			if(attribute==0)
			{
				if(values[i][nattributes-1]==no->equivalent)
				{
					negativectr++;
				}
			}
			else
			{
				for(j=0; j<nattributes; j++)
				{
					if(values[i][j]==attribute && values[i][nattributes-1]==no->equivalent)
					{
						negativectr++;
					}
				}
			}
		}
	}
	else
	{
		curr=test->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		no=test->head;
		while(no!=NULL)
		{
			if(strcmp(no->attname, "No")==0)
			{
				break;
			}
			no=no->next;
		}

		for(i=0; i<nvalues; i++)
		{
			for(j=0; j<nattributes; j++)
			{
				for(k=0; k<nattributes; k++)
				{
					if(values[i][k]==S && values[i][j]==attribute && values[i][nattributes-1]==no->equivalent)
					{
						negativectr++;
					}
					//printf("[%d][%d]%d==%d && [%d][%d]%d==%d && [%d][%d]%d==%d\n", i, k, values[i][k], S, i, j, values[i][j], attribute, i, nattributes-1, values[i][nattributes-1], no->equivalent);
				}
			}
		}
	}
	printf("Negative: %d\n", negativectr);
	return negativectr;
}