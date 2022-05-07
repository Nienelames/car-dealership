#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "userInput.c"

#define MAX_STR_LENGTH 256

void getCarData();
void printCars();
void printSales();
void buyCars();
void sortCarsBySalesDesc();
void sortCarsByStockDesc();

struct CarArray {
    struct Car {
        double price;
        double profit;
        unsigned int stock;
        unsigned int sales;
        char modelName[MAX_STR_LENGTH];
    }*pCars;
    size_t size;
}carArray;

int main() {
    getCarData();

    do {
        printf("+-----------------------------+\n");
        printf("|  Welcome to the Dealership! |\n");
        printf("+-----------------------------+\n");
        printf("|        0 - Quit             |\n");
        printf("|        1 - View Cars        |\n");
        printf("|        2 - Buy Cars         |\n");
        printf("|        3 - View Sales       |\n");
        printf("+-----------------------------+\n");

        switch (getUserMenuOpiton()) {
            case 0:
                printf("Thank you and goodbye!");
                return 0;
            case 1:
                printCars();
                break;
            case 2:
                buyCars();
                break;
            case 3:
                printSales();
                break;
        }
    } while (1);

    return 0;
}

void printCars() {
    printf("+-----------------------------+\n");
    printf("|        Available Cars       |\n");
    printf("+-----------------------------+\n");

    sortCarsByStockDesc();

    for (unsigned int i = 0, j = 0; i < carArray.size; i++) {
        if (carArray.pCars[i].stock != 0) {
            printf("%d -\tModel: %s\n", j, carArray.pCars[i].modelName);
            printf("\tPrice: %0.2lf £\n", carArray.pCars[i].price);
            printf("\tStock: %d\n", carArray.pCars[i].stock);
            printf("+-----------------------------+\n");
            j++;
        }
    }

    printf("Press Enter to continue...");
    while (getchar() != '\n');
}

void sortCarsByStockDesc() {
    for (unsigned int i = 0; i < carArray.size; i++) {
        for (unsigned int j = i + 1; j < carArray.size; j++) {
            if (carArray.pCars[i].stock < carArray.pCars[j].stock) {
                struct Car temp = *(carArray.pCars + i);
                *(carArray.pCars + i) = *(carArray.pCars + j);
                *(carArray.pCars + j) = temp;
            }
        }
    }
}

void printSales() {
    printf("+-----------------------------+\n");
    printf("|          Car Sales          |\n");
    printf("+-----------------------------+\n");  

    sortCarsBySalesDesc();

    for (unsigned int i = 0; i < carArray.size; i++) {
        printf("%d -\tModel: %s \n", i, carArray.pCars[i].modelName);
        printf("\tProfit: %0.2lf £ \n", carArray.pCars[i].profit);
        printf("\tSales: %d\n", carArray.pCars[i].sales);
        printf("+-----------------------------+\n");
    }  

    printf("Press Enter to continue...");
    while (getchar() != '\n');
}

void sortCarsBySalesDesc() {
    for (unsigned int i = 0; i < carArray.size; i++) {
        for (unsigned int j = i + 1; j < carArray.size; j++) {
            if (carArray.pCars[i].sales < carArray.pCars[j].sales) {
                struct Car temp = *(carArray.pCars + i);
                *(carArray.pCars + i) = *(carArray.pCars + j);
                *(carArray.pCars + j) = temp;
            }
        }
    }
}

