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
