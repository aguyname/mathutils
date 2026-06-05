//--------------------------------------------MATHUTILS_C----------------------------------------
//Date: 2/05/2026
//File: mathutils.c 
//This file is created to provide utilities in math

#include "mathutils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


__attribute__((hot)) __int128 factrl(int n) {
    if (n < 0) return 0;

    __int128 result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}



int plotGraph(map *obj, const f64 *x, const f64 *y, size_t size) {
    if (!obj || !x || !y || size == 0)
        return -1;

    FILE *gnuplot = popen("gnuplot -persistent", "w");

    if (!gnuplot) {
        fprintf(stderr, "Error: gnuplot not found!\n");
        return -1;
    }

    if (obj->title && *obj->title)
        fprintf(gnuplot, "set title '%s'\n", obj->title);

    fprintf(gnuplot,"set xlabel '%s'\n",
            (obj->xlabel && *obj->xlabel) ? obj->xlabel : "x");

    fprintf(gnuplot,"set ylabel '%s'\n", 
    (obj->ylabel && *obj->ylabel) ? obj->ylabel : "y");

    fprintf(gnuplot, "set grid\n");

    fprintf(gnuplot,"plot '-' using 1:2 with %s title '%s'\n",
        (obj->plotstyle && *obj->plotstyle) ? obj->plotstyle : STD_PLOT,
        (obj->plotTitle && *obj->plotTitle) ? obj->plotTitle : "Data");

    for (size_t i = 0; i < size; ++i) 
        fprintf(gnuplot, "%.15g %.15g\n", x[i], y[i]);
    

    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
    pclose(gnuplot);

    return 0;
}





Arena g;


__attribute__((destructor)) void freeArena(void) {
    if (g.ptr) {
        free(g.ptr);
        g.ptr = NULL;
        g.size = 0;
        g.offset = 0;
    }
}


void handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
	freeArena();
        exit(0);
    }
}



__attribute__((constructor)) void initArena(void) {
    size_t capacity = ARENA_SIZE;
    g.ptr = malloc(capacity);

    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    signal(SIGTSTP, SIG_IGN);


    if(!g.ptr) {
        perror("Arena allocation failed");
        exit(EXIT_FAILURE);
    }
    g.size = capacity;
    g.offset = 0;
}







void* arena_alloc(size_t bytes) {
    size_t aligned_bytes = (bytes + 7) & ~(size_t)7;
   
    if (g.offset + aligned_bytes > g.size) {
        fprintf(stderr, "Arena Error: Out of memory\n");
        return NULL;
    }
   
    void *allocation = g.ptr + g.offset;
    g.offset += aligned_bytes;
    return allocation;
}




void arena_reset(void) {
    g.offset = 0;
}





muArr _create_array(size_t capacity, DataType type) {
    size_t element_size = 0;
    switch(type) {
        case TYPE_int:  element_size = sizeof(int); break;
        case TYPE_f64:  element_size = sizeof(f64); break;
        case TYPE_f128: element_size = sizeof(f128); break;
        default: return (muArr){0};
    }
   
    size_t total_bytes = capacity * element_size;

    void *mem = arena_alloc(total_bytes);
    if(mem) {
        memset(mem, 0, total_bytes);
    }
   
    return (muArr){
        .data = mem, 
        .size = capacity,  
        .len = 0,          
        .type = type
    };
}




muArr _mapFunc(muArr *input, math_func_t func, DataType out_type) {
    if (!input || !input->data || input->len == 0) 
        return (muArr){0};
    
    muArr output = _create_array(input->len, out_type);
    if (!output.data) return output;
    
    for (size_t i = 0; i < input->len; i++) {
        double val = 0;
        
        switch(input->type) {
            case TYPE_int:  
		val = (double)((int*)input->data)[i]; 
	    break;


            case TYPE_f64:  
	         val = ((f64*)input->data)[i]; 
	    break;

            case TYPE_f128: 
	         val = (double)((f128*)input->data)[i]; 
	    break;
        }
        
        double result = func(val);
        
        switch(out_type) {
            case TYPE_int:  
		 ((int*)output.data)[i] = (int)result; 
	    break;

            case TYPE_f64:  
	         ((f64*)output.data)[i] = result;
	    break;

            case TYPE_f128: 
	         ((f128*)output.data)[i] = (f128)result; 
	    break;
        }
    }
    
    output.len = input->len;
    return output;
}









muArr _create_arange(f64 start, f64 end, f64 step, DataType type) {
    if (start >= end || step <= 0) {
        return (muArr){0};
    }
   
    size_t capacity = (size_t)ceil((end - start) / step);
    muArr arr = _create_array(capacity, type);
    if (!arr.data) return arr;
   

    for(size_t i = 0; i < capacity; i++) {
        f64 val = start + (i * step);
        if(type == TYPE_int)        ((int*)arr.data)[i] = (int)val;
        else if(type == TYPE_f64)   ((f64*)arr.data)[i] = (f64)val;
        else if(type == TYPE_f128)  ((f128*)arr.data)[i] = (f128)val;
    }
    
    arr.len = capacity;  
    return arr;
}






f64 sum(muArr arr) {
    if (!arr.data || arr.len == 0) return 0.0;
    f64 total = 0.0;
   
    for (size_t i = 0; i < arr.len; i++) {  
        if (arr.type == TYPE_int)        total += ((int*)arr.data)[i];
        else if (arr.type == TYPE_f64)   total += ((f64*)arr.data)[i];
        else if (arr.type == TYPE_f128)  total += ((f128*)arr.data)[i];
    }
    return total;
}











f64 mean(muArr arr) {
    if (arr.len == 0) return 0.0;  
    return sum(arr) / arr.len;
}










size_t arrLen(muArr arr) {
    return arr.len;
}














size_t arrCap(muArr arr) {
    return arr.size;
}










bool arr_isempty(muArr arr) {
    return arr.len == 0;
}









void arr_clear(muArr *arr) {
    if (arr) {
        arr->len = 0;
	memset(arr->data, 0, arr->size);
    }
}








//
//
//
//---------------------------------------------EOF------------------------------------------------
