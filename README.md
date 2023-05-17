# Extension of Simple Shell

## New Functionality
- Added functionality to allow for piping commands. For example, ls -laF | tail would pipe the output of the ls command as the input the tail utility command. 
- Added "alias" Bash Builtin Commands to the shell.

## Implementation Specifics
- Continously divide and parse through the input string to get a list of commands
- Start reading from the original stdin and initialize a new pipe between each command execution. For the last command read from last pipe and use the parent process stdout for outputting.
- Add a hashmap to shell class to store key-value pairs.
- Accept "key=value" pairs passed by "alias" and add them to the hashmap.
- When "alias" is called with no flags or "-p" print the key-value pairs in "alias \<key\>=\<value\>" format.