#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENSORS 50
#define MAX_READINGS 10000
#define SENSOR_ID_LEN 50
#define MAX_LINE_LEN 256

typedef struct {
    long long timestamp;
    char sensor_id[SENSOR_ID_LEN];
    char value[SENSOR_ID_LEN];
} Reading;

typedef struct {
    char id[SENSOR_ID_LEN];
    Reading readings[MAX_READINGS];
    int count;
} SensorData;

int compareReadingsDesc(const void *a, const void *b) {
    Reading *readingA = (Reading *)a;
    Reading *readingB = (Reading *)b;
    if (readingB->timestamp > readingA->timestamp) return 1;
    if (readingB->timestamp < readingA->timestamp) return -1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    SensorData sensors[MAX_SENSORS];
    int sensorCount = 0;
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), inputFile)) {
        long long ts;
        char id[SENSOR_ID_LEN], val[SENSOR_ID_LEN];

        if (sscanf(line, "%lld %s %s", &ts, id, val) != 3) {
            fprintf(stderr, "Aviso: Linha mal formatada ignorada: %s", line);
            continue;
        }

        int sensorIndex = -1;
        for (int i = 0; i < sensorCount; i++) {
            if (strcmp(sensors[i].id, id) == 0) {
                sensorIndex = i;
                break;
            }
        }

        if (sensorIndex == -1) {
            if (sensorCount >= MAX_SENSORS) {
                fprintf(stderr, "Erro: Limite de sensores atingido.\n");
                continue;
            }
            sensorIndex = sensorCount;
            strcpy(sensors[sensorIndex].id, id);
            sensors[sensorIndex].count = 0;
            sensorCount++;
        }

        if (sensors[sensorIndex].count < MAX_READINGS) {
            sensors[sensorIndex].readings[sensors[sensorIndex].count].timestamp = ts;
            strcpy(sensors[sensorIndex].readings[sensors[sensorIndex].count].sensor_id, id);
            strcpy(sensors[sensorIndex].readings[sensors[sensorIndex].count].value, val);
            sensors[sensorIndex].count++;
        } else {
            fprintf(stderr, "Aviso: Limite de leituras para o sensor %s atingido.\n", id);
        }
    }
    fclose(inputFile);

    for (int i = 0; i < sensorCount; i++) {
        qsort(sensors[i].readings, sensors[i].count, sizeof(Reading), compareReadingsDesc);

        char filename[100];
        sprintf(filename, "%s.txt", sensors[i].id);
        FILE *outputFile = fopen(filename, "w");
        if (!outputFile) {
            perror("Erro ao criar arquivo de saída");
            continue;
        }

        for (int j = 0; j < sensors[i].count; j++) {
            fprintf(outputFile, "%lld %s %s\n",
                    sensors[i].readings[j].timestamp,
                    sensors[i].readings[j].sensor_id,
                    sensors[i].readings[j].value);
        }
        fclose(outputFile);
        printf("Arquivo gerado para o sensor %s: %s\n", sensors[i].id, filename);
    }

    printf("Processamento concluído.\n");
    return 0;
}