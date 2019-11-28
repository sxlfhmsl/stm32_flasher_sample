/***************************************************************************************
THIS SOFTWARE/DOCUMENTATION  IS FOR GUIDANCE ONLY. ST MAKES NO WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE nor for any infringement of patents
or other rights of third parties which may result from its use.

ST MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT INCIDENTAL OR 
CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING IN CONNECTION WITH OR ARISING
FROM THE FURNISHING, PERFORMANCE, OR USE OF THIS SOFTWARE.

Specifications mentioned in this publication are subject to change without notice. 
This publication supersedes and replaces all information previously supplied. 
STMicroelectronics products are not authorized for use as critical components in 
life support devices or systems without the express written approval of 
STMicroelectronics.
***************************************************************************************/

/*_________________________________________________________________________________
 *
 *			  MFCSample STMicroelectronics
 *
 *
 * 			    Header File : IOMessage.h
 *
 *	Header file associated with IOMessage.c source file. 
 *  It provides function's prototypes.
 *  
 *	Date : Oct 24 2001
 *
 *  Compiler : Microsoft Visual C++ 6.00
 *
  ________________________________________________________________________________*/


extern long CALLBACK PrintMessage(const char *szMessage);
extern long CALLBACK PrintError(const char *szErrorMsg);
extern long CALLBACK PrintWarning(const char *szErrorMsg);
extern long CALLBACK DisplayProgress(int Percentage);

void OpenLog(char *szFileName);
void CloseLog();

