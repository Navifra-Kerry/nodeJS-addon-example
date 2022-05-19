FROM node:16.15.0-buster as builder

RUN apt update && apt install -y \
        unzip \
        wget \
        build-essential \
        pkg-config \
        make \
        g++ \
        apt-utils \
        lsb-release \
        libjpeg-dev \
        libtiff5-dev \
        libpng-dev \
        ffmpeg \ 
        libavcodec-dev \
        libavformat-dev \
        libswscale-dev \
        libxvidcore-dev \
        libx264-dev \
        libxine2-dev \
        libv4l-dev v4l-utils \
        libgstreamer1.0-dev \
        libgstreamer-plugins-base1.0-dev \
        libgtk-3-dev \
        libgtk2.0-dev \
        mesa-utils \
        libgl1-mesa-dri \
        libgtkgl2.0-dev \
        libgtkglext1-dev \ 
        libatlas-base-dev \
        gfortran \
        libeigen3-dev \
        python3-dev \ 
        python3-numpy \
        python3-pip \
        cmake \
        gdb \
        && apt-get clean \
        && rm -rf /var/lib/apt/lists/*

RUN pip3 install numpy
WORKDIR /home/node

ADD https://github.com/opencv/opencv/archive/4.4.0.zip opencv.zip
RUN unzip opencv.zip

ADD https://github.com/opencv/opencv_contrib/archive/4.4.0.zip opencv_contrib.zip
RUN unzip opencv_contrib.zip

WORKDIR /home/node/opencv-4.4.0/
WORKDIR /home/node/opencv-4.4.0/build
RUN cmake -D CMAKE_BUILD_TYPE=RELEASE \ 
-D CMAKE_INSTALL_PREFIX=/usr/local \ 
-D WITH_TBB=OFF \ -D WITH_IPP=OFF \ 
-D WITH_1394=OFF \ 
-D BUILD_WITH_DEBUG_INFO=OFF \ 
-D BUILD_DOCS=OFF \ 
-D INSTALL_C_EXAMPLES=ON \ 
-D INSTALL_PYTHON_EXAMPLES=ON \ 
-D BUILD_EXAMPLES=OFF \ 
-D BUILD_TESTS=OFF \ 
-D BUILD_PERF_TESTS=OFF \ 
-D WITH_QT=OFF \ 
-D WITH_GTK=ON \ 
-D WITH_OPENGL=ON \ 
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.4.0/modules \ 
-D WITH_V4L=ON \ 
-D WITH_FFMPEG=ON \ 
-D WITH_XINE=ON \ 
-D BUILD_NEW_PYTHON_SUPPORT=ON \ 
-D OPENCV_GENERATE_PKGCONFIG=ON ../


ENV Torch_DIR=/home/node/libtorch/share/cmake/Torch

RUN make -j$(nproc)
RUN make install

ENV LD_LIBRARY_PATH=/usr/local/lib:/home/node/libtorch/lib

WORKDIR /home/node/

ADD https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-1.11.0%2Bcpu.zip libtorch.zip
RUN unzip libtorch.zip

RUN rm -rf opencv_contrib.zip opencv.zip libtorch.zip
RUN pip3 install torch torchvision
COPY ./scripts/models/convert.py /home/node/models/convert.py
WORKDIR /home/node/models
RUN python3 /home/node/models/convert.py 

ADD ./ /home/node/classifier
WORKDIR /home/node/classifier

RUN npm install
RUN npm run webpack:prod

FROM node:16.15.0-buster as production
ENV DEPLOY="1"

RUN apt update && apt install -y \
        libgtk-3-0 \  
        && apt-get clean \
        && rm -rf /var/lib/apt/lists/*

ENV LD_LIBRARY_PATH=/usr/local/lib:/home/node/libtorch/lib
WORKDIR /home/node/classifier/

EXPOSE 8000

COPY --from=builder /home/node/classifier/build ./build
COPY --from=builder /home/node/libtorch/ /home/node/libtorch/
COPY --from=builder /usr/local/lib /usr/local/lib
COPY --from=builder /home/node/classifier/package*.json ./
COPY --from=builder /home/node/classifier/public ./public
COPY --from=builder /home/node/classifier/scripts ./scripts
COPY --from=builder /home/node/models/resnet18_Python.pt /home/node/models/resnet18_Python.pt

RUN npm install --only=production
CMD [ "node", "build/bundle.js" ]