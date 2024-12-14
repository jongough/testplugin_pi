# crowdsource_pi
Plugin to test JSON and ODAPI and develop a templated build system

The idea is really to debug the process to see what happens.
The crowdsource should 'do' what a plugin would do to create stuff,

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

## Local build

The simplest way to produce a package that can be imported with the "Import plugin..." button in the OpenCPN plugin manager is to run the following

```
rm build -rf; mkdir build; cd build; bash ../build-local-package-example.sh
```

Notice: You can of course also run the steps inside that script manually. However, in that case be aware that `make package` does not actually
generate a file that can be used in the plugin manager. The generated `.tar.gz` gets modified (in place) to generate such a package by
the next step, `./cloudsmith-upload.sh` (this is the case even if no account info is provided and the upload thus fails).

## IDE setup & debugging

[Describe how to set up your IDE to compile, install and debug the plugin in OpenCPN]

## Renaming

[Describe how to refactor this plugin to have a new name everywhere to use it as a basis for new plugins]
