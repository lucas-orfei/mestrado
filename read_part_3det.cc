/*Reads my CORSIKA part files*/
//2compile: gcc -o read_part read_part.cc 

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string>

FILE *inputfile;
FILE *outputfile;		
FILE *outputfile2;		

float pi=3.141592653589;
float rad2deg=180/pi;

char line[500];
char ch;
char str_file_in[1000];
char str_file_out[1000];
char str_file_out2[1000];
char str1[4],str2[2];
char trig1[10000],trig2[10000],trig3[10000];

float nrun,date,version;
float E0,E1,Ef,slope,nlevels,alt[10],primary,z,theta,phi;
float part,px,py,pz,x,y,t;
float rpart;
float nev,nshowers,nphotons,nelectrons,nhadrons,nmuons,ntotal;

int iev=0,idpart,iphotons,ielectrons,ihadrons,imuons;
float xdet1=0,ydet1=346,zdet1=0,r1,rdet=65;
float xdet2=-400,ydet2=-346,zdet2=0,r2;
float xdet3=400,ydet3=-346,zdet3=0,r3;
float ndet1=0,ndet2=0,ndet3=0;

int main()
{
    
    strcpy(str_file_in,"/home/orfei/mestrado/progs/corsika/corsika-76900/run/dat/DAT4561514");
    strcpy(str_file_out,"/home/orfei/mestrado/progs/3_detectores/data/iron/iron4.64E15");
    strcpy(str_file_out2,"/home/orfei/mestrado/progs/lateral_distribution/data/iron/DAT4561514_tratado");

inputfile=fopen(str_file_in,"r");
if (inputfile==NULL) {
  (void)printf("Can't open inputfile %s\n",str_file_in);
  return 0;}
 
outputfile=fopen(str_file_out,"w");
if (outputfile==NULL) {
  (void)printf("Can't open outputfile %s\n",str_file_out);
  return 0;}

outputfile2=fopen(str_file_out2,"w");
if (outputfile==NULL) {
  (void)printf("Can't open outputfile2 %s\n",str_file_out);
  return 0;}


(void)fprintf(outputfile,"# %s\n",str_file_in);

//int iev=0;

while (1){
  
  ch=fgetc(inputfile);
  if (ch==EOF){
    (void)printf("%.0f showers in file / %d showers analyzed\n",nev,iev);  
    //(void)fprintf(outputfile,"EOF %d showers analyzed\n",iev);  
	break;}
  (void)ungetc(ch,inputfile);

  (void)fgets(line,sizeof(line),inputfile);
  (void)sscanf(line,"%s",&str1);

//	if (ishower<=10){(void)printf("%s \n",str1);}
  
  (void)printf("%s\n",str1); 
  if (strcmp(str1,"LAST")!=0) (void)fprintf(outputfile,"%s",str1); 
  if (strcmp(str1,"READ")==0) (void)rewind(outputfile);
 
  if (strcmp(str1,"RUNH")==0){
    //    (void)printf("iev=%f\n",iev);

    (void)fgets(line,sizeof(line),inputfile);   
    (void)sscanf(line,"%f %f %f %f %f %f",&nrun,&date,&version,&slope,&E0,&Ef);
    (void)printf(" run number %.0f, date %.0f, Corsika version %.4f\n E0=%.3e GeV, Ef=%.3e GeV, E^{%.3f}\n",nrun,date,version,E0,Ef,slope);
    (void)fprintf(outputfile," %.0f %.0f %.4f %.3e %.3e %.3f",nrun,date,version,E0,Ef,slope);
    (void)fgets(line,sizeof(line),inputfile);   
    (void)sscanf(line,"%f %f %f %f %f %f %f %f %f %f %f",&nlevels,&alt[0],&alt[1],&alt[2],&alt[3],&alt[4],&alt[5],&alt[6],&alt[7],&alt[8],&alt[9]);
    (void)printf(" number of observation levels: %.0f\n",nlevels);
    (void)fprintf(outputfile," %.0f",nlevels);
    for (int i=0;i<nlevels;i++) {
      (void)printf(" altitude of level %d: %.2f m\n",i+1,alt[i]/100);
      (void)fprintf(outputfile," %.2f",alt[i]/100);
    }
    (void)fprintf(outputfile,"\n");
  }

  if (strcmp(str1,"EVTH")==0){
    	iev++;
	iphotons=0,ielectrons=0,ihadrons=0,imuons=0;
	//	(void)printf("iev=%d\n",iev);
 	(void)fgets(line,sizeof(line),inputfile);   
	(void)sscanf(line,"%f %f %f %f %f %f",&nev,&primary,&E1,&z,&theta,&phi);
        (void)printf(" event number %.0f\n primary: %.0f, E=%.3e GeV, z=%.3f km, theta=%.3f deg, phi=%.3f deg\n",nev,primary,E1,z/1.E5,theta*rad2deg,phi*rad2deg);
        (void)fprintf(outputfile," %.0f %.0f %.3e %.3f %.3f %.3f\n",nev,primary,E1,z/1.E5,theta*rad2deg,phi*rad2deg);
        strcpy(trig1,"TRIG1");
        strcpy(trig2,"TRIG2");
        strcpy(trig3,"TRIG3");
	//	ch=fgetc(inputfile);
        //str1=ch;
	while (strcmp(str1,"EVTE")!=0){
	  (void)fgets(line,sizeof(line),inputfile);
	  (void)sscanf(line,"%s",&str1);
	  //	  if (strcmp(str1,"EVTE")==0) {
	  //break;
	  //}else{
	      (void)sscanf(line,"%f %f %f %f %f %f",&part,&px,&py,&pz,&x,&y,&t);
	      idpart=int(part/1000);
	      
	      if(idpart<70){
		//		(void)printf("%d ",idpart);
		if (idpart==1) iphotons+=1;
		if ((idpart==2)||(idpart==3)) ielectrons+=1;
		if ((idpart==5)||(idpart==6)) imuons+=1;
		if ((idpart>6)&&(idpart<66)) ihadrons+=1;
		r1=sqrt((x-xdet1)*(x-xdet1)+(y-ydet1)*(y-ydet1));
		r2=sqrt((x-xdet2)*(x-xdet2)+(y-ydet2)*(y-ydet2));
		r3=sqrt((x-xdet3)*(x-xdet3)+(y-ydet3)*(y-ydet3));

		if ((idpart >=2) && (idpart<=6)){
		rpart=sqrt(x*x+y*y);
		
		(void)fprintf(outputfile2,"%d %.3f %.3f %.3f\n",idpart,x,y,rpart);
		
		if (r1<=rdet) {
			ndet1++;
		  //(void)printf("trigger!\n");
		  strcat(trig1," ");
		  //itoa(idpart,str2,10);
		  snprintf(str2, sizeof(str2), "%d", idpart);
     		  //sprintf(str2, "%d", idpart);
		  strcat(trig1,str2);
		  //(void)fprintf(outputfile," %d",idpart);
		}
		if (r2<=rdet) {
			ndet2++;
		  //(void)printf("trigger!\n");
		  strcat(trig2," ");
		  //itoa(idpart,str2,10);
	          snprintf(str2, sizeof(str2), "%d", idpart);
  		  //sprintf(str2, "%d", idpart);
		  strcat(trig2,str2);
		  //(void)fprintf(outputfile," %d",idpart);
		}
		if (r3<=rdet) {
			ndet3++;
		  //(void)printf("trigger!\n");
		  strcat(trig3," ");
		  //itoa(idpart,str2,10);
		  snprintf(str2, sizeof(str2), "%d", idpart);
  		  //sprintf(str2, "%d", idpart);
		  strcat(trig3,str2);
		  //(void)fprintf(outputfile," %d",idpart);
		}
	    }
		}  
	}
	
     (void)printf("%s\n%s\n%s\n",trig1,trig2,trig3);
     (void)fprintf(outputfile,"%s\n%s\n%s\n",trig1,trig2,trig3);
    // free(trig1);
    // free(trig2);
    // free(trig3);
     (void)printf("%s\n",str1); 
     (void)fprintf(outputfile,"%s",str1); 
  }

  if (strcmp(str1,"EVTE")==0){
    //        printf("iev=%d\n",iev);
 	(void)fgets(line,sizeof(line),inputfile);   
	(void)sscanf(line,"%f %f %f %f %f %f",&nev,&nphotons,&nelectrons,&nhadrons,&nmuons,&ntotal);
        (void)printf(" end of shower %.0f, iev=%d\n nphotons=%.0f, nelectrons=%.0f, nhadrons=%.0f, nmuons=%.0f, ntotal=%.0f\n",nev,iev,nphotons,nelectrons,nhadrons,nmuons,ntotal);
		(void)fprintf(outputfile," %.0f %.0f %.0f %.0f %.0f %.0f\n",nev,nphotons,nelectrons,nhadrons,nmuons,ntotal);
	//        (void)printf(" iphotons=%d, ielectrons=%d, ihadrons=%d, imuons=%d, total=%d\n",iphotons,ielectrons,ihadrons,imuons,iphotons+ielectrons+ihadrons+imuons);

  }

  if (strcmp(str1,"RUNE")==0){
 	(void)fgets(line,sizeof(line),inputfile);   
	(void)sscanf(line,"%f %f",&nrun,&nshowers);
        (void)printf("End of run %.0f, nshowers=%.0f\n",nev,nshowers);
        (void)fprintf(outputfile," %.0f %.0f\n",nev,nshowers);
        (void)printf("Average number of particles in each tank=%.3f %.3f %.3f\n",ndet1/100,ndet2/100,ndet3/100); 
  }

 }//end while

(void)fclose(inputfile);
(void)fclose(outputfile);
(void)fclose(outputfile2);

return 0;
}
