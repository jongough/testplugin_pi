#!/usr/bin/env bash

#
#

# bailout on errors and echo commands.
set -xe
#sudo apt-get -qq update

#sudo apt-get install -y apt-transport-https ca-certificates curl gnupg-agent software-properties-common
#sudo sh -c 'curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -'
#sudo apt-key fingerprint 0EBFCD88
#sudo sh -c 'echo "deb https://download.docker.com/linux/ubuntu bionic stable" | tee /etc/apt/sources.list.d/docker.list'
#sudo add-apt-repository \
#   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
#   $(lsb_release -cs) \
#   stable"
#sudo apt-get -qq update
#sudo apt-get install -y docker-ce docker-ce-cli containerd.io

DOCKER_SOCK="unix:///var/run/docker.sock"

echo "DOCKER_OPTS=\"-H tcp://127.0.0.1:2375 -H $DOCKER_SOCK -s devicemapper\"" | sudo tee /etc/default/docker > /dev/null
sudo service docker status
sudo service docker restart
sleep 5;
sudo service docker status

docker run --rm --privileged multiarch/qemu-user-static:register --reset
#docker run --rm --privileged multiarch/qemu-user-static:register --reset

#docker run --privileged -d -ti -e "container=docker"
#      -v ~/source_top:/source_top \
#      -v $(pwd):/ci-source:rw \
#      $DOCKER_IMAGE /bin/bash
docker run --privileged -d -ti -e "container=docker"  $DOCKER_IMAGE /bin/bash
DOCKER_CONTAINER_ID=$(docker ps | grep raspbian | awk '{print $1}')
sleep 5;
#docker exec -ti $DOCKER_CONTAINER_ID sudo apt-get update
#docker exec -ti $DOCKER_CONTAINER_ID echo "------\nEND apt-get update\n"

#docker exec -ti $DOCKER_CONTAINER_ID sudo apt-get -y install git cmake build-essential cmake gettext wx-common libgtk2.0-dev libwxgtk3.0-dev libbz2-dev libcurl4-openssl-dev libexpat1-dev libcairo2-dev libarchive-dev liblzma-dev libexif-dev lsb-release

echo $OCPN_TARGET
docker exec -ti $DOCKER_CONTAINER_ID /bin/bash -xec \
    "export CICLECI=$CIRCLECI; export CIRCLECI_BRANCH=$CIRCLECI_BRANCH; export OCPN_TARGET=$OCPN_TARGET; rm -rf ci-source/build; mkdir ci-source/build; cd ci-source/build; cmake ..; make; make package; chmod -R a+rw ../build;"

echo "Stopping"
docker ps -a
docker stop $DOCKER_CONTAINER_ID
docker rm -v $DOCKER_CONTAINER_ID

