#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int findInSYMTAB(char findLabel[]) {
    FILE *SYMTAB;
    char label[30], addr[30];
    SYMTAB = fopen("symtab.dat", "r");
    if (SYMTAB == NULL) {
        perror("Error opening symtab.dat");
        return -1; // Return an error value if file can't be opened
    }

    while (fscanf(SYMTAB, "%s %s", label, addr) != EOF) {
        if (strcmp(findLabel, label) == 0) {
            fclose(SYMTAB);
            return atoi(addr);
        }
    }
    fclose(SYMTAB);
    return -1; // Return an error value if label is not found
}

int getMnemonicCode(char mnemonic[]) {
    if (strcmp(mnemonic, "LDA") == 0)
        return 33;
    else if (strcmp(mnemonic, "STA") == 0)
        return 44;
    else if (strcmp(mnemonic, "LDCH") == 0)
        return 53;
    else if (strcmp(mnemonic, "STCH") == 0)
        return 57;
    else
        return -1; // Return an error value for unknown mnemonics
}

int main() {
    FILE *INPUT, *OUTPUT, *SYMTAB, *INTERMEDIATE, *FINAL, *OBJ;
    char mnemonic[10][10] = {"START", "LDA", "STA", "LDCH", "STCH", "END"};
    int LOCCTR, start = 0, j = 0, length, Tlength, count = 0, finalAddress, startAddr;
    char label[20], opcode[20], operand[20], address[20];

    INPUT = fopen("input.dat", "r");
    OUTPUT = fopen("output.dat", "w");
    SYMTAB = fopen("symtab.dat", "w");

    if (!INPUT || !OUTPUT || !SYMTAB) {
        perror("Error opening input/output files");
        return EXIT_FAILURE; // Exit if any file can't be opened
    }

    fscanf(INPUT, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);
        LOCCTR = start;
        fprintf(OUTPUT, "%s %s %s\n", label, opcode, operand);
        fscanf(INPUT, "%s %s %s", label, opcode, operand);
    } else {
        LOCCTR = 0;
    }

    while (strcmp(opcode, "END") != 0) {
        j = 0;
        fprintf(OUTPUT, "%d ", LOCCTR);
        
        if (strcmp(label, "**") != 0) {
            fprintf(SYMTAB, "%s %d\n", label, LOCCTR);
        }

        while (strcmp(mnemonic[j], "END") != 0) {
            if (strcmp(mnemonic[j], opcode) == 0) {
                LOCCTR += 3;
            }
            j++;
        }

        if (strcmp(opcode, "WORD") == 0)
            LOCCTR += 3;
        else if (strcmp(opcode, "RESW") == 0) {
            LOCCTR += (3 * atoi(operand));
            count += (3 * atoi(operand));
        } else if (strcmp(opcode, "RESB") == 0) {
            LOCCTR += atoi(operand);
            count += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {    
            LOCCTR += (strlen(operand) - 3); // Assuming format like C'...' or X'...'
        } else {
            //printf("Unknown opcode: %s\n", opcode);
        }

        fprintf(OUTPUT, "%s %s %s\n", label, opcode, operand);
        fscanf(INPUT, "%s %s %s", label, opcode, operand);
    }

    fprintf(OUTPUT,"%d ", LOCCTR);    
    fprintf(OUTPUT,"%s %s %s\n", label, opcode, operand);
    
    printf("\nTHE LENGTH OF THE PROGRAM IS %d\n", LOCCTR - start); 
    length = LOCCTR - start;
    finalAddress = LOCCTR;
    Tlength = length - count;

    fclose(INPUT);
    fclose(OUTPUT);
    fclose(SYMTAB);

    //=============================================================

    INTERMEDIATE = fopen("output.dat", "r");
    OBJ = fopen("obj.dat", "w");
    FINAL = fopen("final.dat", "w");

    if (!INTERMEDIATE || !OBJ || !FINAL) {
        perror("Error opening intermediate/output files");
        return EXIT_FAILURE; // Exit if any file can't be opened
    }

    fscanf(INTERMEDIATE,"%s %s %s", label, opcode, operand);

    startAddr = atoi(operand);

    if(strcmp(opcode,"START")==0){
        fprintf(FINAL,"%s %s %s\n", label, opcode, operand);
        fprintf(OBJ,"H^%s^00%s^00%d\n", label, operand,length);
        
        fscanf(INTERMEDIATE,"%s %s %s %s", address,label,opcode,operand);
        
        fprintf(OBJ,"T^%06d^%d^", atoi(address), Tlength);
        
        while(strcmp(opcode,"END")!=0){
            if(strcmp(label,"**")==0 ){
                fprintf(OBJ,"%d%d^", getMnemonicCode(opcode), findInSYMTAB(operand));
                fprintf(FINAL,"%s %s %s %d %d\n", address,label,opcode,getMnemonicCode(opcode),findInSYMTAB(operand));
                fscanf(INTERMEDIATE,"%s %s %s %s", address,label,opcode,operand);
            } else if(strcmp(opcode,"BYTE")==0){
                fprintf(FINAL,"%s %s %s%s\n", address,label,opcode,operand);
                for(int i=2; i<(strlen(operand)-1); i++){
                    fprintf(OBJ,"%x", operand[i]);
                    fprintf(FINAL,"%x", operand[i]);    
                }       
                fprintf(FINAL,"\n");    
                fscanf(INTERMEDIATE,"%s %s %s %s", address,label,opcode,operand);                    
            } else if(strcmp(opcode,"WORD")==0){
                fprintf(OBJ,"%06x^", atoi(operand));
                fprintf(FINAL,"%s %s %s %d\n", address,label,opcode,getMnemonicCode(opcode));        
                fscanf(INTERMEDIATE,"%s %s %s %s", address,label,opcode,operand);        
            } else {
                fscanf(INTERMEDIATE,"%s %s %s %s", address,label,opcode,operand);
            }
                
        }
        
        fprintf(FINAL,"%s %s %s\n", address,label ,opcode);    
        
        fclose(INTERMEDIATE);
        fclose(OBJ);
        fclose(FINAL);

        
        return EXIT_SUCCESS; // Successful completion
    } // Closing brace for main function
}

