Running the option for loading or saving game files would be done from the command line with the following parameters:

pacman.exe -load|-save [-silent]


The -load/-save option is for deciding whether the run is for saving files while playing (based on user input) or for loading files and playing the game from the files (ignoring any user input).
In the -load mode there is NO menu
In the -save mode there is a menu, Note that each new game overrides the files of the previous game
The -silent option is relevant only for load - the game shall run without printing to screen and just testing that the actual result corresponds to the expected result
