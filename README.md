# save_terminal_exit
Saves the output in stdout in a .txt of your choice

Use make to create executable "save".

Run with input file
----
$ ./save -i [input file name] [ouput file name] [command]

example: $./save -i myinput.txt output.txt ./myprogram ...


Run without input file
----

$ ./save [ouput file name] [command]

example: $./save outpiut.txt ls -l

