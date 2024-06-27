#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (input == NULL || output == NULL) {
        perror("Error while opening file");
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long size = ftell(input);
    fseek(input, -1, SEEK_CUR);

    for (long i = size - 1; i >= 0; i--) {
        fseek(input, i, SEEK_SET);
        char c = fgetc(input);
        fputc(c, output);
    }

    fclose(input);
    fclose(output);

    return 0;   
}