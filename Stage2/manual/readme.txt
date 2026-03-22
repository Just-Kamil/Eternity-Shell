⡄⠀⡄⠀⠀⠀⠠⡄⠀⠀⠀⠀⠀⠀⠀
⡗⠒⡇⡮⠭⠆⠀⡇⠀⡏⠉⡆⠀⠀⠀
⠃⠀⠃⠑⠒⠂⠀⠑⠂⡗⠒⠁⠀⠃⠀
Welcome to Eternity Shell! (simpleshell)
Here's what you can do.

run "help about" for more information on Eternity Shell (simpleshell).

Commands:

    cd - When given a directory path, changes to that directory, otherwise prints the current directory you're in.
    clr - Clears the text on your screen.
    dir - Lists all the files in the current working directory in long format by default, you can add custom ls tags as the first argument.
    environ - Lists all the current environment variables and their values.
    echo - repeats any inputted text back into the command line.
    pause - pauses the shell until you hit enter.
    script - runs a batch script from the file provided as an argument. (See "Batch Files")
    quit - exits the shell
    help - displays general help information by default, can display more detailed information on a topic given an argument.

if you want more information on any command type "help <command name>" 

Other Commands:

    you can run any program/command that can be found in your PATH variable or via a direct path to the file.
    Just type the name of the command and hit enter.

Batch files:

    you can run a list of commands by creating a "batch" script.

    all you have to do is run the shell with a path to the script as an argument or run "script". (See "Commands")

        $ etsh /path/to/script.etsh

        or

        $ simpleshell /path/to/script
    
    for more information run "help batch"

I/O Redirection:

    You can redirect input and output from/into files.

    To redirect all output into a file:
    [COMMAND] > [FILE]
    This redirect creates a new file or overwrites a file with a given name.

    [COMMAND] >> [FILE]
    Same as previous redirect except it appends to the file.

    To redirect all input from a file:
    [COMMAND] < [FILE]
    This redirect will read a file and send all of the input to the command

    for more information run "help redirect"

Background Execution:

To run a non-built-in process in the background, add an ampersand to the end of the command:

    [COMMAND] [ARGS] &

To run the same process but quietly:

    [COMMAND] [ARGS] &-

for more information run "help background"


Environment variables
  
  etshell (simpleshell) changes two of your environment vairables:
    1. SHELL - the variable that denotes which shell you're using, it gets changed to "simpleshell"
    2. PWD - this environment variable denotes which directory you're currently working in, cd is the command that changes the variable to wherever you cd'd to
