
# C3
[![Build Status](https://travis-ci.org/rcthomas/C3.svg?branch=master)](https://travis-ci.org/rcthomas/C3)

Parallel CCD preprocessing framework.  

## Developer getting started.

(1) First run `Build.py` --- this creates the top-level Makefile.  The
`Build.py` only needs to be run the first time you check out the repo or after
you change `Build.py` as it is the thing that generates the top-level
Makefile.  A cmake or autotools build system would be nice in the future.
If `Build.py` fails to understand your environment, feel free to send a pull
request.

(2) Then do `cd test && make test` and see how things go.

## Edison developer setup

The Edison environment should be the Intel one, not the gnu one.  You should
load these modules:

    module load boost
    module load cfitsio
    module load gcc 

Before you run `BUILD.py` on Edison be sure to `module load python` to get a
python 2.7 interpreter.
