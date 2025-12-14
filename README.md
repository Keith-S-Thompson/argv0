A thread on comp.lang.c in 2011, subject "Teaching C", digressed into
a discussion of whether `argv[0]` can be a null pointer.

See `article.txt` for a copy of an article I posted on that thread.

`caller.c` invokes `./callee`, attempting to set its `argv[0]` to
`NULL`.  `callee.c` reports its `argc` and `argv` values.

The behavior reported at the time was that `callee` would have  
`argc == 0 && argv[0] == NULL`.

I've been unable to reproduce this behavior on Ubuntu 24.04.3 LTS.
The output of `callee` when invoked by `caller` is:

```
argc = 1
argv[0] = ""
argv[1] = NULL
```

My best guess is that the system (glibc? the kernel?) has been updated
to prevent programs being invoked with `argv[0] == NULL`.

TODO: Spin up an older or different OS and see if I can reproduce
the old behavior.

Done.  With Ubuntu 10.04 LTS, running `./caller` produces this output from `./callee`:

```
argc = 0
argv[0] = NULL
```

With Ubuntu 24.04 LTS, this is the output:

```
argc = 1
argv[0] = ""
argv[1] = NULL
```

And I've tracked down the change to commit
`dcd46d897adb` in the Linux kernel.  It refers to
[CVE-2021-4034](https://nvd.nist.gov/vuln/detail/cve-2021-4034).
Apparently `pkexec` would try to travers arguments  starting at
`argv[1]`; if `argc1 were 0, then `argv[1]` would point to `envp[0]`.

```
commit dcd46d897adb70d63e025f175a00a89797d31a43
Author: Kees Cook <kees@kernel.org>
Date:   2022-01-31 16:09:47 -0800

    exec: Force single empty string when argv is empty
```

The fix was released in kernels 5.18, 5.19, and 6.0.
