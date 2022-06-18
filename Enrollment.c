#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 30
#define fieldLength 200

#define diskFile "diskFile.dat"
#define courseFile "course.txt"

struct db_type
{
  char name[fieldLength];
  int age;
  char course1[fieldLength];
  char course2[fieldLength];
  char status[fieldLength];
};

struct courseInfo
{
  char code[20]; // e.g., EECS2030
  char title[fieldLength];
  char date[20];
  char time_start[20];
  char time_end[20];
  char location[20];
};

struct courseInfo courseArr[SIZE]; // global variable  array of struc

char prompt_menu(void);
void init_list(struct db_type *pArr[]);
void clearDB(struct db_type *pArr[]);
void init_courseArr(void);

void writeDisk(struct db_type *pArr[]);
void emptyDisk(void);
void loadDisk(struct db_type *pArr[]);

//Helper methods
int checkTimeConflicts(int, int);
int findCourse(char courseEntered[fieldLength]);
int findRecord(char toFind[fieldLength], struct db_type *pArray[]);

void swap(struct db_type *pArr[]);
void sort(struct db_type *pArr[]);
void enterNew(struct db_type *pArr[SIZE]);
void displayDB(struct db_type *pArr[]);
void removeRecord(struct db_type *pArr[]);
void displayCourses(void);

int main(int argc, char *argv[])
{

  struct db_type *db_pArr[SIZE]; // main db storage

  init_list(db_pArr); // set to NULL

  init_courseArr(); // load course from diskfile

  char choice;
  for (;;)
  {
    choice = prompt_menu();
    switch (choice)
    {
    case 'n':
      enterNew(db_pArr);
      break;
    case 'd':
      displayDB(db_pArr);
      break;
    case 'w':
      writeDisk(db_pArr);
      break;
    case 'l':
      loadDisk(db_pArr);
      break;
    case 's':
      sort(db_pArr);
      break;

    case 'c':
      clearDB(db_pArr);
      break;
    case 'e':
      emptyDisk();
      break;

    case 'v':
      displayCourses();
      break;
    case 'p':
      swap(db_pArr);
      break;
    case 'r':
      removeRecord(db_pArr);
      break;

    case 'q':
      exit(1); // terminate the whole program
    }
  }
  return 0;
}

void init_list(struct db_type *pArr[])
{
  int t;
  for (t = 0; t < SIZE; t++)
  {
    pArr[t] = NULL;
  }
}

void clearDB(struct db_type *pArr[])
{
  char c3[3];
  printf("are you sure to clear db? (y) or (n)? ");

  fgets(c3, 3, stdin);

  if (!strcmp(c3, "y\n"))
    init_list(pArr);
}

char prompt_menu(void)
{
  char s[80];
  while (1)
  {
    printf("\n-----------------------------------------------------------------\n");
    printf("|    %-20s", "(N)ew record");
    printf("%-20s", "(R)emove record");
    printf("Swa(p) records\t|\n");
    printf("|    %-20s", "(S)ort database");
    printf("%-20s", "(C)lear database");
    printf("(D)isplay db\t|\n");

    printf("|    %-20s", "(L)oad disk");
    printf("%-20s", "(W)rite disk");
    printf("(E)mpty disk\t|\n");

    printf("|    %-20s", "(V)iew courses"); //|\tSwa(p) record\t(Q)uit\t\t\t\t|\n");
    printf("%-20s", "(Q)uit");
    printf("*Case Insensitive*\t|\n");
    printf("-----------------------------------------------------------------\n");
    printf("choose one: ");

    fgets(s, 50, stdin); // \n added

    if (strlen(s) == 2 && strchr("edlsuqrcwnvpr", tolower(*s)))
      return tolower(*s); // s[0], return the first character of s
    //else
    printf("not a valid input!\n");
  }
}

