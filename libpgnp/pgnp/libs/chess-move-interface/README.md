# Chess Move Interface
The Chess Move Interface (CMI) define a common Interface for standard chess half moves. It allows for a better interoperability among chess libraries.

## CMake Integration
By using the `add_subdirectory()` directive on this repository, you will be able to use the following cmake calls in your project:

    include_directories(${CMI_INCLUDE_DIR})
    target_link_libraries(<YOUR_TARGET> ChessMoveInterface)
Note that the `ChessMoveInterface` target is guaranty to be define only once. Thus, it can be part of several subprojects/submodules in a given project.

## How to use it ?
Create your own implementation of a chess half move by extending `CMI::HalfMove`. See the following projects for more informations:
- [OChess](https://gitlab.com/manzerbredes/ochess) A software for chess games analysis and management
- [CGEditor](https://gitlab.com/manzerbredes/cgeditor) A chess game editor
- [pgnp](https://gitlab.com/manzerbredes/pgnp) An efficient pgn parser