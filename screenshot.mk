##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=screenshot
ConfigurationName      :=Debug
WorkspaceConfiguration := $(ConfigurationName)
WorkspacePath          :=C:/Users/anijas/Documents/myc/myc
ProjectPath            :=C:/Users/anijas/Documents/myc/myc/screenshot
IntermediateDirectory  :=../build-$(ConfigurationName)/screenshot
OutDir                 :=../build-$(ConfigurationName)/screenshot
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=anijas
Date                   :=04/02/2020
CodeLitePath           :="C:/Program Files (x86)/CodeLite"
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=..\build-$(ConfigurationName)\bin\$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  -lws2_32 -lgdi32 -luser32
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW/bin/ar.exe rcu
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
Objects0=../build-$(ConfigurationName)/screenshot/lodepng.c$(ObjectSuffix) ../build-$(ConfigurationName)/screenshot/main.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): ../build-$(ConfigurationName)/screenshot/.d $(Objects) 
	@if not exist "..\build-$(ConfigurationName)\screenshot" mkdir "..\build-$(ConfigurationName)\screenshot"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "..\build-$(ConfigurationName)\screenshot" mkdir "..\build-$(ConfigurationName)\screenshot"
	@if not exist ""..\build-$(ConfigurationName)\bin"" mkdir ""..\build-$(ConfigurationName)\bin""

../build-$(ConfigurationName)/screenshot/.d:
	@if not exist "..\build-$(ConfigurationName)\screenshot" mkdir "..\build-$(ConfigurationName)\screenshot"

PreBuild:


##
## Objects
##
../build-$(ConfigurationName)/screenshot/lodepng.c$(ObjectSuffix): lodepng.c ../build-$(ConfigurationName)/screenshot/lodepng.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/anijas/Documents/myc/myc/screenshot/lodepng.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lodepng.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/screenshot/lodepng.c$(DependSuffix): lodepng.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/screenshot/lodepng.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/screenshot/lodepng.c$(DependSuffix) -MM lodepng.c

../build-$(ConfigurationName)/screenshot/lodepng.c$(PreprocessSuffix): lodepng.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/screenshot/lodepng.c$(PreprocessSuffix) lodepng.c

../build-$(ConfigurationName)/screenshot/main.c$(ObjectSuffix): main.c ../build-$(ConfigurationName)/screenshot/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/anijas/Documents/myc/myc/screenshot/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
../build-$(ConfigurationName)/screenshot/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT../build-$(ConfigurationName)/screenshot/main.c$(ObjectSuffix) -MF../build-$(ConfigurationName)/screenshot/main.c$(DependSuffix) -MM main.c

../build-$(ConfigurationName)/screenshot/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) ../build-$(ConfigurationName)/screenshot/main.c$(PreprocessSuffix) main.c


-include ../build-$(ConfigurationName)/screenshot//*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