/* display all or specified course */
void displayCourses(void)
{
  // the provide sample.out uses "%s\t%-40s%-5s %s-%s   %s\n" as formatting string for printing each course info
  char cCode[50];
  char cCodeDigits[50];
  char EECS[5];
  strcpy(EECS, "EECS");
  int index = 0;
  int found = 1; //works as boolean. If course is not found, this changes to 0.
  printf("course code (or 'a')? ");
  fgets(cCode, 50, stdin);
  int len = strlen(cCode);
  cCode[len - 1] = '\0';
  if (strcmp(cCode, EECS) != 0)
  {
    strcat(EECS, cCode);
    strcat(cCodeDigits, EECS);  //If only digits are entered, concatenates it to EECS and treats it as EECSXXXX
  }
  if (strcmp(cCode, "a") == 0)
  {
    printf("=================================================================================\n");
    while (strcmp(courseArr[index].code, "EECS0000") != 0)
    {
      printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[index].code, courseArr[index].title, courseArr[index].date, courseArr[index].time_start, courseArr[index].time_end, courseArr[index].location);
      index++;
    }
    printf("=================================================================================");
  }

  else if (strncmp(cCode, EECS, 4) == 0)
  {
    while (strcmp(cCode, courseArr[index].code) != 0)
    {
      index++;
      if (strcmp(courseArr[index].code, "EECS0000") == 0) //If the end is reached, terminate the loop.
      {
        printf("error! course does not exist.\n");
        found = 0; //course not found.
        break;
      }
    }
    if (found)
    {
      printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[index].code, courseArr[index].title, courseArr[index].date, courseArr[index].time_start, courseArr[index].time_end, courseArr[index].location);
    }
  }
  else if (strncmp(cCodeDigits, EECS, 4) == 0)
  {
    while (strcmp(cCodeDigits, courseArr[index].code) != 0)
    {
      index++;
      if (strcmp(courseArr[index].code, "EECS0000") == 0)
      {
        printf("error! course does not exist.\n");
        found = 0; //course not found.
        break;
      }
    }
    if (found)
    {
      printf("%s\t%-40s%-5s %s-%s   %s\n", courseArr[index].code, courseArr[index].title, courseArr[index].date, courseArr[index].time_start, courseArr[index].time_end, courseArr[index].location);
    }
  }
  else
  {
    printf("error! course does not exist.\n");
  }
}

/* input items into the list */
void enterNew(struct db_type *pArr[SIZE])
{
  char ageChar[fieldLength];
  char course1Entered[fieldLength];
  char course2Entered[fieldLength];
  int index = 0, flag = 1, found1, found2;
  while (pArr[index]->name != NULL)
  {
    index++;
  }
  pArr[index] = malloc(sizeof(struct db_type));

  printf("name: ");
  fgets(pArr[index]->name, fieldLength, stdin);

  printf("age: ");
  fgets(ageChar, fieldLength, stdin);
  pArr[index]->age = atoi(ageChar);

  //course 1
  printf("course-1: ");
  while (flag)
  {
    fgets(course1Entered, fieldLength, stdin);
    course1Entered[strlen(course1Entered) - 1] = '\0';
    found1 = findCourse(course1Entered);    //checks if the course entered exists
    if (found1 == -1)
    {
      printf("course does not exist, enter again: ");
    }
    else
    {
      flag = 0;
      sprintf(pArr[index]->course1, "%s\t%-40s%-5s %s-%s   %s\n", courseArr[found1].code, courseArr[found1].title, courseArr[found1].date, courseArr[found1].time_start, courseArr[found1].time_end, courseArr[found1].location);
    }
  }

  //course 2
  flag = 1;
  printf("course-2: ");
  while (flag)
  {
    fgets(course2Entered, fieldLength, stdin);
    course2Entered[strlen(course2Entered) - 1] = '\0'; //to eliminate the trailing '\n'
    found2 = findCourse(course2Entered);
    if (found2 == -1)
    {
      printf("course does not exist, enter again: ");
    }
    else
    {
      flag = 0;
      sprintf(pArr[index]->course2, "%s\t%-40s%-5s %s-%s   %s\n", courseArr[found2].code, courseArr[found2].title, courseArr[found2].date, courseArr[found2].time_start, courseArr[found2].time_end, courseArr[found2].location);
    }
  }
    //check for time conflicts.
  int conflict;
  conflict = checkTimeConflicts(found1, found2);
  if (conflict == -1)
  {
    sprintf(pArr[index]->status, "%s", "ATTENTION! time conflict.\n");
    printf("ATTENTION! time conflict.\n");
  }
  else
  {
    sprintf(pArr[index]->status, "%s", "SUCCESSFUL! no time conflict.\n");
    printf("Successfull! no time conflict.\n");
  }
}


