#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <regex.h>

#define MAX_STR_LENGTH 256

int regexCheck(char *string, char *regexPattern) {
    regex_t regex;
    int regexReturn;

    regexReturn = regcomp(&regex, regexPattern, REG_EXTENDED);

    if (regexReturn) {
        perror("A regex error has occured. Exiting program.");

        exit(1);
    }

    regexReturn = regexec(&regex, string, 0, NULL, 0);

    if (!regexReturn) {
        regfree(&regex);

        return 1;
    } else if (regexReturn == REG_NOMATCH) {
        regfree(&regex);

        return 0;
    } else {
        perror("A regex error has occured. Exiting program.");

        exit(1);
    }
}

int getUserMenuOpiton() {
    char menuOption[MAX_STR_LENGTH];

    do {
        printf("Enter option: ");
        fgets(menuOption, MAX_STR_LENGTH, stdin);
        menuOption[strlen(menuOption) - 1] = 0;

        if (regexCheck(menuOption, "[^0-3]") || !menuOption[0]) {
            printf("Invalid option\n");

            continue;
        }

        return atoi(menuOption);
    } while (1);
}

char* getUserName() { 
    char *userName = malloc(MAX_STR_LENGTH);

    do {
        printf("Enter Name: ");
        fgets(userName, MAX_STR_LENGTH, stdin);
        userName[strlen(userName) - 1] = 0;

        if (regexCheck(userName, "[^a-zA-Z ]") || !userName[0]) {
            printf("Only letters A-Z are allowed\n");

            continue;
        }

        return userName;
    } while (1);
}

char getCustomerAge() {
    char customerAgeStr[MAX_STR_LENGTH];
    int customerAge;

    do {
        printf("Enter Age: ");
        fgets(customerAgeStr, MAX_STR_LENGTH, stdin);
        customerAgeStr[strlen(customerAgeStr) - 1] = 0;

        if (regexCheck(customerAgeStr, "[^0-9]") || !customerAgeStr[0]) {
            printf("Only positive integers are allowed\n");

            continue;
        }

        customerAge = atoi(customerAgeStr);

        if (customerAge < 18) {
            printf("You're too young to be buying a car!\n");
            printf("Press Enter to continue...");
            while (getchar() != '\n');
            
            return -1;
        } else if (customerAge > 130) {
            printf("Age value too high!\n");

            continue;
        }

        return customerAge;
    } while (1);
}

int getUserNum() {
    char userNum[MAX_STR_LENGTH];

    do {
        printf("Enter Your Lucy Number: ");
        fgets(userNum, MAX_STR_LENGTH, stdin);
        userNum[strlen(userNum) - 1] = 0;

        if (regexCheck(userNum, "-?[^0-9]") || !userNum[0]) {
            printf("Only numbers are allowed\n");
        
            continue;
        }

        return atoi(userNum);
    } while (1);
}