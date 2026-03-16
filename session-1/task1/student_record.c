/**
 * Implementation of a student record management system using structures
 * 
 * This program demonstrates the use of structures in C to manage student data
 * including personal information and module marks.
 */

#include <stdio.h>
#include <string.h>

#include "student_record.h"

/**
 * Calculate the average mark for a student
 * 
 * param student The student whose average mark is to be calculated
 * return float The average mark
 */
float calculate_average(Student student) {
    // TODO: Implement this function to calculate and return the average
    // of the three module marks
    float sum = 0.0;
    for (int i = 0; i < 3; i++) {
        sum += student.marks[i];
    }
    return sum / 3.0;
}

/**
 * Display student information including name, ID, marks, and average
 * 
 * student The student whose information is to be displayed
 */
void display_student(Student student) {
    // TODO: Implement this function to display student information
    // Format: Name, ID, individual marks, and average mark
    printf("Name: %s\n", student.name);
    printf("ID: %d\n", student.id);
    printf("Marks: %.2f, %.2f, %.2f\n", student.marks[0], student.marks[1], student.marks[2]);
    printf("Average Mark: %.2f\n", calculate_average(student));
}

/*
 * main
 */
int main( void ) {
    // TODO: Create and initialise a student variable with sample data
    // Example: name = "John Smith", id = 12345, marks = {75.0, 68.5, 81.0}
    Student student;
    strncpy(student.name, "John Smith", NAME_LENGTH);
    student.id = 12345;
    student.marks[0] = 75.0;
    student.marks[1] = 68.5;
    student.marks[2] = 81.0;

    display_student(student);

    return 0;
}