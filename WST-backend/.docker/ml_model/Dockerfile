#https://docs.unity3d.com/Packages/com.unity.ml-agents@4.0/manual/Installation.html

FROM python:3.10.12-slim-bullseye

ENV DEBIAN_FRONTEND=noninteractive

RUN pip install mlagents==1.1.0
RUN pip install onnxscript==0.5.7
RUN pip install onnx==1.20.1
RUN pip install protobuf==3.20.0
Run pip install torchvision==0.25.0
Run pip install torchaudio==2.10.0

WORKDIR /work