void buyCars() {
    char userInput[MAX_STR_LENGTH];

    printCars();

    do {
        printf("Enter car index or \"q\" to quit: ");
        fgets(userInput, MAX_STR_LENGTH, stdin);
        userInput[strlen(userInput) - 1] = 0;

        if (!strcmp(userInput, "q"))
            return;

        for (unsigned int i = 0; i < carArray.size; i++) {
            if (atoi(userInput) == i && carArray.pCars[i].stock != 0) {
                char *customerName = getUserName();
                unsigned int customerAge = getCustomerAge();

                if (customerAge == -1)
                    return;

                //Randomising discount with user input
                int luckyNum = getUserNum();
                srand(time(NULL) * luckyNum / time(0));
                unsigned int discount = rand() % 50;
                double profit = (carArray.pCars[i].price * (100 - discount)) / 100;
                carArray.pCars[i].profit += profit;
 
                printf("You got a discount of %d%%!\n", discount);
                printf("Car sold for %0.2lf £\n", profit);

                carArray.pCars[i].sales++;
                carArray.pCars[i].stock--;

                FILE *pfSales = fopen("sales.txt", "a");
                FILE *pfCars = fopen("cars.txt", "w");

                if (pfCars == NULL || pfSales == NULL) {
                    perror("Error opening files. Exiting program.");
                    fclose(pfCars);
                    fclose(pfSales);

                    exit(-1);
                }
                
                //Updating sales history
                fprintf(pfSales, "Name: %s", customerName);
                fprintf(pfSales, "Age: %d\n", customerAge);
                fprintf(pfSales, "Model: %s\n", carArray.pCars[i].modelName);
                fprintf(pfSales, "Discount: %d%%\n", discount);
                fprintf(pfSales, "Profit: %0.2lf£\n", profit);
                fprintf(pfSales, "-----------------------------\n");
                fclose(pfSales);

                sortCarsByStockDesc();

                //Overwriting file with sorted cars
                fprintf(pfCars, "%zu\n", carArray.size);
                fputs("---------------------\n", pfCars);

                for (unsigned int k = 0; k < carArray.size; k++) {
                    fprintf(pfCars, "Model: %s\n", carArray.pCars[k].modelName);
                    fprintf(pfCars, "Price: %0.2lf £\n", carArray.pCars[k].price);
                    fprintf(pfCars, "Profit: %0.2lf £\n", carArray.pCars[k].profit);
                    fprintf(pfCars, "Sales: %d\n", carArray.pCars[k].sales);
                    fprintf(pfCars, "Stock: %d\n", carArray.pCars[k].stock);
                    fputs("---------------------\n", pfCars);
                }

                fclose(pfCars);

                printf("Press Enter to continue...");
                while (getchar() != '\n');

                return;
            }
        }  

        printf("Invalid index.\n");
    } while (1);
}

void getCarData() {
    FILE *pfCars = fopen("cars.txt", "r");
    char line[MAX_STR_LENGTH];

    if (pfCars == NULL) {
        perror("Error opening file. Exiting program.");
        fclose(pfCars);

        exit(-1);
    }

    carArray.size = atoi(fgets(line, MAX_STR_LENGTH, pfCars));
    carArray.pCars = malloc(carArray.size * 64 * sizeof(carArray.pCars));
    fgets(line, MAX_STR_LENGTH, pfCars);

    if (carArray.pCars == NULL) {
        perror("A memory error has occured. Exiting program.");

        exit(-1);
    }

    for (unsigned int i = 0; !feof(pfCars); i++) {
        fscanf(pfCars, "%s", line);
        fgets(line, MAX_STR_LENGTH, pfCars);
        line[strcspn(line, "\r\n")] = 0;
        strcpy(carArray.pCars[i].modelName, line + 1);

        fscanf(pfCars, "%s", line);
        fscanf(pfCars, "%s", line);
        carArray.pCars[i].price = atof(line);
        fscanf(pfCars, "%s", line);

        fscanf(pfCars, "%s", line);
        fscanf(pfCars, "%s", line);
        carArray.pCars[i].profit = atof(line);
        fscanf(pfCars, "%s", line);

        fscanf(pfCars, "%s", line);
        fscanf(pfCars, "%s", line);
        carArray.pCars[i].sales = atoi(line);

        fscanf(pfCars, "%s", line);
        fscanf(pfCars, "%s", line);
        carArray.pCars[i].stock = atoi(line);

        fgets(line, MAX_STR_LENGTH, pfCars);
        fgets(line, MAX_STR_LENGTH, pfCars);
    }

    fclose(pfCars);
}