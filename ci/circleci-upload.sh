#!/usr/bin/env bash

#
# Upload the .tar.gz and .xml artifacts to cloudsmith
#
# Builds are uploaded the either the stable or the unstable
# repo. If there is a tag pointing to current commit it goes
# to stable, otherwise to unstable.
#
# If the environment variable CLOUDSMITH_STABLE_REPO exists it is
# used as the stable repo, defaulting to the hardcoded STABLE_REPO
# value. Likewise for CLOUDSMITH_UNSTABLE_REPO and UNSTABLE_REPO.
#

set -xe

STABLE_REPO=${CLOUDSMITH_STABLE_REPO:-'jon-gough/testplugin_pi-stable'}
UNSTABLE_REPO=${CLOUDSMITH_UNSTABLE_REPO:-'jon-gough/testplugin-pi'}
STABLE_PKG_REPO=${CLOUDSMITH_PKG_REPO:-'jon-gough/manual'}
PKG_EXT=${CLOUDSMITH_PKG_EXT:-'deb'}

if [ -z "$CIRCLECI" ]; then
    exit 0;
fi

if [ -z "$CLOUDSMITH_API_KEY" ]; then
    echo 'Cannot deploy to cloudsmith, missing $CLOUDSMITH_API_KEY'
    exit 0
fi

if pyenv versions 2>&1 >/dev/null; then
    pyenv global 3.7.0
    python -m pip install cloudsmith-cli
    pyenv rehash
elif dnf --version 2>&1 >/dev/null; then
    sudo dnf install python3-pip python3-setuptools
    sudo python3 -m pip install -q cloudsmith-cli
elif apt-get --version 2>&1 >/dev/null; then
    sudo apt-get install python3-pip python3-setuptools
    sudo python3 -m pip install -q cloudsmith-cli
else
    sudo -H python3 -m ensurepip
    sudo -H python3 -m pip install -q setuptools
    sudo -H python3 -m pip install -q cloudsmith-cli
fi

BUILD_ID=${CIRCLE_BUILD_NUM:-1}
commit=$(git rev-parse --short=7 HEAD) || commit="unknown"
tag=$(git tag --contains HEAD)

xml=$(ls $HOME/project/build/*.xml)
tarball=$(ls $HOME/project/build/*.tar.gz)
tarball_basename=${tarball##*/}

source $HOME/project/build/pkg_version.sh
tarball_name=testplugin-linuxmint-19.2-tarball
pkg=$(ls $HOME/project/build/*.)

source $HOME/project/build/pkg_version.sh
if [ -n "$tag" ]; then
    VERSION="$tag"
    REPO="$STABLE_REPO"
    PKG_REPO="$STABLE_PKG_REPO"
else
    VERSION="+."
    REPO="$UNSTABLE_REPO"
    PKG_REPO="$UNSTABLE_REPO"
fi

sudo sed -i -e "s||$REPO|"  $xml
sudo sed -i -e "s||$tarball_name|" $xml
sudo sed -i -e "s||$VERSION|" $xml
sudo sed -i -e "s||$tarball_basename|" $xml

cloudsmith push raw --republish --no-wait-for-sync \
    --name testplugin-linuxmint-19.2-metadata \
    --version  \
    --summary "testplugin opencpn plugin metadata for automatic installation" \
    $REPO $xml

cloudsmith push raw --republish --no-wait-for-sync \
    --name $tarball_name \
    --version  \
    --summary "testplugin opencpn plugin tarball for automatic installation" \
    $REPO $tarball

cloudsmith push raw --republish --no-wait-for-sync \
    --name testplugin-linuxmint-19.2.${PKG_EXT} \
    --version  \
    --summary "testplugin .${PKG_EXT} installation package" \
    $PKG_REPO $pkg
