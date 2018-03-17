#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAXNUM 0x40
#define prefix_student 1
#define prefix_course 2
#define prefix_course_taken 3

char *prefix[0x20] = {"_student.dat", "_course.dat", "courseTaken.dat"};
char *grade_lists[9] = {"A+", "A-", "B+", "B-", "C+" ,"C-", "D+", "D-", "Fail"};

typedef struct{
	unsigned int stdID;
	char studentName[0x30];
	char studentMajor[0x30];
}student;

typedef struct{
	unsigned int courseID;
	char courseName[0x30];
}course;

typedef struct{
	unsigned int No;
	unsigned int studentID;
	unsigned int courseID;
	char grade[4];
}course_taken;

student *std_array[MAXNUM];
course *std_course[MAXNUM];
course_taken *std_course_taken[MAXNUM];

int quit = 0;

int intro(void){
	int user_input = 0;
	puts("\n[+] asiagaming's mysql database\n");
	puts("1. create table");
	puts("2. insert table");
	puts("3. show tables");
	puts("4. show table's datas");
	puts("5. binary IO");
	puts("6. quit");
	printf("\n> ");
	scanf("%d", &user_input);
	return user_input;
}

void show_insert_menu(void){
	puts("\n1. student");
	puts("2. course");
	puts("3. course_taken");
	printf("\n> ");
}

void create(void);
void insert(void);
void insert_to_student(void);
void insert_to_course(void);
int insert_to_course_taken(void);
void show_tables(void);
void show_table_datas(void);
void binary_IO(void);
void binary_open(void);
void binary_save(void);
void save_student(char *_nameset, FILE *_fp);
void save_course(char *_nameset, FILE *_fp);
void save_course_taken(char *_nameset, FILE *fp);

int main(int argc, char *argv[]){
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stderr, 0, 2, 0);
	while(quit != 6){
		quit = intro();
		switch(quit){
			case 1:
				create();
				break;
			case 2:
				insert();
				break;
			case 3:
				show_tables();
				break;
			case 4:
				show_table_datas();
				break;
			case 5:
				binary_IO();
				break;
			case 6:
				break;
			default:
				puts("[-] Wrong.");
				break;
		}
	}
	return 0;
}
		
void create(void){
	puts("[+] create");
	puts("[-] No meaning in this task...");
}
void insert(void){
	puts("[+] insert");
	show_insert_menu();
	int iCase = 0;
	scanf("%d", &iCase);
	switch(iCase){
		case 1:
			insert_to_student();
			break;
		case 2:
			insert_to_course();
			break;
		case 3:
			insert_to_course_taken();
			break;
		default:
			puts("[-] Wrong");
			break;
	}
}

/*  
  9 typedef struct{
 10     unsigned int stdID;
 11     char studentName[0x30];
 12     char studentMajor[0x30];
 13 }student;
*/

void insert_to_student(void){
	puts("[+] insert_to_student");
	int index = 0;
	for( index = 0; index < 0x40; index++ ){
		if( !std_array[index] ){
			std_array[index] = (student *)malloc(sizeof(student));
			break;
		}
	}
	printf("student ID : ");
	scanf("%u", &std_array[index]->stdID);
	printf("student name : ");
	scanf("%47s", std_array[index]->studentName);
	printf("student major : ");
	scanf("%47s", std_array[index]->studentMajor);
}

/*
 15 typedef struct{
 16     unsigned int courseID;
 17     char courseName[0x30];
 18 }course;
 19
 20 typedef struct{
 21     unsigned int No;
 22     unsigned int studentID;
 23     unsigned int courseID;
 24     char grade[4];
 25 }course_taken;
*/

void insert_to_course(void){
	puts("[+] insert_to_course");
	int index = 0;
	for( index = 0; index < 0x40; index++ ){
		if( !std_course[index] ){
			std_course[index] = (course *)malloc(sizeof(course));
			break;
		}
	}
	printf("course ID : ");
	scanf("%u", &std_course[index]->courseID);
	printf("course name : ");
	scanf("%47s", std_course[index]->courseName);
}

