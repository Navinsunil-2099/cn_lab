#include <stdio.h>
#include <string.h>

void process(int matrix[][100], int n, char result[])
{
    int upper = 1;
    int lower = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            
            if (i > j && matrix[i][j] != 0)
                upper = 0;

            
            if (i < j && matrix[i][j] != 0)
                lower = 0;
        }
    }

    if (upper && lower)
        strcpy(result, "Diagonal Matrix");
    else if (upper)
        strcpy(result, "Upper Triangular Matrix");
    else if (lower)
        strcpy(result, "Lower Triangular Matrix");
    else
        strcpy(result, "Neither Upper nor Lower Triangular Matrix");
}

int main()
{
    int n;
    int matrix[100][100];
    char line[256];
    char result[100];

   
    printf("Enter N: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &n);

   
    printf("Enter matrix:\n");

    for (int i = 0; i < n; i++)
    {
        fgets(line, sizeof(line), stdin);

        char *ptr = line;

        for (int j = 0; j < n; j++)
        {
            sscanf(ptr, "%d", &matrix[i][j]);

            while (*ptr != ' ' && *ptr != '\0' && *ptr != '\n')
                ptr++;

            while (*ptr == ' ')
                ptr++;
        }
    }

    process(matrix, n, result);

    printf("\nMatrix Type: %s\n", result);

    return 0;
}