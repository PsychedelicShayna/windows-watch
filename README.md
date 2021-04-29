# Windows Watch
Very basic implementation of the watch utility from Linux, but for Windows. Nothing too fancy, but it gets the job done.
If this is your first time hearing about watch, it's essentially a utility that repeats a given command at a set interval
and refreshes the screen as it does so.

## Usage - From The Help Page
```
--help (-h)      |     Prints This Help Page
                 |
--interval (-i)  |     Sets the command repetition interval in milliseconds. Defaults to 1000ms.
                 |
--verbose (-v)   |     Toggles the printing of additional information related to the command's execution.
                 |     Currently only displays the return code after every execution.
                 |
--command (-c)   |     The most important argument. Everything after this will be treated as the command that
                 |     should be watched. This should be placed at the very end of all other arguments.
                 |     In some some cases (e.g. pipes) encapsulating the command in quotes may be necessary.
```

I must stress that `--command (-c)` should be the very last argument, as everything after it will be treated as a command, allowing for syntax such as this: `watch -c ipconfig /all` 
where `ipconfig` and `/all` are two separate argv arguments but get treated as one as they are concatenated to form a single string `ipconfig /all` without the need of quotes.

In some cases, encapsulating the command you want to watch with quotes is going to be necessary, such as when using `>` or `|` as part of the command, since `cmd.exe` won't treat it as plain text otherwise.


### Example
`watch -i 5000 -c "tasklist | find \"firefox\" /i"`
#### Outputs Every 5 Seconds
```
firefox.exe                  18512 Console                    1    396,172 K
firefox.exe                  19132 Console                    1    189,620 K
firefox.exe                  19268 Console                    1     13,932 K
firefox.exe                  18360 Console                    1    429,084 K
firefox.exe                  20068 Console                    1    902,108 K
firefox.exe                  19720 Console                    1     92,124 K
firefox.exe                  20440 Console                    1    251,048 K
firefox.exe                  12580 Console                    1    245,092 K
firefox.exe                  23468 Console                    1    198,384 K
firefox.exe                  22984 Console                    1    204,628 K
firefox.exe                  23796 Console                    1    218,352 K
firefox.exe                  24116 Console                    1    304,460 K
firefox.exe                  13976 Console                    1    170,504 K
firefox.exe                  21972 Console                    1     29,640 K
```
