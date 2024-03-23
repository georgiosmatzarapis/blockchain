# Blockchain

This is an implementation approach of a distributed and decentralized system following the principles of Blockchain 1.0, representing the first generation of blockchain technology. Having this, I have chosen as best match candidate for such system the bitcoin blockchain ledger.

The idea came up from my need to develop my modern C++ skills and apply them to an interesting and challenging project. Of course, I do not go through all the implementation details and complexity that bitcoin solution carries on, but I try to follow the basic mechanisms.

Please, follow the repository [issues](https://github.com/georgiosmatzarapis/blockchain/issues) to track the current progress.

## Environment

Windows subsystem for Linux (WSL) with Ubuntu-22.04 distribution, has been chosen as my development space.

On top of this,

- Default C++ standard: C++23.
- Compiler: GNU gcc/g++ packages both in 13.1.0 version.
- Debugger: GNU gdb (GDB) package in 13.2 version.
- cmake: version 3.27.7 - suite maintained and supported by Kitware.
- ninja: version 1.11.1.

Editor: I have chosen Visual Studio Code (version: 1.87.2) for the development of the current project, as it works pretty smoothly with WSL and provides the desired level of configurability.

Also, to highlight that CMake has been chosen for the build orchestration using the ninja build tool.

## Acknowledgments

Special thanks to the creators and maintainers of the following projects and libraries:

- [GNU/Linux](https://www.gnu.org/gnu/linux-and-gnu.en.html)
- [Kitware](https://kitware.com/cmake)
- [OpenSSL](https://www.openssl.org/)
- [Bitcoin](https://github.com/bitcoin/bitcoin)

---

### Author

@[georgiosmatzarapis](https://georgiosmatzarapis.com)