int insert_to_course_taken(void){
	puts("[+] insert_to_course_taken");
	int index = 0;
	int check = 0;
	for( index = 0; index < MAXNUM; index++ ){
		if( !std_course_taken[index] ){
			std_course_taken[index] = (course_taken *)malloc(sizeof(course_taken));
			break;
		}
	}
	std_course_taken[index]->No = (unsigned int)index;
	printf("student id : ");
	scanf("%u", &std_course_taken[index]->studentID);
	for(int i = 0; i < MAXNUM; i++ ){
		if( std_array[i] && std_course_taken[index]->studentID == std_array[i]->stdID ){
			check = 1;
			break;
		}
	}
	if( !check ){
		printf("[-] No matched studentID\n");
		std_course_taken[index] = 0;		// initalize to 0
		return -1;
	}
	printf("course id : ");
	scanf("%u", &std_course_taken[index]->courseID);
	check = 0;
	for(int i = 0; i < MAXNUM; i++ ){
		if( std_course[i] && std_course_taken[index]->courseID == std_course[i]->courseID ){
			check = 1;
			break;
		}
	}
	if( !check ){
		printf("[-] No matched courseID\n");
		std_course_taken[index] = 0;		// initalize to 0
		return -1;
	}
	printf("grade [A+, A-, ... , Fail] : ");
	scanf("%3s", std_course_taken[index]->grade);
	for( int i = 0; i < 9; i++ ){
		if( !strcmp( grade_lists[i], std_course_taken[index]->grade ) ){
			break;
		}
		if( i == 8 ){
			puts("[-] Invalid grade");
			std_course_taken[index] = 0;		// initalize to 0
			return -1;
		}
	}
	return 0;
}

void show_tables(void){
	puts("[+] show_tables");
}
void show_table_datas(void){
	puts("[+] show_table_datas");
}
void binary_IO(void){
	puts("[+] binary IO");
	printf("\n1. binary open -> read\n");
	printf("\n2. binary save\n");
	int bCase = 0;
	scanf("%d", &bCase);
	switch(bCase){
		case 1:
			binary_open();
			break;
		case 2:
			binary_save();
			break;
		default:
			puts("[-] Wrong");
			break;
	}
}

void binary_open(void){
	puts("[+] binary open -> read");
}

void save_file(int num){
	FILE *fp = 0;
	char name_set[0x30] = {0, };
	printf("name initial : ");
	scanf("%10s", name_set);
	strcat(name_set, prefix[num]);
	fp = fopen(name_set, "wb");

	switch(num){
		case prefix_student:
			save_student(name_set, fp);
			break;
		case prefix_course:
			save_course(name_set, fp);
			break;
		case prefix_course_taken:
			save_course_taken(name_set, fp);
			break;
		default:
			puts("[-] Wrong");
			break;
	}
}

void binary_save(void){
	int fCase = 0;
	puts("[+] binary save");
	printf("\n1. student file\n2. course file\n3. course_taken file\n\n> ");
	scanf("%d", &fCase);
	save_file(fCase);
}

/*
typedef struct{
	unsigned int stdID;
	char studentName[0x30];
	char studentMajor[0x30];
}student;
*/

void save_student(char *_nameset, FILE *_fp){
	int index = 0;
	char id[0x10] = {0, };
	for( index; index < MAXNUM; index++ ){
		if( std_array[index] ){
			sprintf(id, "%u\n", std_array[index]->stdID);
			fwrite(id, strlen(id), 1, _fp);
			fwrite(std_array[index]->studentName, strlen(std_array[index]->studentName), 1, _fp);
			fwrite(std_array[index]->studentMajor, strlen(std_array[index]->studentMajor), 1, _fp);
		}
	}
	fclose(_fp);
}

void save_course(char *_nameset, FILE *_fp){
	int index = 0;
	char id[0x10] = {0, };
	for( index; index < MAXNUM; index++ ){
		if( std_course[index] ){
			sprintf(id, "%u\n", std_course[index]->courseID);
			fwrite(id, strlen(id), 1, _fp);
			fwrite(std_course[index]->courseName, strlen(std_course[index]->courseName), 1, _fp);
		}
	}
	fclose(_fp);
}

/*
typedef struct{
	unsigned int No;
	unsigned int studentID;
	unsigned int courseID;
	char grade[4];
}course_taken;
*/

void save_course_taken(char *_nameset, FILE *_fp){
	int index = 0;
	char id[0x10] = {0, };
	for( index; index < MAXNUM; index++ ){
		if( std_course_taken[index] ){
			sprintf(id, "%u\n", std_course_taken[index]->No);
			fwrite(id, strlen(id), 1, _fp);
			sprintf(id, "%u\n", std_course_taken[index]->studentID);
			fwrite(id, strlen(id), 1, _fp);
			sprintf(id, "%u\n", std_course_taken[index]->courseID);
			fwrite(id, strlen(id), 1, _fp);
			fwrite(std_course_taken[index]->grade, 4, 1, _fp);
		}
	}
	fclose(_fp);
}
