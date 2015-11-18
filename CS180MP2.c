#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define LENGTH 100

typedef struct attnode attnode;
typedef struct att att;
typedef struct tree tree;

struct attnode
{
	char attname[LENGTH];
	int x;
	int y;
	int equivalent;
	int used;
	int positivetracker;
	int negativetracker;
	struct attnode * next;
	struct attnode * testnext;
	struct attnode * globnext;
	struct attnode * LSON;
	struct attnode * RSON;
	struct attnode * parent;
};

struct att
{
	attnode * head;
	attnode * tail;
};

struct tree
{
	attnode * top;
};

char * removeNewline(char * str);
void init(att * foo);
void initTree(tree * foo);
void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent);
void showList(att * foo);
int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train);
int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train);
float getEntropy(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train);
float getGain(int parent, int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train);
int getMaxGain(int parent, int S, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train);
int isUsed(int attribute, att * train);
void addSubtree(int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train, tree * decisionTree);
void updatePosNeg(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train);

int classify(att * test, att * train, tree * decisionTree);

att glob;

int main()
{
	int i, j, k, l; // Iterators
	int valuesize;
	char strInput[LENGTH]; // Buffer for scanning string inputs
	int values[LENGTH][LENGTH]; // 2D Array to contain integer value equivalents of training data
	int testing[LENGTH]; // Array to contain integer value equivalents of test data
	int nattributes; // Number of attributes
	int nchoices; // Number of choices per attribute
	int ntraining; // Number of values in training data
	int ntesting;
	FILE * file;
	FILE * file2;
	FILE * file3;
	att train;
	att test;
	tree decisionTree;
	init(&train);
	init(&test);
	initTree(&decisionTree);
	init(&glob);
	attnode * node;
	attnode * curr;
	attnode * findtest;
	attnode * testcurr;

	/*
		Format of initialize.txt:
			2 					// Number of attributes
			Outlook             // 1st Attribute
			3                   // Number of values for 1st attribute
			Sunny               // 1st value of 1st attribute
			Overcast            // 2nd value of 1st attribute
			Rain                // 3rd value of 1st attribute
			Temperature         // 2nd Attribute
			3                   // Number of values for 2nd attribute
			Hot                 // 1st value of 2nd attribute
			Mild                // 2nd value of 2nd attribute
			Cool                // 3rd value of 2nd attribute
	*/

	file=fopen("initialize.txt", "r");
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
		addNode(&train, strInput, 0, k, j);
		j--;

		fscanf(file, "%d\n", &nchoices);
		for(i=0; i<nchoices; i++)
		{
			fgets(strInput, LENGTH, file);
			removeNewline(strInput);
			addNode(&train, strInput, i+1, k, valuesize);
			valuesize++;
		}
	}

	showList(&train);

	/*
		Format of training.txt:
		5                               // Number of values
		Sunny Hot High Light No         // 1st input value
		Sunny Hot High Strong No        // 2nd input value
		Overcast Hot High Light Yes     // 3rd input value
		Rain Mild High Light Yes        // 4th input value
		Rain Cool Normal Light Yes      // 5th input value
	*/

	file2=fopen("trainingE02.txt", "r");
	if(file2==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
	}

	fscanf(file2, "%d\n", &ntraining);
	for(i=0; i<ntraining; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			fscanf(file2, "%s ", strInput);
			curr=train.head;
			while(curr!=NULL)
			{
				if(strcmp(strInput, curr->attname)==0)
				{
					//printf("%s ", curr->attname);
					values[i][j]=curr->equivalent;
					break;
				}
				curr=curr->next;
			}

		}
		//printf("\n");
	}

	printf("\n");   

	/*for(i=0; i<ntraining; i++)
	{
		for(j=0; j<nattributes; j++)
		{
			printf("%d ", values[i][j]);
		}
		printf("\n");
	}*/ 

	addSubtree(0, values, ntraining, nattributes, &train, &decisionTree);
	node=glob.head;
	while(node!=NULL)
	{
		addSubtree(node->equivalent, values, ntraining, nattributes, &train, &decisionTree);
		node=node->globnext;
	}
	printf("\n");  

	/*
		Format of testing.txt:
		5                               // Number of values
		Sunny Hot High Light 	        // 1st input value
		Sunny Hot High Strong	        // 2nd input value
		Overcast Hot High Light			// 3rd input value
		Rain Mild High Light	        // 4th input value
		Rain Cool Normal Light			// 5th input value
	*/

	file3=fopen("testingE02.txt", "r");
	if(file3==NULL)
	{
		printf("Missing or empty file.\n");
		return 0;
	}

	fscanf(file3, "%d\n", &ntesting);
	for(i=0; i<ntesting; i++)
	{
		testcurr=test.head;
		for(j=0; j<nattributes-1; j++)
		{
			fscanf(file3, "%s ", strInput);
			curr=train.head;
			while(curr!=NULL)
			{
				if(strcmp(strInput, curr->attname)==0)
				{
					printf("%s ", curr->attname);
					testing[j]=curr->equivalent;
					break;
				}
				curr=curr->next;
			}

		
			findtest=train.head;
			while(findtest!=NULL)
			{
				if(findtest->equivalent==testing[j])
				{
					//printf("Found attribute %s\n", findtest->attname);
					break;
				}
				findtest=findtest->next;
			}
			if(test.head==NULL)
			{
				test.head=findtest;
				test.tail=findtest;
				test.head->testnext=NULL;
			}
			else
			{
				testcurr=test.head;
				while(testcurr->testnext!=NULL)
				{
					testcurr=testcurr->testnext;
				}
				findtest->testnext=NULL;
				testcurr->testnext=findtest;
			}
		}
		classify(&test, &train, &decisionTree);
	}

	/*for(i=0; i<nattributes-1; i++)
	{
		//printf("%d ", testing[i]);
	}

	int testing[6]={1, 5, 10, 13, 17, 21};
	int testingctr=6;

	testcurr=test.head;
	for(i=0; i<testingctr; i++)
	{
		findtest=train.head;
		while(findtest!=NULL)
		{
			if(findtest->equivalent==testing[i])
			{
				break;
			}
			findtest=findtest->next;
		}
		if(test.head==NULL)
		{
			test.head=findtest;
			test.tail=findtest;
			test.head->testnext=NULL;
		}
		else
		{
			testcurr=test.head;
			while(testcurr->testnext!=NULL)
			{
				testcurr=testcurr->testnext;
			}
			findtest->testnext=NULL;
			testcurr->testnext=findtest;
		}
	}

	testcurr=test.head;
	while(testcurr!=NULL)
	{
		printf("%s ", testcurr->attname);
		testcurr=testcurr->testnext;
	}


	classify(&test, &train, &decisionTree);
	int testing2[6]={1, 3, 10, 12, 16, 20};
	testcurr=test.head;
	for(i=0; i<testingctr; i++)
	{
		findtest=train.head;
		while(findtest!=NULL)
		{
			if(findtest->equivalent==testing2[i])
			{
				break;
			}
			findtest=findtest->next;
		}
		if(test.head==NULL)
		{
			test.head=findtest;
			test.tail=findtest;
			test.head->testnext=NULL;
		}
		else
		{
			testcurr=test.head;
			while(testcurr->testnext!=NULL)
			{
				testcurr=testcurr->testnext;
			}
			findtest->testnext=NULL;
			testcurr->testnext=findtest;
		}
	}

	testcurr=test.head;
	while(testcurr!=NULL)
	{
		printf("%s ", testcurr->attname);
		testcurr=testcurr->testnext;
	}
	classify(&test, &train, &decisionTree);*/
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

