ECHO  "Create build directory"
mkdir build
cd ./build

ECHO  "Generate Project"
cmake.exe ../
cd ../

ECHO  "Copy dll files"
mkdir bin\Debug\data
mkdir bin\Release\data


copy data\* bin\Debug\data\*
for /r ./  %%v in (*.dll) do copy %%v bin\Debug\

copy data\* bin\Release\data\*
for /r ./  %%v in (*.dll) do copy %%v bin\Release\


for /D %%D in (%SYSTEMROOT%\..\"Program Files (x86)"\MSBuild\12.0\*) do (
	set msbuild.exe="%%D\MSBuild.exe" 
	if exist "%%D\MSBuild.exe" goto BuildProject
)

for /D %%D in (%SYSTEMROOT%\..\"Program Files"\MSBuild\12.0\*) do (
	set msbuild.exe=%%D\MSBuild.exe
	if exist "%%D\MSBuild.exe" goto BuildProject
)


for /D %%D in (%SYSTEMROOT%\Microsoft.NET\Framework\v4*) do (
	set msbuild.exe=%%D\MSBuild.exe
	if exist "%%D\MSBuild.exe" goto BuildProject
)

for /D %%D in (%SYSTEMROOT%\Microsoft.NET\Framework64\v*) do (
	set msbuild.exe=%%D\MSBuild.exe
	if exist "%%D\MSBuild.exe" goto BuildProject
)

if not defined msbuild.exe echo error: can't find MSBuild.exe & goto :eof
if not exist %msbuild.exe% echo error: %msbuild.exe%: not found & goto :eof

:BuildProject
echo "%msbuild.exe%" found !
%msbuild.exe% build\DemoProject.sln
