// Enhan Zhao enz889 11097118 cmpt214 

#include <stdio.h>
#include <string.h>
	

//main: get input from user an if its a valid integer display it in binary form. Terminate if not valid. 
int main(){

	int num;               //the decimal number 
	int storage[32] = {0}; //array to store the bits of an int. 
	

	printf("Please enter an int decimal, int hexadicimal, or int octal: ");

	//if the input does not match format, return.
	if (scanf("%i", &num) != 1){ 		
		return 0;
	}

	//if the number is negative, make the last bit 1. Also convert negative to positive. 
	if (num < 0){	
		storage[31] = 1;
		num = num * -1;
	}


	//keep % number by 2 until num becomes 0. For each %, if there is remainder, put 0 for that bit. Other wise put 1. 
	int remainder = num %2;
	int i = 0;
	while (num != 0){
		num = num / 2;
		if (remainder == 0){
			storage[i] = 0;
			i ++;
			}
		else{
			storage[i] = 1;
			i++;
			}
		remainder = num % 2;
        }
	
	
	printf("The binary representation of the number is: \n");

	//iterate through the array. back wards and print each bit. 
	for (int n = 31; n >= 0; n--){
		if (n == 7 || n == 15 || n == 23){
			printf(" ");
			}
		printf("%d", storage[n]);
		}


	printf("\n");

	return 0;
}


