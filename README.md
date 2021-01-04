# DebugAudioWriter
Juce based simple writer (writes last 10s after button hit)

also includes a button to include a click at the end of a block (to see issues on block borders)

The CMakeLists.txt is not fully operational. This is a subprojekt. You need the following lines in a CMakeLists.txt -file in a  directory above.

```console
cmake_minimum_required(VERSION 3.15)

project(REPOSTART VERSION 0.0.1)

option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

add_subdirectory(DebugAudioWriter)
```

