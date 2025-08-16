//LME Alghoritm

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//dichiarazione funzioni
char* lmeAsh(char* kpub, char* msgChiaro, char* pwMD5);
char* ashFinal(char* msg, double* kprivata, int bitLong);
char* lmeDeAsh(char* kpub, char* msgCriptato, int bit, char* pwMD5);
char* deAshFinal(double* msgCript, double* kprivata, int bit, int size);
char* createBitString(int log, int lMess);
double* kprivToDouble(char* kpriv);
char* keyPrivCalc(char* kpub, char* pwMD5);
char* lmeGenKey(int bit);
int* ForShadowing(char* pwMd5, int lengthIntVector, int modifier);

//Ashing
char* lmeAsh(char* kpub, char* msgChiaro, char* pwMD5) {
    char* kpriv;
    double* kprivDouble;
    char* msgCriptato;

    // Calcolo chiave privata
    kpriv = keyPrivCalc(kpub, pwMD5);

    // Converte kpriv in un array di double
    kprivDouble = kprivToDouble(kpriv);
        
    // Cripta il messaggio
    msgCriptato = ashFinal(msgChiaro, kprivDouble, strlen(kpub));

    // Pulizia
    free(kprivDouble);

    return msgCriptato;
}

char* ashFinal(char* msg, double* kprivata, int bitLong) {
    char* cript = (char*)calloc((bitLong * 20) , sizeof(char)); // Assumendo che non avrai mai più di 20 caratteri per numero
    int calc = 0;
    int calc2 = 0;
    double calc3 = 0;
    int j = 0;
    double senoX = 0;
    int x = 0;
    const int zModularSet = 10000; //n = 10000 --> Zn
    int k = 0;
    const int modulePow = 150;
	int i;
	int pp;
    char* stringaBit = createBitString((int)(log(bitLong) / log(2)), strlen(msg));
	
    for(i = 0; i < bitLong; i++)
    {
        calc = (int)kprivata[i] * 100000;
        calc3 = kprivata[i]*100000;
        calc2 = (int)calc3;
        calc2 = calc2 - calc;

        int powAt = (calc2 + (int)sqrt(i+1)) * (int)sqrt(i+1 * 10) % modulePow;
        powAt+=3;
        powAt = abs(powAt);

        int calc2Temp = calc2;

        for(pp = 1; pp < powAt; pp++)
        {
            calc2 = (int) ((int)((calc2+(i+1)) * (calc2Temp + kprivata[i] / (i+1))) % zModularSet);
        }

        int zcalc2Sum = abs((int)kprivata[i])%50 + 50;
        calc2 = abs(calc2);
        calc2+=zcalc2Sum;

        calc3 = sin(calc2);
        senoX = calc3;

        if(i < bitLong - (int)(log(bitLong) / log(2)))
            calc3 = msg[k] * kprivata[i];
        else
        {
            if(x == 0)
            {
                j = 0;
                x = 1;
            }

            calc3 = stringaBit[j] * kprivata[i];
            j++;
            
        }

        calc3 = senoX * calc3;

        char temp[200];
        sprintf(temp, "%f|", calc3);
        strcat(cript, temp);

        if(k == strlen(msg)-1)
            k = 0;
        else
            k++;
    }

    return cript;
}

//


//DeAshing

char* lmeDeAsh(char* kpub, char* msgCriptato, int bit, char* pwMD5) {
    char* kpriv;
    double* kprivDouble;
    double* msgCriptatoDouble;
    char* msgChiaro;
    int kprivLength;
    

    // Calcolo chiave privata
    kpriv = keyPrivCalc(kpub, pwMD5);
    
    
        //printf("\n\n\n%s\n\n", kpriv);


    // Converte kpriv in un array di double
    kprivDouble = kprivToDouble(kpriv);

    // Converte msgCriptato in un array di double
    msgCriptatoDouble = kprivToDouble(msgCriptato);
    
    // Decripta il messaggio
    msgChiaro = deAshFinal(msgCriptatoDouble, kprivDouble, bit, pow(2, bit));

    // Pulizia
    free(kprivDouble);
    free(msgCriptatoDouble);

    return msgChiaro;
}



char* deAshFinal(double* msgCript, double* kprivata, int bit, int size) {
    int calc = 0;
    int calc2 = 0;
    double calc3 = 0;
    char* deCript = (char*)calloc(size , sizeof(char));
    double senoX = 0;
    double chiaro = 0;
    double chiaro2 = 0;
    char lunghezzaS[50] = ""; // Assumendo che non avrai mai più di 50 caratteri
    int lunghezzaInt = 0;
    int i = -1;
    int pp;
    const int zModularSet = 10000; //n = 10000 --> Zn
    const int modulePow = 150;

    do {
    	i++;
        calc = (int)kprivata[i] * 100000;
        calc3 = kprivata[i]*100000;
        calc2 = (int)calc3;
        calc2 = calc2 - calc;

        int powAt = (calc2 + (int)sqrt(i+1)) * (int)sqrt(i+1 * 10) % modulePow;
        powAt+=3;
        powAt = abs(powAt);

        int calc2Temp = calc2;

        for(pp = 1; pp < powAt; pp++)
        {
            calc2 = (int) ((int)((calc2+(i+1)) * (calc2Temp + kprivata[i] / (i+1))) % zModularSet);
        }

        int zcalc2Sum = abs((int)kprivata[i])%50 + 50;
        calc2 = abs(calc2);
        calc2+=zcalc2Sum;

        calc3 = sin(calc2);
        senoX = calc3;
        
        calc3 = msgCript[i] / senoX;
        chiaro = calc3 / kprivata[i];
        chiaro2 = chiaro;
        chiaro = round(chiaro);
        deCript[i] = (char)chiaro;
    } while((char)chiaro != 0);

    deCript[i] = '\0'; // termina la stringa

    int index = strlen(deCript) - bit -1;
    
    for(i = strlen(deCript)-1; i > index; i--)
        strncat(lunghezzaS, &deCript[i], 1);

    int lunghezzaMsgInt = 0;

    for(i = 0; i < strlen(lunghezzaS); i++) {
        if(lunghezzaS[i] == '9')
            lunghezzaMsgInt += pow(2, i);
    }

    char* stringaFinaleDecriptata = (char*)calloc((lunghezzaMsgInt + 1) , sizeof(char));

	for(i = 0; i < lunghezzaMsgInt; i++)
    {
        stringaFinaleDecriptata[i] = deCript[i];
    }

    stringaFinaleDecriptata[lunghezzaMsgInt] = '\0'; // termina la stringa

    // Pulizia
    free(deCript);

    return stringaFinaleDecriptata;
}

