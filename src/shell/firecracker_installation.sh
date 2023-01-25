#!/bin/bash

# Download and installation of Firecracker.
# 
# Author: Manuel Domínguez Montero.
#
#

firecracker_path="${HOME}/.local/bin"
release_url="https://github.com/firecracker-microvm/firecracker/releases"
latest=$(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} ${release_url}/latest))
arch=`uname -m`

curl -L ${release_url}/download/${latest}/firecracker-${latest}-${arch}.tgz | tar -xz
mv release-${latest}-$(uname -m)/firecracker-${latest}-$(uname -m) ${firecracker_path}/firecracker
rm -rf release-${latest}-$(uname -m)

echo "Firecracker has been installed inside ${firecracker_path}"


