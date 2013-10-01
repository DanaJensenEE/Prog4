/**************************************************************************//**
 * @file
 *
 * @mainpage Program 4 - Printer Simulation
 *
 * @section course_section Course Information
 *
 * @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
 *
 * @date April 26, 2013
 *
 * @par Instructor:
 *        Roger Schrader
 *
 * @par Course:
 *         CSC 250 - Section 1 - 10:00 am
 *
 * @par Location:
 *         Classroom Building
 *
 * @section program_section Program Information
 *
 * @details This program determine the amount of time a printer has been idle.
 * Documents that need to be printed are placed into a queue in order of 
 * arrival time. A counter will record the idle time of the printer and return
 * the time between printing documents, time to print each page, number of 
 * documents printed, the total idle time, and the number of documents still 
 * left to print if any. The program can get its documents from either randomly 
 * generated values or random values from a file. 
 *
 * @section compile_section Compiling and Usage
 *
 * @par Compiling Instructions:
 *      None
 *
 * @par Usage:
 * @verbatim
 * c:\prog4.exe 40 3 -r(or -f)
 * Average time in between Print Jobs >=31
 * Time to print 1 Page >0
 * "-r" for random seeding "-f" for file seeding
 * @endverbatim
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 *
 * @bug
 *
 * @todo Finish documenting with doxygen tags
 *
 * @par Modifications and Development Timeline:
   @verbatim
   Date          Modification
   ------------  --------------------------------------------------------------
   Apr 20, 2013  Wrote all functions except generateDocument.
   Apr 21, 2013	 Wrote generateDocument, removed unnecessary variables
   Apr 26, 2013	 Added doxygen comments

   @endverbatim
 *
 *****************************************************************************/

#include "queue.h"
using namespace std;

/******************************************************************************
*						Function prototypes									  
******************************************************************************/
bool generateDocument ( document &doc, char *argv[], int &prevTime,
                        queue<document> &q1, ifstream &finPage, 
                        ifstream &finTime );
int getArrival ( int prevTime, char *argv[] ,ifstream &finTime);
int getPages ( char *argv[] ,ifstream &finPage);
unsigned int randPage();
unsigned int randArrival ( char *argv[] );
unsigned int filePage(ifstream &finPage);
unsigned int fileArrival ( char *argv[] ,ifstream &finTime);

/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This main function will open input files if they are needed and add documents
* to the queue as they come in and removes them after they have finished 
* printing. After the end of the day is reached (time = 28800) stats are output
* to the command window including time between documents, time to print each
* page, number of documents printed, the total idle time, and the number of
* documents still left to print, if any.
*
* @param[in] argc - the number of command line arguments
* @param[in] argv - the names of the arguments being passed in
*
* @returns None
******************************************************************************/
int main ( int argc, char *argv[] )
{
    srand((unsigned int) time(NULL));
    if ( argc != 4 )
    {
        cout<<"Incorrect Usage (Prog4.exe)"; 
        cout<<" (#AvgJobTime) (#TimeToPrintPage) (\"-r\" or \"-f\")"<<endl;
        system("pause");
        return 1;
    }
    static ifstream finPage;
    static ifstream finTime;
    int idleTime = 0, documents = 0, pages = 0, prevTime = 0;
    int time = 0, left_in_queue = 0;
    int print_time = atoi ( argv[2] );
    queue<document> q1;
    document printingDoc, doc;
	finTime.open ( "arrival.rand" );
	finPage.open ( "pages.rand" );
    printingDoc.pages = 0;
    while ( time <= 28800 )
    {
        if ( prevTime < 28800 && time >= prevTime )
        {
            generateDocument ( doc, argv, prevTime, q1, finPage, finTime );
            q1.enque ( doc );
        }
        if ( time == 0 )
            q1.front ( printingDoc );
        if ( printingDoc.time_end_printing == time )
            q1.front ( printingDoc );
        if ( printingDoc.time_arrived <= time 
             && printingDoc.time_end_printing == 0 )
        {
            documents++;
            q1.deque ( printingDoc );
            printingDoc.time_dequed = time;
            printingDoc.time_started_print = time;
            printingDoc.time_end_printing = time + 
                ( printingDoc.pages * print_time );
        }
        if ( printingDoc.time_arrived > time )
            idleTime++;
        time++;
    }
    while ( q1.deque ( doc ) )
        left_in_queue++;
    //Stats Show
    cout << "Seconds bewtween documents arrival: " << argv[1] << endl;
    cout << "Seconds to print a page:            " << argv[2] << endl;
    cout << "Number of documents Printed:        " << documents << endl;
    cout << "Seconds of idle time:               " << idleTime << endl;
    cout << "Number of documents still in Queue: " << left_in_queue << endl;
    system ( "pause" );
    return 0;
}

