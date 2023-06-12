#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define ENTER 13
#define TAB 9
#define BCKSPC 8

struct User
{
    char password[50];
    char username[50];
};

typedef struct
{
    char section[50];
    char name[50];
    char studNo[50];
} Student;

void mainScreen();
void takePassword(char pwd[50]);
int groupFormation();
void groupStudents(Student students[], int numStudents);
void sortStudentNames();
void sortStudentsByName(Student students[], int numStudents);

int main()
{
    mainScreen();
    return 0;
}

void mainScreen()
{
    char letterChoice;
    struct User user;
    char username[] = "tupadmin";
    char password[] = "Group2TUP";

    printf("\n\t\t\tProfessor's Student Access Module\n\n");
    printf("\t\t\tUsername: ");
    scanf("%s", user.username);
    printf("\t\t\tPassword: ");
    takePassword(user.password);

    if (strcmp(user.username, username) != 0 || strcmp(user.password, password) != 0)
    {
        printf("LOGIN UNSUCCESSFUL\n");
    }
    else
    {
        groupFormation();
    }
}

void takePassword(char pwd[50])
{
    int i = 0;
    char ch;
    while (1)
    {
        ch = getch();
        if (ch == ENTER || ch == TAB)
        {
            pwd[i] = '\0';
            break;
        }
        else if (ch == BCKSPC)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            pwd[i++] = ch;
            printf("* \b");
        }
    }
}

int groupFormation()
{
    FILE *file;
    file = fopen("studentslist.csv", "r");

    if (file == NULL)
    {
        printf("Error Opening File.\n");
        return 1;
    }

    Student students[100];
    int readCount = 0;
    int records = 0;

    do
    {
        readCount = fscanf(file, "%[^,],%[^,],%[^,\n]\n",
                           students[records].section,
                           students[records].name,
                           students[records].studNo);

        if (readCount == 3)
        {
            records++;
        }

        if (readCount != 3 && !feof(file))
        {
            printf("\nFile format incorrect.\n");
            return 1;
        }

        if (ferror(file))
        {
            printf("Error reading file.\n");
            return 1;
        }

    } while (!feof(file));

    fclose(file);

    printf("\n%d records read.\n\n", records);

    for (int i = 0; i < records; i++)
    {
        printf("%s %s %s\n",
               students[i].section,
               students[i].name,
               students[i].studNo);
    }

    sortStudentsByName(students, records);
    groupStudents(students, records);

    printf("\n");
    return 0;
}

void groupStudents(Student students[], int numStudents)
{
    int numMembers;
    int ans;
    printf("Enter the number of members per group: ");
    scanf("%d", &numMembers);

    int numGroups = numStudents / numMembers;
    int remainder = numStudents % numMembers;

    if (remainder != 0)
    {
        numGroups++; // Increase the number of groups if there is a remainder
    }

    // Randomize the student array
    srand(time(NULL)); // Seed the random number generator with current time

    for (int i = numStudents - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Student temp = students[i];
        students[i] = students[j];
        students[j] = temp;
    }

    printf("\nGrouping students...\n");

    int studentIndex = 0;
    for (int i = 0; i < numGroups; i++)
    {
        printf("\nGroup %d:\n", i + 1);
        int membersCount = (i < numGroups - 1 || remainder == 0) ? numMembers : remainder;

        for (int j = 0; j < membersCount; j++)
        {
            printf("%s %s %s\n", students[studentIndex].section, students[studentIndex].name, students[studentIndex].studNo);
            studentIndex++;
        }
    }

    printf("Would you like to create a csv file for the Grouped Formed? Press (1) if YES, otherwise, Press (2): ");
    scanf("%d", &ans);

    if (ans == 1)
    {
        FILE *csvFile;
        csvFile = fopen("grouped_form.csv", "w");

        if (csvFile == NULL)
        {
            printf("Error creating file.\n");
            return;
        }

        // Write the grouped information to the CSV file
        studentIndex = 0;
        for (int i = 0; i < numGroups; i++)
        {
            fprintf(csvFile, "Group %d:\n", i + 1);
            int membersCount = (i < numGroups - 1 || remainder == 0) ? numMembers : remainder;

            for (int j = 0; j < membersCount; j++)
            {
                fprintf(csvFile, "%s, %s, %s\n", students[studentIndex].section, students[studentIndex].name, students[studentIndex].studNo);
                studentIndex++;
            }
        }

        fclose(csvFile);
        printf("CSV file created successfully: grouped_form.csv\n");
    }
    else
    {
        printf("Okay! Type -BACK- if you want to go back to mainscreen\n");
    }
}

void sortStudentsByName(Student students[], int numStudents)
{
    int ans;
    for (int i = 0; i < numStudents - 1; i++)
    {
        for (int j = 0; j < numStudents - i - 1; j++)
        {
            if (strcmp(students[j].name, students[j + 1].name) > 0)
            {
                // Swap the positions of students
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("\nStudents sorted by name:\n");
    for (int i = 0; i < numStudents; i++)
    {
        printf("%s %s %s\n",
               students[i].section,
               students[i].name,
               students[i].studNo);
    }

    printf("Would you like to create a csv file for the sorted student names? Press (1) if YES, otherwise, Press (2): ");
    scanf("%d", &ans);

    if (ans == 1)
    {
        FILE *csvFile;
        csvFile = fopen("sortedStudentNames.csv", "w");

        if (csvFile == NULL)
        {
            printf("Error creating file.\n");
            return;
        }

        // Write the grouped information to the CSV file
        for (int i = 0; i < numStudents; i++)
        {
            fprintf(csvFile, "%s %s %s\n",
                    students[i].section,
                    students[i].name,
                    students[i].studNo);
        }

        fclose(csvFile);
        printf("CSV file created successfully: grouped_form.csv\n");
    }
    else
    {
        printf("Okay! Type -BACK- if you want to go back to mainscreen\n");
    }
}
