# Microsoft Developer Studio Project File - Name="VocabTester" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VocabTester - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VocabTester.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VocabTester.mak" CFG="VocabTester - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VocabTester - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VocabTester - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "VocabTester"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VocabTester - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "VocabTester - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VocabTester - Win32 Release"
# Name "VocabTester - Win32 Debug"
# Begin Group "Library"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Library\Accel.h
# End Source File
# Begin Source File

SOURCE=.\Library\Active.h
# End Source File
# Begin Source File

SOURCE=.\Library\Anim.h
# End Source File
# Begin Source File

SOURCE=.\Library\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Bitmap.h
# End Source File
# Begin Source File

SOURCE=.\Library\Button.h
# End Source File
# Begin Source File

SOURCE=.\Library\Canvas.h
# End Source File
# Begin Source File

SOURCE=.\Library\ChooseColor.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ChooseColor.h
# End Source File
# Begin Source File

SOURCE=.\Library\Class.h
# End Source File
# Begin Source File

SOURCE=.\Library\Clipboard.h
# End Source File
# Begin Source File

SOURCE=.\Library\CmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\Library\ComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ComboBox.h
# End Source File
# Begin Source File

SOURCE=.\Library\Controller.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Controller.h
# End Source File
# Begin Source File

SOURCE=.\Library\Crc.h
# End Source File
# Begin Source File

SOURCE=.\Library\DeferPos.h
# End Source File
# Begin Source File

SOURCE=.\Library\Dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Dialog.h
# End Source File
# Begin Source File

SOURCE=.\Library\DoubleZeroString.h
# End Source File
# Begin Source File

SOURCE=.\Library\DragDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\DragDrop.h
# End Source File
# Begin Source File

SOURCE=.\Library\Edit.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Edit.h
# End Source File
# Begin Source File

SOURCE=.\Library\File.h
# End Source File
# Begin Source File

SOURCE=.\Library\FileList.h
# End Source File
# Begin Source File

SOURCE=.\Library\FindFile.h
# End Source File
# Begin Source File

SOURCE=.\Library\FindFile.old.h
# End Source File
# Begin Source File

SOURCE=.\Library\FolderTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\FolderTree.h
# End Source File
# Begin Source File

SOURCE=.\Library\FolderWatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\FolderWatcher.h
# End Source File
# Begin Source File

SOURCE=.\Library\GetFileName.h
# End Source File
# Begin Source File

SOURCE=.\Library\GlobalMem.h
# End Source File
# Begin Source File

SOURCE=.\Library\Icon.h
# End Source File
# Begin Source File

SOURCE=.\Library\ImageList.h
# End Source File
# Begin Source File

SOURCE=.\Library\ListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ListBox.h
# End Source File
# Begin Source File

SOURCE=.\Library\ListView.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ListView.h
# End Source File
# Begin Source File

SOURCE=.\Library\logger.h
# End Source File
# Begin Source File

SOURCE=.\Library\Mailslot.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Mailslot.h
# End Source File
# Begin Source File

SOURCE=.\Library\Maker.h
# End Source File
# Begin Source File

SOURCE=.\Library\Menu.h
# End Source File
# Begin Source File

SOURCE=.\Library\MessageData.h
# End Source File
# Begin Source File

SOURCE=.\Library\MsgLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\MsgLoop.h
# End Source File
# Begin Source File

SOURCE=.\Library\MultiString.h
# End Source File
# Begin Source File

SOURCE=.\Library\NotifyHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\NotifyHandler.h
# End Source File
# Begin Source File

SOURCE=.\Library\Procedure.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\Library\Rebar.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Rebar.h
# End Source File
# Begin Source File

SOURCE=.\Library\Rect.h
# End Source File
# Begin Source File

SOURCE=.\Library\Reg.h
# End Source File
# Begin Source File

SOURCE=.\Library\RichEdit.h
# End Source File
# Begin Source File

SOURCE=.\Library\Serializer.h
# End Source File
# Begin Source File

SOURCE=.\Library\Shell.h
# End Source File
# Begin Source File

SOURCE=.\Library\ShellDfs.h
# End Source File
# Begin Source File

SOURCE=.\Library\SmartHandle.h
# End Source File
# Begin Source File

SOURCE=.\Library\SmartLib.h
# End Source File
# Begin Source File

SOURCE=.\Library\Splitter.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Splitter.h
# End Source File
# Begin Source File

SOURCE=.\Library\Status.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Status.h
# End Source File
# Begin Source File

SOURCE=.\Library\StrStg.h
# End Source File
# Begin Source File

SOURCE=.\Library\SyncActive.h
# End Source File
# Begin Source File

SOURCE=.\Library\SyncQueue.h
# End Source File
# Begin Source File

SOURCE=.\Library\TaskBar.h
# End Source File
# Begin Source File

SOURCE=.\Library\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Library\tiostream.h
# End Source File
# Begin Source File

SOURCE=.\Library\ToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Library\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\Tools.h
# End Source File
# Begin Source File

SOURCE=.\Library\ToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\ToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Library\TreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\TreeView.h
# End Source File
# Begin Source File

SOURCE=.\Library\tstring.h
# End Source File
# Begin Source File

SOURCE=.\Library\Version.h
# End Source File
# Begin Source File

SOURCE=.\Library\WaveIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\WaveIn.h
# End Source File
# Begin Source File

SOURCE=.\Library\WaveOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\WaveOut.h
# End Source File
# Begin Source File

SOURCE=.\Library\Window.h
# End Source File
# Begin Source File

SOURCE=.\Library\WinString.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\WinString.h
# End Source File
# Begin Source File

SOURCE=.\Library\XWin.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\XWin.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resources\resource.h
# End Source File
# Begin Source File

SOURCE=.\Resources\VocabTester.rc
# End Source File
# End Group
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\control.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\control.h
# End Source File
# Begin Source File

SOURCE=.\Application\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\main.h
# End Source File
# Begin Source File

SOURCE=.\Application\message.h
# End Source File
# Begin Source File

SOURCE=.\Application\PaneNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\PaneNotify.h
# End Source File
# Begin Source File

SOURCE=.\Application\SplitBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Splitbar.h
# End Source File
# End Group
# End Target
# End Project
