FROM ubuntu:18.04
LABEL maintainer="Taekyung Heo <tkheo@casys.kaist.ac.kr>"

RUN apt-get -y update && \
    apt-get -y install \
    gcc g++ gfortran make

COPY NPB3.4 /root/NPB3.4
COPY make.def /root/NPB3.4/NPB3.4-OMP/config
COPY suite.def /root/NPB3.4/NPB3.4-OMP/config
RUN cd /root/NPB3.4/NPB3.4-OMP/sys \
    && make
RUN cd /root/NPB3.4/NPB3.4-OMP \
    && mkdir /root/NPB3.4/NPB3.4-OMP/bin \
    && make suite -j$(nproc)

COPY docker-entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
