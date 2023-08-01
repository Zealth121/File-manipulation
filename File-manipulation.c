#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

// student struct that gets added to the file
typedef struct student{
    char name[30];
    int age;
    float gpa;
}student;

void file_create(FILE*, bool);
void file_retrieve_student_data(FILE*);
void file_add_student(FILE*);
void file_find_student_by_name(FILE*, bool);
void file_remove_line(FILE*, int);

int main(){
    
    // this is the student file that is used all throughout the program
    FILE *fptr;
    file_create(fptr, 1);

    char response;
    do{
        // prints the options that the user can choose from and scans for the response
        printf("\nWhat would you like to do with the file?\n\n");
        printf("1. Add student data to the file\n");
        printf("2. Clear the file\n");
        printf("3. Retrieve student data\n");
        printf("4. Search for student by name\n");
        printf("5. Remove student from the file\n");
        printf("6. Quit this application\n");

        scanf(" %c", &response);
        switch(response){
            // depending on the number chosen I will call the function necessary
            case '1':
                file_add_student(fptr);
                break;
            case '2':
                file_create(fptr, 0);
                break;
            case '3': 
                file_retrieve_student_data(fptr);
                break;
            case '4':
                file_find_student_by_name(fptr, 0);
                break;
            case '5':
                file_find_student_by_name(fptr, 1);
                break;
            case '6':
                printf("\nThank you for using this application!\n");
                break;
            default:
                printf("You have not chosen a valid option!\n");
                break;
        }
    }while(response != '6');

    return 0;
}
void file_create(FILE *fptr, bool clearFile){
    // creates the students file or clears the file depending on the clearFile boolean
    fptr = fopen("students.txt", "r");
    if(fptr == NULL || clearFile == 0){
        fptr = fopen("students.txt", "w");
        fclose(fptr);
        // lets the user know the file has been cleared
        if(clearFile == 0){
            printf("The file has been successfully cleared\n");
        }
    }
}
void file_retrieve_student_data(FILE *fptr){
    // opens the student file
    fptr = fopen("students.txt", "r");

    // declares the variables I will need to use when retrieving data
    char firstName[15];
    char lastName[15];
    int age;
    float gpa;

    char line[256];
    // this while loop goes through each line of the file and searches for specific data
    while(fgets(line, sizeof(line), fptr) != NULL){
        sscanf(line, "%s %s %i, %f", &firstName, &lastName, &age, &gpa);
        for(int i = 0; i < 15; i++){
            if(lastName[i] == ','){
                // removes the comma from the last name due to how scanning strings work
                lastName[i] = '\0';
                break;
            }
        }
        // prints out the data from the line
        printf("\nName: %s %s\n", firstName, lastName);
        printf("Age: %i\n", age);
        printf("gpa: %.1f\n", gpa);
    }

    fclose(fptr);
}
void file_add_student(FILE *fptr){

    // opens the student file
    fptr = fopen("students.txt", "a");
    if(fptr == NULL){
        printf("file can't be opened\n");
    }

    // creates the student struct and changes its values from the user input
    student student1;

    printf("\nEnter your full name: ");
    scanf(" %c");
    fgets(student1.name, 30, stdin);
    student1.name[strlen(student1.name)-1] = '\0';

    printf("\nEnter your age: ");
    scanf(" %i", &student1.age);

    printf("\nEnter your gpa: ");
    scanf(" %f", &student1.gpa);

    // writes to the file the student data
    fprintf(fptr, "%s, %i, %f\n", student1.name, student1.age, student1.gpa);
    fclose(fptr);
}
void file_find_student_by_name(FILE *fptr, bool retrieveData){

    char name[30];
    char tempName[30];
    char lastName[15];
    int age;
    float gpa;

    // gathers the name of the student they want to retrieve/delete
    if(retrieveData == 0){
        printf("Full name of student: ");
    }else{
        printf("Full name of student you would like to remove: ");
    }
    scanf(" %c");
    fgets(name, 30, stdin);
    name[strlen(name)-1] = '\0';

    // opens the student file
    fptr = fopen("students.txt", "r");
    if(fptr == NULL){
        printf("file can't be opened");
    }

    char line[256];
    int counter = 0;
    /* 
    for every line in the file I look for the name given
    if the name from the line and the name given match I send that data
    */
    while(fgets(line, sizeof(line), fptr) != NULL){
        sscanf(line, "%s %s %i, %f", &tempName, &lastName, &age, &gpa);
        for(int i = 0; i < 15; i++){
            if(lastName[i] == ','){
                lastName[i] = '\0';
                break;
            }
        }

        strcat(tempName, " ");
        strcat(tempName, lastName);

        char ch1;
        char ch2;
        for(int i = 0; i < 30; i++){
                ch1 = toupper(tempName[i]);
                ch2 = toupper(name[i]);
            if(tempName[i] == '\0' && name[i] == '\0'){
                // if the name matches then I print out the data or delete it
                if(retrieveData == 0){
                    printf("\nName: %s\n", tempName);
                    printf("Age: %i\n", age);
                    printf("GPA: %.2f\n", gpa);
                }else{
                    file_remove_line(fptr, counter);
                }
            } else if(ch1 == ch2){
                continue;
            }
            break;
        }
        counter++;
    }

    fclose(fptr);
}
void file_remove_line(FILE *fptr, int lineCounter){

    // creates a temporary file
    FILE *temp;
    temp = fopen("temp.txt", "w");
    freopen("temp.txt", "a", temp);
    if(temp == NULL){
        printf("temp file can't be created");
    }

    // opens the student file
    fptr = fopen("students.txt", "r");
    if(fptr == NULL){
        printf("file can't be opened");
    }

    // I write all of the student data into the temp file
    // except for the line we want to delete
    char line[256];
    int count = 0;
    while(fgets(line, sizeof(line), fptr) != NULL){
        if(count != lineCounter){
            fprintf(temp, line);
        }
        count++;
    }

    // I change the temp file to read and clear the student file
    freopen("temp.txt", "r", temp);

    freopen("students.txt", "w", fptr);
    freopen("students.txt", "a", fptr);
    while(fgets(line, sizeof(line), temp) != NULL){
        //I copy every line from the temp file into  the student file
        fprintf(fptr, line);
    }

    // delete the temporary file
    fclose(fptr);
    fclose(temp);
    remove("temp.txt");

    printf("\nSuccesfully removed student\n");
}