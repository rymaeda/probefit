/* --- The following code comes from C:\lcc\lib64\wizard\textmode.tpl. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INOP 512	//amount to increment the size of list of points
#define iX 3*i
#define iY 3*i+1
#define iZ 3*i+2

char FileName[256];
double mx, my, mz;

void Usage(char *programName){
	fprintf(stderr,"%s usage:\n",programName);
	/* Modify here to add your usage message when the program is
	* called without arguments */
}

/* returns the index of the first argument that is not an option; i.e.
does not start with a dash or a slash
*/
int HandleOptions(int argc,char *argv[]){
	int i,firstnonoption=0;
	for (i=1; i< argc;i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			switch (argv[i][1]) {
				/* An argument -? means help is requested */
			case '?':
				Usage(argv[0]);
				break;
			case 'h':
			case 'H':
				if (!stricmp(argv[i]+1,"help")) {
					Usage(argv[0]);
					break;
				}
				break;
			case 'f':
				//filename
				strcpy(FileName,argv[i+1]);
				i++;
				break;
			case 'm':
				//milimeters/metric system
				mx/=25.4;
				my/=25.4;
				mz/=25.4;
				//printf("m");
				break;
			case 'x':
				//x scale
				mx*= strtod(argv[i+1], NULL);
				i++;
				break;
			case 'y':
				//y scale
				my*= strtod(argv[i+1], NULL);
				i++;
				break;
			case 'z':
				//z scale
				mz*= strtod(argv[i+1], NULL);
				i++;
				break;
			default:
				fprintf(stderr,"unknown option %s\n",argv[i]);
				break;
			}
		}
		else {
			firstnonoption = i;
			break;
		}
	}
	if (strlen(FileName)==0){
		Usage(argv[0]);
		exit(-1);
	}
	return firstnonoption;
}

double *PointList;
int V; // u grid
int U; // v grid
int PLN; // number of points

void PLReadFile(char *filename){
	FILE *fp;
	char oneline[255]; //buffer for the current line
	char *c;
	int list_end= 0; // number of point of the list
	int i= 0; // number of current file line read
	double x, y, z;
	fp = fopen(filename, "r");
	if (!fp){
		fprintf(stderr, "File not found!\n");
		return;
	}
	//c = fgets(oneline, 254, fp);
	do {
		if (list_end==i){
			//debug
			//printf("<<<Reallocking memory!>>>\n");
			list_end+= INOP;
			PointList= (double*)realloc(PointList, list_end * 3 * sizeof(double));
			if (!PointList){
				fprintf(stderr,"Fatal error: can't allocate memory\n");
				exit(-2);
			}
		}
		if (c = fgets(oneline, 254, fp)){
			sscanf(oneline, "%lf %lf %lf", &x, &y, &z);
			//printf("(%d)Oneline: %s",i,c);
			PointList[iX]= x * mx;
			PointList[iY]= y * my;
			PointList[iZ]= z * mz;
			//debug
			//printf("\t[%.3lf][%.3lf][%.3lf]\n", PointList[iX], PointList[iY], PointList[iZ]);
			i++;
		}
	}
	while (c != NULL);              /* repeat until NULL          */
	fclose(fp);
	PLN= i;
	//debug
	//printf("PLN= %d\n", PLN);
}

void PLWriteFile(char *filename){
	FILE *fp;
	char filenameO[256];
	int i;
	strcpy(filenameO, "VCP_");
	strcat(filenameO, filename);
	fp = fopen(filenameO, "w");
	if (!fp){
		fprintf(stderr,"File output fatal error.\n");
		exit(-3);
	}
	fprintf(fp,"%d %d\n", U, V);
	for (i=0; i< (V * U); i++){
		fprintf(fp, "%.11lf %.11lf %.11lf\n", PointList[iX], PointList[iY], PointList[iZ]);
		//printf(oneline);
	}
	fclose(fp);
}

void PLAnalysis(void){
	int cu= 0;
	int cv= 0;
	int i;
	for (i= 0; i< (PLN - 1); i++){
		//debug
		//printf("%.11lf %.11lf %.11lf\n", PointList[iX], PointList[iY], PointList[iZ]);
		if (PointList[iX]==PointList[iX+3])			cu++;
		if (PointList[iY]==PointList[iY+3])			cv++;
	}
	//debug
	//printf("cu= %d  cv= %d   PNL/(cu+1)= %d\n", cu, cv, PLN/(cu+1));
	U= cu +1;
	V= PLN/U;
}

void PLAdjustment(void){
	int i, j, k;
	int a, b;
	double aux;

	//inicial= V*2*j ate V*(2*j+1) com j=0..(U/2)
	for (j= 0; j< (U/2); j++){
		//	printf("(%d)", i);
		for (k= 0; k< V/2; k++){
			//x:
			aux= PointList[3*(V*(j*2+1)+k)];
			PointList[3*(V*(j*2+1)+k)]= PointList[3*(V*(j*2+2)-1-k)];
			PointList[3*(V*(j*2+2)-1-k)]= aux;
			//y:
			aux= PointList[3*(V*(j*2+1)+k)+1];
			PointList[3*(V*(j*2+1)+k)+1]= PointList[3*(V*(j*2+2)-1-k)+1];
			PointList[3*(V*(j*2+2)-1-k)+1]= aux;
			//z:
			aux= PointList[3*(V*(j*2+1)+k)+2];
			PointList[3*(V*(j*2+1)+k)+2]= PointList[3*(V*(j*2+2)-1-k)+2];
			PointList[3*(V*(j*2+2)-1-k)+2]= aux;
			//printf("%d - %d\n", (V*(j*2+1)+k), (V*(j*2+2)-1-k));
		}
	}
}

int main(int argc,char *argv[]){
	if (argc == 1) {
		/* If no arguments we call the Usage routine and exit */
		Usage(argv[0]);
		return 1;
	}
	mx= my= mz= 1.0;
	HandleOptions(argc,argv);
	PLReadFile(FileName);
	PLAnalysis();
	PLAdjustment();
	PLWriteFile(FileName);
	/* handle the program options */
	//HandleOptions(argc,argv);
	/* The code of your application goes here */
	return 0;
}
