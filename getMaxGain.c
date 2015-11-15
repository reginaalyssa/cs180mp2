int getMaxGain(int S, int values[LENGTH][LENGTH], int nvalues, int nattributes, att * test){
	float max=0;
	int maxAtt;
	int i;
	float gain;

	for (i=-1; i--; i>-nattributes){
		if (isUsed(test, i) == 0){
			gain=getGain(S, i, values, nvalues, nattributes, test);
			if (gain>max){
				max=gain;
				maxatt=i;
			}
		}
	}

	return i;
}