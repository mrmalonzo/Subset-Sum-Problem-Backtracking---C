#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int N; //global variable for the size of N

void checkMatrix(int Matrix[N+2][N+2]){ //prints the matrix
	printf("Option Matrix: \n");
	for(int i=0; i<N+2; i++){
		for(int j=0; j<N+2; j++){
			printf("%d\t", Matrix[i][j]);
		}
		printf("\n");
	}
}

void checkArray(int Array[N+2]){ //prints the nopts array
	printf("Nopts List: ");
	for(int i=0; i<N+2; i++){
		printf("%d\t", Array[i]);
	}
	printf("\n");
}

void printAnswer(int Array[N+2][N+2]){ //prints the answer
	for(int i=1; i<N+1; i++){
		if(Array[1][i] == 0) continue;
		printf("%d\t", Array[1][i]);
	}
	if(Array[1][1] != 0)printf("\n"); //if the array is not empty
}

int adjustMatrix(int Matrix[N+2][N+2], int col){ //pops the TOS of the current col
	int TOS = Matrix[1][col];
	for(int i=1; i<N+1; i++){
		Matrix[i][col] = Matrix[i+1][col]; //adjust to remove the TOS
	}
	return TOS; //return the TOS
}

void adjustUsed(int Array[N+2], int TOS){ //adjust and removes the TOS in the Used array
	for(int i=1; i<N+1; i++){
		if(Array[i] == TOS){
			Array[i] = 0;
		}
	}
}

int checkSum(int Array[N+2][N+2]){ //returns the sum of the set of TOS
	int sum=0;
	for(int i = 1; i< N + 1 ; i++)
		sum += Array[1][i];
	return sum;
}

int findNumberElements(int *Array){ //gets the size of the array
	int counter =0;
	while(Array[counter]!=0){
		counter++;
	}
	return counter;
}

void sortArray(int *array, int n){ // a simple insertion sort to sort my array

    int i, key, j;
    for(i=1; i<n;i++) {
        key= array[i];
        j = i-1;

        while(j>= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j -= 1;
        }
        array[j+1]=key;
    }
}

int checkIndex(int *array,  int TOS, int n){ //checks the index of the current TOS
	for(int i=0; i<n; i++){
		if(array[i]==TOS){
			return i;
		}
	}
}


int main(){
	FILE *fp; //open a file where you will place all your data, acts like the script 
	fp=fopen("input.txt", "r");
	int numberOps, sum, n, solfound = 0; //for file reading

	clock_t t1,t2; //for recording time
	double t;

	fscanf(fp, "%d", &numberOps);

	for(int p=0; p<numberOps; p++){ //iterate over the number of ops
		printf("================OPERATION %d============\n", p+1 );
		char string[50]; //storage of the line of digits
		int elements[50]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //dummy int array

		fscanf(fp, "%d\n", &sum); //get the sum to be desired

		fscanf(fp, "%[^\n]", string);  //put the file in a string

		char * token = strtok(string, " "); //split the string into tokens
		int i = 0 ;
		while( token != NULL ) { //iterate over the tokens and put it in an array of int
	      elements[i++] = atoi(token);
	      token = strtok(NULL, " ");
	   	}

	   	N = findNumberElements(elements); //to get the size of the matrix
	   	sortArray(elements, N); //sort my array so it can be used easily later
	   	// checkArray(elements, N);

		int nopts[N+2]; //array of top of stacks
		int used[N+2]; //array for the used numbers
		int option[N+2][N+2]; //array of stacks of options
		int flag, col, move = 1; //flags and counters used

		for(int i=0; i<N+2; i++){ //fill the initial array with 0's
			for(int j=0; j<N+2; j++){
				option[i][j]=0;
			}
		}

		for(int i=0; i<N+2; i++){
			nopts[i] = 0;
		}

		nopts[0] = -1; //place these as markers
		nopts[N+1] = -1;

		solfound = 0; //flag if the solution is found
		t1 = clock(); //record start time
		do{ //do while our options have not been all done
			// printf("=============\n");
			// checkMatrix(option);
			// checkArray(nopts);

			// printf("CURRENT TOS: %d\n", option[1][move-1]);
			if(nopts[move]>=1)
				move++; //to make it move to the next col
			// printf("MOVE: %d\n", move);

			// printf("CheckSUM: %d\n", checkSum(option));
			if(checkSum(option) < sum && move < N+1 && option[1][move-1]!=elements[N-1]){ //if the set of TOS is less than the wanted sum and it is not over the limit of the matrix, and the current TOS is not the last element
				col = 1;
				// printf("FILL\n");
				int index = checkIndex(elements, option[1][move-1], N);
				// printf("INdex %d\n", index);
				for(int i= (index == N ? 0 : index+1); i<N; i++){ //this is for the candidate numbers to be put inside, but start with the +1 index of the last TOS since it is sorted
					// printf("%d\n", i);
					flag = 0;
					for(int j=1; j<N+1; j++){
						if(elements[i] == option[1][j]){
							flag = 1;
							// checkArray(used);
							// printf("flag turned on for i = %d and TOS = %d\n", i, option[1][j] );
						} //check if the number has been used then turn the flag true
							
					}
					if(flag == 1){
						continue;
					} // if it is used, then dont place it
					else{
						// printf("PLACE IT IN THIS ROW: %d AND COL: %d\n", col, move);
						option[col++][move] = elements[i]; //else place it in the current col
						nopts[move]++; //increment the number of options in that col
					}
				}
			}else{  //if the current set of TOS is equal or greater than the wanted SUM
				if(checkSum(option) == sum){
					printAnswer(option);
					solfound++;
				} //if it is a subset, then print
				// printf("BACKTRACK\n");

				move--; //backtrack

				if(nopts[move] <= 1){ //if the nummber of opts is only 1, then backtrack twice
					nopts[move]--; //reduce the number of options in curent col
					option[1][move] = 0; //set the TOS to 0
					move--; //backtrack
				}
				adjustMatrix(option, move); //adjust the matrix to pop the TOS of the current COL
				nopts[move]--;

			}

		}while(move > 0);
		t2 = clock(); //record end time
		t = (double) (t2-t1)/ (double)CLOCKS_PER_SEC;
	
		printf("Time elapsed: %f\n", t); 

		if(!solfound){ //if there is no solution founded
			printf("NO SOLUTION FOUND!\n");
		}else{
			printf("# of Solutions Found: %d\n", solfound);
		}
	}
	fclose(fp);
	

}