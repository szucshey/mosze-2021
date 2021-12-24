FROM ubuntu:latest
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update 
RUN apt-get install -y doxygen 
RUN apt-get install -y graphviz 
RUN apt-get install -y texlive-latex-extra 
RUN apt-get install -y g++ 
RUN apt-get install -y cmake 
RUN apt-get install -y make
RUN apt-get install -y valgrind
RUN apt-get install -y libgtest-dev
RUN rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/gtest \
    && cmake CMakeLists.txt \
    && make \
    && ln -st /usr/lib/ /usr/src/gtest/lib/libgtest.a \
    && ln -st /usr/lib/ /usr/src/gtest/lib/libgtest_main.a