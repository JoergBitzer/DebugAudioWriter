# DebugAudioWriter
## Description
Juce based simple audio signal writer (writes last 10s after button hit in the directory c:\Temp or ~/Temp with the filename TimeSinceIdontKnow_in_ms.wav)
also includes a button to include a click (last sample = 1) at the end of each processed audio block (to see issues on block borders)

## Build remarks
The included CMakeLists.txt is not fully operational. 
This is a subprojekt. You need the following lines in a CMakeLists.txt -file in a  directory above.

```console
cmake_minimum_required(VERSION 3.15)

project(REPOSTART VERSION 0.0.1)

option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

add_subdirectory(DebugAudioWriter)
```

