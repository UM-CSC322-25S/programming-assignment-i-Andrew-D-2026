

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#define MAX_STRING 128
#define SEPARATORS ","

typedef char String[MAX_STRING];

typedef enum {
    slip,
    land,
    trailor,
    storage,
    no_place
} PlaceType;

typedef union{
    int SlipInfo;
    char LandInfo;
    String TrailorInfo;
    int StorageInfo;
} TagInfo;

typedef struct {
    String name;
    int length;
    PlaceType place;
    TagInfo tag; 
    double money;
} NewBoat;
    

PlaceType StringToPlaceType(char * PlaceString) {

    if (!strcasecmp(PlaceString,"slip")) {
        return(slip);
    }
    if (!strcasecmp(PlaceString,"land")) {
        return(land);
    }
    if (!strcasecmp(PlaceString,"trailor")) {
        return(trailor);
    }
    if (!strcasecmp(PlaceString,"storage")) {
        return(storage);
    }
    return(no_place);
}

char * PlaceToString(PlaceType Place) {

    switch (Place) {
        case slip:
            return("slip");
        case land:
            return("land");
        case trailor:
            return("trailor");
        case storage:
            return("storage");
        case no_place:
            return("no_place");
        default:
            printf("How the faaark did I get here?\n");
            exit(EXIT_FAILURE);
            break;
    }
}

int CompareNames(void const *A, void const *B){


    NewBoat *comp1 = *(NewBoat**)A;
    NewBoat *comp2 = *(NewBoat**)B;

    return strcasecmp(comp1->name, comp2->name);

}


int addBoat(NewBoat *BoatList[120], NewBoat addition, int *size){
     
    for (int i = 0; i < 120; i++){
    
        if (BoatList[i] == NULL){

            if ((BoatList[i] = (NewBoat *)malloc(sizeof(NewBoat)))  == NULL){
                perror("Cannot malloc");
            }   
            *BoatList[i] = addition;
            *size = (*size) + 1;
            break;

        }

    }

    qsort(BoatList, *size, sizeof(NewBoat *), CompareNames);
    return (EXIT_SUCCESS);
}


int NewMonth (NewBoat *BoatList[120], int *size){

    for (int i = 0; i < *size; i++){

        if (BoatList[i]->place == 0){
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 12.5);
        }
        else if (BoatList[i]->place == 1){
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 14.0);
        }
        else if (BoatList[i]->place == 2){
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 25.0);
        }
        else {
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 11.2);
        }
    }

    return (EXIT_SUCCESS);

}


int RemoveBoat(NewBoat *BoatList[120], int *size){

    String name;

    printf("Please enter the boat name: ");

    fflush(stdin);

    if (fgets(name,128,stdin) != NULL){
        name[strlen(name)-1] = '\0';
    }
    else {
        printf("Error reading the input name");
    }

    for (int i = 0; i < *size; i++){
        
        if (strcasecmp(BoatList[i]->name,name) == 0){

            BoatList[i] = NULL;       

            for (int k = i; k < (*size) -1; k++){

            BoatList[k] = BoatList[k+1];
            BoatList[k+1] = NULL;
           
            }

            *size = (*size) - 1;

            return (EXIT_SUCCESS);

        }   

    }

    printf("No boat with the name %s\n", name);
    return (EXIT_FAILURE);
}

int MakePayment(NewBoat *BoatList[120], int*size){

    String name;

    printf("Please enter the boat name: ");
    
    fflush(stdin);

    if (fgets(name,128,stdin) != NULL){
        name[strlen(name)-1] = '\0';
    }
    else {
        printf("Error reading the input name");
    }

    double paid;

    for (int i = 0; i < *size; i++){

        if (strcasecmp(BoatList[i]->name,name) == 0){

            printf("Please enter the amount to be paid: ");
            scanf("%lf", &paid);

            if (paid > BoatList[i]->money){
                printf("That is more than the amount owed, %.2lf\n", BoatList[i]->money);
                return (EXIT_FAILURE);
            }

            else {
                BoatList[i]->money = BoatList[i]->money - paid;
                return (EXIT_SUCCESS);
            }
        }
    }

    printf("No boat with the name %s\n",name);
    return (EXIT_FAILURE);

}


