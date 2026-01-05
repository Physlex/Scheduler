# GraniteBox

## Overview

This project defines the GraniteBox micro-kernel linux-as-a-framework application development kit.

That's a lot of keywords, so let's break it down into some specifics.


### GraniteBox

GraniteBox (or GBox) is an application development toolkit used to implement cross-platform applications, using an *honest* open-source software license, and without the restrictions that come typical of other equivalent modern application development kits.

For justification, see: `docs/justification.md`


### Requirements

Currently, the project assumes you are building it using Linux or WSL2.

run:

```bash
$ ./tools/install.sh
```

to get dependencies. Then run:

```bash
$ . activate
```

To source the `bake` build wrapper environment.


### Execution

run:

```bash
$ cmake -B build -S .
$ cmake --build build
$ ./build/bin/gbox-cli
```


### TODO: Testing
