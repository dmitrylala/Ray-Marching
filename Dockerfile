FROM mcr.microsoft.com/devcontainers/cpp
RUN apt-get update && apt-get install -y python3

RUN mkdir /home/ray-marching
WORKDIR /home/ray-marching

# RUN curl https://bootstrap.pypa.io/get-pip.py --output get-pip.py && \
#     python3 get-pip.py && pip3 install poetry

# COPY pyproject.toml poetry.lock ./


