

/*
File path

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char filePath[] = "/home/omnix/Desktop/OmnixOne_StepCounter/IMU_Arduino_ESP32/src/log326-CommaTrim.txt";
FILE *fileSquareRoot;
FILE *S_detectCandidate;
FILE *peak;
FILE *vally;


typedef struct
{
    int jtime;
    float x;
    float y;
    float z;
    double squareRoot;
} oneInst;

double calcualteAmp(oneInst *arg_oi)
{
    double squareRoot = sqrt(arg_oi->x * arg_oi->x + arg_oi->y * arg_oi->y + arg_oi->z * arg_oi->z);
    arg_oi->squareRoot = squareRoot;
    // printf("Square root of %.2lf\n", squareRoot);
    return squareRoot;
}

/*int current_tavalleyk = 0;
double current_avalleyk = 0;
int prev_tavalleyk = 121670;
double prev_avalleyk = 0;

int current_tapeakk = 0;
double current_apeakk = 0;
int prev_tapeakk = 121670;
double prev_apeakk = 0;
*/
int detectCandidate(oneInst *oi0, oneInst *oi1, oneInst *oi2)
{
    double amp_oi0 = calcualteAmp(oi0);
    double amp_oi1 = calcualteAmp(oi0);
    double amp_oi2 = calcualteAmp(oi1);

    fprintf(fileSquareRoot,"%.2lf\n",amp_oi0);

    // amp_oi0 = amp_oi0 < 0 ? amp_oi0 * -1 : amp_oi0;
    double mod_amp_oi1 = amp_oi1 < 0 ? amp_oi1 * -1 : amp_oi1;
    double mod_amp_oi2 = amp_oi2 < 0 ? amp_oi2 * -1 : amp_oi2;

    if (mod_amp_oi1 > mod_amp_oi2){
        // current_tapeakk = oi1->jtime;
        // current_apeakk = amp_oi1;
        return 1;
    }else if (mod_amp_oi1 < mod_amp_oi2) {
        // current_tavalleyk = oi1->jtime;
        // current_avalleyk = amp_oi1;
        return -1;
    }else{
        return 0;
    }

}


int thu = 500; // Time threshold for updating acceleration peak/valley
int Old_S;

double peakUpdate(
    int arg_thu, int arg_Old_S, int arg_S, 
    double *arg_apeakk, double *arg_avalleyk, 
    int *arg_tapeakk, int *arg_tavalleyk, 
    oneInst *arg_oi1
    ){
        printf("peakUpdate\n");


    if (arg_Old_S == arg_S){

        // t_apeak = Time of latest acceleration peak
        // t_avalley = Time of latest acceleration valley
        // apeakk	Kth peak magnitude

        /*printf("current_apeakk prev_apeakk\t: %f %f\n", current_apeakk, prev_apeakk);
        printf("current_tapeakk prev_tapeakk\t: %d %d\n", current_tapeakk, prev_tapeakk);
        printf("current_apeakk > prev_apeakk\t: %d\n", current_apeakk > prev_apeakk);
        
        printf("current_avalleyk, prev_avalleyk\t: %f %f\n", current_avalleyk, prev_avalleyk);
        printf("current_tavalleyk, prev_tavalleyk\t: %d %d\n", current_tavalleyk, prev_tavalleyk);
        printf("current_avalleyk > prev_avalleyk\t: %d\n", current_avalleyk > prev_avalleyk);
        */

        printf("arg_S\t: %d\n", arg_S);
        if(arg_S == 1 && 
        (arg_oi1->jtime - *arg_tapeakk) < thu && 
        arg_oi1->squareRoot > *arg_apeakk){

            *arg_apeakk = arg_oi1->squareRoot;
            *arg_tapeakk = arg_oi1->jtime;
            // fprintf(peak,"%d,%.2lf\n",current_tapeakk, current_apeakk);
            printf ("\t\tThis is more\n");
        // }else if(arg_S == -1 && (current_tavalleyk - prev_tavalleyk) < thu && current_avalleyk > prev_avalleyk){
        }else if(arg_S == -1 && 
        (arg_oi1->jtime - *arg_tavalleyk) < thu && 
        arg_oi1->squareRoot > *arg_avalleyk){

            *arg_tavalleyk = arg_oi1->jtime;
            *arg_avalleyk = arg_oi1->squareRoot;
            // fprintf(vally,"%d,%.2lf\n",current_tavalleyk, current_avalleyk);
            printf ("\t\tThis is less\n");
        }
    }else{
        // printf("1No Peak Update\t\t current_apeakk prev_apeakk %f %f\n", current_apeakk, prev_apeakk);
        // printf("2No Peak Update\t\t current_avalleyk prev_avalleyk %f %f\n", current_avalleyk, prev_avalleyk);
    }
}

