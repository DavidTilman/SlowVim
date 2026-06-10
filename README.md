# SlowVim

A minimal command-line text file editor written in C. Operates on `.txt` files via a simple REPL interface with support for line-level editing, file management, and automatic change logging.

## Building

```sh
gcc main.c fileops.c -o slowvim
```

## Usage

```sh
./slowvim
```

Commands are entered at the `cmd >` prompt. Arguments are prompted interactively after the command.

### Commands

| Command       | Alt   | Arguments                        | Description                              |
|---------------|-------|----------------------------------|------------------------------------------|
| `help`        | `h`   |                                  | Show the help message                    |
| `setfile`     | `set` | `<filename>`                     | Set a default filename for all commands  |
| `unsetfile`   | `uns` |                                  | Clear the default filename               |
| `create`      | `crt` | `<filename>`                     | Create a new `.txt` file                 |
| `deletefile`  | `dfl` | `<filename>`                     | Delete a `.txt` file                     |
| `renamefile`  | `rnm` | `<filename> <new name>`          | Rename a file                            |
| `copy`        | `cpy` | `<src> <dst>`                    | Copy file contents to another file       |
| `show`        | `shw` | `<filename>`                     | Print file contents with line numbers    |
| `append`      | `app` | `<filename> <text>`              | Append a line to a file                  |
| `deleteline`  | `dln` | `<filename> <line number>`       | Delete a specific line                   |
| `insertline`  | `iln` | `<filename> <text> <line number>`| Insert a line at a given position        |
| `showline`    | `sln` | `<filename> <line number>`       | Print a specific line                    |
| `showlines`   | `sls` | `<filename>`                     | Print total line count                   |
| `listdir`     | `dir` |                                  | List all `.txt` files in the working directory |
| `quit`        | `q`   |                                  | Exit the program                         |

If a default file is set with `setfile`, filename arguments are skipped for most commands.

## Change Log

Every file operation is automatically appended to `change-log.txt` in the working directory in the format:

```
[YYYY-MM-DD HH:MM:SS] OPERATION: filename (line count)
```
