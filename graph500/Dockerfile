FROM ubuntu:18.04
LABEL maintainer="Taekyung Heo <tkheo@casys.kaist.ac.kr>"

RUN apt-get -y update && apt-get -y install\
    make gcc g++ gfortran mpich

COPY graph500 /root/graph500
RUN cd /root/graph500/src \
    && make

COPY docker-entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