//



//Other

char* createBitString(int log, int lMess) {
    char* s = (char*)malloc((log + 1) * sizeof(char));
    char* tempS = (char*)malloc((log + 1) * sizeof(char));
    int t = lMess;
	int i, j;
    for(i = 0; i < log; i++) {
        if(t % 2 == 0) {
            tempS[i] = '5';
        } else if(t % 2 != 0) {
            tempS[i] = '9';
        }
        t /= 2;
    }

    for(i = log - 1, j = 0; i >= 0; i--, j++) {
        s[j] = tempS[i];
    }

    s[log] = '\0'; // termina la stringa

    free(tempS); // libera la memoria allocata per tempS

    return s;
}


double* kprivToDouble(char* kpriv) {
    int length = strlen(kpriv);
    double* valore = (double*)calloc(length, sizeof(double));
    double v = 0;
    int j = 0;
    int i;
    char temp[100] = " ";
	
	
	for(i = 0; i < length; i++){
        if(kpriv[i] != '|'){
            char carattere[2] = {kpriv[i], '\0'};  // Crea una stringa con un singolo carattere
            strcat(temp, carattere);
        } else {
            // Converte la sottostringa in double e la assegna a valore[j]
            v = atof(temp);
            valore[j] = v;
			memset(temp, 0, sizeof(temp)); // Resetta la stringa tmep
            j++;
			
        }
    }
    
    return valore;

}
    
char* keyPrivCalc(char* kpub, char* pwMd5) {
    // Random modifier for the shadowing of the conversion of pw from md5
    int ShadowingModifier = 2811;

    // In this way the calculation of the private key also occurs thanks to the use of the vault password.
    int *vectorShadow = ForShadowing(pwMd5, strlen(kpub), ShadowingModifier);

    double* calc = (double*)calloc(strlen(kpub), sizeof(double));
    char* kprivato = (char*)calloc((strlen(kpub) * 200), sizeof(char)); // Assuming no more than 50 characters per number
    char temp[100];
    int i;

    for(i = 0; i < strlen(kpub); i++) {
        strcpy(temp, "");
        
        calc[i] = (double)kpub[i];
        double sinValue = sin(sqrt(vectorShadow[i]));

        // Check if sinValue is 0 and adjust it slightly to avoid multiplication by zero
        if (sinValue == 0) {
            sinValue = 1e-7; // Small adjustment value
        }

        calc[i] *= sinValue;
        double sqrtKpub = sqrt(kpub[i]);

        // Check if sqrtKpub is 0 and adjust it slightly to avoid multiplication by zero
        if (sqrtKpub == 0) {
            sqrtKpub = 1e-7; // Small adjustment value
        }

        calc[i] *= sqrtKpub * (kpub[i] % 20 + 20);
        
        // Ensure calc[i] is not zero
        if (calc[i] == 0) {
            calc[i] = 1e-7; // Small adjustment value
        }
        
        if(calc[i] == 0)
        {
        	printf("Error .h983 :< Contact DataShield support and report the bug.", i, calc[i]);
        	system("PAUSE");
		}

        sprintf(temp, "%lf|", calc[i]);
        strcat(kprivato, temp);
    }

    free(calc);
    free(vectorShadow);

    return kprivato;
}


char* lmeGenKey(int bit)
{
	srand(time(NULL));
	
	int i = 0;
	int x = pow(2, bit);
	char *dinamicVector;
	dinamicVector = (char *)calloc(x, sizeof(char));
	char* publicKey = (char *)calloc(x, sizeof(char));
	

	for(i = 0; i < x; i++)
		dinamicVector[i] = (char)((int)(rand()%92)+34);//generates numbers from 33 to 125
	dinamicVector[x] = '\0';


	
	strcpy(publicKey, dinamicVector);
	free(dinamicVector);
	return publicKey;
}


int* ForShadowing(char* pwMd5, int lengthIntVector, int modifier) {


	int costantJ = 18;
	int costantI = 5;

    // Allocazione dinamica del vettore di interi
    int* result = (int*)malloc(lengthIntVector * sizeof(int));

    // Controllo se l'allocazione è riuscita
    if (result == NULL) {
        printf("Errore nell'allocazione della memoria.\n");
        exit(1);
    }

	int i = 0, j = 0;
	
    //for(i = 0; i < lengthIntVector; i++)
    while(i < lengthIntVector)
	{
		if(j == strlen(pwMd5))
			j = 0;
		
		result[i] = (int)pwMd5[j] * (((j+costantJ) * (i+costantI)) % modifier);
						
		i++;
		j++;
	}

    return result;
}

