void initTree(tree * foo)
{
	foo->top=NULL;
}

void addNode(att * foo, char attname[LENGTH], int x, int y, int equivalent)
{
	attnode * node;
	attnode * curr;

	node=(attnode*)malloc(sizeof(attnode));
	strcpy(node->attname, attname);
	node->x=x;
	node->y=y;
	node->equivalent=equivalent;
	node->used=0;
	node->positivetracker=0;
	node->negativetracker=0;
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
	attnode * curr;
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

int positiveValues(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train)
{
	attnode * curr;
	attnode * yes;
	int i, j, k;
	int positivectr=0;
	if(S==0)
	{
		yes=train->head;
		while(yes!=NULL)
		{
			if(strcmp(yes->attname, "Yes")==0)
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<ntraining; i++)
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
		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		yes=train->head;
		while(yes!=NULL)
		{
			if(strcmp(yes->attname, "Yes")==0)
			{
				break;
			}
			yes=yes->next;
		}

		for(i=0; i<ntraining; i++)
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
	return positivectr;

}

int negativeValues(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train)
{
	attnode * curr;
	attnode * no;
	int i, j, k;
	int negativectr=0;
	if(S==0)
	{
		no=train->head;
		while(no!=NULL)
		{
			if(strcmp(no->attname, "No")==0)
			{
				break;
			}
			no=no->next;
		}

		for(i=0; i<ntraining; i++)
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
		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}

		no=train->head;
		while(no!=NULL)
		{
			if(strcmp(no->attname, "No")==0)
			{
				break;
			}
			no=no->next;
		}

		for(i=0; i<ntraining; i++)
		{
			for(j=0; j<nattributes; j++)
			{
				for(k=0; k<nattributes; k++)
				{
					if(values[i][k]==S && values[i][j]==attribute && values[i][nattributes-1]==no->equivalent)
					{
						negativectr++;
					}   
				}
			}
		}
	}
	return negativectr;
}

float getEntropy(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train)
{
	float entropy;
	int positive;
	int negative;
	attnode * curr;
	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			break;
		}
		curr=curr->next;
	}
	if(S==0)
	{
		positive=positiveValues(S, attribute, values, ntraining, nattributes, train);
		negative=negativeValues(S, attribute, values, ntraining, nattributes, train);
	}
	else
	{
		updatePosNeg(S, attribute, values, ntraining, nattributes, train);
		positive=curr->positivetracker;
		negative=curr->negativetracker;
	}
	float pos;
	float neg;
	float logpos;
	float logneg;
	int total;

	total=positive+negative;
	pos=(float)positive/total;
	neg=(float)negative/total;
	logpos=log2(pos);
	logneg=log2(neg);
	if(positive==0||negative==0)
	{
		entropy=0;
	}
	else if(positive==negative)
	{
		entropy=1;
	}
	else
	{
		entropy=(-pos*logpos)-(neg*logneg);
	}
	return entropy;
}

