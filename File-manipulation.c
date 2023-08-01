#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct student{
    char name[30];
    int age;
    float gpa;
}student;

void file_create(FILE*, bool);
void file_read(FILE*);
void file_retrieve_student_data(FILE*);
void file_add_student(FILE*);
void file_remove_student(FILE*);
void file_find_student_by_name(FILE*);
void file_remove_line(FILE*, int);

int main(){
    
    FILE *fptr;
    file_create(fptr, 1);

    int response;

    do{
        printf("\nWhat would you like to do with the file?\n\n");
        printf("1. Add student data to the file\n");
        printf("2. Clear the file\n");
        printf("3. Retrieve student data\n");
        printf("4. Search for student by name\n");
        printf("5. Remove student from the file\n");
        printf("6. Quit this application\n");

        scanf(" %i", &response);
        switch(response){
            case 1:
                file_add_student(fptr);
                break;
            case 2:
                file_create(fptr, 0);
                break;
            case 3: 
                file_retrieve_student_data(fptr);
                break;
            case 4:
                file_find_student_by_name(fptr);
                break;
            case 5:
                file_remove_student(fptr);
                break;
            case 6:
                printf("\nThank you for using this application!\n");
                break;
            default:
                printf("You have not chosen a valid option!\n");
                break;
        }
    }while(response != 6);

    return 0;
}
void file_create(FILE *fptr, bool rewriteFile){
    fptr = fopen("students.txt", "r");
    if(fptr == NULL || rewriteFile == 0){
        fptr = fopen("students.txt", "w");
        fclose(fptr);   
    }
}
void file_read(FILE *fptr){
    char ch;

    fptr = fopen("students.txt", "r");
    if(fptr == NULL){
        printf("file can't be opened\n");
        file_create(fptr, 1);
        file_read(fptr);
    }

    printf("content of this file: \n\n");
    do{
        ch = fgetc(fptr);
        printf("%c", ch);

    }while(ch != EOF);

    fclose(fptr);
}
void file_retrieve_student_data(FILE *fptr){
    fptr = fopen("students.txt", "r");

    char firstName[15];
    char lastName[15];
    int age;
    float gpa;

    char line[256];
    while(fgets(line, sizeof(line), fptr) != NULL){
        sscanf(line, "%s %s %i, %f", &firstName, &lastName, &age, &gpa);
        for(int i = 0; i < 15; i++){
            if(lastName[i] == ','){
                lastName[i] = '\0';
                break;
            }
        }
        printf("\nName: %s %s\n", firstName, lastName);
        printf("Age: %i\n", age);
        printf("gpa: %.1f\n", gpa);
    }

    fclose(fptr);
}
void file_add_student(FILE *fptr){

    fptr = fopen("students.txt", "a");

    if(fptr == NULL){
        printf("file can't be opened\n");
    }

    student student1;

    printf("\nEnter your full name: ");
    scanf(" %c");
    fgets(student1.name, 30, stdin);
    student1.name[strlen(student1.name)-1] = '\0';

    printf("\nEnter your age: ");
    scanf(" %i", &student1.age);

    printf("\nEnter your gpa: ");
    scanf(" %f", &student1.gpa);

    fprintf(fptr, "%s, %i, %f\n", student1.name, student1.age, student1.gpa);
    fclose(fptr);
}
void file_remove_student(FILE *fptr){

    char name[30];
    char tempName[30];
    char lastName[15];

    printf("Full name of student you would like to remove: ");
    scanf(" %c");
    fgets(name, 30, stdin);
    name[strlen(name)-1] = '\0';

    fptr = fopen("students.txt", "r");
    if(fptr == NULL){
        printf("file can't be opened");
    }

    float f;
    int counter = 0;
    char line[256];
    while(fgets(line, sizeof(line), fptr) != NULL){
        sscanf(line, "%s %s %i, %f", &tempName, &lastName, &f, &f);
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
                file_remove_line(fptr, counter);
            } else if(ch1 == ch2){
                continue;
            }
            break;
        }
        counter++;
    }

    fclose(fptr);
}
void file_find_student_by_name(FILE *fptr){

}
void file_remove_line(FILE *fptr, int lineCounter){
    
    FILE *temp;
    temp = fopen("temp.txt", "w");
    freopen("temp.txt", "a", temp);
    if(temp == NULL){
        printf("temp file can't be created");
    }

    fptr = fopen("students.txt", "r");
    if(fptr == NULL){
        printf("file can't be opened");
    }

    char line[256];
    int count = 0;
    while(fgets(line, sizeof(line), fptr) != NULL){
        if(count != lineCounter){
            fprintf(temp, line);
        }
        count++;
    }

    freopen("temp.txt", "r", temp);

    freopen("students.txt", "w", fptr);
    freopen("students.txt", "a", fptr);
    while(fgets(line, sizeof(line), temp) != NULL){
        fprintf(fptr, line);
    }

    fclose(fptr);
    fclose(temp);
    remove("temp.txt");

    printf("\nSuccesfully removed student\n");
}