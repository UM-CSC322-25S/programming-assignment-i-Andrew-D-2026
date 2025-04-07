

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#define MAX_STRING 128 // length for strings
#define SEPARATORS "," // used to tokenize our strings

typedef char String[MAX_STRING]; //typedef for String

typedef enum { // enum to enumerate the place types
    slip,
    land,
    trailor,
    storage,
    no_place
} PlaceType;

typedef union{ // union for the additional info
    int SlipInfo;
    char LandInfo;
    String TrailorInfo;
    int StorageInfo;
} TagInfo;

typedef struct { // our data structure for the boats
    String name;
    int length;
    PlaceType place;
    TagInfo tag; 
    double money;
} NewBoat;
    
//------------------------

PlaceType StringToPlaceType(char * PlaceString) { // method to convert strings to the PlaceType enum

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

//------------------------

char * PlaceToString(PlaceType Place) { // method to convert the PlaceType enum to a String. Used frequently for printing. 

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

//------------------------

int CompareNames(void const *A, void const *B){ // our compare function which is used for qsort


    NewBoat *comp1 = *(NewBoat**)A;
    NewBoat *comp2 = *(NewBoat**)B;

    return strcasecmp(comp1->name, comp2->name); //strcasecmp for case insensitive comparison

}

//------------------------

int addBoat(NewBoat *BoatList[120], NewBoat addition, int *size){ // method to add a pointer to a boat to the array 
     
    for (int i = 0; i < 120; i++){
    
        if (BoatList[i] == NULL){ // find the first empty spot to keep the array packed

            if ((BoatList[i] = (NewBoat *)malloc(sizeof(NewBoat)))  == NULL){
                perror("Cannot malloc"); // allocate and check the return value of memory
                return (EXIT_FAILURE);
            }   
            *BoatList[i] = addition; // set the pointer to point at the added boat
            *size = (*size) + 1; // increase the size of the array to keep track of how many boats
            break;

        }

    }

    qsort(BoatList, *size, sizeof(NewBoat *), CompareNames); // use qsort to sort the boats
    return (EXIT_SUCCESS);
}

//------------------------

int NewMonth (NewBoat *BoatList[120], int *size){ // update the money owed

// calculate the monthly amount using placetype specific calculations

    for (int i = 0; i < *size; i++){

        if (BoatList[i]->place == 0){ // slip
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 12.5);
        }
        else if (BoatList[i]->place == 1){ // land
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 14.0);
        }
        else if (BoatList[i]->place == 2){ // trailor
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 25.0);
        }
        else { // storage
            BoatList[i]->money = BoatList[i]->money + (BoatList[i]->length * 11.2);
        }
    }

    return (EXIT_SUCCESS);

}

//------------------------

int RemoveBoat(NewBoat *BoatList[120], int *size){ // method to remove boat

    String name;

    printf("Please enter the boat name: ");

    fflush(stdin); // flush the standard input to clear it out

    if (fgets(name,128,stdin) != NULL){ //using fgets in case boat name is more than one word
        name[strlen(name)-1] = '\0'; // set last character to termination character
    }
    else {
        printf("Error reading the input name"); // if name is null
    }

    for (int i = 0; i < *size; i++){ // iterate through the boat list
        
        if (strcasecmp(BoatList[i]->name,name) == 0){

            BoatList[i] = NULL; // remove the boat values

            for (int k = i; k < (*size) -1; k++){ // shift every following boat up

            BoatList[k] = BoatList[k+1];
            BoatList[k+1] = NULL;
           
            }

            *size = (*size) - 1; // remove one to account for the boat removal

            return (EXIT_SUCCESS);

        }   

    }

    printf("No boat with the name %s\n", name); // if name input does not match
    return (EXIT_FAILURE);
}

//------------------------

