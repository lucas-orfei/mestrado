/*Reads my CORSIKA part files*/
// Compile: gcc -o read_part read_part.cc 

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <float.h>

// Declaração dos arquivos de entrada e saída
FILE *inputfile;
FILE *outputfile;        
FILE *outputfile2;       

// Constantes úteis
float pi = 3.141592653589;
float rad2deg = 180 / pi;

// Strings para leitura e escrita de arquivos
char line[500];
char ch;
char str_file_in[1000];
char str_file_out[1000];
char str_file_out2[1000];
char str1[4], str2[2];
char trig1[10000], trig2[10000], trig3[10000];

// Variáveis para parâmetros gerais
float nrun, date, version;
float E0, E1, Ef, slope, nlevels, alt[10], primary, z, theta, phi;
float part, px, py, pz, x, y, t;
float rpart;
float nev, nshowers, nphotons, nelectrons, nhadrons, nmuons, ntotal;

int iev = 0, idpart, iphotons, ielectrons, ihadrons, imuons;
// Coordenadas dos detectores e raio de detecção
float xdet1 = 0, ydet1 = 800/sqrt(3), zdet1 = 0, r1, rdet = 65;
float xdet2 = -400, ydet2 = -400/sqrt(3), zdet2 = 0, r2;
float xdet3 = 400, ydet3 = -400/sqrt(3), zdet3 = 0, r3;
// Variáveis de menor tempo de disparo
float minTime1, minTime2, minTime3;

// Contadores separados para cada tipo de partícula em cada detector
int ndet1_pos = 0, ndet1_ele = 0, ndet1_muplus = 0, ndet1_muminus = 0;
int ndet2_pos = 0, ndet2_ele = 0, ndet2_muplus = 0, ndet2_muminus = 0;
int ndet3_pos = 0, ndet3_ele = 0, ndet3_muplus = 0, ndet3_muminus = 0;

