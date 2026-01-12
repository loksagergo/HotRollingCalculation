// hotrolling.cpp : This file contains the 'main' function. Program execution begins and ends there.
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <math.h>
int main()

{
    int szurasok;

    //tömbökbe
    double pnevl[100];
    double A0[100];
    double A1[100];
    double kf[100];
    double y[100];
    double b1[100];
    double Mal[100];
    double Pal[100];
    double Pal2[100];
    double deltahmax[100];
    double v[100];
    double pi = 3.141592654;
    double N[100];
    double r[100];
    double d[100];

    //tárolni több számjegyet
    double si;
    double omegahen;
    double b1cs;
    double h0atl;
    double h1atl;
    double deltahatl;
    double hkoz;
    double deltahcs;
    double egydatl;
    double A1ny;
    double kk;
    double hengero;
    double omega;
    double T_start, T_drop, T_current;
    double C, Mn, Cr;
    int aceltipus = 0;
    int smollibrary = 0;
    //-----------------NUMBER OF PASSES AND TEMPERATURES-----------------------/
    do {
        printf("Enter the number of passes (max 100): ");
        scanf_s("%d", &szurasok);

        if (szurasok > 100) {
            printf("Error: Too many passes! Please enter a value between 1 and 100.\n");
        }
        if (szurasok <= 0) {
            printf("Error: The number of passes must be at least 1!\n");
        }

    } while (szurasok > 100 || szurasok <= 0);


    printf("Starting temperature [C]: ");
    scanf_s("%lf", &T_start);
    if (T_start <= 750) {
        printf("\n--- WARNING: Temperature too low (%.1f C) ---\n", T_start);
        printf("Material reached the phase transformation zone.\n");
        printf("Rolling should be stopped or material reheated!\n");
         // Kilépünk a ciklusból, mert veszélyes tovább hengerelni
    }

    printf("Temperature drop per pass [C] (e.g. 30): ");
    scanf_s("%lf", &T_drop);
//------------------------------LIBRARY STARTS---------------------------------------------//
         printf(" do you want to use a small library for steel types? (1: Yes ; 2:No )");
             scanf_s("%d", &smollibrary);
              while (getchar() != '\n');

        switch (smollibrary) {

        case 2:
        {//no library, gets inputs//
            printf("Steel Carbon content [%%]: ");
            scanf_s("%lf", &C);


            printf("Steel Manganese content [%%]: ");
            scanf_s("%lf", &Mn);

            printf("Steel Chromium content [%%] (0 if none): ");
            scanf_s("%lf", &Cr);
            break;
        }

        case 1://library here 
            {
            printf("\n--- ACEL ADATBAZIS ---\n");
            printf("Valassz acelt (1: C20, 2: C45, 3: S355, 4: S235, 5: 42CrMo4, 6: 37MnSi5 ): ");
            scanf_s("%d", &aceltipus);

            switch (aceltipus) {
            case 1: C = 0.20; Mn = 0.45; Cr = 0.15; break; // C20
            case 2: C = 0.45; Mn = 0.65; Cr = 0.20; break; // C45
            case 3: C = 0.20; Mn = 1.45; Cr = 0.10; break; // S355
            case 4: C = 0.17; Mn = 1.20; Cr = 0.05; break; // S235
            case 5: C = 0.42; Mn = 0.75; Cr = 1.10; break; // 42CrMo4
            case 6: C = 0.37; Mn = 1.35; Cr = 0.30; break; //37MnSi5
                             
                break;
            }
            printf("Anyagminoseg beallitva.\n");
            break; // Ez a külső case 1 lezárása          

            }
        }
        //---------------------------------------LIBRARY STOPS-------------------------------------//
        
//cycle for calculations-------------------------------------------------------------//
        for (int i = 0; i < szurasok; i++) {
            if (szurasok > 100) { printf("Exceeded the maximum amount of passes please return");
            } else {

                printf("\nRated Motor Power [kW]: \n"); //pnevl
                scanf_s("%lf", &pnevl[i]);

                while (getchar() != '\n');

                printf("Entry Area [mm2]: \n"); //A0
                scanf_s("%lf", &A0[i]);

                while (getchar() != '\n');

                printf("Exit Area [mm2]: \n"); //A1
                scanf_s("%lf", &A1[i]);

                while (getchar() != '\n');

                //printf("Flow Stress (kf) [N/mm2]: "); //kf bent hagyom ha nem mukodne a kiszamolt ertekes kf meg T 
                //scanf_s("%lf", &kf[i]);

                printf("Roll Speed [RPM]  "); //N
                scanf_s("%lf", &N[i]);

                while (getchar() != '\n');

                printf("dolgozo henger atmero[mm]:  "); //r
                scanf_s("%lf", &d[i]);

                while (getchar() != '\n');

                printf(" adja meg hogy milyen szelessegre hengerel: szurasbeli termek merete[mm] "); //b1
                scanf_s("%lf", &b1[i]);

                r[i] = d[i] / 2;
                while (getchar() != '\n');
                // CALCULATION STARTS -----------------------------------------------------------------------------------------
                T_current = T_start - (i * T_drop);

                // 1. Súrlódási tényező (Ekelund y)
                 y[i] = 1.05 - (0.0005 * T_current);

                // 2. Alakítási ellenállás (Ekelund kf)
                 kf[i] = (1.0 - 0.0005 * T_current) * (1.4 + C + Mn + 0.3 * Cr) * 98.1;

                                                   omegahen = (2 * 3.14159 * N[i]) / 60.0; // 1/s
                                                 v[i] = (r[i]) * pi * N[i] / 60000.0;
                                               si = 0.95; // [-]
                                            deltahmax[i] = r[i] * pow(y[i], 2.0);
                                             omega = v[i] / r[i]; // 1/s
                                               b1cs = si * b1[i]; // mm
                                                 h0atl = A0[i] / b1cs; // mm
                                                    h1atl = A1[i] / b1cs; // mm
                                                      deltahatl = h0atl - h1atl; // mm
                                                         hkoz = (h0atl + h1atl) / 2.0; //  mm
                                                             deltahcs = 0.25 * (3.0 * deltahatl + deltahmax[i]); // mm
                                                                  printf("DEBUG: deltahcs = %f\n", deltahcs);
                                                                     egydatl = (sqrt(r[i] * deltahcs)); // mm
                                                                          A1ny = (b1cs * egydatl); // mm^2
                                                                             kk = kf[i] * (1.0 + 0.5 * y[i] * ((egydatl / hkoz))); // N/mm2
                                                                                hengero = kk * A1ny; // Newton

                                                                                printf("DEBUG: KF : %lf", kf[i]);
                                                                                printf("DEBUG: y: %lf", y[i]);

                Mal[i] = 2 * (egydatl * 0.7 / 1000) * hengero; // Nm

                Pal[i] = (Mal[i] * omegahen) / 1000.0; // kW

                Pal2[i] = (hengero * v[i]) / 1000.0; // kW
//---------------------------------------------------------------------------------------------------------
                double utilization = (Pal[i] / pnevl[i]) * 100.0;
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 

                printf("\n----------------------------------\n");
                printf("Capacity Utilization: %.2f %%\n", utilization);

                if (utilization > 100.0) {
                    //red
                    SetConsoleTextAttribute(hConsole, 12);
                    printf("STATUS: [DANGER] MOTOR OVERLOAD!\n");
                    printf("Action: Reduce Roll Speed (N) or Entry Area (A0)!\n");
                }
                else if (utilization > 85.0) {
                    //yellow
                    SetConsoleTextAttribute(hConsole, 14);
                    printf("STATUS: [WARNING] Near limit!\n");
                }
                else {
                    //green
                    SetConsoleTextAttribute(hConsole, 10);
                    printf("STATUS: [SAFE] Operation is stable.\n");
                }
//----------------------------------------------------------------------------------
                // Szín VISSZAÁLLÍTÁSA fehérre 
                SetConsoleTextAttribute(hConsole, 7);
                printf("----------------------------------\n");
                    //CALCULATION ENDS
                    printf("\n Rolling force [kN]: %.4f", hengero / 1000.0);
                    printf("\n Torque [kNm]: %lf", Mal[i] / 1000.0); // kNm
                    printf("\n Deformation power [kW] : %lf ", Pal2[i]);
                    printf("\n Shaft power [kW] : %lf \n", Pal[i]);
                    printf("Temperature: %.1f C\n", T_current);
                    //------------
                    //Next Rolling Temperature// 
                    T_current = T_current - T_drop;

                }
            }
            return 0;
        }

       
     


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
