//Enhan Zhao enz889 11097118 cmpt214 a3

//////cgr.c///////

#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#define MIN_SCALE 16
#define MAX_SCALE 64
#define NUM_NTIDES 8
//global variable 
int Scale; //stores the scale of the graph. Will be 16, 32, or 64.
char **Plot; //stores the plot array, dimension Scale*Scale
//datatypes

/*
		coord_t
data structure that represent a fraction contains two uint64_t variables 
n (numerator) and q (denominator). The value of fraction will be between 
0 and 1 inclusive (q >= n)
*/
typedef struct coord_t {
  uint64_t n;
  uint64_t q;
} coort_t;

/*
		point_t
data structures that represents a point on a graph with x and y values.
x and y values are coord_t datatype. 
*/

typedef struct point_t {
  struct coord_t x_coord;
  struct coord_t y_coord;
} point_t;

/*
		ntide_t
data structure that contains the point_t cooridinates of the graph corners.
ATGC are all point_t datatype. 
*/
typedef struct ntide_t {
  struct point_t A;
  struct point_t T;
  struct point_t G;
  struct point_t C;
} ntide_t;


//functions
/*
		initialize_plot()
Assign each element in Plot to a space character.
In: nothing
Out: nothing
Assumes: global variable Scale and Plot is present and Plot is dimensions 
Scale*Scale.
*/
void initialize_plot( void ) {
  for(int i=0; i < Scale; i++ ) {
      for(int j=0; j<Scale; j++ ) {
        Plot[i][j] = ' ';
      }
  } 
}

/*
		is_power_of_2()
Determine if the input argument is a power of 2.
In: an integer value.
Out: boolean true if val is a pwer of 2, false otherwise.
*/
_Bool is_power_of_2(int val){
  return (val != 0) && ((val & (val - 1)) == 0);
}


/*
		mostSigBit()
Returns the decimal representation of the highest order bit.
In: unsigned long int n that is the number to be determined
Out: int highestbit that is the decimal representation of the highest bit
*/
int mostSigBit(unsigned long n){
  if (n==0){
    return 0;
    }
  int highestbit = 0;
  while (n != 0){
    n /= 2;
    highestbit++;
    }
  return highestbit;
}

/*
		reduce_coord()
If the q value in a coord_t data is higher than 2^63, 
reduce n and q by half.
In: old_coord that is the coord to be reduced.
Out: old_coord if it does not need reducing; 
     modified old_coord if it needs reducing. 
*/
struct coord_t reduce_coord( struct coord_t old_coord ){
  if (mostSigBit(old_coord.q) > 63){
    old_coord.q = old_coord.q /2;
    old_coord.n = old_coord.n /2;
  }
  return old_coord;
}

/*
		reduce_point()
Use reduce_coord() to reduce the x and y vaues of point_t.
In: point_t data old_point to be reduced.
Out: old_point if not changed; modified old_point if changed. 
*/
point_t reduce_point( point_t old_point ){
  old_point.x_coord = reduce_coord(old_point.x_coord);
  old_point.y_coord = reduce_coord(old_point.y_coord);
  return old_point;
}

/*
		print_plot()
prints the Plot global variable, along with added boarder and 
AGTC labels at the corners.
In: nothing
Out: nothing
*/
void print_plot( void ){
  int i, j;
  printf("C+");
  for (i = 0; i < Scale;i++){
    printf("-");
  }

  printf("+G\n");
  for (i = Scale-1 ; i >= 0; i--){
    printf(" |");
    for (j = 0; j < Scale; j++){

	printf("%c", Plot[i][j]);
      }
    printf("|\n");

  }
  
  printf("A+");
  for (i = 0; i < Scale;i++){
    printf("-");
  }
  printf("+T\n");
}

/*
		scale_coord()
Scale up the coord_t by a factor of Scale. Depending on q and Scale value,
return the scaled coordinates. 
In: coord, coord_t to be scaled up
Out: unsigned int that is the scaled value.
Assumes that coord contains q and n where q>=n.
*/
unsigned scale_coord(struct coord_t coord){
  coord = reduce_coord(coord);
  if ((uint64_t)Scale == coord.q){
    return floor(coord.n);
  }
  if ((uint64_t)Scale < coord.q){
    return floor(coord.n/(coord.q/Scale));
  }
  if ((uint64_t)Scale > coord.q){
    return floor(coord.n*(Scale/coord.q));
  }
  return 0;
}

