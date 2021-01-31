#!/usr/bin/env bash

#
# Build for Raspbian in a docker container
#

# bailout on errors and echo commands.
set -xe
sudo apt-get -qq update

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" | sudo tee /etc/default/docker > /dev/null
sudo service docker restart
sleep 5;

if [ "$BUILD_ENV" = "raspbian" ]; then
    docker run --rm --privileged multiarch/qemu-user-static:register --reset
else
    docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
fi

docker run --privileged -d -ti -e "container=docker"  -v $(pwd):/ci-source:rw -v ~/source_top:/source_top $DOCKER_IMAGE /bin/bash

DOCKER_CONTAINER_ID=$(docker ps | grep $DOCKER_IMAGE | awk '{print $1}')

echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    -e "CIRCLECI=$CIRCLECI" \
    -e "CIRCLE_BRANCH=$CIRCLE_BRANCH" \
    -e "CIRCLE_TAG=$CIRCLE_TAG" \
    -e "CIRCLE_PROJECT_USERNAME=$CIRCLE_PROJECT_USERNAME" \
    -e "CIRCLE_PROJECT_REPONAME=$CIRCLE_PROJECT_REPONAME" \
    -e "GIT_REPOSITORY_SERVER=$GIT_REPOSITORY_SERVER" \
    -e "OCPN_TARGET=$OCPN_TARGET" \
    -e "BUILD_GTK3=$BUILD_GTK3"

# Run build script
if [ "$BUILD_ENV" = "raspbian" ];
    cat > build.sh << "EOF"
    apt-get -q update
    apt install -y curl gnupg
    curl http://mirrordirector.raspbian.org/raspbian.public.key  > raspikey
    apt-key add raspikey
    curl http://archive.raspbian.org/raspbian.public.key  > raspikey
    apt-key add raspikey
    EOF
fi

cat >> build.sh << "EOF"
apt -q update
apt-get -y install --no-install-recommends \
    git cmake build-essential cmake gettext wx-common libgtk2.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release
EOF

docker exec -ti \
    $DOCKER_CONTAINER_ID /bin/bash -xec "bash -xe /ci-source/build.sh; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