/**
 * A helper method which checks if there is any time conflict between the 2 enrolled courses
 * */
int checkTimeConflicts(int found1, int found2)
{
  int j, conflict, startTimeCourse1, endTimeCourse1, startTimeCourse2;
  for (j = 0; j < 3; j++)   // 3 because maximum number of days a course is scheduled to take place is 3.
  {
    if (courseArr[found1].date[j] == courseArr[found2].date[j])
    {
      startTimeCourse1 = atoi(courseArr[found1].time_start);
      printf("\nstartTimeCourse1: %d\n", startTimeCourse1);
      endTimeCourse1 = atoi(courseArr[found1].time_end);
       printf("\nendTimeCourse1: %d\n", endTimeCourse1);
      startTimeCourse2 = atoi(courseArr[found2].time_start);
       printf("\nstartTimeCourse2: %d\n", startTimeCourse2);

      if (startTimeCourse2 >= startTimeCourse1 && startTimeCourse2 < endTimeCourse1)
      {
        conflict = -1;
      }
      else
      {
        conflict = 1;
      }
      break;
    }
  }
  return conflict;
}


/**
 * Helper method to find if the course entered exists or not.
 */
int findCourse(char courseEntered[fieldLength])
{
  int index = 0;
  char courseCode[fieldLength];
  strcpy(courseCode, "EECS");
  if (strncmp(courseEntered, courseCode, 4) != 0)
  {
    strcat(courseCode, courseEntered);
  }
  else
  {
    strcpy(courseCode, courseEntered);
  }
  while (strcmp(courseArr[index].code, courseCode) != 0)
  {
    if (strcmp(courseArr[index].code, "EECS0000") == 0)
    {
      return -1;
    }
    index++;
  }

  return index;
}


/* display records */
void displayDB(struct db_type *pArr[])
{
  int records = 0, i;
  printf("===============================\n");
  for (i = 0; i < SIZE; i++)
  {
    if (pArr[i] != NULL)
    {
      printf("\nname:    %s", pArr[i]->name);
      printf("age:     %d\n", pArr[i]->age);
      printf("course1: %s", pArr[i]->course1);
      printf("course2: %s", pArr[i]->course2);
      printf("remarks: %s\n", pArr[i]->status);
      records++;
    }
  }
  printf("\n========== %d records ==========\n", records);
}

/* remove an existing item */
void removeRecord(struct db_type *pArr[])
{
  char toRemove[fieldLength];
  char removed[fieldLength];
  int foundRecord, i;
  printf("enter full name to remove: ");
  fgets(toRemove, fieldLength, stdin);
  foundRecord = findRecord(toRemove, pArr);   //checks if a reocrd of the entered student name exists in the database.
  if (foundRecord == -1)
  {
    printf("record not found\n");
  }
  else
  {
    strcpy(removed, pArr[foundRecord]->name);
    removed[strlen(removed) - 1] = '\0';
    while (pArr[foundRecord] != NULL)
    {
      pArr[foundRecord] = pArr[foundRecord + 1];
      foundRecord++;
    }
    printf("record [%s] removed successfully.\n", removed);
  }
}


/**
 * A helper method that searches for a record and returns 1 if the record is found,
 * and -1 if the record with the entered name does not exist.
 * */
int findRecord(char toFind[fieldLength], struct db_type *pArray[])
{
  int index = 0;
  while (pArray[index]->name != NULL)
  {

    if (strcmp(pArray[index]->name, toFind) == 0)
    {
      return index;
    }
    index++;
  }
  return -1;
}