/*
		plot_point()
Given the coordinate of the next point, plot it in the global Plot.
In: nothing
Out: nothing
*/
void plot_point(point_t point){
  unsigned int x,y;
  x=scale_coord(point.x_coord);
  y=scale_coord(point.y_coord);
  Plot[x][y] = '*';
}

/*
		determine_midpoint()
Given 2 point_t points, determine the trucated midpoint.
Multply n, q of pnt2 (which always have values of 1 or 0) by
the q of pnt1 and then another 2 (to prevent round down to 0)
inorder to allow addition.
In: pnt1, pnt2, point_t data structures
Out: modified pnt1 that is the midpoint of the 2 points.
*/
point_t determine_midpoint( point_t pnt1, point_t pnt2 ){
  pnt1 = reduce_point(pnt1);

  pnt1.x_coord.n *=2;
  pnt1.x_coord.q *=2;
  pnt1.y_coord.n *=2;
  pnt1.y_coord.q *=2;

  pnt2.x_coord.n *= pnt1.x_coord.q;
  pnt2.x_coord.q *= pnt1.x_coord.q;
  pnt2.y_coord.n *= pnt1.y_coord.q;
  pnt2.y_coord.q *= pnt1.y_coord.q;
  //find midpoint
  pnt1.x_coord.n = ((pnt1.x_coord.n + pnt2.x_coord.n)/2);
  pnt1.y_coord.n = ((pnt1.y_coord.n + pnt2.y_coord.n)/2);

  return pnt1;
}

/*
		cgr()
The function that converts in_char to upper, 
calls reduce_point(), determine_midpoint(), plot_point().
If in_char is not Aa Cc Tt Gg, return in_point
plot the midpoints of previous point and new cooridinate
In: in_point, point_t data that is the last midpoint.
    in_char the next letter to be read in that determines 
    where the next midpoint is.
Out: the determined midpoint if valid in_char; old midpoint
     if invalid in_char.
*/
point_t cgr(point_t in_point, char in_char){
  //A
  struct coord_t ax = {0, 1};
  struct coord_t ay = {0, 1};
  point_t A = {ax, ay};
  //T
  struct coord_t tx = {0, 1};
  struct coord_t ty = {1, 1};
  point_t T = {tx, ty};
  //C
  struct coord_t cx = {1, 1};
  struct coord_t cy = {0, 1};
  point_t C = {cx, cy};
  //G
  struct coord_t gx = {1, 1};
  struct coord_t gy = {1, 1};
  point_t G = {gx, gy};
  ///////////////////////////
  ntide_t ATGC = {A,T,G,C};//
  ///////////////////////////
  if (in_char >= 'a' && in_char <= 'z'){
    in_char = toupper(in_char);
  }
  //////cases
  in_point = reduce_point(in_point);
  point_t next;
  if (in_char == 'A'){
    next = determine_midpoint(in_point, ATGC.A);
  }
  else if (in_char == 'T'){
    next = determine_midpoint(in_point, ATGC.T);
  }
  else if (in_char == 'G'){
    next = determine_midpoint(in_point, ATGC.G);
  }
  else if (in_char == 'C'){
    next = determine_midpoint(in_point, ATGC.C);
  }
  else{return in_point;}
  plot_point(next);
  return next;
}

/*
	main()
the main function of cgr.c takes in a Scale from commandline,
as well as input string from standard input. After checking 
for valid Scale, memory is dynamically allocated for **Plot,
Plot is initialized and cgr() is called to plot each point,
after reading the string 1 at a time.	

*/

int main(int argc, char *argv[]) {
  
  if (argc < 2){
    printf("Not enough arguemnts.\n");
  }
  if (argc > 2){
  printf("Too many arguments.\n");
  }
  Scale = atoi(argv[1]);
  if (is_power_of_2(Scale) != 1 || Scale < MIN_SCALE || Scale > MAX_SCALE){
    printf("Invalid input, must be 16, 32 or 64.\n");
    return 0;
  }
  int i;
  Plot = (char**)malloc((Scale)*sizeof(char*));
  for (i =0; i < Scale; i++){
    Plot[i] = (char*)malloc((Scale)*sizeof(char));
  }

  //Start point
  struct coord_t sx = {1, 2};
  struct coord_t sy = {1, 2};
  point_t S = {sx, sy};

  initialize_plot();
  char c = 0;
  while (scanf("%c", &c) == 1){
   S = cgr(S, c);
  }
  
  print_plot();
  free(Plot);
  return EXIT_SUCCESS;
}
