# Benchmark Dockerfile
## Quick Start
```
$ cd <workload directory>
$ ./build.sh
```

## How to Install Docker
```
# apt -y update
# apt -y install \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common
# curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
# apt-key fingerprint 0EBFCD88
# add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
# apt -y update
# apt -y install docker-ce
```

## Notes
* Make sure that your kernel is configured to run docker.
  * [A script to check kernel configurations](https://gist.github.com/MariusRumpf/7a0858b5fadb68adbd75)
