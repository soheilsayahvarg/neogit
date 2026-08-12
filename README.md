# neogit

A version control system written from scratch in C — no libraries, no `libgit2`, just
the C standard library and POSIX file I/O.

`neogit` reimplements the parts of Git that matter for understanding how a VCS actually
works: a staging area, commits, branches, checkout, revert, tags, log filtering, and a
configurable pre-commit hook framework. Roughly 3,700 lines across 16 source files.

Written as the final project for **Fundamentals of Programming** (Sharif University of
Technology, 2023–24) — my first year of undergrad, and my first large C program.

---

## Why this exists

Most people learn Git as a set of commands to memorize. The fastest way past that is to
build one. The interesting problems turn out not to be the commands themselves but the
machinery underneath them:

- **Repository discovery** — walking up the directory tree from any subdirectory to find
  the enclosing `.neogit/`, the way Git does, so commands work from anywhere in the tree.
- **Staging as real state** — the index has to survive between invocations, since each
  command is a separate process with no memory of the last one.
- **Snapshot and restore** — checkout has to reconstruct a working tree from a commit and
  correctly handle files that were added, deleted, or had their permissions changed.
- **Undo semantics** — `add -redo` and `reset -undo` require the last operation to be
  replayable, which means recording it rather than just applying it.

---

## Building

Linux (or WSL). The code uses POSIX APIs — `dirent.d_type`, `mkdir(path, mode)`,
`chdir`, `getcwd` — so it does not build on Windows with MinGW.

```bash
make
```

No configuration needed. Global settings live in `$HOME/.neogit-global/`, which is
created on first use. To run `neogit` from anywhere, put the built binary on your `PATH`.

---

## Quick start

```bash
neogit init                                  # create a .neogit repository
neogit config user.name  "Your Name"
neogit config user.email "you@example.com"

neogit add -f main.c utils.c                 # stage files
neogit status                                # inspect the working tree
neogit commit -m "initial commit"

neogit log -n 5                              # the last 5 commits
neogit branch feature                        # create a branch
neogit checkout feature                      # switch to it
```

---

## Command reference

### Repository and configuration

| Command | Description |
| --- | --- |
| `neogit init` | Create a `.neogit/` repository in the current directory |
| `neogit config user.name <value>` | Set the repository-local author name |
| `neogit config user.email <value>` | Set the repository-local author email |
| `neogit config -global user.name <value>` | Set the global author name |
| `neogit config alias.<name> "<command>"` | Define a command alias |
| `neogit config -global alias.<name> "<command>"` | Define a global alias |
| `neogit <alias>` | Run a defined alias |

### Staging

| Command | Description |
| --- | --- |
| `neogit add -f <file>...` | Stage one or more files or directories |
| `neogit add -n <depth>` | Stage files up to a given directory depth |
| `neogit add -redo` | Replay the previous `add` |
| `neogit reset -f <file>...` | Unstage specific files |
| `neogit reset -undo` | Undo the last staging operation |
| `neogit status` | Show added / modified / deleted files and permission changes |

### Committing

| Command | Description |
| --- | --- |
| `neogit commit -m "<message>"` | Commit the staged files |
| `neogit commit -s <shortcut>` | Commit using a saved message shortcut |
| `neogit set -m "<message>" -s <shortcut>` | Save a message under a shortcut name |
| `neogit replace -m "<message>" -s <shortcut>` | Overwrite an existing shortcut |
| `neogit remove -s <shortcut>` | Delete a shortcut |

### History

| Command | Description |
| --- | --- |
| `neogit log` | Show the full commit history |
| `neogit log -n <count>` | Show the most recent `<count>` commits |
| `neogit log -branch <name>` | Filter by branch |
| `neogit log -author <name>` | Filter by author |
| `neogit log -since <date>` | Commits after a date |
| `neogit log -before <date>` | Commits before a date |
| `neogit log -search <word>...` | Search commit messages, with wildcard support |

### Branching and navigation

| Command | Description |
| --- | --- |
| `neogit branch` | List branches |
| `neogit branch <name>` | Create a branch |
| `neogit checkout <branch>` | Switch branches |
| `neogit checkout <commit-id>` | Check out a specific commit (detached) |
| `neogit checkout HEAD` | Return to the branch tip |
| `neogit revert -n <count>` | Revert the last `<count>` commits |
| `neogit revert <commit-id>` | Revert to a specific commit |
| `neogit revert -m "<message>"` | Revert with a custom commit message |

### Tags

