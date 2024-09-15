# Vocabtester

This is a project I wrote in c++ just before I wrote the database 'engine' at https://github.com/millerthegorilla/cplusplusdbase

I am a qualified and experienced TESOL teacher, and so have an interest in augmenting student techniques to learn vocabulary.

From memory Vocabtester has a variety of different tools, including a wordsearch generator.

The project was written using visual C++ 6.0, and so I doubt I will get around to compiling it any time soon to find out precisely what is in it...

The final code (I think) is in the code directory.  It uses the RSWL - the reliable software library - which furnished me with such luxuries as an autopointer, a pointer class with memory management built in to it...  from memory, at least...

As with all my code, it is presented here with an MIT license.

From the original readme (.doc file)

Vocabulary aid C++ project

The aim of this project is to create a program that aids people with learning vocabulary by allowing them to create word lists with the English translations to the right that may be passed to add in programs that create word games using the lists.  The word lists are organised in a list view in the left hand pane of the main window and the word games are have icons in the right pane that may be drag and dropped on to the appropriate word list.  The list view should be fully configurable by the user so that the user can organise the word lists to their own preference.

The add in programs will be windowed programs in their own right.  The icons that represent them in the right hand pane of the application will have a pointer to a string that names the path and filename of the executable.  When an icon is dropped on to a wordlist the wordlist is transformed into a string and passed to the called program.

The windowed app will have a vertical splitter bar and two panes.  In order to create new word lists the user will choose Create new wordlist from the menu bar and a dialog will appear that will have two edit boxes side by side above a listbox with a button to the left of the edit boxes entitled ‘add’.  When ‘add’ is pressed the contents of the edit boxes will be transferred to the edit box.  There will also be a ‘remove’ button and an option to set/change the title of the word list.  This title will be displayed in the list view.  This same dialog box will be called up if the user double clicks on a title in the list view. 