int MakePayment(NewBoat *BoatList[120], int*size){ // to pay the money owed

    String name;

    printf("Please enter the boat name: ");
    
    fflush(stdin); // clear out standard input

    if (fgets(name,128,stdin) != NULL){ // check input value
        name[strlen(name)-1] = '\0'; // insert termination character
    }
    else {
        printf("Error reading the input name"); //if name is null
    }

    double paid; // how much user wants to pay

    for (int i = 0; i < *size; i++){

        if (strcasecmp(BoatList[i]->name,name) == 0){

            printf("Please enter the amount to be paid: ");
            scanf("%lf", &paid);

            if (paid > BoatList[i]->money){ // check if pay amount exceeds amount owed
                printf("That is more than the amount owed, %.2lf\n", BoatList[i]->money);
                return (EXIT_FAILURE);
            }

            else {
                BoatList[i]->money = BoatList[i]->money - paid; //update money owed
                return (EXIT_SUCCESS);
            }
        }
    }

    printf("No boat with the name %s\n",name); // if we can't locate boat name
    return (EXIT_FAILURE);

}

//------------------------

int tokenizeFile(FILE *InputFile, NewBoat *BoatList[120], int *size){ // function to tokenize the file and add boat

    String S; // string we are tokenizing
    char *Word; // individual token
    NewBoat addition; // boat we want to add

// using strtok to tokenize the string

    while(fgets(S,MAX_STRING,InputFile) != NULL){
        Word = strtok(S,SEPARATORS); // separators = ","

        strcpy(addition.name, Word);

        Word = strtok(NULL, SEPARATORS);

        addition.length = atoi(Word); // atoi to convert string to int

        Word = strtok(NULL, SEPARATORS);

        addition.place = StringToPlaceType(Word); // to convert string to placetype

        Word = strtok(NULL, SEPARATORS);

        if (addition.place == 0){
            addition.tag.SlipInfo = atoi(Word);
        }
        else if (addition.place == 1){
            addition.tag.LandInfo = Word[0]; // to get the first character, char
        }
        else if (addition.place == 2){
            strcpy(addition.tag.TrailorInfo, Word);
        }
        else {
            addition.tag.StorageInfo = atoi(Word);
        }

        Word = strtok(NULL, SEPARATORS);

        addition.money = atof(Word); // atof to convert string to double

        addBoat(BoatList, addition, size); //once tokenized and defined, add the boat
 
    }

    return (EXIT_SUCCESS);
}

//------------------------

int tokenizeAdd(String S, NewBoat *BoatList[120], int *size){ // method for tokenizing a single boat from the user
    
    // same technique as above, in this case for single boat from user input

    char *Word;
    NewBoat addition;

    Word = strtok(S,SEPARATORS);

    strcpy(addition.name, Word);

    Word = strtok(NULL, SEPARATORS);

    addition.length = atoi(Word); // string -> int

    Word = strtok(NULL, SEPARATORS);

    addition.place = StringToPlaceType(Word); // string -> placetype

    Word = strtok(NULL, SEPARATORS);

    if (addition.place == 0){
        addition.tag.SlipInfo = atoi(Word);
    }
    else if (addition.place == 1){
        addition.tag.LandInfo = Word[0]; // string -> char
    }
    else if (addition.place == 2){
        strcpy(addition.tag.TrailorInfo, Word);
        }
    else {
        addition.tag.StorageInfo = atoi(Word);
    }

    Word = strtok(NULL, SEPARATORS);

    addition.money = atof(Word); // string -> double

    addBoat(BoatList, addition, size); // add the boat

    return (EXIT_SUCCESS);

} 

//------------------------

int addBoatPrompts(NewBoat *BoatList[120], int *size){ // simple prompts for the user


    char S[300];
    printf("Please enter the boat data in CSV format                    :");
    
    fflush(stdin); // clear out standard input

    if (fgets(S,300,stdin) != NULL){ // check input value
        S[strlen(S)-1] = '\0'; // insert termination character
    }
    else {
        printf("Error reading the input"); //if name is null
    }

    tokenizeAdd(S, BoatList, size);
    return (EXIT_SUCCESS);

}

//------------------------

