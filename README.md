# testplugin_pi
Plugin to test JSON and ODAPI and develop a templated build system

The idea is really to debug the process to see what happens.
The testplugin should 'do' what a plugin would do to create stuff,

Only ODAPI is working at the moment. JSON is being developed now.
1. With the ODAPI select the type of object you want to create,
2. click on the chart to pick the click location for its creation or provide the lat/lon,
3. then click create.
4. The object 'should' be created.

For more details see: https://opencpn-manuals.github.io/main/opencpn-dev/odraw-messaging.html

The templated build process is known and FrontEnd 2 (FE2), which simplifies the information needed to build a plugin to the
CMakeLists.txt file for most builds. All the other files needed for a plugin are supplied to allow automated building
of the current environments supported by OpenCPN. The templated system allows for extensions per plugin to be addede by the
developer.

NOTE this is currently a work in progress and should be considered 'Alpha/Beta'. It may have issues at times as new functionality is added. It is a test tool not a production plugin.

The build process can be used in a few ways:
  1. A standard cmake/make process on the command line
  2. Run a batch file to create a 'Plugin Manager' installable. An example is `build-local-package-example.sh` which basically runs a script that is used on circleci locally and creates the installable files in the build directory
  3. Run 'circleci local' which will run what circleci does in the cloud locally for non-ORB based environment, and example being shown in `run-circleci-local.txt`
  4. Run a build process on pushing/merging a change on github, and when creating a new tag/version

# Examples
## Local build

The simplest way to produce a package that can be imported with the "Import plugin..." button in the OpenCPN plugin manager is to run the following

```
rm build -rf; mkdir build; cd build; bash ../build-local-package-example.sh
```

Notice: You can of course also run the steps inside that script manually. However, in that case be aware that `make package` does not actually
generate a file that can be used in the plugin manager. The generated `.tar.gz` gets modified (in place) to generate such a package by
the next step, `./cloudsmith-upload.sh` (this is the case even if no account info is provided and the upload thus fails).

## IDE setup & debugging

This is totally dependant on the IDE that is being used. Basically the IDE needs to have both OpenCPN and the Plugin open in the IDE so that the IDE can do debugging.
OpenCPN will/should be built with DEBUG as should the Plugin. The simplest way to continue is to use Plugin Manger once to setup your plugin correctly, i.e. all the directories and files. This can be done once you have a working plugin, testplugin_pi does work so if you base your plugin on this code, to start with, you should have an installable plugin if it compiles successfully. Then you can use the 'Local build' process to create the installable package.

The IDE needs to be setup to run OpenCPN with the "-p" option so that all data and programs exist in the location the opencpn executable is or sub-directories of this location. Now you can use Plugin Manager to install your plugin. You will need to have set 'CatalogExpert=1' in the [Plugins] section of the opencpn.conf/ini file so that you get full access to the facilities in Plugin Manager.

Once you have installed the plugin and checked that it works you can continue to build the new functionality. When doing this you then just need to copy the plugin library to the '(opencpn build directory)/plugins/lib' directory with OpenCPN stopped, then start OpenCPN and it will load the new version of your plugin. If both OpenCPN and your Plugin are known to the IDE you should be able to single step (or other debugging processes) through both sets of code if needed.

## Renaming

To start a new plugin, clone this repo, then run

```
git remote rm origin
git remote add origin url-to-new-repo
bash make-new-plugin.sh newname
git commit -a -m "Start of new plugin newname"
```