oneInst oi_previous1; // n-1
oneInst oi_previous2; // n-2
int tar = 1;
int last_time_since_valley_detected = 121600;
int last_time_since_peak_detected = 121600;

void jprocess(oneInst *arg_oi)
{
    int S = detectCandidate(arg_oi, &oi_previous1, &oi_previous2);
    printf("S Old_S\t: %d %d\n", S, Old_S);
    fprintf(S_detectCandidate,"%d\n",S);

    double apeakk = 0;
    double avalleyk = 0;
    
    int tapeakk = oi_previous1.jtime;
    int tavalleyk = oi_previous1.jtime;

    if (S == 1){
        // Time threshold for end of rejection zone and beginning of candidate detection

        int time_since_valley = last_time_since_valley_detected - arg_oi->jtime;
        if (Old_S == -1 && time_since_valley > tar){
            last_time_since_peak_detected = arg_oi->jtime;
            printf("---------> Valid peak detected\n");
        }else if (Old_S == S){
            peakUpdate(thu, Old_S, S, &apeakk, &avalleyk, &tapeakk, &tavalleyk, &oi_previous1);
            // printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        }
        else{
            printf("Doing nothing: ignore\n");
        }

    }else if(S == -1){
        int time_since_peak = last_time_since_peak_detected - arg_oi->jtime;
        if (Old_S == 1 && time_since_peak > tar){
            last_time_since_valley_detected = arg_oi->jtime;
            printf("---------> Valid valley detected\n");
        }else if (Old_S == S){

        }
    }
    

    // peakUpdate(thu, Old_S, S);
    // if (S != 0){
        Old_S = S;
    // }
    
    // printf("time int\t: %d\n", arg_oi->jtime);
    // printf("x float\t: %f\n", arg_oi->x);
    // printf("y float\t: %f\n", arg_oi->y);
    // printf("z float\t: %f\n", arg_oi->z);
    oi_previous2 = oi_previous1;
    oi_previous1 = *arg_oi;
}

int main()
{
    // printf() displays the string inside quotation
    printf("Hello, World!\n");
    
    fileSquareRoot = fopen("fileSquareRoot.txt","w");
    S_detectCandidate = fopen("S_detectCandidate.txt","w");    
    peak = fopen("peak.txt","w");    
    vally = fopen("vally.txt","w");    
    // fclose(fptr);



    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(filePath, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        // printf("Retrieved line of length %zu %s", read, line);
        oneInst oi;
        char *pt;
        pt = strtok(line, ",");

        for (int lp1 = 0; pt != NULL; lp1++)
        {
            // printf("%s\n", pt);
            if (lp1 == 0)
            {
                oi.jtime = atoi(pt);
                // printf("Time int\t: %d\n", oi.jtime);
            }
            else
            {
                if (lp1 == 1)
                {
                    oi.x = (float)atof(pt);
                    // printf("x float\t: %f\n", oi.x);
                }
                if (lp1 == 2)
                {
                    oi.y = (float)atof(pt);
                    // printf("y float\t: %f\n", oi.y);
                }
                if (lp1 == 3)
                {
                    oi.z = (float)atof(pt);
                    // printf("z float\t: %f\n", oi.z);
                }
            }
            pt = strtok(NULL, ",");
        }
        jprocess(&oi);
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

    return 0;
}
