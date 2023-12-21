FROM debian

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update
RUN apt-get install qtbase5-dev libqt5sql5-psql qtchooser qt5-qmake qtbase5-dev-tools -y
RUN apt-get install build-essential -y
RUN apt-get install wget -y
RUN apt-get install libtool libssl-dev -y

WORKDIR /root/
RUN mkdir server
WORKDIR /root/server/
COPY *.cpp /root/server/
COPY *.h /root/server/
COPY *.pro /root/server/

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.bz2
RUN tar --bzip2 -xf boost_1_83_0.tar.bz2     
RUN cd  boost_1_83_0 && ./bootstrap.sh  && ./b2 install

RUN qmake echoServer.pro
RUN make

ENTRYPOINT ["./echoServer"]
