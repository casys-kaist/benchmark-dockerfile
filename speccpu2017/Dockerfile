FROM ubuntu:18.04
LABEL maintainer="Taekyung Heo <tkheo@casys.kaist.ac.kr>"

RUN apt-get -y update \
    && apt-get -y install\
    gcc g++ gfortran make

ENV BASE_PATH /root/
ENV SPECCPU2017_INSTALLER speccpu2017-installer
ENV SPECCPU2017_INSTALL_PATH /root/speccpu2017

COPY $SPECCPU2017_INSTALLER.tar $BASE_PATH
RUN cd $BASE_PATH \
    && tar -xvf $SPECCPU2017_INSTALLER.tar
RUN cd $BASE_PATH/$SPECCPU2017_INSTALLER \
    && ./install.sh -d $SPECCPU2017_INSTALL_PATH -f \
    && cd .. \
    && rm -rf $SPECCPU2017_INSTALLER \
    && rm $SPECCPU2017_INSTALLER.tar
SHELL ["/bin/bash", "-c"]

COPY default.cfg $SPECCPU2017_INSTALL_PATH/config/

# control the execution time of SPEC CPU 2017
COPY spec_ref.par /root/speccpu2017/benchspec/CPU/507.cactuBSSN_r/data/refspeed/input
COPY bwaves_1.in /root/speccpu2017/benchspec/CPU/503.bwaves_r/data/refspeed/input
COPY bwaves_2.in /root/speccpu2017/benchspec/CPU/503.bwaves_r/data/refspeed/input
COPY drv_in.in /root/speccpu2017/benchspec/CPU/628.pop2_s/data/refspeed/input
COPY ocean_benchmark1.in.x /root/speccpu2017/benchspec/CPU/554.roms_r/data/refspeed/input/
COPY ocean_benchmark3.in.x /root/speccpu2017/benchspec/CPU/554.roms_r/data/refspeed/input/
COPY 657.xz_s.control /root/speccpu2017/benchspec/CPU/557.xz_r/data/refspeed/input/control
COPY 657.xz_s.control /root/speccpu2017/benchspec/CPU/657.xz_s/run/run_base_refspeed_mytest-m64.0000/control

RUN cd $SPECCPU2017_INSTALL_PATH \
    && . shrc \
    && runcpu intspeed fpspeed
