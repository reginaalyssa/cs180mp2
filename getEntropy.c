float getEntropy(int S, int attribute){
	// S=0: apply to all tuples
	float entropy;
	float pos;
	float neg;
	int total;

	total = 14; //getpositive + getnegative
	pos = 9/14; //(getpositive over total)
	neg = 5/14; //(getnegative over total)
	entropy = (-pos*log2(pos))-(neg*log2(neg));
	printf("\nanswer: %lf\n", entropy);
	return entropy;
}