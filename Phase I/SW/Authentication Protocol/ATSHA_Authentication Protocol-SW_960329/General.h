/*******************************************************************************
********************************************************************************
*@ File:	Main.cpp
****************************************
*@ Company:	Sadr Electronic Dena Co.

*@ Version:	1.0
*@ Update:	Last:95/09/18	Previous:95/09/09	Created:95/09/09
*@ Author:	M. Aghamohamadian 

*@ Brief:	This header is used in main.cpp of program for AT45DB321D QC test.

********************************************************************************
*******************************************************************************/

/*----- Include Files --------------------------------------------------------*/
#ifndef GENERAL_h
#define GENERAL_h


/*----- CONSTANT DEFINITIONS -------------------------------------------------*/
#define       BUFFER_SIZE            528
#define       MIN_BYTES_PER_PAGE     500 // Minimum bytes to be read correctly from a page to ensure that the content of that page is read correctly.     

/*------ Global Variable Definition ------------------------------------------*/
 extern    char    buffer_data[528];


/*------ Function Definition -------------------------------------------------*/
void setup(void);
/*
Input: 
          No input.
Output: 
          No output.
Description:
          This functions sets up the microcontroller registors.
*/

char verify_data(int page, int page_size);
/*
Input: 
          page:  The page to be read from memory.
          page_size:  Size of each page of memory.
Output: 
          No output.
Description:
          This functions verifies that the page data is read correctly
          by comparing it against the default data saved in that page.
*/

#endif 


