#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *input_fp;
    char input_file[100];
    FILE *output_fp;
    char output_file[100];

    printf("Name of the input file: ");
    gets(input_file);
    printf("Name of the output file: ");
    gets(output_file);

    if ((input_fp = fopen(input_file, "r")) == NULL)
    {
        printf("Can not find %s\n", input_file);
        exit(0);
    }

    if ((output_fp = fopen(output_file, "r")) != NULL)
    {
        printf("File %s already exists\n", output_file);
        exit(0);
    }
    else
    {
        output_fp = fopen(output_file, "w");
    }

    char c;
    while ((c = fgetc(input_fp)) != EOF)
    {
        fputc(c, output_fp);
    }

    fclose(input_fp);
    fclose(output_fp);
    printf("System call completed!\n");

    return 0;
}
