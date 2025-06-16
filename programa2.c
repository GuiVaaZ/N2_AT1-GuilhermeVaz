#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_READINGS 10000 
typedef struct {
    long long timestamp;
    char sensor_id[50];
    char value[50];
} Reading;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ID_SENSOR> <timestamp_consulta>\n", argv[0]);
        return 1;
    }

    char *sensor_id = argv[1];
    long long target_ts = atoll(argv[2]);

    char filename[100];
    sprintf(filename, "%s.txt", sensor_id);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo do sensor");
        return 1;
    }

    Reading readings[MAX_READINGS];
    int count = 0;
    while (count < MAX_READINGS && fscanf(file, "%lld %*s %s", &readings[count].timestamp, readings[count].value) == 2) {
        count++;
    }
    fclose(file);

    if (count == 0) {
        printf("Nenhuma leitura encontrada para o sensor %s.\n", sensor_id);
        return 0;
    }

    int low = 0, high = count - 1;
    int closest_idx = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (readings[mid].timestamp == target_ts) {
            closest_idx = mid;
            break;
        }

        if (readings[mid].timestamp < target_ts) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }

    
    if (closest_idx == -1) {
        if (low >= count) {             closest_idx = count - 1;
        } else if (high < 0) {
            closest_idx = 0;
        } else {
            if (llabs(readings[low].timestamp - target_ts) < llabs(readings[high].timestamp - target_ts)) {
                closest_idx = low;
            } else {
                closest_idx = high;
            }
        }
    }

    printf("Leitura mais próxima encontrada:\n");
    printf("  Timestamp: %lld\n", readings[closest_idx].timestamp);
    printf("  Valor: %s\n", readings[closest_idx].value);

    return 0;
}