/* address_list.c */
// This program should be completed such that it maintains a list of students
// X TO DO 1: Write a function inputStudent(int i), which allows the user to enter first name and last name and stores it at position i in the array.
// X TO DO 2: Use a variable to count the number of students stored in the array.
// X TO DO 3: Write a function printStudent(i), which prints student i stored in the array
// X TO DO 4: Write functions addStudent(), for adding a student at the end of the array, and printAllStudents() for printing all students stored.
// X TO DO 5: Write a function menu(), which allows the user to select one of the actions
//            - add Student
//            - print all students
//            - quit program
// X TO DO 6: Extend the student data and the corresponding functions with the gender male/female (using "enum").
// X TO DO 7: Create a structure for handling dates (day, month, year) and use it for birth date, date of enrollment etc.
// X TO DO 8: Extend the program such it also handles address information (street, number, zip-code and city).
// TO DO 9: optional: Add a function and menu item for deleting a selected student.
//          --> sry, but that breaks some things, I would have to reimplement, and I'm too lazy for that...
// TO DO 10: optional: Add a function to sort the data according to given criteria

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG

//no need to use typedef, but why not?
typedef enum {DIVERS, FEMALE, MALE} gender;
const char* gender_string[] = {"divers", "female", "male"};

struct address{
    char street[30];
    int street_number;
    int zip_code;
    char city[30];
};

struct date{
    int day;
    int month;
    int year;
};

struct person{
    int student_id;
    char first_name[30];
    char last_name[30];
    gender gender;
    struct date birthdate;
    struct date enrollmentdate;
    struct address student_address;
};

void inputStudent(int i);
void printStudent(int id);
void printAllStudents();
void printMenu();
void addStudent(char first_name[], char last_name[], char gender[], struct date birthdate, struct  date enrollmentdate, struct address address);
void dateToString(char* buffer, struct date d);
void inputFormStudent();
void storeStudentsData(int slot);
void readStudentsData(int slot);

struct person students[100];
//current and unused student_id && number of registered students
//not the best solution, I know...
int student_id = 0;


#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(){
    char input[30];
    printMenu();
    while(1){
        printf("Enter operation: ");
        scanf("%s", &input);
        //Just ignore the input-thing
        if(strcmp(input,"input") == 0){
            int add_s_id = -1;
            //printf("Enter student_id: ");
            scanf("%d", &add_s_id);
            if(add_s_id < 0){
                printf("This student_id does not exist! May SAP help you...");
                continue;
            }
            inputStudent(add_s_id);
            student_id++;
        }
        else if(strcmp(input, "add") == 0) inputFormStudent();
        else if(strcmp(input, "print") == 0){
            //printf("Enter victim-id: ");
            // Don't default to 0, as this is a valid student_id
            int s_id = -1;
            scanf("%d", &s_id);
            if(s_id < 0){
                printf("This student_id does not exist! May SAP help you...");
                continue;
            }
            printStudent(s_id);
        }
        else if(strcmp(input, "list") == 0) printAllStudents();
        #if defined(DEBUG)
        else if(strcmp(input, "cs") == 0) printf("\nCurrent student_id=%d\n", student_id);
        #endif
        else if(strcmp(input, "help") == 0 || strcmp(input, "menu") == 0) printMenu();
        else if(strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) exit(0);
        else if(strcmp(input, "save") == 0){
            int db_slot = -1;
            scanf("%d", &db_slot);
            if(db_slot < 0){
                printf("This is not a valid db_slot! Maybe SAP knows what you mean...");
                continue;
            }
            storeStudentsData(db_slot);
        }
        else if(strcmp(input, "load") == 0){
            int db_slot = -1;
            scanf("%d", &db_slot);
            if(db_slot < 0){
                printf("This is not a valid db_slot! Maybe SAP knows what you mean...");
                continue;
            }
            readStudentsData(db_slot);
        }
        else printf("\n%s is no valid command!\n", input);
    }
}
#pragma clang diagnostic pop

//legacy
//I don't think there is a need for it if addStudent exists...
//Also, I'm too lazy to update it... Just ignore it!
void inputStudent(int s_id){
    char first_name[30];
    char last_name[30];

    scanf("%s", &first_name);
    scanf("%s", &last_name);

    struct person s;
    strcpy(s.first_name, first_name);
    strcpy(s.last_name, last_name);
    s.student_id = s_id;

    students[s_id] = s;

    printf("Added following student:\n");
    printStudent(s_id);
}

