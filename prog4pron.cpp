#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;


void randomselector ( int &page, int&arravil, int argv[3], fstream &fin, fstream &fin2 )
{
    if ( argv[3] == '-r' )
    {
        page = randpage ( page );
        arravil = randarravil ( arravil );
        return;
    }
    if ( argv[3] == '-f' )
    {
        fin >> page; // the random page file
        fin2 >> arravil; //random arravil file

    }
    else
    {
        cout << "invalid argument in comandline " << endl;
        cout << "the third argument should either by -r or -f" << endl;
        exit ( 1 );
    }

}
unsigned int randpage ( unsigned int spongbob )
{
    spongbob = rand();
    spongbob = spongbob % ( 25 + 1 );
    spongbob = spongbob + 15;
    return spongbob;
}
unsigned int randarravil ( unsigned int spongbob )
{
    spongbob = rand();
    spongbob = spongbob % ( 80 + 1 );
    spongbob = spongbob + 20;
    return spongbob;
}