int printInventory(NewBoat *BoatList[120], int *size){ // printing out the values for each boat

    for (int i = 0; i < *size; i++){
        if (BoatList[i]->place == 0){ 
            printf("%-22s %d' %-8s #%-8d Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.SlipInfo, BoatList[i]->money); }
        else if (BoatList[i]->place == 1){
            printf("%-22s %d' %-8s %-8c  Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.LandInfo, BoatList[i]->money);
        }
        else if (BoatList[i]->place == 2){
            printf("%-22s %d' %-8s %-8s  Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.TrailorInfo, BoatList[i]->money);
        }
        else {
            printf("%-22s %d' %-8s #%-8d Owes $%-4.2lf\n", BoatList[i]->name, BoatList[i]->length, PlaceToString(BoatList[i]->place), BoatList[i]->tag.StorageInfo, BoatList[i]->money);
        }
    }

    return (EXIT_SUCCESS);

}

//------------------------

int OutputFile(FILE *OutputFile, NewBoat *BoatList[120], int *size){ // method for outputting data to original file

    char output[300]; // 300 as the total length of each line in the csv
    char temp[128]; // temp for each individual piece of information in each line

    for (int i = 0; i < *size; i++){ // iterate through the boats

        strcpy(output, BoatList[i]->name);

        sprintf(temp, "%d", BoatList[i]->length); // using sprintf to convert data types to string
        strcat(output, ",");
        strcat(output, temp);
        strcat(output, ",");

        strcat(output, PlaceToString(BoatList[i]->place));
        strcat(output, ",");

        // specific cases for each placetype
    
        if (BoatList[i]->place == 0){
            sprintf(temp, "%d", BoatList[i]->tag.SlipInfo);
            strcat(output, temp);
        }
        else if (BoatList[i]->place == 1){
            sprintf(temp, "%c", BoatList[i]->tag.LandInfo);
            strcat(output, temp);
        }
        else if (BoatList[i]->place == 2){
            strcat(output, BoatList[i]->tag.TrailorInfo);
        }
        else {
            sprintf(temp, "%d", BoatList[i]->tag.StorageInfo);
            strcat(output, temp);
        }

        strcat(output,",");

        sprintf(temp, "%.2lf", BoatList[i]->money);
        strcat(output, temp);
        
        strcat(output, "\n");

        fputs(output, OutputFile); // fputs to put each line into the output/input file
    }
    
    return (EXIT_SUCCESS);

}

//------------------------

int main(int argc, char* argv[]){

    FILE *InputFile; // declaring our Input/Output file
    int sizeval = 0;
    int *size = &sizeval; // declaring and initializing our array size tracker


    NewBoat *BoatList[120] = {NULL}; // setting each boat pointer to null


    if (argc != 2){ // checking the amount of arguments provided
        printf("ERROR: Incorrect number of arguments given\n");
        return(EXIT_FAILURE);
    }

    if ((InputFile = fopen(argv[1],"r")) != NULL) { // checking that the file can be read
        tokenizeFile(InputFile, BoatList, size);
    }   
    else {
        printf("Error opening file\n");
        }

    printf("Welcome to the Boat Management System"); // welcome statement
    printf("\n-------------------------------------\n");

    char choice = '\0'; // user choice for menu

    do {

        choice = '\0';
        fflush(stdin); // clearing out standard input

        printf("\n(I)nventory, (A)dd, (R)emove, (P)ayment, (M)onth, e(X)it: ");
        scanf("%c",&choice);

        // going through each input case, case insensitive

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

    } while (choice != 'X' && choice != 'x'); // end if user inputs X or x

    if ((InputFile = fopen(argv[1], "w")) != NULL){ // check that file can be written to
        OutputFile(InputFile, BoatList, size);
    }
    else {
        printf("Error writing to Output File");
    }

    for (int i = 0; i < *size; i++){ // free the memory
        free(BoatList[i]);
    }

    printf("Exiting the Boat Management System\n"); // exit statement
}

