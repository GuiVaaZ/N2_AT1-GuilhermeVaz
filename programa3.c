#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int parse_datetime(const char *datetime_str, struct tm *t) {
    if (sscanf(datetime_str, "%d/%d/%d %d:%d:%d",
               &t->tm_mday, &t->tm_mon, &t->tm_year,
               &t->tm_hour, &t->tm_min, &t->tm_sec) != 6) {
        return 0; 
    }
    t->tm_year -= 1900;
    t->tm_mon -= 1;
    t->tm_isdst = -1;
    return 1; 
}

void generate_random_string(char *s, const int len) {
    static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
}

int main(int argc, char *argv[]) {
    if (argc < 6 || (argc - 4) % 2 != 0) {
        fprintf(stderr, "Uso: %s <arquivo> \"data_inicio\" \"data_fim\" <sensor1> <tipo1> [<sensor2> <tipo2>...]\n", argv[0]);
        fprintf(stderr, "Tipos válidos: CONJ_Z, CONJ_Q, TEXTO, BINARIO\n");
        fprintf(stderr, "Exemplo de data: \"10/06/2025 08:00:00\"\n");
        return 1;
    }

    char *output_filename = argv[1];
    struct tm start_tm = {0}, end_tm = {0};

    if (!parse_datetime(argv[2], &start_tm) || !parse_datetime(argv[3], &end_tm)) {
        fprintf(stderr, "Formato de data/hora inválido. Use \"DD/MM/AAAA HH:MM:SS\".\n");
        return 1;
    }

    time_t start_ts = mktime(&start_tm);
    time_t end_ts = mktime(&end_tm);

    if (start_ts == -1 || end_ts == -1 || start_ts > end_ts) {
        fprintf(stderr, "Intervalo de datas inválido.\n");
        return 1;
    }

    FILE *outfile = fopen(output_filename, "w");
    if (!outfile) {
        perror("Erro ao criar o arquivo de saída");
        return 1;
    }

    srand(time(NULL));

    int num_sensors = (argc - 4) / 2;
    printf("Gerando dados para %d sensor(es).\n", num_sensors);

    for (int i = 0; i < num_sensors; i++) {
        char *sensor_id = argv[4 + i * 2];
        char *sensor_type = argv[5 + i * 2];

        printf("Sensor: %s, Tipo: %s\n", sensor_id, sensor_type);

        for (int j = 0; j < 2000; j++) {
           
            time_t random_ts = start_ts + rand() % (end_ts - start_ts + 1);

            fprintf(outfile, "%lld %s ", (long long)random_ts, sensor_id);

            if (strcmp(sensor_type, "CONJ_Z") == 0) {
                fprintf(outfile, "%d\n", rand() % 1000);
            } else if (strcmp(sensor_type, "CONJ_Q") == 0) {
                fprintf(outfile, "%.4f\n", (float)rand() / (float)(RAND_MAX / 100.0));
            } else if (strcmp(sensor_type, "TEXTO") == 0) {
                char random_str[17];
                generate_random_string(random_str, 16);
                fprintf(outfile, "%s\n", random_str); 
            } else if (strcmp(sensor_type, "BINARIO") == 0) {
                fprintf(outfile, "%s\n", (rand() % 2 == 0) ? "false" : "true"); 
            } else {
                fprintf(stderr, "Aviso: Tipo '%s' para sensor '%s' é inválido. Usando 0 como padrão.\n", sensor_type, sensor_id);
                fprintf(outfile, "0\n");
            }
        }
    }

    fclose(outfile);
    printf("Arquivo de teste '%s' gerado com sucesso!\n", output_filename);

    return 0;
}