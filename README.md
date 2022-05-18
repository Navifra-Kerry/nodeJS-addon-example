# NodeJS C++ add-on 을 이용한 백엔드 응용

-   립토치를 사용 해서 클래스 분류기 만들기

```c++
개발 환경은 도커를 활용 해서
docker-compose가 작성 전이기 때문에
docker build . -t classifier 로 빌드 후
docker run --name classifier -v ${PWD}:/home/node/classifier -it classifier /bin/bash
로 도커 접속 후에 npm install 하면 add-on이 빌드 되고
npm run webpack:dev 서버 코드 빌드 후에
npm run start 하면 서버 실행 됨
```

# Windows

```
윈도우즈에서 사용 할 때는 OpenCV_DIR , Torch_DIR을 시스템 환경 변수에 등록 해줘야 함
시스템 환경 변수 PATH에 아래 두개 설정 해야함
%OpenCV_DIR%\x64\vc14\bin
%Torch_DIR%\lib
```
