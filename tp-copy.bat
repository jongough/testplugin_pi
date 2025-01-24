REM "tp-copy.bat"  Local TP Frontend2 Template to local Plugin
REM Source Folder:        ..src\testplugin_pi
REM DestinationFolder:    ..src\[plugin_pi]
REM Batch File Location:  ..src\[plugin_pi] - Execute from here
REM
@echo off
echo  Start script to copy Template Folders
pause   
echo .
xcopy ..\testplugin_pi\.circleci\* .\.circleci\ /s /v /y
xcopy ..\testplugin_pi\build-deps\* .\build-deps\ /s /v /y
xcopy ..\testplugin_pi\buildosx\* .\buildosx\ /s /v /y
xcopy ..\testplugin_pi\ci\* .\ci\ /v /y
xcopy ..\testplugin_pi\ci\extras\circleci-build-flatpak_extra_libs.txt .\ci\extras\ /v /y
xcopy ..\testplugin_pi\cmake\* .\cmake\ /s /v /y
xcopy ..\testplugin_pi\mingw\* .\mingw\ /s /v /y
xcopy ..\testplugin_pi\msvc\* .\msvc\ /s /v /y
echo Template Folders are copied
echo .
echo Start script to copy Template files. 
pause
echo .
xcopy ..\testplugin_pi\.clang-format .\ /v /y
xcopy ..\testplugin_pi\.cmake-format.yaml .\ /v /y
xcopy ..\testplugin_pi\.editorconfig .\ /v /y
xcopy ..\testplugin_pi\.gitignore .\ /v /y
xcopy ..\testplugin_pi\.travis.yml .\ /v /y
xcopy ..\testplugin_pi\appveyor.yml .\ /v /y
xcopy ..\testplugin_pi\bld.bat .\ /v /y
xcopy ..\testplugin_pi\build-local-package-example.sh .\ /v /y
xcopy ..\testplugin_pi\compile.bat .\ /v /y
xcopy ..\testplugin_pi\make-new-plugin.sh .\ /v /y
xcopy ..\testplugin_pi\run-circleci-local.txt .\ /v /y
xcopy ..\testplugin_pi\submod-up.bat .\ /v /y
xcopy ..\testplugin_pi\update_submodule_branch.sh .\ /v /y
echo Template Folders are copied
echo .
echo Template files are copied. 
echo .
echo Git status lists the files changed.
echo If it looks ok, execute add-files.bat
pause
git status
echo .
echo .
echo -----------------------------------------
echo Check and Compare CMakeLists.txt
echo -----------------------------------------
echo CMakeLists.txt needs to be compared and updated.
pause
echo Update the submodule if needed "submod-up.bat" 
echo Use "tp-add.bat"  to git add template files
echo .
echo  git add 
echo  git commit -am "[version] + TP[tpversion]"
echo  git tag v[version]
echo  git push --tags origin master
echo
