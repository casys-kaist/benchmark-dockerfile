FROM ubuntu:18.04
LABEL maintainer="Taekyung Heo <tkheo@casys.kaist.ac.kr>"

RUN apt-get -y update && apt-get -y install\
    gcc g++ make python3

ADD src /root/src
RUN cd /root/src \
    && make all

ENTRYPOINT ["/bin/bash"]