| Command | Description |
| --- | --- |
| `neogit tag` | List all tags |
| `neogit tag show <name>` | Show a tag's details |
| `neogit tag -a <name> [-m "<msg>"] [-c <commit>] [-f]` | Create a tag, optionally with a message, bound to a specific commit, forcing overwrite |

### Search

| Command | Description |
| --- | --- |
| `neogit grep -f <file> -p <pattern>` | Search a file for a pattern, with the match highlighted |
| `neogit grep -f <file> -p <pattern> -n` | Include line numbers |
| `neogit grep -f <file> -p <pattern> -c <commit>` | Search the file as of a given commit |

### Pre-commit hooks

| Command | Description |
| --- | --- |
| `neogit pre-commit hooks list` | List all available hooks |
| `neogit pre-commit applied hooks` | List hooks enabled in this repository |
| `neogit pre-commit add hook <id>` | Enable a hook |
| `neogit pre-commit remove hook <id>` | Disable a hook |
| `neogit pre-commit -f <file>` | Run the enabled hooks against a single file |

Enabled hooks run automatically on `neogit commit` and block the commit on failure.

| Hook | Checks |
| --- | --- |
| `todo-check` | No leftover `TODO` markers |
| `eof-blank-space` | No trailing blank space at end of file |
| `format-check` | Basic source formatting |
| `balance-braces` | Braces are balanced |
| `file-size-check` | File is under the size limit (5 KB) |
| `character-limit` | File is under the character limit (20,000) |
| `indentation-check` | *(registered, not implemented)* |
| `static-error-check` | *(registered, not implemented)* |
| `time-limit` | *(registered, not implemented)* |

---

## How it works

A repository is a plain directory tree under `.neogit/`. There is no object database and
no hashing — every commit stores a full copy of its files. That is deliberately simpler
than Git's content-addressed model, and the tradeoff is discussed under
[Known limitations](#known-limitations).

```text
.neogit/
├── config                    # repository-local user.name / user.email
├── alias/                    # command aliases
├── branch                    # name of the current branch
├── branches                  # every branch that exists
├── stage/                    # copies of currently staged files
├── all_stage                 # the staging index
├── add, last_add             # last add operation, for -redo / -undo
├── commits_data/commit N     # author, email, branch, message, timestamp
├── commits_files/commit N/   # full snapshot of the files in commit N
├── commits_message           # saved commit messages
├── commits_shortcuts         # shortcut name -> message mapping
└── last_commit_id            # monotonic commit counter
```

Commit metadata is stored as readable text, one field per line:

```text
username : soheil, useremail : soheilsayahvarg@gmail.com
branch : master
message : initial commit
date : Mon Feb  5 23:33:28 2024
```

### Source layout

| File | Responsibility |
| --- | --- |
| `src/main.c` | Command dispatch |
| `src/init.c` | Repository creation and upward `.neogit/` discovery |
| `src/config.c` | Local and global configuration, aliases |
| `src/add.c` | Staging, depth-limited add, redo |
| `src/reset.c` | Unstaging and undo |
| `src/status.c` | Working-tree diff, file comparison, permission checks |
| `src/commit.c` | Commit creation, message shortcuts |
| `src/log.c` | History display and filtering |
| `src/branch.c` | Branch creation and listing |
| `src/checkout.c` | Working-tree reconstruction from a commit |
| `src/revert.c` | Reverting commits |
| `src/tag.c` | Tag creation and lookup |
| `src/pre-commit.c` | Hook framework and hook implementations |
| `src/grep.c` | File search with match highlighting |
| `src/time.c` | Date parsing and comparison for log filters |

---

## Known limitations

These are real and worth stating plainly rather than leaving for a reader to discover:

- **Linux only.** POSIX-specific calls (`dirent.d_type`, `mkdir` with a mode argument)
  mean the code does not compile on Windows without changes.
- **Exit codes follow an internal convention, not the POSIX one.** Commands return `1`
  for success and `0` for failure, which is inverted from what a shell expects, so
  `neogit init && neogit add -f x` does not chain correctly.
- **Snapshots are full file copies.** Storage grows linearly with the number of commits
  rather than with the amount of changed content. Git solves this with content-addressed
  blobs; that was out of scope here.
- **Three hooks are declared but unimplemented.** `indentation-check`,
  `static-error-check`, and `time-limit` are wired into the hook dispatcher but only
  detect the relevant file type — they do not perform the check.
- **No test suite.** `tests/` holds fixture data from manual testing, not automated tests.

---

## Author

## License

Released under the [MIT license](LICENSE).

---

Soheil Sayah Varg — B.Sc. Computer Engineering, Sharif University of Technology.
