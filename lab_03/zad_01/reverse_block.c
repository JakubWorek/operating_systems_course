#include <stdio.h>

# define BUFFER_SIZE 1024

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
    char buffer[BUFFER_SIZE];

    while(size > 0) {
        long read_size = size > BUFFER_SIZE ? BUFFER_SIZE : size;
        fseek(input, -read_size, SEEK_CUR);

        size_t read = fread(buffer, sizeof(char), read_size, input);

        // Reverse buffer
        char c;
        for (size_t i = 0; i < read / 2; i++) {
            c = buffer[i];
            buffer[i] = buffer[read - i - 1];
            buffer[read - i - 1] = c;
        }

        fwrite(buffer, sizeof(char), read, output);
        fseek(input, -BUFFER_SIZE, SEEK_CUR);
        size -= read;
    }

    fclose(input);
    fclose(output);

    return 0;
}