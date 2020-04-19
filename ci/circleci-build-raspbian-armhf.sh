#!/usr/bin/env bash

#
#

# bailout on errors and echo commands.
set -xe

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" | sudo tee /etc/default/docker > /dev/null
sudo service docker restart
sleep 5;

docker run --rm --privileged multiarch/qemu-user-static:register --reset

docker run --privileged -d -ti -e "container=docker"  -v $(pwd):/ci-source:rw $DOCKER_IMAGE /bin/bash
DOCKER_CONTAINER_ID=$(docker ps | grep raspbian | awk '{print $1}')

echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "export CICLECI=$CIRCLECI; export CIRCLECI_BRANCH=$CIRCLECI_BRANCH; export OCPN_TARGET=$OCPN_TARGET; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make $BUILD_FLAGS; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

