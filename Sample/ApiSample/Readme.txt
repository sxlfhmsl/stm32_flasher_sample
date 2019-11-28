***************************************************************************************
THIS SOFTWARE AND DOCUMENTATION  IS FOR GUIDANCE ONLY. ST MAKES NO WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, WITH REGARD TO THIS SOFTWARE, nor for any infringement of patents
or other rights of third parties which may result from its use.

STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT INCIDENTAL OR 
CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING IN CONNECTION WITH OR ARISING
FROM THE FURNISHING, PERFORMANCE, OR USE OF THIS SOFTWARE.

Specifications mentioned in this publication are subject to change without notice. 
This publication supersedes and replaces all information previously supplied. 
STMicroelectronics products are not authorized for use as critical components in 
life support devices or systems without the express written approval of 
STMicroelectronics.
***************************************************************************************


STVP_CmdLine.exe is a simple application that illustrates how you can use the STVP 
DLLs (Programming API) to connect to any of the supported programming boards
and program a supported ST device.

When executed, STVP_CmdLine.exe loads dummy application code in the "PROGRAM MEMORY"
(progmem.hex) and in "OPTION BYTE" (option.hex) and then loops on
programming and verification of both memory areas until the <ESC> key is hit.

STVP_CmdLine.exe has been designed for the following programming configuration:
An STM8L15xC6 using the SWIM protocol with the ST-LINK connected 
to an USB port.
Before using STVP_CmdLine.exe you will have to set your own configuration in main.cpp.

However, STVP_CmdLine.exe can be called with arguments and it is not mandatory to 
recompile it for your configuration.
If no argument options are passed to the program, the default configuration is used.
Type STVP_CmdLine -help to display help:

Usage:STVP_CmdLine [-BoardName=STxxx] [-Port=xxx] [-ProgMode=xxx] [-Device=STxxx]
[-help] [-version] [-verbose]|[-no_verbose] [-log]|[-no_log] [-loop]|[-no_loop]
[-no_progProg] [-no_progData] [-no_progOption] [-progress]|[-no_progress]
[-warn_protect]|[-no_warn_protect] [-readProg] [-readData] [-readOption]
[-erase]|[-no_erase] [-blank]|[-no_blank] [-verif]|[-no_verif]
[-FileProg=fname.hex/s19] [-FileData=fname.hex/s19] [-FileOption=fname.hex/s19]
With:
Arguments options description :
[-BoardName=STxxx] ==========> Programming Tool name
[-Port=xxx] =================> Communication Port (USB, LPT1)
[-ProgMode=xxx] =============> Programming mode (protocol)
[-Device=STxxx] =============> Device name
[-version] ==================> Display version of the STVP_CmdLine.exe application
[-verbose] ==================> Display messages, warnings, errors
[-log] ======================> Generate or append Result.log log file
[-loop] =====================> Loop on actions until ESC key hit
[-progress] =================> Display progress of each action
[-warn_protect] =============> Message Box if programming Option Byte protection
[-no_progProg] ==============> Do not program PROGRAM MEMORY (used to verify device from a file)
[-no_progData] ==============> Do not program DATA MEMORY (used to verify device from a file)
[-no_progOption] ============> Do not program OPTION BYTE (used to verify device from a file)
[-readProg] =================> Read PROGRAM MEMORY
[-readData] =================> Read DATA MEMORY
[-readOption] ===============> Read OPTION BYTE
[-erase] ====================> Erase the device (before programming)
[-blank] ====================> Blank Check the device (before programming)
[-verif] ====================> Verify the device after programming
[-FileProg=fname.hex/s19] ===> File name to program PROGRAM MEMORY area (hex or s19)
[-FileData=fname.hex/s19] ===> File name to program DATA MEMORY area (hex or s19)
[-FileOption=fname.hex/s19] => File name to program OPTION BYTE area (hex or s19)

Argument options are NOT case sensitive.

The STVP_CmdLine.exe program exits with code 0 if no error, 1 if error.

To ease the API access, a set of generic C sources files, called the "Programming 
ToolKit," have been designed to interface with the DLLs API. 

========================================================================
       ST PROGRAMMING TOOLKIT: Generic files
========================================================================
PtkAPI.cpp    : the file in charge of loading the DLLs and calling each API 
		function in the appropriated DLLS 
 
PtkToolBox.cpp: the file containing helper function that allow you to get more
		information from the configuration files (.cnf) for example a 
		list of supported hardware, list of protocols supported by a 
		hardware, devices that can be driven using a given protocol on a 
		given board, the list of memory areas for a given device,...
PtkAPI.h      :	Header file for PtkAPI.cpp
PtkToolBox.h  :	Header file for PtkToolBox.cpp

===========================================
      STVP_CmdLine.exe specific Files :
===========================================

main.cpp      : contains the main function
callbacks.cpp : contains API callbacks initialisations (for progress display,
                error and message notification) and log file access
callbacks.h   : Header file for callbacks.cpp


========================================================
      Compiler used to build this STVP_CmdLine.exe :
========================================================

STVP_CmdLine was developped with Microsoft Visual C++ 6.0.
Using another compiler will probably entail some modifications.


==================================================
      Files required to run STVP_CmdLine.exe :
==================================================

To execute STVP_CmdLine.exe, the following files should be added in Release directory :
. All the DLLs that are provided with STVP package
. the dbcfile.cnf and tools.cnf configuration files
. It is also recommended that you add all the .s19 and .bin files delivered within STVP.
  Note that all the .s19 files are not required, however identifying and adding only
  the necessary files is very time consuming. This requires running the STVP_CmdLine.exe
  several times, checking the log file and see which files are missing, then copying 
  the missing files to the directory.
. Config directory, which is required for some targets
. progmem.hex and option.hex, which are dummy ST binary code that is used to 
  program the device's "PROGRAM MEMORY" and the "OPTION BYTE," respectively.


