int isUsed(att * test, int attribute){
	attnode * curr;
	curr = test->head;

	while (curr!=NULL){
		if (curr->equivalent == attribute && curr->used == 1){
			return 1;
		}
		curr=curr->next;
	}

	return 0;
}