/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function will generate the documents that are being sent to the print 
* queue. The function will call the getArrival and getPages functions for the 
* arrival time and number of pages in the document respectively. 
*
* @param[in] doc - a document that will hold the information for the document
* @param[in] argv[] - the name of the argument being passed in
* @param[in] prevTime - time the last document arrived at the queue
* @param[in] q1 - the queue holding the documents to be printed
* @param[in] finPage - a file that holds the number of pages per document
* @param[in] finTime - a file that hold the time the document arrived in the 
*						queue
*
* @returns true - document was created
******************************************************************************/
bool generateDocument ( document &doc, char *argv[], int &prevTime, 
                        queue<document> &q1, ifstream &finPage, 
                        ifstream &finTime )
{
    int pages;
    int arrivalTime;
    arrivalTime = getArrival ( prevTime, argv ,finTime);
    prevTime = arrivalTime;
    pages = getPages ( argv ,finPage);
    doc.pages = pages;
    doc.time_arrived = arrivalTime;
    doc.time_dequed = 0;
    doc.time_started_print = 0;
    doc.time_end_printing = 0;
    return true;
}
/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function generate the arrival time for the next document in the 
* queue. This function will call randArrival, or fileArrival to generate the 
* arrival time.
*
* @param[in] prevTime - time the last document arrived at the queue
* @param[in] *argv[] - the name of the argument being passed in
* @param[in] finTime - a file that hold the time the document arrived in the 
*						queue
*
* @returns time - the time the document arrived overall
******************************************************************************/
int getArrival ( int prevTime, char *argv[], ifstream &finTime )
{
    int time;
    int average;
    string s;
    s = argv[3];
    average = atoi ( argv[1] );
    if ( s == "-r" || s == "-R" )
        time = randArrival ( argv );
    if ( s == "-f" || s == "-F" )
	{
		if(!finTime)
		{
			cout<<"Could not open file";
			exit(0);
		}
        time = fileArrival ( argv , finTime);
	}

    time += prevTime;
    return time;
}
/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function generate the number of pages for the next document in the 
* queue. This function will call randPage, or filePage to generate the pages 
* in the document. 
*
* @param[in] *argv[] - the name of the argument being passed in
* @param[in] finPage - a file that holds the number of pages per document
*
* @returns page - the total number of pages in the document 
******************************************************************************/
int getPages ( char *argv[], ifstream &finPage )
{
    int page;
    string s;
    s = argv[3];
    if ( s == "-r" || s == "-R" )
        page = randPage();
    if ( s == "-f" || s == "-F" )
	{
		if(!finPage)
		{
			cout<<"Could not open file";
			exit(0);
		}
		page = filePage(finPage);
	}
    return page;
}
/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function the rand() math function to generate a random value
* between 15 and 25 inclusive for the number of pages in the document.
*
* @returns page - the total number of pages in the document 
******************************************************************************/
unsigned int randPage()
{
    unsigned int page;
    page = rand();
    page = page % ( 25 + 1 );
    page = page + 15;
    return page;
}
/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function the rand() math function to generate a random value
* within +/- 30 seconds of the average print time given from command arguments. 
*
* @param[in] *argv[] - the name of the argument being passed in
*
* @returns time - the time of arrival of the next document
******************************************************************************/
unsigned int randArrival ( char *argv[] )
{
    unsigned int time;
    int average;
    average = atoi ( argv[1] );
    time = rand();
    time = time % ( 60 + 1 );
    time = time + average - 30;
    return time;
}
/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function will read in a number from a file and use that to generate the
* number of pages in the next document in the queue
*
* @param[in] finPage - a file that holds the number of pages per document
*
* @returns page - the total number of pages in the document 
******************************************************************************/
unsigned int filePage(ifstream &finPage)
{
    unsigned int page;
    finPage >> page;
    page = page % ( 10 + 1 );
    page = page + 15;
    return page;
}
/**************************************************************************//**
* @authors Dana Jensen, Daniel Carney, Loyde Bjerke, Ryan Quasney
*
* @par Description:
* This function will read in a number from a file and use that to generate the
* time the document arrived in the queue
*
* @param[in] *argv[] - the name of the argument being passed in
* @param[in] finTime - a file that hold the time the document arrived in the 
*						queue
*
* @returns time - the time the document arrives from the last
******************************************************************************/
unsigned int fileArrival ( char *argv[], ifstream &finTime )
{
    unsigned int time;
    int average;
    average = atoi ( argv[1] );
    finTime >> time;
    time = time % ( 60 + 1 );
    time = time + average - 30;
    return time;
}