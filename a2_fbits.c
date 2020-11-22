//Enhan Zhao 11097118 enz889 cmpt214


#include <stdio.h> 


//a function that converts an int into binary.
void num_to_bin(int num, int n){
	
	int storage[n];

	int i;
	for (i=0; i < n; i++){

		storage[i] = num % 2;
		num = num / 2;
        }

	for (int k = n-1; k >= 0; k--){
		printf("%d", storage[k]);
		}
}

//create a a union that holds the 3 parts of the unsigned int.

typedef union{
	float f;
	//storage for mantissa, exponent and sign.
	struct{
		unsigned int m: 23;
		unsigned int e: 8;
		unsigned int s: 1;
		}raw;
}myfloat;



int main(){
	
	//num has 3 parts. 
	myfloat num;
	
	printf("Please enter a single-precision float: ");
 
	if (scanf("%f", &num.f) != 1){ 		
		return 0;
	}
	
	printf("%d%s", num.raw.s, " ");
	//convert exponent into binary
	num_to_bin(num.raw.e, 8);
	printf(" ");
	//convert mantissa into binary
	num_to_bin(num.raw.m, 23);
	printf("\n");




	return main();

}