float getGain(int parent, int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train)
{
	attnode * curr;
	attnode * curr2;
	attnode * findS;
	float entropy;
	float entropy2;
	float sum=0;
	int denominator;
	int numerator;
	float gain;
	int positive;
	int negative;
	entropy=getEntropy(parent, S, values, ntraining, nattributes, train);

	findS=train->head;
	while(findS!=NULL)
	{
		if(findS->equivalent==S)
		{
			break;
		}
		findS=findS->next;
	}
	if(S==0)
	{
		positive=positiveValues(S, S, values, ntraining, nattributes, train);
		negative=negativeValues(S, S, values, ntraining, nattributes, train);	
	}
	else
	{
		updatePosNeg(parent, S, values, ntraining, nattributes, train);
		positive=findS->positivetracker;
		negative=findS->negativetracker;

	}
	denominator=positive+negative;

	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			break;
		}
		curr=curr->next;
	}

	curr2=curr->next;
	while(curr2!=NULL)
	{
		if(curr2->y==curr->y)
		{
			entropy2=getEntropy(S, curr2->equivalent, values, ntraining, nattributes, train);
			if(entropy2!=0)
			{
				if(S==0)
				{
					positive=positiveValues(S, curr2->equivalent, values, ntraining, nattributes, train);
					negative=negativeValues(S, curr2->equivalent, values, ntraining, nattributes, train);
				}
				else
				{
					updatePosNeg(S, curr2->equivalent, values, ntraining, nattributes, train);
					positive=curr2->positivetracker;
					negative=curr2->negativetracker;
				}
				numerator=positive+negative;
				sum+=(float)numerator/denominator*entropy2;
			}
		}
		curr2=curr2->next;
	}
	gain=entropy-sum;
	if(S==0)
	{
		printf("Gain of %s is %lf\n", curr->attname, gain);
	}
	else
	{
		printf("Gain of %s and %s is %lf\n", findS->attname, curr->attname, gain);
	}
	return gain;
}