/* swap records */
void swap(struct db_type *pArr[])
{
  struct db_type *tempRecord;
  tempRecord = malloc(sizeof(struct db_type));
  int index = 0;
  while (pArr[index] != NULL && pArr[index + 1] != NULL)
  {
    tempRecord = pArr[index];
    pArr[index] = pArr[index + 1];
    pArr[index + 1] = tempRecord;
    index += 2;
  }
}

/* load from course.txt, store into (global) array of courses  */
void init_courseArr(void)
{

  FILE *fr; //file pointer
  fr = fopen(courseFile, "r");
  char arr[50];
  int count = 0;
  while ((fgets(arr, 100, fr)) != NULL)
  {
    strncpy(courseArr[count].code, arr, strlen(arr) - 1); // remove \n
    courseArr[count].code[strlen(arr) - 1] = '\0';        //to be safe

    fgets(arr, 100, fr);
    strncpy(courseArr[count].title, arr, strlen(arr) - 1);
    courseArr[count].title[strlen(arr) - 1] = '\0'; //to be safe

    fgets(arr, 100, fr);
    strncpy(courseArr[count].date, arr, strlen(arr) - 1);
    courseArr[count].date[strlen(arr) - 1] = '\0'; //to be safe

    // other ways, remove \n before
    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].time_start, arr);

    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].time_end, arr);

    fgets(arr, 100, fr);
    arr[strlen(arr) - 1] = '\0';
    strcpy(courseArr[count].location, arr);

    count++;
    fgets(arr, 100, fr); // read the empty line
  }

  strcpy(courseArr[count].code, "EECS0000"); // a terminator token structure

  fclose(fr);
}

/************************ DISK IO *****************************************************************/

void writeDisk(struct db_type *pArr[])
{
  FILE *fp;
  int i;

  if ((fp = fopen(diskFile, "ab")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  for (i = 0; i < SIZE; i++)
  {
    if (pArr[i] != NULL)
    {
      if (fwrite(pArr[i], sizeof(struct db_type), 1, fp) != 1)
      {
        printf("file write error\n");
      }
    }
  }
  fclose(fp);
}

void emptyDisk(void)
{

  FILE *fp;

  char c3[3];
  printf("are you sure to empty disk? (y) or (n)? ");
  fgets(c3, 3, stdin);
  if (strcmp(c3, "y\n"))
    return;

  //else
  if ((fp = fopen(diskFile, "w")) == NULL)
  {
    printf("no file to open\n");
    return;
  }
  fclose(fp); // open close, will empty the file
}

void loadDisk(struct db_type *pArr[])
{
  FILE *fp;
  int i;
  char c3[3];
  printf("will overwrite current records. are you sure to load disk? (y) or (n)? ");
  fgets(c3, 3, stdin);
  if (strcmp(c3, "y\n"))
    return;

  struct db_type *tmp;

  if ((fp = fopen(diskFile, "r")) == NULL)
  {
    printf("cannot open file\n");
    return;
  }

  init_list(pArr);
  for (i = 0; i < SIZE; i++)
  {

    tmp = (struct db_type *)malloc(sizeof(struct db_type));

    if (fread(tmp, sizeof(struct db_type), 1, fp) != 1)
    {
      if (feof(fp))
      {
        fclose(fp);
        return;
      }
      printf("file read error\n");
    }
    else
      pArr[i] = tmp;
  }
}

/****bonus*************************************************************************************/

/* sort the records by ages */
void sort(struct db_type *pArr[])
{
  int n = 0, i, j, smallest;
  struct db_type *tempRecord;
  tempRecord = malloc(sizeof(struct db_type));
  while (pArr[n] != NULL)
  {
    n++;
  }
  for (i = 0; i < n - 1; i++)
  {
    smallest = i;
    for (j = i + 1; j < n; j++)
    {
      if (pArr[j]->age < pArr[smallest]->age)
      {
        smallest = j;
      }
    }
    tempRecord = pArr[i];
    pArr[i] = pArr[smallest];
    pArr[smallest] = tempRecord;
  }
}
