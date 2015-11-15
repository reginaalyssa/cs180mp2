void addSubtree(int attribute, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test, tree * decisionTree)
{
	attnode * curr;
	attnode * curr2;
	attnode * decurr;
	attnode * first;
	int toAdd;
	int i=0;

	// check if tree is null

	if (decisionTree->top==NULL)
	{
		// getMaxGain
		toAdd = getMaxGain(0, values, nvalues, nattributes, test);

		curr=test->head;
		while (curr!=NULL){
			if (curr->equivalent == toAdd){
				curr->used=1;
				decisionTree->top=curr;
				decisionTree->top->LSON=NULL;
				decisionTree->top->RSON=NULL;

				break;
			}
			curr=curr->next;
		}
		decurr=decisionTree->top;
		printf("\nTop nung dec tree: %s\n", decurr->attname);
		//addSubtree(decisionTree, test, decisionTree->top->equivalent, nattributes);

		// Append values of winning attribute
		curr2=curr->next;
		//printf("decurr: %s\n", decurr->attname);
		//printf("curr: %s\n", curr->attname);
		while(curr2!=NULL)
		{
			//printf("curr2: %s\n", curr2->attname);
			if(curr2->y==curr->y)
			{
				//printf("curr2 is a value\n");
				i++;
				if (i==1)
				{
					printf("i=1 (first value)\n");
					decurr->LSON=curr2;
					decurr->RSON=NULL;
					decurr->next=NULL;
					printf("%s -LSON-> %s", decurr->attname, decurr->LSON->attname);
					decurr=decurr->LSON;
					first=decurr;
				}
				else
				{
					printf("i>1 (next value)\n");
					decurr->LSON=NULL;
					decurr->RSON=curr2;
					decurr->next=NULL;
					printf("%s -RSON-> %s", decurr->attname, decurr->RSON->attname);
					decurr=decurr->RSON;
				}
			}
			curr2=curr2->next;
		}

		decurr=first;
		
		printf("\nfirst: %s\n", first->attname);
		while(decurr!=NULL){
			printf("decurr: %s\n", decurr->attname);
			addSubtree(decurr->equivalent, values, nvalues, nattributes, test, decisionTree);
			decurr=decurr->RSON;
		}
	}

	else
	{
		printf("\nAttribute: %d\n", attribute);
		toAdd = getMaxGain(attribute, values, nvalues, nattributes, test);
		printf("%d\n", toAdd);
	}
}