void addStudent(char first_name[], char last_name[], char gender[], struct date birthdate, struct  date enrollmentdate, struct address address){
    struct person s;
    strcpy(s.first_name, first_name);
    strcpy(s.last_name, last_name);
    s.student_id = student_id;
    s.birthdate = birthdate;
    s.enrollmentdate = enrollmentdate;

    if(strcmp(gender, "divers") == 0) s.gender=DIVERS;
    if(strcmp(gender, "female") == 0) s.gender=FEMALE;
    if(strcmp(gender, "male") == 0) s.gender=MALE;

    s.student_address = address;

    students[student_id] = s;
    student_id++;

    printf("\nThe following students has just been added:");
    printStudent(student_id-1);
}

void printStudent(int s_id){
    char birthdate_buffer[13];
    char enrollmentdate_buffer[13];
    dateToString(birthdate_buffer, students[s_id].birthdate);
    dateToString(enrollmentdate_buffer, students[s_id].enrollmentdate);
    printf("\n\n#### BEGIN Student %d ####\nFirstname: %s\nLastname: %s\nGender: %s\nBirthdate: %s\nEnrollmentdate: %s\n\nAddress:\n%s %d\n%d %s\n#### END Student %d ####\n\n",
           s_id, students[s_id].first_name, students[s_id].last_name, gender_string[students[s_id].gender], birthdate_buffer, enrollmentdate_buffer, students[s_id].student_address.street, students[s_id].student_address.street_number, students[s_id].student_address.zip_code, students[s_id].student_address.city, s_id);
}

void printAllStudents(){for (int i=0; i<student_id; i++) printStudent(i);}

//renamed menu to printMenu --> makes more sense for this implementation
void printMenu(){
    //\t is kinda handy, I should use it more
    printf("\n########################## MENU ##########################\n"
           "\"add\":\topens a dialog to add a new student (auto_id)\n"
           "\"list\":\tprints all stored students\n"
           "\"help\":\tprint this menu\n"
           "\"quit\":\texits the program\n"
           "\"save\":\tsaves current database to disk in selected slot\n"
           "\"load\":\tloads database in selected slot from disk\n"
           "\nFor additional help call 0800 5343424\n"
           "##########################################################\n\n");
}

void dateToString(char* buffer, struct date d){
    //I forgot about the NULL-Byte... duh...
    char day[3];
    char month[3];
    char year[5];

    snprintf(day, 3, "%2d", d.day);
    snprintf(month, 3, "%2d", d.month);
    snprintf(year, 5, "%4d", d.year);

    //hack in the leading-zero
    if(day[0] == ' ') day[0] = '0';
    if(month[0] == ' ') month[0] = '0';

    strcpy(buffer, day);
    strcat(buffer, ".");
    strcat(buffer, month);
    strcat(buffer, ".");
    strcat(buffer, year);
}

void inputFormStudent(){
    char first_name[30];
    char last_name[30];
    char gender[30];
    printf("\n\n### Input student %d ###\n", student_id);
    printf("Enter first_name: ");
    scanf("%s", &first_name);
    printf("Enter last_name: ");
    scanf("%s", &last_name);
    printf("\nEnter gender: ");
    scanf("%s", &gender);

    struct date bd;
    struct date ed;

    printf("\nEnter birthdate (without '.' and in the format: DAY MONTH YEAR): ");
    scanf("%d", &bd.day);
    scanf("%d", &bd.month);
    scanf("%d", &bd.year);

    printf("Enter enrollmentdate (without '.' and in the format: DAY MONTH YEAR): ");
    scanf("%d", &ed.day);
    scanf("%d", &ed.month);
    scanf("%d", &ed.year);

    struct address a;

    printf("\n### Enter address ###\nStreet and street number (Format: Street street number): ");
    scanf("%s", &a.street);
    scanf("%d", &a.street_number);
    printf("Enter zip-code and city (Format: zip-code city): ");
    scanf("%d", &a.zip_code);
    scanf("%s", &a.city);

    addStudent(first_name, last_name, gender, bd, ed, a);
}

//I couldn't be bothered more to put it all in one file
void storeStudentsData(int slot){
    char db_filename[30];
    char db_metadata_filename[30];
    snprintf(db_filename, 30, "students_%d.db", slot);
    snprintf(db_metadata_filename, 30, "students_%d_db.meta", slot);

    FILE *f = fopen(db_filename, "wb");
    fwrite(students, sizeof(struct person), sizeof(students), f);
    fclose(f);
    FILE *s = fopen(db_metadata_filename, "w");
    putw(student_id, s);
    fclose(s);
}

void readStudentsData(int slot){
    char db_filename[30];
    char db_metadata_filename[30];
    snprintf(db_filename, 30, "students_%d.db", slot);
    snprintf(db_metadata_filename, 30, "students_%d_db.meta", slot);

    FILE *f = fopen(db_filename, "rb");
    fread(students, sizeof(struct person), sizeof(students), f);
    fclose(f);
    FILE *s = fopen(db_metadata_filename, "r");
    student_id = getw(s);
    fclose(s);
}