int tokenizeFile(FILE *InputFile, NewBoat *BoatList[120], int *size){

    String S;
    char *Word;
    NewBoat addition;

    while(fgets(S,MAX_STRING,InputFile) != NULL){
        Word = strtok(S,SEPARATORS);

        strcpy(addition.name, Word);

        Word = strtok(NULL, SEPARATORS);

        addition.length = atoi(Word);

        Word = strtok(NULL, SEPARATORS);

        addition.place = StringToPlaceType(Word);

        Word = strtok(NULL, SEPARATORS);

        if (addition.place == 0){
            addition.tag.SlipInfo = atoi(Word);
        }
        else if (addition.place == 1){
            addition.tag.LandInfo = Word[0];
        }
        else if (addition.place == 2){
            strcpy(addition.tag.TrailorInfo, Word);
        }
        else {
            addition.tag.StorageInfo = atoi(Word);
        }

        Word = strtok(NULL, SEPARATORS);

        addition.money = atof(Word);

        addBoat(BoatList, addition, size);

    }

    return (EXIT_SUCCESS);
}

int tokenizeAdd(String S, NewBoat *BoatList[120], int *size){
    char *Word;
    NewBoat addition;

    Word = strtok(S,SEPARATORS);

    strcpy(addition.name, Word);

    Word = strtok(NULL, SEPARATORS);

    addition.length = atoi(Word);

    Word = strtok(NULL, SEPARATORS);

    addition.place = StringToPlaceType(Word);

    Word = strtok(NULL, SEPARATORS);

    if (addition.place == 0){
        addition.tag.SlipInfo = atoi(Word);
    }
    else if (addition.place == 1){
        addition.tag.LandInfo = Word[0];
    }
    else if (addition.place == 2){
        strcpy(addition.tag.TrailorInfo, Word);
        }
    else {
        addition.tag.StorageInfo = atoi(Word);
    }

    Word = strtok(NULL, SEPARATORS);

    addition.money = atof(Word);

    addBoat(BoatList, addition, size);

    return (EXIT_SUCCESS);

} 


int addBoatPrompts(NewBoat *BoatList[120], int *size){


    String S;
    printf("Please enter the boat data in CSV format                    :");
    scanf("%s", S);
    tokenizeAdd(S, BoatList, size);
    return (EXIT_SUCCESS);

}

int printInventory(NewBoat *BoatList[120], int *size){

    for (int i = 0; i < *size; i++){
        if (BoatList[i]->place == 0){ 
            printf("%-20s %d' %-8s #%-8d Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.SlipInfo, BoatList[i]->money); }
        else if (BoatList[i]->place == 1){
            printf("%-20s %d' %-8s %-8c  Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.LandInfo, BoatList[i]->money);
        }
        else if (BoatList[i]->place == 2){
            printf("%-20s %d' %-8s %-8s  Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.TrailorInfo, BoatList[i]->money);
        }
        else {
            printf("%-20s %d' %-8s #%-8d Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.StorageInfo, BoatList[i]->money);
        }
    }

    return (EXIT_SUCCESS);

}
    


int main(int argc, char* argv[]){

    FILE *InputFile;
    //FILE *OutputFile;
    int sizeval = 0;
    int *size = &sizeval;

    
    NewBoat *BoatList[120] = {NULL};

    if (argc != 2){
        printf("ERROR: Incorrect number of arguments given\n");
        return(EXIT_FAILURE);
    }

    if ((InputFile = fopen(argv[1],"r")) != NULL) {       
        tokenizeFile(InputFile, BoatList, size);
    }   

    printf("Welcome to the Boat Management System");
    printf("\n-------------------------------------\n");

    char choice = '\0';

    do {


        choice = '\0';
        fflush(stdin);

        printf("\n(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it: ");
        scanf("%c",&choice);

        if (choice == 'A' || choice == 'a'){

            addBoatPrompts(BoatList, size);

        }

        else if (choice == 'I' || choice == 'i'){

            printInventory(BoatList, size);

        }

        else if (choice == 'R' || choice == 'r'){

            RemoveBoat(BoatList, size);

        }

        else if (choice == 'P' || choice == 'p'){
 
            MakePayment(BoatList, size);

        }

        else if (choice == 'M' || choice == 'm'){

            NewMonth(BoatList, size);

        }

        else if (choice != 'X' && choice != 'x'){

            printf("Invalid option %c\n", choice);

        }
    } while (choice != 'X' && choice != 'x');


}
