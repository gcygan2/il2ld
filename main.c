/*
Program przetwarza plik wejściowy <nazwa>.il
zawierający program w język IL
buduje reprezentację graficzną programu w języku LD
i zapisuje do pliku tekstowego <nazwa>.ld
*/

#include <stdio.h>
#include <string.h>

#define SZERKOM 8
#define WYSKOM 3
#define SZERPRG 10
#define WYSPRG 70
#define MAXKOL (SZERPRG * SZERKOM)
#define MAXLINE (WYSPRG * WYSKOM)

#define KOM_PUSTY 0
#define KOM_STYK_NO 1
#define KOM_STYK_NC 2
#define KOM_CEWKA_NO 3
#define KOM_CEWKA_NC 4
#define KOM_CEWKA_S 5
#define KOM_CEWKA_R 6
#define KOM_CEWKA_NO_KON 7
#define KOM_CEWKA_NC_KON 8
#define KOM_CEWKA_S_KON 9
#define KOM_CEWKA_R_KON 10
#define KOM_POZIOM 11
#define KOM_PION 12
#define KOM_TON 13
#define KOM_TON_KON 14
#define KOM_PION_GRUBA 15
char komorka[][SZERKOM*WYSKOM] = {
{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},/*0  - pusty*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','[',' ',']','-','-',' ',' ',' ',' ',' ',' ',' ',' '},/*1  - styk NO*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','[','/',']','-','-',' ',' ',' ',' ',' ',' ',' ',' '},/*2  - styk NC*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','(',' ',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*3  - cewka NO*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','(','/',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*4  - cewka NC*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','(','S',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*5  - cewka S*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','(','R',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*6  - cewka R*/
{'|',' ',' ',' ',' ',' ',' ',' ','+','-','-','(',' ',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*7  - cewka NO kon*/
{'|',' ',' ',' ',' ',' ',' ',' ','+','-','-','(','/',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*8  - cewka NC kon*/
{'|',' ',' ',' ',' ',' ',' ',' ','+','-','-','(','S',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*9  - cewka S kon*/
{'|',' ',' ',' ',' ',' ',' ',' ','+','-','-','(','R',')','-','|',' ',' ',' ',' ',' ',' ',' ',' '},/*10 - cewka R kon*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','-','-','-','-','-','-',' ',' ',' ',' ',' ',' ',' ',' '},/*11 - piozioma*/
{'|',' ',' ',' ',' ',' ',' ',' ','+',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},/*12 - pionowa*/
{' ',' ',' ',' ',' ',' ',' ',' ','-','-','(','T','O','N',')','|',' ',' ',' ',' ',' ',' ',' ',' '},/*13 - TON*/
{'|',' ',' ',' ',' ',' ',' ',' ','+','-','(','T','O','N',')','|',' ',' ',' ',' ',' ',' ',' ',' '},/*14 - TON kon*/
{'|','|',' ',' ',' ',' ',' ',' ','|','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},/*15 - pionowa gruba*/
};

char progTags[WYSPRG][SZERPRG][SZERKOM];
int program [WYSPRG][SZERPRG];

#include <stdio.h>

typedef struct {int y; int x;} wspolrzedne_t;

#define MAX_STOS 10
static int Stos (wspolrzedne_t *wsp, int dir)
{
    static int pt = 0;
    static wspolrzedne_t stos[MAX_STOS];
    static int max_x = 0;
    
    if (dir == 1) {//na stos
        if (wsp->x > max_x){
            max_x = wsp->x;
        }
        if (pt >= MAX_STOS){
            return -1;
        }
        stos[pt].y = wsp->y;
        stos[pt].x = wsp->x;
        pt++;
    } else if (dir == 0) {//ze stosu
        if (pt > 0) {
            pt--;
            //printf ("Stos: pt=%d\n", pt);
            wsp->y = stos[pt].y;
            wsp->x = stos[pt].x;
        } else {
            max_x = 0;
            return -2;
        }
    } else if (dir == 2) {
        wsp->x = max_x;
        wsp->y = stos[pt-1].y;
        if (!pt){
            return -3;
        }
    }
    return 0;
}

void drukuj (char *plikOut, int ile)
{
    FILE *fpOut;
    int x, y;
    int row, col;
    int xKom, yKom;
    
    fpOut = fopen (plikOut,"wt");
    for (y = 0; y <  ile * WYSKOM ; y++){
        row = y / WYSKOM;
        yKom = y % WYSKOM;
        for (x = 0; x < MAXKOL; x++){
            col = x / SZERKOM;
            xKom = x % SZERKOM;
            if (x == 0) {
               fputc ('|', fpOut);
            } else {
                if (yKom == 0) {/*pierwsza linia*/
                    if (progTags[row][col][xKom] != '\0'){
                        fputc (progTags[row][col][xKom], fpOut);
                    } else {
                        fputc (komorka [program[row][col]][xKom + SZERKOM * yKom], fpOut);
                    }
                } else {
                    fputc (komorka [program[row][col]][xKom + SZERKOM * yKom], fpOut);
                }
            }
        }
        fputc ('\n', fpOut);
    }
    fclose (fpOut);
}

int drukujCewke (int funkcja, int xStart, int y, char *rozkaz, char *argument)
{
    int i;
    static int ofset = 0;
    if (funkcja == 1) {
        i = ofset;
        ofset = 0;
        return i;
    }
    if (!ofset) {
        for (i = xStart; i < SZERPRG-1; i++) {
            program[y][i] = KOM_POZIOM;
        }
    }
    if (rozkaz[0] == 'S' && rozkaz[1] == 'T'){
        if (ofset) {
            program[y+ofset][SZERPRG-1] = (rozkaz[2] == 'N')? KOM_CEWKA_NC_KON : KOM_CEWKA_NO_KON;
        } else {
            program[y][SZERPRG-1] = (rozkaz[2] == 'N')? KOM_CEWKA_NC : KOM_CEWKA_NO;
        }
    } else if (rozkaz[0] == 'R') {
        if (ofset) {
            program[y+ofset][SZERPRG-1] = KOM_CEWKA_R_KON;
        } else {
            program[y][SZERPRG-1] = KOM_CEWKA_R;
        }
    } else if (rozkaz[0] == 'S') {
        if (ofset) {
            program[y+ofset][SZERPRG-1] = KOM_CEWKA_S_KON;
        } else {
            program[y][SZERPRG-1] = KOM_CEWKA_S;
        }
    } else {
        if (ofset) {
            program[y+ofset][SZERPRG-1] = KOM_TON_KON;
        } else {
            program[y][SZERPRG-1] = KOM_TON;
        }
    }
    strcpy (&progTags[y+ofset][SZERPRG-1][3], argument);
    ofset++;
    return 0;
}

int main (int argc, char **argv)
{
    FILE *fp;
    char *slowo;
    char linia[128];
    char rozkaz[32];
    char argument[32];
    int i, max_y = 0;

    char plikIn[32], plikOut[32];
    int xof, yof, y0 = 0, bylaCewka = 0;
    
    
    if (argc < 2) {
        printf ("Brak pliku wejsciowego.\n");
    }
    
    strcpy(plikIn, argv[1]);
    strcat(plikIn, ".il");
    strcpy(plikOut, argv[1]);
    strcat(plikOut, ".ld");
    
    memset (progTags, '\0', sizeof(progTags));
    memset (program, KOM_PUSTY, sizeof(program));
    
    fp = fopen (plikIn,"rt");
    if (fp == NULL) {
        printf ("Plik %s nie istnieje.\n", plikIn);
        return -1;
    }
    
    while (fgets(linia, sizeof(linia), fp) != NULL) {
        slowo = strtok(linia, " \t\n");
        //printf("%s*\n", slowo);
        strncpy (rozkaz, slowo, 4);
        slowo = strtok(NULL, " \t\n");
        if (slowo != NULL){
            strncpy (argument, slowo, 4);
            //("%s*\n", argument);
        }
        if (!strncmp (rozkaz, "LD", 2)){
            if (bylaCewka) {
                int ofs = drukujCewke (1, 0, 0, NULL, NULL);
                y0 = (max_y + 1> y0 + ofs)? max_y + 1 : y0 + ofs;
                bylaCewka = 0;
            }
            yof = 0;
            xof = 0;
            
            if (max_y < y0 + yof) {
                max_y = y0 + yof;
            }
            
            drukujCewke (1, 0, 0, NULL, NULL);
            if (rozkaz[2] == 'N'){
                program[y0][0] = KOM_STYK_NC;
            } else {
                program[y0][0] = KOM_STYK_NO;
            }
            strcpy (&progTags[y0][0][3], argument);
            // = y0;
        } else if (!strncmp (rozkaz, "OR", 2)){
            if (argument[0] == '(') {
                wspolrzedne_t st;
                //printf ("xof=%d, y0=%d, yof=%d\n", xof, y0, yof);
                st.y = y0;
                st.x = xof;
                Stos (&st, 1);
                y0 += yof + 1;
                xof = 0;
                yof = 0;
            } else {
                yof++;
                if (max_y < y0 + yof) {
                    max_y = y0 + yof;
                }
                if (rozkaz[2] == 'N'){
                    program[y0+yof][0] = KOM_STYK_NC;
                } else {
                    program[y0+yof][0] = KOM_STYK_NO;
                }
                strcpy (&progTags[y0+yof][0][3], argument);
                for (i = 1; i <= xof; i++) {
                    program[y0+yof][i] = KOM_POZIOM;
                }
                for (i = y0+1; i <= y0+yof; i++) {
                    program[i][xof+1] = KOM_PION;
                }
            }
        } else if (!strncmp (rozkaz, "AND", 3)){
            xof++;
            if (rozkaz[3] == 'N'){
                program[y0][xof] = KOM_STYK_NC;
            } else {
                program[y0][xof] = KOM_STYK_NO;
            }
            strcpy (&progTags[y0][xof][3], argument);
        } else if (!strncmp (rozkaz, ")", 1)){
        } else if (rozkaz[0] == 'S' || rozkaz[0] == 'R' || rozkaz[0] == 'T') { //pasuje do ST,S,R,TON
            wspolrzedne_t st, st2;
            if (!Stos(&st2, 2)) {
                st.y = y0;
                st.x = xof;
                Stos (&st, 1);               
    
                int x_pom = st2.x + 1;
                //int y_pom = st2.y + 1;
                yof = st2.y + 1;
                while (!Stos(&st2, 0)) {
                    for (i = st2.x + 1; i < x_pom; i++) {
                        program[st2.y][i] = KOM_POZIOM;//??
                    }
                    
                }
                //printf ("STOS: x=%d, y=%d, r=%s, a=%s\n", x_pom, st2.y, rozkaz, argument);
                //drukujCewke (0, x_pom, st2.y, rozkaz, argument);
                xof = st2.x;
                y0 = st2.y;
                for (i = st2.y; i <= yof; i++) {
                    program[i][x_pom] = KOM_PION;//??
                }
            }
            //printf ("OUT: y0=%d, max_y=%d, r=%s, a=%s\n", y0, max_y, rozkaz, argument);
            drukujCewke (0, xof + 1, y0, rozkaz, argument);
            bylaCewka = 1;

        }
        
    }
    fclose (fp);
    
    //drukuj (plikOut, WYSPRG);
    drukuj (plikOut, y0 + 8);

    return 0;
}