int getMaxGain(int parent, int S, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train)
{
	float max=0;
	int maxAtt;
	int i;
	int usedctr=0;
	float gain;
	attnode * curr;

	for(i=-1; i>-nattributes; i--)
	{
		if(isUsed(i, train)==0)
		{
			gain=getGain(parent, S, i, values, ntraining, nattributes, train);
			if(gain>max)
			{
				max=gain;
				maxAtt=i;
			}
		}
		else
		{
			usedctr++;
		}
	}

	if(usedctr==nattributes-1)
	{
		maxAtt=102;
	}

	else
	{
		if(max==0)
		{
			int pos=positiveValues(0, S, values, ntraining, nattributes, train);
			int neg=negativeValues(0, S, values, ntraining, nattributes, train);
			if(pos>neg)
			{
				maxAtt=100;
			}
			else
			{
				maxAtt=101;
			}
		}
	}
	if(maxAtt!=100 && maxAtt!=102 && maxAtt!=100)
	{
		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==maxAtt)
			{
				break;
			}
			curr=curr->next;
		}
		printf("Attribute chosen is %s\n", curr->attname);
	}
	return maxAtt;
}

int isUsed(int attribute, att * train)
{
	attnode * curr;
	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			if(curr->used==1)
			{
				printf("%s is already a part of the tree\n", curr->attname);
				return 1;
			}
			else
			{
				return 0;
			}
		}
		curr=curr->next;
	}
	return 0;
}

