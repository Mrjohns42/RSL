VLC Portable Launcher 1.3
=========================
Copyright (C) 2004-2007 John T. Haller of PortableApps.com

Website: http://PortableApps.com/VLCPortable

This software is OSI Certified Open Source Software.
OSI Certified is a certification mark of the Open Source Initiative.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.


ABOUT VLC PORTABLE
==================
The VLC Portable Launcher allows you to run VLC from a removable drive whose letter changes as you move it to another computer.  It allows you to split the program directory from your profiles directory without hand editing any files.  The program can be entirely self-contained on the drive and then used on any Windows computer.


LICENSE
=======
This code is released under the GPL.  The full code is included with this package as VLCPortable.nsi.


INSTALLATION / DIRECTORY STRUCTURE
==================================
By default, the program expects one of these directory structures:

-\ <--- Directory with VLCPortable.exe
  +\App\
    +\vlc\
  +\Data\
    +\settings\

OR

-\ <--- Directory with VLCPortable.exe
  +\VLCPortable\
    +\App\
      +\vlc\
    +\Data\
      +\settings\


It can be used in other directory configurations by including the VLCPortable.ini file in the same directory as VLCPortable.exe and configuring it as details in the INI file section below.


VLCPortable.INI CONFIGURATION
=============================
The VLC Portable Launcher will look for an ini file called VLCPortable.ini (see above section for complete details).  If you are happy with the default options, it is not necessary, though.  The INI file is formatted as follows:

[VLCPortable]
VLCDirectory=App\vlc
SettingsDirectory=Data\settings
VLCExecutable=vlc.exe
AdditionalParameters=
WaitForVLC=false
DisableSplashScreen=false

The VLCDirectory and SettingsDirectory entries should be set to the *relative* path to the appropriate directories from the current directory.  All must be a subdirectory (or multiple subdirectories) of the directory containing VLCPortable.exe.  The default entries for these are described in the installation section above.

The AdditionalParameters entry allows you to pass additional commandline parameter entries to vlc.exe.  Whatever you enter here will be appended to the call to vlc.exe.

The VLCExecutable entry allows you to set the VLC Portable Launcher to use an alternate EXE call to launch VLC.  This is helpful if you are using a machine that is set to deny vlc.exe from running.  You'll need to rename the vlc.exe file and then enter the name you gave it on the vlcexecutable= line of the INI.

The WaitForVLC entry allows you to set the VLC Portable Launcher to wait for VLC to close before it closes.  This option is mainly of use when VLCPortable.exe is called by another program that awaits it's conclusion to perform a task.

The DisableSplashScreen entry allows you to run the VLC Portable Launcher without the splash screen showing up.  The default is false.


PROGRAM HISTORY / ABOUT THE AUTHORS
===================================
This launcher contains ideas from multiple sources.  It is loosely based on the original Portable Firefox Launcher, which contained ideas from mai9, tracon and myself.