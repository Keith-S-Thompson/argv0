A thread on comp.lang.c in 2011, subject "Teaching C", digressed into a
discussion of whether `argv[0]` can be a null pointer.  The C standard
allows this, but normally `argv[0]` will be a non-null pointer to a
string representing the *program name*.

See `article.txt` for a copy of an article I posted on that thread.

Instructions:

- Run `make` to create `caller` and `callee executables.
- Run `./caller`, which should invoke `./callee`.

`caller` invokes `./callee`, attempting to set its `argv[0]` to
`NULL`.  `callee.c` reports its `argc` and `argv` values.

The behavior reported at the time was the output would be:

```
argc = 0
argv[0] = NULL
```

With Ubuntu 24.04.3 LTS, the output is:

```
argc = 1
argv[0] = ""
argv[1] = NULL
```

I've tracked down the change to commit `dcd46d897adb`
in the Linux kernel.  It refers to
[CVE-2021-4034](https://nvd.nist.gov/vuln/detail/cve-2021-4034).
Apparently `pkexec` would try to travers arguments starting at
`argv[1]`; if `argc1 were 0, then `argv[1]` would point to `envp[0]`.

```
commit dcd46d897adb70d63e025f175a00a89797d31a43
Author: Kees Cook <kees@kernel.org>
Date:   2022-01-31 16:09:47 -0800

    exec: Force single empty string when argv is empty
```

The fix was released in kernels 5.18, 5.19, and 6.0, and has been
backported to other systems.

For non-Linux systems:

- On Cygwin, execve() fails and sets errno=7 (E2BIG), and the callee  
  is not invoked.
- On Android/Termux, I get the same output as on recent Linux systems.  
  This is actually Linux-based, version 5.4.274-moto-g7fd1d430bf42.
- On FreeBSD 14.2, I get the same output as on recent Linux systems.  
  See this change:
  <https://github.com/freebsd/freebsd-src/commit/773fa8cd136a5775241c3e3a70f1997633ebeedf>
- On NetBSD 10.1, I get the bad output of older Linux systems,
  indicating a possible vulnerability to a bug.  I've submitted a bug report,
  [kern/59837](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=59837).  
  A comment points to an earlier bug report,
  [kern/56673](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=56673).  
  Much of the discussion there suggests that the real problem is any
  applications that assume `argv[0] != NULL`.  Apparently an existing
  NetBSD change disallows `argv[0] == NULL` for set*id binaries.

Another discussion of this issue: <https://lwn.net/Articles/882799/>

MS Windows apparently guarantees that `argc > 0` and `argv[0] != NULL`.

<https://learn.microsoft.com/en-us/cpp/c-language/argument-description?view=msvc-170>

> The `argc` parameter in the `main` and `wmain` functions is an
> integer specifying how many arguments are passed to the program
> from the command line. Since the program name is considered an
> argument, the value of `argc` is at least one.
