
#include <stdio.h>
#include "shapes.h"

int main( void ) {

    // complete the structure definition in the header file first
    // code in main to test the structures and functions
    Point p = { .x=1, .y=2 };
    Rectangle r = makeRectangle(p, 3.0, 4.0);
    printf("Area of rectangle: %f\n", area(r));
    
    return 0;
}

Rectangle makeRectangle( Point p, float width, float height ){
    Rectangle r;

    return r;
}

float area( Rectangle r ) {
    float a = 8.0;

    return a;
}

void shiftRectangle( Rectangle *r, Point dp ) {

    return;
}

void scaleRectangle( Rectangle *r, float scale ) {

    return;
}
