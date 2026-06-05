/*************************************************
                 MATHUTILS.H V0
This is a comprehensive library which is useful to 
draw plots using various math functions which includes 
sin, cos, tan and many more, This library is made as an 
alternative to matlab, unlike matlab, mathutils is open source
and free to use. 

Author Name: T.Vikram (a useless person with no friends)
Date: 05/06/2026
Version: 0


As an contrubution to CVR College of enggneering
special thanks to:
gnuplot for providing their GUI interface
************************************************/

#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>



//to select different plot styles 
//i.e continuous -> choose STD_PLOT
//for discrete -> choose STEM_PLOT
//for dotted points -> choose SCATTER_PLOT
#define STD_PLOT "lines"
#define STEM_PLOT "impulses"
#define SCATTER_PLOT "points"


//macros
//Array: this macro makes an array filled with zeros 
//arange: this macro makes an array incremented by the step size
//mapFunc: this macro applies the various math functions given by the user
#define Array(cap, type) _create_array(cap, TYPE_##type)
#define arange(start, end, step, type) _create_arange(start, end, step, TYPE_##type)
#define mapFunc(input, math_func, type) _mapFunc(&input, math_func, TYPE_##type)




//This is a guard to prevent compiling from GCC 
//your file must be compiled with mbuild only 
#ifndef MBUILD
#error "This project must be compiled using mbuild."
#endif




#ifndef ARENA_SIZE
#define ARENA_SIZE (1024 * 512)
#endif


//typedefs
typedef double f64;
typedef long double f128;
typedef double (*math_func_t)(double);





//---------------------MAIN STRUCTURE---------------------

//main important struct for the plotGraph
typedef struct map {
	const char *title; // Title of the graph
	const char *xlabel; // xlabel (i.e label the x-axis of the plot) 
	const char *ylabel; // ylabel (i.e label the y-axis of the plot)
	const char *plotTitle; // this is the legend of the plot
	const char *plotstyle; // this is to choose between different styles i.e STD_PLOT ect
} map;


//----------------------END--------------------------------








//----------------MUARRAY STRUCTURE-----------------
//enum for the muArr
typedef enum {
    TYPE_int,    
    TYPE_f64,
    TYPE_f128
} DataType;


typedef struct muArr {
    void *data; 
    size_t size;
    size_t len;
    DataType type;
} muArr;


//-----------------END------------------------------




//----------------GLOBAL ARENA STRUCTURE--------------------
typedef struct Arena {
    uint8_t *ptr;
    size_t size;
    size_t offset;
} Arena;


//context: The mathutils library uses a global arena to manage 
//memory, in this manner the memory doesn't get leaked but there are
//several disadvantages that come with this approch 
//Disadvantages:
//1. the size of the arena remains fixed for the entire program  (std size = 512kb, it can be changed) 
//2. not thread safe, in the case of concurrency the global arena and that it uses malloc
//makes this approch not thread safe as there be race conditions 
//----------------------END--------------------------------









//------------------------MATHUTILS FUNCTIONS-------------------------




//function1: factrl 
//-> this function provides the factorial of the function 
//and max factorial it can provide is 33 to 34.
//->beyond that the integer overflow takes place
__int128 factrl(int n);



//function2: plotGraph
//-> this function takes arrays 'x' and 'y' and plots them using gnuplot
//note: the arrays x, y must be the same size or the graph may look 
//unfinished
int plotGraph(map *obj, const f64 *x, const f64 *y, size_t size);




//function3: sum
//->This function takes an input array and returns the sum of all the values in the arrray
f64 sum(muArr arr);



//function4: _create_array
//->this function returns an array allocated by the arena 
//all filled with zeros (calloc behaviour)
muArr _create_array(size_t cap, DataType type);



//function5:
//->this function returns an array, aranged with values
//dictated by start, end, step
muArr _create_arange(f64 start, f64 end, f64 step, DataType type); 


//function6:
//->this function takes an input array and returns the mean of the all the values in the array
f64 mean(muArr arr);


//function7:
//->this function takes an input array and applies the function 
//to all the values in the array as the output
muArr _mapFunc(muArr *input, math_func_t func, DataType out_type); 


//function8:
//->this function creates an array allocated by the arena 
//->all filled with zeroes
muArr _create_array(size_t cap, DataType type);



//function 9: 
//->this function returns the length of the array
size_t arrLen(muArr arr);

//function 10:
//->this function returns the current capacity of the array 
size_t arrCap(muArr arr);

//function 11:
//->this funtion returns true if array i empty
bool arr_isempty(muArr arr);


//function 12:
//->this function fills the array with zeroes
void arr_clean(muArr *arr);




#if ARENA_ACCESS
void initArena(void);
void freeArena(void);
void arena_reset(void);
void *arena_alloc(size_t bytes);
#endif



//--------------------------END----------------------------------------
#endif
