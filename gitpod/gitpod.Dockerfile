FROM gitpod/workspace-full:latest

USER root

RUN true \
	&& apt-get update \
	&& apt-get install clangd -y