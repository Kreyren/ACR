FROM gitpod/workspace-full:latest

USER root

RUN apt-get update \
    && apt-get install -yq \
        clang-format \
        clang-tidy \
        clang \
        # clang-tools \ # breaks the build atm
        clangd \
        gdb \
        lld \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/*