void addSubtree(int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train, tree * decisionTree)
{
	attnode * curr;
	attnode * curr2;
	attnode * decurr;
	attnode * from;
	attnode * globcurr;
	attnode * first;
	attnode * yes;
	attnode * no;
	int toAdd;
	int i=0;
	int numRSON;

	if(decisionTree->top==NULL)
	{
		toAdd=getMaxGain(0, 0, values, ntraining, nattributes, train);

		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==toAdd)
			{
				curr->used=1;
				decisionTree->top=curr;
				decisionTree->top->LSON=NULL;
				decisionTree->top->RSON=NULL;
				decisionTree->top->parent=NULL;

				break;
			}
			curr=curr->next;
		}
		decurr=decisionTree->top;

		printf("Root of Decision Tree: %s\n", decurr->attname);

		first=decurr;

		curr2=curr->next;
		while(curr2!=NULL)
		{
			if(curr2->y==curr->y)
			{
				i++;
				if(i==1)
				{
					decurr->LSON=curr2;
					curr2->parent=decurr;
					updatePosNeg(curr2->parent->equivalent, curr2->equivalent, values, ntraining, nattributes, train);
					decurr->RSON=NULL;
					printf("%s -LSON-> %s\n", decurr->attname, decurr->LSON->attname);
					decurr=decurr->LSON;
					if(glob.head==NULL)
					{
						decurr->globnext=NULL;
						glob.head=decurr;
						//printf("\nAdded %s to glob\n", decurr->attname);
					}
					else
					{
						globcurr=glob.head;
						while(globcurr->globnext!=NULL)
						{
							globcurr=globcurr->globnext;
						}
						decurr->parent=first;
						decurr->globnext=NULL;
						globcurr->globnext=decurr;
						//printf("\nAdded %s to glob\n", decurr->attname);
					}
				}
				else
				{
					decurr->LSON=NULL;
					decurr->RSON=curr2;

					curr2->parent=first;

					updatePosNeg(curr2->parent->equivalent, curr2->equivalent, values, ntraining, nattributes, train);
					printf("%s -RSON-> %s\n", decurr->attname, decurr->RSON->attname);
					decurr=decurr->RSON;
					if(glob.head==NULL)
					{
						decurr->globnext=NULL;
						glob.head=decurr;
						//printf("\nAdded %s to glob\n", decurr->attname);
					}
					else
					{
						globcurr=glob.head;
						while(globcurr->globnext!=NULL)
						{
							globcurr=globcurr->globnext;
						}
						decurr->parent=first;
						decurr->globnext=NULL;
						globcurr->globnext=decurr;
						//printf("\nAdded %s to glob\n", decurr->attname);
					}
				}
			}
			curr2=curr2->next;
		}
	}

	else
	{
		decurr=decisionTree->top;
		curr=train->head;
		while(curr!=NULL)
		{
			if(curr->equivalent==attribute)
			{
				break;
			}
			curr=curr->next;
		}
		printf("\nAttribute: %s\n", curr->attname);

		if(curr->positivetracker==0)
		{
			//printf("Leaf Node with value No\n");
			no=train->head;
			while(no!=NULL)
			{
				if(strcmp(no->attname, "No")==0)
				{
					break;
				}
				no=no->next;
			}
			printf("%s -OUTCOME-> %s\n", curr->attname, no->attname);
			curr->LSON=no;
			no->parent=curr;

		}
		else if(curr->negativetracker==0)
		{
			yes=train->head;
			while(yes!=NULL)
			{
				if(strcmp(yes->attname, "Yes")==0)
				{
					break;
				}
				yes=yes->next;
			}
			printf("%s -OUTCOME-> %s\n", curr->attname, yes->attname);
			curr->LSON=yes;
			yes->parent=curr;
		}

		else
		{
			curr=train->head;
			while(curr!=NULL)
			{
				if(curr->equivalent==attribute)
				{
					break;
				}
				curr=curr->next;
			}

			toAdd=getMaxGain(curr->parent->equivalent, curr->equivalent, values, ntraining, nattributes, train);
			if(toAdd==102||toAdd==100||toAdd==101)
			{
				//printf("Dahil used na si %s:\n", curr->attname);
				curr=train->head;
				while(curr!=NULL)
				{
					if(curr->equivalent==attribute)
					{
						break;
					}
					curr=curr->next;
				}

				if(curr->positivetracker>curr->negativetracker)
				{
					//printf("Leaf Node with value Yes\n");
					//printf("APPEND YES %d dahil wala na (No: %d)\n", curr->positivetracker, curr->negativetracker);
					yes=train->head;
					while(yes!=NULL)
					{
						if(strcmp(yes->attname, "Yes")==0)
						{
							break;
						}
						yes=yes->next;
					}
					printf("%s -OUTCOME-> %s\n", curr->attname, yes->attname);
					curr->LSON=yes;
					yes->parent=curr;
				}
				else
				{
					//printf("Leaf Node with value No\n");
					//printf("APPEND NO %d dahil wala na (Yes: %d)\n", curr->negativetracker, curr->positivetracker);
					no=train->head;
					while(no!=NULL)
					{
						if(strcmp(no->attname, "No")==0)
						{
							break;
						}
						no=no->next;
					}

					printf("%s -OUTCOME-> %s\n", curr->attname, no->attname);
					curr->LSON=no;
					no->parent=curr;
				}
			}

			else
			{
				
				decurr=decisionTree->top;
				curr=train->head;
				while(curr!=NULL)
				{
					if(curr->equivalent==toAdd)
					{
						curr->used=1;
						decurr=curr;
						decurr->LSON=NULL;
						decurr->RSON=NULL;
						break;
					}
					curr=curr->next;
				}
				curr->used=1;
				
				curr=train->head;
				while(curr!=NULL)
				{
					if(curr->equivalent==attribute)
					{
						from=curr;
						break;
					}
					curr=curr->next;
				}

				printf("%s -LSON-> %s\n", from->attname, decurr->attname);

				from->LSON=decurr;
				decurr->parent=from;
				updatePosNeg(decurr->parent->equivalent, decurr->equivalent, values, ntraining, nattributes, train);
				curr=decurr;

				first=decurr;
				curr2=curr->next;
				while(curr2!=NULL)
				{
					if(curr2->y==curr->y)
					{
						i++;
						if(i==1)
						{
							decurr->LSON=curr2;
							curr2->parent=decurr;
							updatePosNeg(curr2->parent->equivalent, curr2->equivalent, values, ntraining, nattributes, train);
							decurr->RSON=NULL;
							printf("%s -LSON-> %s\n", decurr->attname, decurr->LSON->attname);
							decurr=decurr->LSON;
							if(glob.head==NULL)
							{
								decurr->globnext=NULL;
								glob.head=decurr;
								//printf("\nAdded %s to glob\n", decurr->attname);
							}
							else
							{
								globcurr=glob.head;
								while(globcurr->globnext!=NULL)
								{
									globcurr=globcurr->globnext;
								}
								decurr->parent=first;
								decurr->globnext=NULL;
								globcurr->globnext=decurr;
								//printf("\nAdded %s to glob\n", decurr->attname);
							}
						}
						else
						{
							decurr->LSON=NULL;
							decurr->RSON=curr2;

							curr2->parent=first;

							printf("%s -RSON-> %s\n", decurr->attname, decurr->RSON->attname);
							decurr=decurr->RSON;
							updatePosNeg(decurr->parent->equivalent, decurr->equivalent, values, ntraining, nattributes, train);
							if(glob.head==NULL)
							{
								glob.head=decurr;
								//printf("\nAdded %s to glob\n", decurr->attname);
							}
							else
							{
								globcurr=glob.head;
								while(globcurr->globnext!=NULL)
								{
									globcurr=globcurr->globnext;
								}
								decurr->parent=first;
								decurr->globnext=NULL;
								globcurr->globnext=decurr;
								//printf("\nAdded %s to glob\n", decurr->attname);
							}
						}
					}
					curr2=curr2->next;
				}
			}
		}
	}
}

