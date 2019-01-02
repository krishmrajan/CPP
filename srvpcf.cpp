/***************************************************************************/
/*                                                                         */
/* (c) Copyright IBM Corp. 1997  All rights reserved.                      */
/*                                                                         */
/* This sample program is owned by International Business Machines         */
/* Corporation or one of its subsidiaries ("IBM") and is copyrighted       */
/* and licensed, not sold.                                                 */
/*                                                                         */
/* You may copy, modify, and distribute this sample program in any         */
/* form without payment to IBM,  for any purpose including developing,     */
/* using, marketing or distributing programs that include or are           */
/* derivative works of the sample program.                                 */
/*                                                                         */
/* The sample program is provided to you on an "AS IS" basis, without      */
/* warranty of any kind.  IBM HEREBY  EXPRESSLY DISCLAIMS ALL WARRANTIES,  */
/* EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.     */
/* Some jurisdictions do not allow for the exclusion or limitation of      */
/* implied warranties, so the above limitations or exclusions may not      */
/* apply to you.  IBM shall not be liable for any damages you suffer as    */
/* a result of using, modifying or distributing the sample program or      */
/* its derivatives.                                                        */
/*                                                                         */
/* Each copy of any portion of this sample program or any derivative       */
/* work,  must include a the above copyright notice and disclaimer of      */
/* warranty.                                                               */
/*                                                                         */
/***************************************************************************/
/*                                                                         */
/* The following is a small sample PCF program, provided by Ron Bower,     */
/* IBM SDMTSC, that can be used to change the batchsize parameter on a     */
/* channel.  It is run by typing:                                          */
/*                                                                         */
/*   srvpcf /c<channel> /b<batchsize> /m<qmgr>                             */
/*     or                                                                  */
/*   srvpcf -c<channel> -b<batchsize> -m<qmgr>                             */
/*                                                                         */
/* The parameters are as follows:                                          */
/*                                                                         */
/*   /c<channel>: the channel name to change                               */
/*   /b<batchsize>: the new batchsize                                      */
/*   /m<qmgr>: the queue manager that has the channel                      */
/*                                                                         */
/* The program does minimal error checking.  It is strictly designed as a  */
/* sample program to show how PCF commands are built and sent to the       */
/* command server.                                                         */
/*                                                                         */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream.h>
#include <iomanip.h>

#include <cmqc.h>
#include <cmqcfc.h>
#include <cmqxc.h>

long parseArgs(short argc, char *argv[]);

#define BUFFERLEN 8192
char cBatchSize[10];
char cQMGR[50];
char cChannel[MQ_CHANNEL_NAME_LENGTH+1];

