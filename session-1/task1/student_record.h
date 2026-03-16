
#define NAME_LENGTH 50
#define NUM_MODULES 3

/**
 * Structure to represent a student
 */
struct student_record {
    char name[NAME_LENGTH];  // Student's name
    int id;                  // Student's ID number
    float marks[NUM_MODULES]; // Marks for three modules
};

typedef struct student_record Student;

/*
 * function headers
 */
float calculate_average(Student student);
void display_student(Student student);