void updatePosNeg(int S, int attribute, int values[LENGTH][LENGTH], int ntraining, int nattributes, att * train)
{
	attnode * curr;
	attnode * curr2;
	attnode * findS;
	attnode * realnode;
	attnode * yes;
	attnode * no;
	int parents[LENGTH];
	int parentsctr=0;
	int positivectr=0;
	int negativectr=0;
	int i, j, k;

	findS=train->head;
	while(findS!=NULL)
	{
		if(findS->equivalent==S)
		{
			break;
		}
		findS=findS->next;
	}

	curr=train->head;
	while(curr!=NULL)
	{
		if(curr->equivalent==attribute)
		{
			parents[parentsctr]=curr->equivalent;
			parentsctr++;
			realnode=curr;
			if(findS->equivalent>0)
			{
				parents[parentsctr]=findS->equivalent;
				parentsctr++;				
			}
			break;
		}
		curr=curr->next;
	}

	curr=findS;
	while(curr!=NULL)
	{
		if(curr->parent!=NULL && curr->parent->equivalent>0)
		{
			parents[parentsctr]=curr->parent->equivalent;
			parentsctr++;				
		}
		curr=curr->parent;
	}

	yes=train->head;
	while(yes!=NULL)
	{
		if(strcmp(yes->attname, "Yes")==0)
		{
			break;
		}
		yes=yes->next;
	}

	no=train->head;
	while(no!=NULL)
	{
		if(strcmp(no->attname, "No")==0)
		{
			break;
		}
		no=no->next;
	}

	int breakouter=0;
	int matchingctr;
	for(i=0; i<ntraining; i++)
	{
		matchingctr=0;
		for(j=0; j<nattributes; j++)
		{
			for(k=0; k<parentsctr; k++)
			{
				if(parents[k]==values[i][j])
				{
					matchingctr++;
				}
			}
		}
		if(matchingctr==parentsctr)
		{
			if(values[i][nattributes-1]==no->equivalent)
			{
				negativectr++;
			}
			else if(values[i][nattributes-1]==yes->equivalent)
			{
				positivectr++;
			}
		}
	}
	if(realnode->equivalent<0)
	{
		realnode->positivetracker=-1;
		realnode->negativetracker=-1;
	}
	else
	{
		realnode->positivetracker=positivectr;
		realnode->negativetracker=negativectr;
	}
}


int classify(att * test, att * train, tree * decisionTree)
{
	attnode * curr;
	attnode * curr2;
	attnode * decurr;
	attnode * yes;
	attnode * no;

	yes=train->head;
	while(yes!=NULL)
	{
		if(strcmp(yes->attname, "Yes")==0)
		{
			break;
		}
		yes=yes->next;
	}

	no=train->head;
	while(no!=NULL)
	{
		if(strcmp(no->attname, "No")==0)
		{
			break;
		}
		no=no->next;
	}
	
	decurr=decisionTree->top;
	//printf("Top: %s\n", decurr->attname);
	while(decurr->equivalent!=yes->equivalent && decurr->equivalent!=no->equivalent)
	{
		//printf("Di pa nagyes/no. %d==%d? or %d==%d\n", decurr->equivalent, yes->equivalent, decurr->equivalent, no->equivalent);
		curr=test->head;
		while(curr!=NULL)
		{
			if(curr->y==decurr->y)
			{
				//printf("Found matching input with equivalent y value %s.\n", curr->attname);
				break;
			}
			curr=curr->testnext;
		}
		decurr=decurr->LSON;
		//printf("Move to LSON with value %s\n", decurr->attname);
		while(curr->equivalent!=decurr->equivalent)
		{
			decurr=decurr->RSON;
			//printf("Move to RSON with value %s\n", decurr->attname);
		}
		decurr=decurr->LSON;
		//printf("Value is now %s\n", decurr->attname);
	}
	printf("%s\n", decurr->attname);

	test->head=NULL;

	if(decurr->equivalent==no->equivalent)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}