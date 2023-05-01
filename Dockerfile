FROM amazoncorretto:11

ARG BAZELISK_VERSION=v1.7.4

RUN yum -y install clang gcc g++ unzip tar gzip zip curl bash patch git && yum -y clean all && rm -rf /var/cache/yum

RUN curl -fsSL -o /usr/local/bin/bazel https://github.com/bazelbuild/bazelisk/releases/download/${BAZELISK_VERSION}/bazelisk-linux-amd64 && \
    chmod +x /usr/local/bin/bazel

WORKDIR /kvcache

COPY ./src ./

WORKDIR /kvcache/src

RUN /usr/local/bin/bazel build //server:server


