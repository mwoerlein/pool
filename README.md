# POOL, the pure object oriented language
[GridOS](https://github.com/mwoerlein/gridos) |
[Pool](https://github.com/mwoerlein/pool) 

## Everything is an object
Numbers, strings, files, drivers, configuration, hardware descriptions, ... everything is just data.

- Every data can be represented and modified as an object.
- Every object might be located anywhere in a (distributed) system.
- Every object is owned by anyone.
- ...

## System (in)dependency 
Describing data as objects is per se system independent.
An intermediate precompiled representation of pool classes will be the base of each pool runtime.
These representations might be compiled to native code or just interpreted on execution.
 
As part of [GridOS](https://github.com/mwoerlein/gridos) the language must also be capable to specify hardware dependent classes and even assembly code.
To reach this goal, classes/assembly can be tagged to be valid just for specific architectures.

## Roadmap
  - [bootstrap (x86-)compiler (in C++)](https://github.com/mwoerlein/pool/projects/1)
  - [(x86-)compiler (in Pool)](https://github.com/mwoerlein/pool/projects/2)
  - [design intermediate code](https://github.com/mwoerlein/pool/projects/3)
  - ...

## License
Pool is distributed under [GNU General Public License v3.0](https://github.com/mwoerlein/pool/blob/master/LICENSE).  
