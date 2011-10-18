/*
 * Dlt Client test utilities - Diagnostic Log and Trace
 * @licence app begin@
 *
 * Copyright (C) 2011, BMW AG - Alexander Wenzel <alexander.wenzel@bmw.de>
 * 
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU Lesser General Public License, version 2.1, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General 
 * Public License, version 2.1, for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License, version 2.1, along 
 * with this program; if not, see <http://www.gnu.org/licenses/lgpl-2.1.html>.
 * 
 * Note that the copyright holders assume that the GNU Lesser General Public License, version 2.1, may 
 * also be applicable to programs even in cases in which the program is not a library in the technical sense.
 * 
 * Linking DLT statically or dynamically with other modules is making a combined work based on DLT. You may 
 * license such other modules under the GNU Lesser General Public License, version 2.1. If you do not want to 
 * license your linked modules under the GNU Lesser General Public License, version 2.1, you 
 * may use the program under the following exception.
 * 
 * As a special exception, the copyright holders of DLT give you permission to combine DLT 
 * with software programs or libraries that are released under any license unless such a combination is not
 * permitted by the license of such a software program or library. You may copy and distribute such a 
 * system following the terms of the GNU Lesser General Public License, version 2.1, including this
 * special exception, for DLT and the licenses of the other code concerned.
 * 
 * Note that people who make modified versions of DLT are not obligated to grant this special exception 
 * for their modified versions; it is their choice whether to do so. The GNU Lesser General Public License, 
 * version 2.1, gives permission to release a modified version without this exception; this exception 
 * also makes it possible to release a modified version which carries forward this exception.
 *
 * @licence end@
 */


/*******************************************************************************
**                                                                            **
**  SRC-MODULE: dlt-test-internal.c                                           **
**                                                                            **
**  TARGET    : linux                                                         **
**                                                                            **
**  PROJECT   : DLT                                                           **
**                                                                            **
**  AUTHOR    : Alexander Wenzel Alexander.AW.Wenzel@bmw.de                   **
**              Markus Klein                                                  **
**                                                                            **
**  PURPOSE   :                                                               **
**                                                                            **
**  REMARKS   :                                                               **
**                                                                            **
**  PLATFORM DEPENDANT [yes/no]: yes                                          **
**                                                                            **
**  TO BE CHANGED BY USER [yes/no]: no                                        **
**                                                                            **
*******************************************************************************/

/*******************************************************************************
**                      Author Identity                                       **
********************************************************************************
**                                                                            **
** Initials     Name                       Company                            **
** --------     -------------------------  ---------------------------------- **
**  aw          Alexander Wenzel           BMW                                **
**  mk          Markus Klein               Fraunhofer ESK                     **
*******************************************************************************/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/

/*
 * $LastChangedRevision: 1670 $
 * $LastChangedDate: 2011-04-08 15:12:06 +0200 (Fr, 08. Apr 2011) $
 * $LastChangedBy$
 Initials    Date         Comment
 aw          08.10.2010   initial
 */

#include <ctype.h>      /* for isprint() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */

#include "dlt.h"
#include "dlt_common.h" /* for dlt_get_version() */

#define MAX_TESTS 1

int vflag = 0;
int tests_passed = 0;
int tests_failed = 0;

void internal1(void);

/**
 * Print usage information of tool.
 */
void usage()
{
    char version[255];

    dlt_get_version(version);

    printf("Usage: dlt-test-internal [options]\n");
    printf("Test application executing several internal tests.\n");
    printf("%s \n", version);
    printf("Options:\n");
    printf("  -v            Verbose mode\n");
    printf("  -1            Execute test 1 (Test ringbuffer)\n");
}

/**
 * Main function of tool.
 */
int main(int argc, char* argv[])
{
    int test[MAX_TESTS];

    int i,c,help;

    for (i=0;i<MAX_TESTS;i++)
    {
        test[i]=0;
    }

    opterr = 0;

    while ((c = getopt (argc, argv, "v1")) != -1)
    {
        switch (c)
        {
        case 'v':
        {
            vflag = 1;
            break;
        }
        case '1':
        {
            test[0] = 1;
            break;
        }
        case '?':
        {
            if (isprint (optopt))
            {
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            }
            else
            {
                fprintf (stderr, "Unknown option character `\\x%x'.\n",optopt);
            }
            /* unknown or wrong option used, show usage information and terminate */
            usage();
            return -1;
        }
        default:
        {
            abort ();
        }
        }
    }

    help=0;
    for (i=0;i<MAX_TESTS;i++)
    {
        if (test[i]==1)
        {
            help=1;
            break;
        }
    }

    if (help==0)
    {
        usage();
        return -1;
    }

    if (test[0])
    {
    	internal1();
    }

    printf("\n");
    printf("%d tests passed\n",tests_passed);
    printf("%d tests failed\n",tests_failed);

    return 0;
}

void internal1(void)
{
    int index,result_index;
	size_t c;
    unsigned int size;

    char buf[1024],result[1024];

    DltRingBuffer mybuf;

    printf("Test1i: Ringbuffer, writing and reading \n");

    for (size=8;size<=30;size++)
    {

        dlt_ringbuffer_init(&mybuf, size);

        memset(result,0,1024);

        if (vflag)
        {
        	printf("\nRingbuffer Size = %d \n\n",size);
        }

        /* Write several times to ringbuffer */
        for (index=0; index<6; index++)
        {
            memset(buf,0,1024);

            sprintf(buf,"%d",index);
            dlt_ringbuffer_put(&mybuf,buf,strlen(buf));

            if (vflag)
            {
                printf("W[%d], Bytes = %d, Hex: ", index, (int)strlen(buf));
                dlt_print_hex((uint8_t *)buf, strlen(buf));
                printf("\n");
            }
        }

        if (vflag)
        {
			printf("\nCount=%d, Max. by buffer size %d = %d\n",mybuf.count, size, (int)(size/(strlen(buf)+sizeof(unsigned int))));
        }

        /* Check value of mybuf.count, counting the elements in ringbuffer */
        if (mybuf.count!=(int)(size/(strlen(buf)+sizeof(unsigned int))))
        {
            tests_failed++;
            printf("Test1i FAILED\n");

            break;
        }

        result_index = 0;

        /* Read several times from ringbuffer */
        for (index=0; index<6; index++)
        {
            memset(buf,0,1024);

            if (dlt_ringbuffer_get(&mybuf,buf,&c)!=-1)
            {
                if (vflag)
                {
                    printf("R[%d], Bytes = %d, Hex: ", index, (int)c);
                    dlt_print_hex((uint8_t *)buf, c);
                    printf("\n");
                }

                if (c==1)
                {
					result[result_index] = buf[0];
                }
                result_index++;
            }
        }

        /* Check value of mybuf.count, counting the elements in ringbuffer, must be 0 now */
        if (mybuf.count!=0)
        {
            tests_failed++;
            printf("Test1i FAILED\n");

            dlt_ringbuffer_free(&mybuf);
            return;
        }

        /* Check the read elements */
        for (index=0; index<result_index; index++)
        {
            sprintf(buf,"%d",((6-result_index)+index));
            if (result[index]!=buf[0])
            {
                tests_failed++;
                printf("Test1i FAILED\n");

                dlt_ringbuffer_free(&mybuf);
                return;
            }
        }

        if (vflag)
        {
        	printf("\n");
        }

        dlt_ringbuffer_free(&mybuf);
    }

    tests_passed++;
    printf("Test1i PASSED\n");
}