int main(int argc, char **argv)
{
  MQOD     od  = {MQOD_DEFAULT};   /* Object Descriptor for reply   */
  MQOD     odr = {MQOD_DEFAULT};   /* Object Descriptor for reply   */
  MQMD     md  = {MQMD_DEFAULT};   /* Message Descriptor            */
  MQPMO    pmo = {MQPMO_DEFAULT};  /* put message options           */
  MQGMO    gmo = {MQGMO_DEFAULT};  /* get message options           */
  MQHCONN  Hcon;                   /* connection handle             */
  MQLONG   O_options;
  MQHOBJ   Hreply;
  MQLONG   OpenCode;
  MQLONG   CompCode;               /* completion code               */
  MQLONG   Reason;                 /* reason code (MQOPEN,MQCLOSE)  */
  MQLONG   CReason;                /* reason code (MQCONN)          */
  MQLONG   messlen;                /* message length received       */
  MQLONG   replylen;
  char    *pBuffer;
  char    *pParmPtr;
  int      iBatch;
  MQCFST  *pMQCFST;
  MQCFH   *pMQCFH;
  MQCFH    oPCFHeader      = {MQCFH_DEFAULT};
  MQCFST   sParm1ChanName  = {MQCFST_DEFAULT};
  MQCFIN   sParm2ChanType  = {MQCFIN_DEFAULT};
  MQCFIN   sParm3BatchSize = {MQCFIN_DEFAULT};

  /************************************************************/
  /* Verify that the user inputs have been entered correctly. */
  /************************************************************/
  if (argc != 4) {
     cout << "Program usage: srvpcf /c<channel> /b<batch> /m<qmgr>" << endl;
     exit(0);
  } else {
     if (parseArgs(argc, argv)) {
        cout << "Program not run correctly." << endl;
        cout << "Program usage: srvpcf /c<channel> /b<batch> /m<qmgr>" << endl;
        exit(0);
     } else {
        iBatch = atoi(cBatchSize);
     } /* endif */
  }

  /*******************************************/
  /* Connect to the QMGR.  Exit if an error. */
  /*******************************************/
  MQCONN(cQMGR, &Hcon, &CompCode, &CReason);
  if (CompCode == MQCC_FAILED) {
      cout << "MQCONN failed with reason code: " << CReason << endl;
      exit(CReason);
  }

  /*********************************************/
  /* Build the reply queue for the PCF results */
  /*********************************************/
  O_options = MQOO_INPUT_EXCLUSIVE + MQOO_FAIL_IF_QUIESCING;
  strcpy(odr.ObjectName, "SYSTEM.SAMPLE.REPLY");
  strcpy(odr.DynamicQName, "PCF_QUEUE");
  MQOPEN(Hcon, &odr, O_options, &Hreply, &OpenCode, &Reason);
  if (Reason != MQRC_NONE) {
     cout << "MQOPEN reply failed with reason: " << Reason << endl;
     exit(CReason);
  } /* endif */

  /******************************************************/
  /* Allocate the buffer that will hold the command.    */
  /* Ordinarily, the buffer allocation would be for a   */
  /* size that would hold just what is needed.  For     */
  /* this run, the buffer will just be very large and   */
  /* the program won't have to worry about overwriting. */
  /******************************************************/
  pBuffer = (char *)malloc(BUFFERLEN);

  /*******************************************************/
  /* Build the PCF Header first.                         */
  /*   The PCF command that will be executed is a change */
  /*   channel command to alter the batch size for the   */
  /*   channel.                                          */
  /*******************************************************/
  oPCFHeader.Command        = MQCMD_CHANGE_CHANNEL;
  oPCFHeader.ParameterCount = 3;

  pParmPtr = pBuffer; /* pParmPtr will be incremented through  */
                      /* the buffer that holds the PCF command */
  memcpy(pParmPtr, &oPCFHeader, sizeof(oPCFHeader));
  pParmPtr = pParmPtr + sizeof(oPCFHeader);

  /***************************************************/
  /* Build the parameters next                       */
  /*   Parameter 1 is the name of the channel        */
  /*   Parameter 2 is the type of the channel.  This */
  /*     is hard-coded to SENDER.                    */
  /*   Parameter 3 is the batch size of the channel  */
  /***************************************************/
  sParm1ChanName.Parameter    = MQCACH_CHANNEL_NAME;
  sParm1ChanName.StrucLength += MQ_CHANNEL_NAME_LENGTH;
  sParm1ChanName.StringLength = strlen(cChannel);
  memcpy(pParmPtr, &sParm1ChanName, sizeof(sParm1ChanName));
  pParmPtr += MQCFST_STRUC_LENGTH_FIXED;
  strcpy(pParmPtr, cChannel);
  pParmPtr += MQ_CHANNEL_NAME_LENGTH;

  sParm2ChanType.Parameter    = MQIACH_CHANNEL_TYPE;
  sParm2ChanType.Value        = MQCHT_SENDER;
  memcpy(pParmPtr, &sParm2ChanType, sizeof(sParm2ChanType));
  pParmPtr += sizeof(sParm2ChanType);

  sParm3BatchSize.Parameter    = MQIACH_BATCH_SIZE;
  sParm3BatchSize.Value        = iBatch;
  memcpy(pParmPtr, &sParm3BatchSize, sizeof(sParm3BatchSize));

  /********************************************************/
  /* Build the rest of the structures so that the command */
  /* can be sent over to the QMGR.                        */
  /********************************************************/
  messlen =   sizeof(oPCFHeader)
            + sParm1ChanName.StrucLength
            + sizeof(sParm2ChanType)
            + sizeof(sParm3BatchSize);

  md.MsgType  = MQMT_REQUEST;
  md.Feedback = MQFB_NONE;
  strcpy(od.ObjectName, "SYSTEM.ADMIN.COMMAND.QUEUE");
  strncpy(md.ReplyToQ, odr.ObjectName, MQ_Q_NAME_LENGTH);
  memcpy(md.Format, MQFMT_ADMIN, MQ_FORMAT_LENGTH);

  MQPUT1(Hcon, &od, &md, &pmo, messlen,
         pBuffer, &CompCode, &Reason);

  free(pBuffer);
  if (Reason != MQRC_NONE) {
     cout << "MQPUT1 failed with reason code: " << Reason << endl;
  } else {
     /***********************************************/
     /* Let's get the results and see if it worked. */
     /***********************************************/
     gmo.WaitInterval  = 30000;
     gmo.Options       =   MQGMO_WAIT
                         + MQGMO_CONVERT + MQGMO_ACCEPT_TRUNCATED_MSG;
     md.Encoding       = MQENC_NATIVE;
     md.CodedCharSetId = MQCCSI_Q_MGR;
     memcpy(md.MsgId, MQMI_NONE, sizeof(md.MsgId));
     memcpy(md.CorrelId, MQMI_NONE, sizeof(md.CorrelId));
     pMQCFH  = (MQCFH *)malloc(BUFFERLEN);

     MQGET(Hcon, Hreply, &md, &gmo, BUFFERLEN, pMQCFH, &replylen,
           &CompCode, &Reason);

     if (CompCode != MQCC_OK) {
        cout << "MQGET results failed Comp/Reason: "
             << CompCode << "/" << Reason << endl;
     } else {
        cout << "Results: " << endl;
        cout << "  CompCode: "  <<  pMQCFH->CompCode        << endl;
        cout << "  Reason  : "  <<  pMQCFH->Reason          << endl;
     }
     free(pMQCFH);
  }

  MQCLOSE(Hcon, &Hreply, MQCO_DELETE, &CompCode, &Reason);
  if (Reason != MQRC_NONE) {
     cout << "MQCLOSE reply failed: " << Reason << endl;
  } /* endif */

  if (CReason != MQRC_ALREADY_CONNECTED) {
     MQDISC(&Hcon, &CompCode, &Reason);

     if (Reason != MQRC_NONE) {
        cout << "MQDISC ended with reason code: " << Reason << endl;
     }
  }

}

long parseArgs(short argc, char *argv[])
{
    short   i;
    short iChannel, iBatch, iQMGR = 0;

    if ( argc > 1 ) {
       for (i=1; i < argc; i++ ) {

          /* is it an option switch */
          if ((*argv[i] == '/') || (*argv[i] == '-')) {
             switch ( *(argv[i] + 1) ) {
                case 'c':
                case 'C':
                   strcpy(cChannel, &argv[i][2]);
                   cout << "Channel=" << cChannel << endl;
                   iChannel = 1;
                   break;

                case 'b':
                case 'B':
                   strcpy(cBatchSize, &argv[i][2]);
                   cout << "Batch  =" << cBatchSize << endl;
                   iBatch = 1;
                   break;

                case 'm':
                case 'M':
                   strcpy(cQMGR, &argv[i][2]);
                   cout << "QMGR   =" << cQMGR << endl;
                   iQMGR = 1;

             } /* endswitch */
      