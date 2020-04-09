# KEEP EXISTING DIRECTORIES AND FILES
-----------------------------------------------------
#### Important: 
1. Make these changes on a new branch "frontend1" or "ci" (if possible).
1. Keep your currently working "master" branch intact. 
1. Rename CMakeLists.txt ----> CMakeLists.save.txt for reference
1. Rename appveyor.yml ----> appveyor.save.yml for reference
1. Rename .travis.yml ----> .travis.save.yml for reference
1. Rename the cmake directory ---> "cmake.save" for reference
1. Keep any other specialized plugin directories

#### KEEP these directories, don't over-write them!:
- Include
- Data
- src
- po
- Any other specific plugin directories

# LIST of FOLDERS & FILES copied from TESTPLUGIN_PI
----------------------------------------------------
#### Add these Directories + Sub-directories + Files

Copy the following directories and files from testplugin_pi to the same location in the plugin directory you are working on:
testplugin_pi/.circleci
testplugin_pi/api-16
testplugin_pi/ci
testplugin_pi/cmake
testplugin_pi/buildosx
testplugin_pi/buildwin
testplugin_pi/debian
testplugin_pi/mingw

#### Files
testplugin_pi/appveyor.yml
testplugin_pi/.travis.yml

#### Directories NOT needed
The following directories and files are not needed from testplugin_pi
testplugin_pi/data
testplugin_pi/forms
testplugin_pi/extinclude
testplugin_pi/extsrc
testplugin_pi/include
testplugin_pi/lib
testplugin_pi/ocpninclude
testplugin_pi/ocpnsrc
testplugin_pi/src


# CHANGES REQUIRED
----------------------------------------------------------------
1. Rename CMakeLists.txt, appveyor.yml, .travis.yml adding  .save for reference.
1. Modify CMakeLists.txt file, following the in-line notes
 (not completed, draft from FrontEnd1.00).
   - Modify Plugin Specifics about Ln 40?.
   - Determine CommonName and edit <squiddio>_plugin.xml.in
   - Modify/configure 'Include' Directories about Ln 317?, use CMakeLists.old.txt
   - Modify/configure 'Set' Directories about Ln 362? use CMakeLists.old.txt
   - Modify/configure 'Add Library' listings for the plugin about Ln 550?
   - Edit the file  <squiiddio>_pi.xml.in, about Ln 612?
   - Edit the file  <squiddio>_pi.xml, about Ln 612?
1. Check that buildosx/InstallOSX/plugin_pi.pkgproj.in exists
   - PluginPackage.cmake Ln 184 has a configure_file to make this file.
   - File inside is generic and uses a variable for plugin project name (7x's inside file).
1. Modify flatpak\org.opencpn.OpenCPN.Plugin.oesenc.yaml filename
   - Rename to <verbose_name> and rename all 3 instances of previous name to <verbose_name>.
   - See Line 233 for more instructions.
1. Modify cmake/PluginConfigure.cmake: Ln 56 -Only if necessary (squiddio req'd) 
   - Change to SET(wxWidgets_USE_LIBS base core net xml html adv aui)
   - adding 'aui' due to errors for aui.
1. Modify src/squiddioPrefsDialogBase.h
   - Removed first line: #include "wxWTranslateCatalog.h" due to errors.
   - "cmake/wxWTranslateCatalog.h.in" is not working right now.
1. First get the ci/environment scripts working on Circleci, Appveyor and .travis-ci
1. Then get the uploads to Cloudsmith working. Also see CMakeLists.txt Ln 66
   - Example Cloudsmith path uploads
     - OCPN_STABLE_REPO=mauro-calvi/squiddio-stable
     - OCPN_UNSTABLE_REPO=mauro-calvi/squiddio-pi
     - OCPN_PKG_REPO=mauro-calvi/squiddio-manual
     - Mauro's repositories https://cloudsmith.io/~mauro-calvi/repos/
   - Script path for uploads
     - .travis.yml ---> ci/travis-build-raspbian-armhf.sh 
     - appveyor.yml ---> ci/appveyor-uploads.sh
     - circleci scripts --->ci/circleci-upload.sh generally except trusty.
   - Configure uploads to Cloudsmith
     - ci/circleci-upload.sh modify Cloudsmith destinations
     - ci/appveyor-uploads.sh modify Cloudsmith destinations
     - ci/travis-build-raspbian-armhf.sh -modify Cloudsmith destinations
   