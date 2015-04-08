
yaml-cpp
========

This is based on version 0.5.2 which supports full YAML 1.2.

This version depends on boost, but the developer is working on a version that
just uses C++11 constructs.  So when that happens we can quit depending on
boost.

The yaml-cpp package uses CMake and I had trouble getting it set up on edison.
It was much easier to just dump the source files in here and make my own 
makefile.  Someone smarter than me could back that out.

Also we could depend on an external yaml-cpp module but I have not asked for 
anyone at NERSC or HPCports to support it.
