# This Dockerfile is used to build an image containing basic stuff to be used as a Jenkins slave build node.
FROM ubuntu:trusty

# In case you need proxy
#RUN echo 'Acquire::http::Proxy "http://127.0.0.1:8080";' >> /etc/apt/apt.conf

# Add locales after locale-gen as needed
# Upgrade packages on image
# Preparations for sshd
run locale-gen en_US.UTF-8 &&\
    apt-get -q update &&\
    DEBIAN_FRONTEND="noninteractive" apt-get -q upgrade -y -o Dpkg::Options::="--force-confnew" --no-install-recommends &&\
    DEBIAN_FRONTEND="noninteractive" apt-get -q install -y -o Dpkg::Options::="--force-confnew"  --no-install-recommends openssh-server &&\
    apt-get -q autoremove &&\
    apt-get -q clean -y && rm -rf /var/lib/apt/lists/* && rm -f /var/cache/apt/*.bin &&\
    sed -i 's|session    required     pam_loginuid.so|session    optional     pam_loginuid.so|g' /etc/pam.d/sshd &&\
    mkdir -p /var/run/sshd

ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# Install JDK 7 (latest edition)
RUN apt-get -q update &&\
    DEBIAN_FRONTEND="noninteractive" apt-get -q install -y -o Dpkg::Options::="--force-confnew"  --no-install-recommends openjdk-7-jre-headless &&\
    apt-get -q clean -y && rm -rf /var/lib/apt/lists/* && rm -f /var/cache/apt/*.bin

# Set user jenkins to the image
RUN useradd -m -d /home/jenkins -s /bin/bash jenkins &&\
    echo "jenkins:jenkins" | chpasswd

# Standard SSH port
EXPOSE 22

RUN apt-get update &&\
	apt-get -q install -y -o Dpkg::Options::="--force-confnew"  --no-install-recommends autoconf automake autopoint bash bison bzip2 flex gettext\
		git g++ gperf intltool libffi-dev libgdk-pixbuf2.0-dev libtool libltdl-dev libssl-dev libxml-parser-perl make \
		openssl p7zip-full patch perl pkg-config python ruby scons sed unzip wget xz-utils g++-multilib libc6-dev-i386 libtool python-dev libreadline-dev zip &&\
	apt-get -q clean -y && rm -rf /var/lib/apt/lists/* && rm -f /var/cache/apt/*.bin

USER jenkins
RUN git clone https://github.com/mxe/mxe.git /home/jenkins/mxe

USER root
RUN mv /home/jenkins/mxe /opt/mxe

USER jenkins
RUN echo "export PATH=/opt/mxe/usr/bin:$PATH" >>/home/jenkins/.bash_profile && \
    echo "export PKG_CONFIG_PATH_x86_64_w64_mingw32_static=/opt/mxe/lib/pkgconfig" >>/home/jenkins/.bash_profile

WORKDIR /opt/mxe
RUN sed -i 's/--without-ssl/--with-ca-bundle=cacert.pem/g' /opt/mxe/src/curl.mk && \
    sed -i '/--with-gnutls \\/d'  /opt/mxe/src/curl.mk
RUN make MXE_TARGETS='x86_64-w64-mingw32.static' gcc libxml2 openssl curl lzma readline libusb1 termcap libzip
RUN wget https://curl.haxx.se/ca/cacert.pem -O /opt/mxe/cacert.pem

WORKDIR /home/jenkins
USER root
# Default command
CMD ["/usr/sbin/sshd", "-D"]
