/*Reads my CORSIKA part files*/
// Compile: gcc -o read_part read_part.cc 

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

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
float part, px, py, pz, x, y, t, th;
float rpart;
float nev, nshowers, nphotons, nelectrons, nhadrons, nmuons, ntotal;

int iev = 0, idpart, iphotons, ielectrons, ihadrons, imuons;
// Coordenadas dos detectores e raio de detecção
float xdet1 = 0, ydet1 = 346, zdet1 = 0, r1, rdet = 65;
float xdet2 = -400, ydet2 = -346, zdet2 = 0, r2;
float xdet3 = 400, ydet3 = -346, zdet3 = 0, r3;

// Contadores separados para cada tipo de partícula em cada detector
int ndet1_pos = 0, ndet1_ele = 0, ndet1_muplus = 0, ndet1_muminus = 0;
int ndet2_pos = 0, ndet2_ele = 0, ndet2_muplus = 0, ndet2_muminus = 0;
int ndet3_pos = 0, ndet3_ele = 0, ndet3_muplus = 0, ndet3_muminus = 0;

int main()
{
    // Configuração inicial dos nomes dos arquivos
    strcpy(str_file_in, "./OUT400161");
    strcpy(str_file_out, "./tratados_3det/photon/photon1E16");
    strcpy(str_file_out2, "./tratados_fdl/photon/OUT400161_tratado");

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

            (void)fgets(line, sizeof(line), inputfile);   
            (void)sscanf(line, "%f %f %f %f %f %f", &nev, &primary, &E1, &z, &theta, &phi);
            (void)printf(" event number %.0f\n primary: %.0f, E=%.3e GeV, z=%.3f km, theta=%.3f deg, phi=%.3f deg\n", nev, primary, E1, z / 1.E5, theta * rad2deg, phi * rad2deg);
            (void)fprintf(outputfile, " %.0f %.0f %.3e %.3f %.3f %.3f\n", nev, primary, E1, z / 1.E5, theta * rad2deg, phi * rad2deg);

            while (strcmp(str1, "EVTE") != 0) {
                (void)fgets(line, sizeof(line), inputfile);
                (void)sscanf(line, "%s", &str1);
                
                if (strcmp(str1, "EVTE") == 0) break;
                
                (void)sscanf(line, "%f %f %f %f %f %f %f %f", &part, &px, &py, &pz, &x, &y, &t, &th);
                idpart = int(part / 1000);

                // Verifica se a partícula está no intervalo de interesse
                if (idpart >= 2 && idpart <= 6) {
                    r1 = sqrt((x - xdet1) * (x - xdet1) + (y - ydet1) * (y - ydet1));
                    r2 = sqrt((x - xdet2) * (x - xdet2) + (y - ydet2) * (y - ydet2));
                    r3 = sqrt((x - xdet3) * (x - xdet3) + (y - ydet3) * (y - ydet3));
                    rpart = sqrt(x * x + y * y);

		            (void)fprintf(outputfile2,"%d %.3f %.3f %.3f %.3f\n",idpart,x,y,rpart,th);

                    // Atualiza os contadores do detector 1
                    if (r1 <= rdet) {
                        if (idpart == 2) ndet1_pos+=th;
                        if (idpart == 3) ndet1_ele+=th;
                        if (idpart == 5) ndet1_muplus+=th;
                        if (idpart == 6) ndet1_muminus+=th;
                    }

                    // Atualiza os contadores do detector 2
                    if (r2 <= rdet) {
                        if (idpart == 2) ndet2_pos+=th;
                        if (idpart == 3) ndet2_ele+=th;
                        if (idpart == 5) ndet2_muplus+=th;
                        if (idpart == 6) ndet2_muminus+=th;
                    }

                    // Atualiza os contadores do detector 3
                    if (r3 <= rdet) {
                        if (idpart == 2) ndet3_pos+=th;
                        if (idpart == 3) ndet3_ele+=th;
                        if (idpart == 5) ndet3_muplus+=th;
                        if (idpart == 6) ndet3_muminus+=th;
                    }
                }
            }

            // Escreve os contadores de cada detector no arquivo de saída
            (void)fprintf(outputfile, "TRIG1 %d %d %d %d\n", ndet1_pos, ndet1_ele, ndet1_muplus, ndet1_muminus);
            (void)fprintf(outputfile, "TRIG2 %d %d %d %d\n", ndet2_pos, ndet2_ele, ndet2_muplus, ndet2_muminus);
            (void)fprintf(outputfile, "TRIG3 %d %d %d %d\n", ndet3_pos, ndet3_ele, ndet3_muplus, ndet3_muminus);

            (void)printf("TRIG1 %d %d %d %d\n", ndet1_pos, ndet1_ele, ndet1_muplus, ndet1_muminus);
            (void)printf("TRIG2 %d %d %d %d\n", ndet2_pos, ndet2_ele, ndet2_muplus, ndet2_muminus);
            (void)printf("TRIG3 %d %d %d %d\n", ndet3_pos, ndet3_ele, ndet3_muplus, ndet3_muminus);
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
