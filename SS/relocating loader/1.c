#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char add[6], length[10], input[10], bitmask[12], relocbit;
    int start, len, i, address, opcode, addr, actualadd;
    FILE *fp1, *fp2;

    // Clear the screen (optional; can be removed if not needed)
    system("cls");  // Use "cls" for Windows

    printf("Enter the actual starting address: ");
    scanf("%d", &start);

    fp1 = fopen("relinput.dat", "r");
    if (fp1 == NULL) {
        perror("Error opening relinput.dat");
        return EXIT_FAILURE;
    }
    
    fp2 = fopen("reloutput.dat", "w");
    if (fp2 == NULL) {
        perror("Error opening reloutput.dat");
        fclose(fp1);
        return EXIT_FAILURE;
    }

    fscanf(fp1, "%s", input);
    while (strcmp(input, "E") != 0) {
        if (strcmp(input, "H") == 0) {
            fscanf(fp1, "%s", add);
            fscanf(fp1, "%s", length);
            fscanf(fp1, "%s", input);
            printf("%s %s %s ",add,length,input);

        }

        if (strcmp(input, "T") == 0) {
            fscanf(fp1, "%d", &address);
            fscanf(fp1, "%s", bitmask);
            printf("\n%d %s\n",address,bitmask);//
            address += start;
            len = strlen(bitmask);

            for (i = 0; i < len; i++) {
                fscanf(fp1, "%d", &opcode);
                fscanf(fp1, "%d", &addr);
                relocbit = bitmask[i];

                if (relocbit == '0')
                    actualadd = addr;
                else
                    actualadd = addr + start;

                fprintf(fp2, "%d\t%d\t%d\n", address, opcode, actualadd);
                address += 3;
            }

            fscanf(fp1, "%s", input);
        }
    }

    fclose(fp1);
    fclose(fp2);
    printf("FINISHED\n");
    getchar();  // Consume the newline left by scanf
    getchar();  // Wait for another key press

    return 0;
}