int main()
{
    // Configuração inicial dos nomes dos arquivos
    strcpy(str_file_in, "./OUT400152");
    strcpy(str_file_out, "./tratados_3det/photon/photon3.16E15");
    strcpy(str_file_out2, "./tratados_fdl/photon/OUT400152_tratado");

    // Abertura dos arquivos de entrada e saída
    inputfile = fopen(str_file_in, "r");
    if (inputfile == NULL) {
        (void)printf("Can't open inputfile %s\n", str_file_in);
        return 0;
    }

    outputfile = fopen(str_file_out, "w");
    if (outputfile == NULL) {
        (void)printf("Can't open outputfile %s\n", str_file_out);
        return 0;
    }

    outputfile2 = fopen(str_file_out2, "w");
    if (outputfile2 == NULL) {
        (void)printf("Can't open outputfile2 %s\n", str_file_out2);
        return 0;
    }

    // Escreve no início do arquivo de saída o nome do arquivo de entrada
    (void)fprintf(outputfile, "# %s\n", str_file_in);

    // Loop principal para processar o arquivo de entrada
    while (1) {
        // Lê o próximo caractere do arquivo para verificar fim do arquivo
        ch = fgetc(inputfile);
        if (ch == EOF) {
            (void)printf("%.0f showers in file / %d showers analyzed\n", nev, iev);  
            break;
        }
        (void)ungetc(ch, inputfile);

        // Lê a próxima linha do arquivo
        (void)fgets(line, sizeof(line), inputfile);
        (void)sscanf(line, "%s", &str1);

        (void)printf("%s\n", str1); 
        if (strcmp(str1, "LAST") != 0) (void)fprintf(outputfile, "%s", str1); 
        if (strcmp(str1, "READ") == 0) (void)rewind(outputfile);
        
        // Processamento de cada evento
        if (strcmp(str1, "EVTH") == 0) {
            iev++;
            
            // Reinicializa os contadores para cada evento
            ndet1_pos = ndet1_ele = ndet1_muplus = ndet1_muminus = 0;
            ndet2_pos = ndet2_ele = ndet2_muplus = ndet2_muminus = 0;
            ndet3_pos = ndet3_ele = ndet3_muplus = ndet3_muminus = 0;

            minTime1 = FLT_MAX;
            minTime2 = FLT_MAX;
            minTime3 = FLT_MAX;

            (void)fgets(line, sizeof(line), inputfile);   
            (void)sscanf(line, "%f %f %f %f %f %f", &nev, &primary, &E1, &z, &theta, &phi);
            (void)printf(" event number %.0f\n primary: %.0f, E=%.3e GeV, z=%.3f km, theta=%.3f deg, phi=%.3f deg\n", nev, primary, E1, z / 1.E5, theta * rad2deg, phi * rad2deg);
            (void)fprintf(outputfile, " %.0f %.0f %.3e %.3f %.3f %.3f\n", nev, primary, E1, z / 1.E5, theta * rad2deg, phi * rad2deg);

            while (strcmp(str1, "EVTE") != 0) {
                (void)fgets(line, sizeof(line), inputfile);
                (void)sscanf(line, "%s", &str1);
                
                if (strcmp(str1, "EVTE") == 0) break;
                
                (void)sscanf(line, "%f %f %f %f %f %f %f", &part, &px, &py, &pz, &x, &y, &t);
                idpart = int(part / 1000);

                // Verifica se a partícula está no intervalo de interesse
                if (idpart >= 2 && idpart <= 6) {
                    r1 = sqrt((x - xdet1) * (x - xdet1) + (y - ydet1) * (y - ydet1));
                    r2 = sqrt((x - xdet2) * (x - xdet2) + (y - ydet2) * (y - ydet2));
                    r3 = sqrt((x - xdet3) * (x - xdet3) + (y - ydet3) * (y - ydet3));
                    rpart = sqrt(x * x + y * y);

					(void)fprintf(outputfile2,"%d %.3f %.3f %.3f %.3f\n",idpart,x,y,rpart);

                    // Atualiza os contadores do detector 1
                    if (r1 <= rdet) {
                        if (idpart == 2) ndet1_pos++;
                        if (idpart == 3) ndet1_ele++;
                        if (idpart == 5) ndet1_muplus++;
                        if (idpart == 6) ndet1_muminus++;

                        if(t < minTime1) {
                            minTime1 = t;
                        }
                    }

                    // Atualiza os contadores do detector 2
                    if (r2 <= rdet) {
                        if (idpart == 2) ndet2_pos++;
                        if (idpart == 3) ndet2_ele++;
                        if (idpart == 5) ndet2_muplus++;
                        if (idpart == 6) ndet2_muminus++;

                        if(t < minTime2) {
                            minTime2 = t;
                        }
                    }
                    
                    // Atualiza os contadores do detector 3
                    if (r3 <= rdet) {
                        if (idpart == 2) ndet3_pos++;
                        if (idpart == 3) ndet3_ele++;
                        if (idpart == 5) ndet3_muplus++;
                        if (idpart == 6) ndet3_muminus++;

                        if(t < minTime3) {
                            minTime3 = t;
                        }
                    }
                }
            }
            // Substitui FLT_MAX por 0 se necessário
            float currentMinTime1 = (minTime1 != FLT_MAX) ? minTime1 : 0.0f;
            float currentMinTime2 = (minTime2 != FLT_MAX) ? minTime2 : 0.0f;
            float currentMinTime3 = (minTime3 != FLT_MAX) ? minTime3 : 0.0f;

            // Escreve os contadores de cada detector no arquivo de saída
            (void)fprintf(outputfile, "TRIG1 %d %d %d %d %.5e\n", ndet1_pos, ndet1_ele, ndet1_muplus, ndet1_muminus, currentMinTime1);
            (void)fprintf(outputfile, "TRIG2 %d %d %d %d %.5e\n", ndet2_pos, ndet2_ele, ndet2_muplus, ndet2_muminus, currentMinTime2);
            (void)fprintf(outputfile, "TRIG3 %d %d %d %d %.5e\n", ndet3_pos, ndet3_ele, ndet3_muplus, ndet3_muminus, currentMinTime3);

            (void)printf("TRIG1 %d %d %d %d %.5e\n", ndet1_pos, ndet1_ele, ndet1_muplus, ndet1_muminus, currentMinTime1);
            (void)printf("TRIG2 %d %d %d %d %.5e\n", ndet2_pos, ndet2_ele, ndet2_muplus, ndet2_muminus, currentMinTime2);
            (void)printf("TRIG3 %d %d %d %d %.5e\n", ndet3_pos, ndet3_ele, ndet3_muplus, ndet3_muminus, currentMinTime3);
        }

        // Processa informações do final da execução (RUNE)
        if (strcmp(str1, "RUNE") == 0) {
            (void)fgets(line, sizeof(line), inputfile);   
            (void)sscanf(line, "%f %f", &nrun, &nshowers);
            (void)printf("End of run %.0f, nshowers=%.0f\n", nev, nshowers);
            (void)fprintf(outputfile, " %.0f %.0f\n", nev, nshowers);
        }

    } //end while

    // Fecha os arquivos
    (void)fclose(inputfile);
    (void)fclose(outputfile);
    (void)fclose(outputfile2);

    return 0;
}
