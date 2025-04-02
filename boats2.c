
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

int sort(NewBoat *BoatList[120], int *size){


    int i = 0;
    int j = 0;
    NewBoat temp;

    for (i = 0; i < *size; i++){

        for (j = 0; j < *size - 1; j++){

            int k = 0;
            int h = 0;

            char str1[120];
            strcpy(str1, BoatList[j]->name);
            while (str1[k]){
                str1[k] = toupper(str1[k]);
                k++;
            }

            char str2[120];
            strcpy(str2, BoatList[j+1]->name);
            while (str2[h]){
                str2[h] = toupper(str2[h]);
                h++;
            }

            if (strcmp(str1, str2) > 0){

                temp = *BoatList[j];
                *BoatList[j] = *BoatList[j+1];
                *BoatList[j+1] = temp;

            }
        }
    }

    return (EXIT_SUCCESS);
}


int addBoat(NewBoat *BoatList[120], NewBoat addition, int *size){
     
    for (int i = 0; i < 120; i++){
    
        if (BoatList[i] == NULL){

            BoatList[i] = (NewBoat *)malloc(sizeof(NewBoat));   
            *BoatList[i] = addition;
            *size = (*size) + 1;
            break;

        }

    }

    sort(BoatList, size);
    return (EXIT_SUCCESS);
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

        if (addition.place == 1){
            addition.tag.SlipInfo = atoi(Word);
        }
        else if (addition.place == 2){
            addition.tag.LandInfo = atoi(Word);
        }
        else if (addition.place == 3){
            strcpy(addition.tag.TrailorInfo, Word);
        }
        else {
            addition.tag.StorageInfo = atoi(Word);
        }

        Word = strtok(NULL, SEPARATORS);

        addition.money = atoi(Word);

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

    if (addition.place == 1){
        addition.tag.SlipInfo = atoi(Word);
    }
    else if (addition.place == 2){
        addition.tag.LandInfo = atoi(Word);
    }
    else if (addition.place == 3){
        strcpy(addition.tag.TrailorInfo, Word);
        }
    else {
        addition.tag.StorageInfo = atoi(Word);
    }

    Word = strtok(NULL, SEPARATORS);

    addition.money = atoi(Word);

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
        printf("%s, %d", BoatList[i]->name, BoatList[i]->length);
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

    addBoatPrompts(BoatList, size); 

    for (int i = 0; i < *size; i++){
        printf("%s, %d\n", BoatList[i]->name, BoatList[i]->length);
    }

    

}

