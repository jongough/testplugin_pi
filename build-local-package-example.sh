export OCPN_TARGET=bookworm
export BUILD_GTK3=true
export WX_VER=32
export LOCAL_DEPLOY=true
rm *.xml
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j2
make package
chmod a+x cloudsmith-upload.sh
./cloudsmith-upload.sh
