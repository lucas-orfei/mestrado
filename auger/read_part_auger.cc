#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>
#include <cfloat>

FILE *inputfile;
FILE *inputfile2;
FILE *outputfile;
FILE *outputfile2;

float pi=3.141592653589;
float rad2deg=180/pi;

char line[500];
char ch;
char str_file_in[1000];
char str_file_in2[1000];
char str_file_out[1000];
char str_file_out2[1000];
char str1[4],str2[2];

float nrun, date, version;
float E0, E1, Ef, slope, nlevels, alt[10], primary, z, theta, phi;
float part, px, py, pz, x, y, t, th;
float rpart;
float nev, nshowers, nphotons, nelectrons, nhadrons, nmuons, ntotal;

int iev=0, idpart, iphotons, ielectrons, ihadrons, imuons;
const int numDetectors = 1636;

float id[numDetectors];
float x_det[numDetectors];
float y_det[numDetectors];
float minTime[numDetectors];
float detectorRadius = 1.8;

int positronCount[numDetectors] = {0};
int electronCount[numDetectors] = {0};
int muonPlusCount[numDetectors] = {0};
int muonMinusCount[numDetectors] = {0};

int main()
{
    // Leitura dos dados e local de saída dos arquivos
    strcpy(str_file_in, "./OUT400191");
    strcpy(str_file_in2, "./map_auger.csv");
    strcpy(str_file_out, "./tratados_auger/photon/photon1E19_auger");
    strcpy(str_file_out2, "./tratados_fdl/photon/OUT400191_tratado");

    inputfile = fopen(str_file_in, "r");
    if (inputfile == NULL) {
        (void)printf("Can't open inputfile %s\n", str_file_in);
        return 0;
    }

    inputfile2 = fopen(str_file_in2, "r");
    if (inputfile2 == NULL) {
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
        printf("Can't open outputfile2 %s\n", str_file_out);
        return 0;
    }

    (void)fprintf(outputfile, "# %s\n", str_file_in);

    // Criação das posições dos tanques
    int index = 0;
    
    for (int i = 0; i < numDetectors; i++){
        if (fgets(line, sizeof(line), inputfile2)){
            sscanf(line, "%f %f %f", &id[i], &x_det[i], &y_det[i]);
            //if (i < 5 || i >= numDetectors - 5) {
            //    printf("Detector %d: x=%.2f, y=%.2f\n", i+1, x_det[i], y_det[i]);
            //}
        }
    }


    while (1) {
        ch = fgetc(inputfile);
        if (ch == EOF) {
            (void)printf("%.0f showers in file / %d showers analyzed\n", nev, iev);
            //fprintf(outputfile, "EOF %d showers analyzed\n", iev);
            break;
        }
        (void)ungetc(ch, inputfile);

        (void)fgets(line, sizeof(line), inputfile);
        (void)sscanf(line, "%s", str1);

        (void)printf("%s\n", str1);
        if (strcmp(str1, "LAST") != 0) (void)fprintf(outputfile, "%s", str1);
        if (strcmp(str1, "READ") == 0) (void)rewind(outputfile);

        if (strcmp(str1, "RUNH") == 0) {
            (void)fgets(line, sizeof(line), inputfile);
            (void)sscanf(line, "%f %f %f %f %f %f", &nrun, &date, &version, &slope, &E0, &Ef);
            (void)printf(" run number %.0f, date %.0f, Corsika version %.4f\n E0=%.3e GeV, Ef=%.3e GeV, E^{%.3f}\n",
                   nrun, date, version, E0, Ef, slope);
            (void)fprintf(outputfile, " %.0f %.0f %.4f %.3e %.3e %.3f", nrun, date, version, E0, Ef, slope);
            (void)fgets(line, sizeof(line), inputfile);
            (void)sscanf(line, "%f %f %f %f %f %f %f %f %f %f %f",
                   &nlevels, &alt[0], &alt[1], &alt[2], &alt[3], &alt[4], &alt[5], &alt[6], &alt[7], &alt[8], &alt[9]);
            (void)printf(" number of observation levels: %.0f\n", nlevels);
            (void)fprintf(outputfile, " %.0f", nlevels);
            for (int i = 0; i < nlevels; i++) {
                (void)printf(" altitude of level %d: %.2f m\n", i + 1, alt[i] / 100);
                (void)fprintf(outputfile, " %.2f", alt[i] / 100);
            }
            (void)fprintf(outputfile, "\n");
        }

        if (strcmp(str1, "EVTH") == 0) {
            iev++;
            iphotons = 0;
            ielectrons = 0;
            ihadrons = 0;
            imuons = 0;
            // Reiniciar o contador para minTime
            for (int i = 0; i < numDetectors; ++i) {
                minTime[i] = FLT_MAX;
            }
            (void)fgets(line, sizeof(line), inputfile);
            (void)sscanf(line, "%f %f %f %f %f %f", &nev, &primary, &E1, &z, &theta, &phi);
            (void)printf(" event number %.0f\n primary: %.0f, E=%.3e GeV, z=%.3f km, theta=%.3f deg, phi=%.3f deg\n",
                   nev, primary, E1, z / 1.E5, theta * rad2deg, phi * rad2deg);
            (void)fprintf(outputfile, " %.0f %.0f %.3e %.3f %.3f %.3f\n", nev, primary, E1, z / 1.E5, theta * rad2deg, phi * rad2deg);

            while (strcmp(str1, "EVTE") != 0) {
                (void)fgets(line, sizeof(line), inputfile);
                (void)sscanf(line, "%s", str1);
                (void)sscanf(line, "%f %f %f %f %f %f %f %f", &part, &px, &py, &pz, &x, &y, &t, &th);
		rpart = sqrt(x * x + y * y);
                idpart = int(part / 1000);

                if (idpart < 70) {
                    if (idpart == 1) iphotons += 1;
                    if ((idpart == 2) || (idpart == 3)) ielectrons += 1;
                    if ((idpart == 5) || (idpart == 6)) imuons += 1;
                    if ((idpart > 6) && (idpart < 66)) ihadrons += 1;

                    if ((idpart >= 2) && (idpart <= 6)) {
                        //rpart = sqrt(x * x + y * y);
                        (void)fprintf(outputfile2, "%d %d %.3f %.3f %.3f %.3f\n", iev, idpart, x, y, rpart, th);

                        // Conta quantas partículas têm em cada tanque
                        for (int i = 0; i < numDetectors; ++i) {
                            float dx = x/100 - x_det[i];
                            float dy = y/100 - y_det[i];
                            float distance = sqrt(dx * dx + dy * dy);

                            //if (i < 5 || i >= numDetectors - 5) {
                               //printf("Detector %d: Distância: %.2f x_part: %.2f y_part: %.2f x_det: %.2f y_det: %.2f\n", i+1, distance, x/100, y/100, x_det[i], y_det[i]);
                            //}
                            
                            if (distance <= detectorRadius) {
                                if (idpart == 2) positronCount[i]+=th;
                                if (idpart == 3) electronCount[i]+=th;
                                if (idpart == 5) muonPlusCount[i]+=th;
                                if (idpart == 6) muonMinusCount[i]+=th;
                                
                                if (t < minTime[i]) {
                                    minTime[i] = t;
                                }
                            }
                        }
                    }
                }
            }
                // Print dos TRIGS
            for (int i = 0; i < numDetectors; ++i) {
                float currentMinTime = (minTime[i] != FLT_MAX) ? minTime[i] : 0.0f;
                (void)printf("TRIG%d %d %d %d %d %.3e\n", i + 1, positronCount[i], electronCount[i], muonPlusCount[i], muonMinusCount[i], currentMinTime);    
                (void)fprintf(outputfile, "TRIG%d %d %d %d %d %.3e\n", i + 1, positronCount[i], electronCount[i], muonPlusCount[i], muonMinusCount[i], currentMinTime);
            }
            // Reset dos TRIGS   
            for (int i = 0; i < numDetectors; ++i) {
                positronCount[i] = 0; 
                electronCount[i] = 0;
                muonPlusCount[i] = 0;
                muonMinusCount[i] = 0;
            }
        }

        if (strcmp(str1, "EVTE") == 0) {
            (void)fgets(line, sizeof(line), inputfile);
            (void)sscanf(line, "%f %f %f %f %f %f", &nev, &nphotons, &nelectrons, &nhadrons, &nmuons, &ntotal);
            (void)printf(" end of shower %.0f, iev=%d\n nphotons=%.0f, nelectrons=%.0f, nhadrons=%.0f, nmuons=%.0f, ntotal=%.0f\n",
                   nev, iev, nphotons, nelectrons, nhadrons, nmuons, ntotal);
            (void)fprintf(outputfile, " %.0f %.0f %.0f %.0f %.0f %.0f\n", nev, nphotons, nelectrons, nhadrons, nmuons, ntotal);
        }

        if (strcmp(str1, "RUNE") == 0) {
            (void)fgets(line, sizeof(line), inputfile);
            (void)sscanf(line, "%f %f", &nrun, &nshowers);
            (void)printf("End of run %.0f, nshowers=%.0f\n", nev, nshowers);
            (void)fprintf(outputfile, " %.0f %.0f\n", nev, nshowers);
        }
        
         if (strcmp(str1, "LONG") == 0) {
            break;
        }
    } // end while

    (void)fclose(inputfile);
    (void)fclose(inputfile2);
    (void)fclose(outputfile);
    (void)fclose(outputfile2);

    return 0;
}
