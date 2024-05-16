FROM ubuntu:22.04

# Set environment variables to non-interactive (This prevents some prompts)
ENV DEBIAN_FRONTEND=noninteractive

COPY replace_deb.sh /tmp/replace_deb.sh

RUN bash /tmp/replace_deb.sh && apt update

# Update and install packages
RUN apt-get update && apt-get install -y \
    sudo \
    lsb-release \
    software-properties-common \
    curl \
    wget \
    vim \
    htop \
    git \
    build-essential \
    cmake \
    cmake-curses-gui

RUN apt-get install -y \
    libboost-dev \
    libgtest-dev \
    libgoogle-glog-dev \
    libeigen3-dev \
    clangd-12 \
    clang-format \
    gdb

RUN apt-get install -y \
    qtbase5-dev qt5-qmake

RUN update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-12 100

ARG USER_ID
ARG GROUP_ID
ARG USER_NAME

# Create a new group and user with the passed user name, then add them to the 'sudo' group
RUN groupadd -g ${GROUP_ID} ${USER_NAME} && \
    useradd -m -u ${USER_ID} -g ${USER_NAME} -s /bin/bash ${USER_NAME} && \
    mkdir -p /etc/sudoers.d/ && \
    echo "${USER_NAME} ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/${USER_NAME}

# Change to non-root privilege
USER ${USER_NAME}

WORKDIR /home/${USER_NAME}

# Set the default command for the container
CMD ["bash"]