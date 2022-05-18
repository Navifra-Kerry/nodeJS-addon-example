# NodeJS C++ add-on 을 이용한 백엔드 응용

-   립토치를 사용 해서 클래스 분류기 만들기

# docker

```c++
docker-compose build or docker-compose up -d 후에
vscode remote-develover container 접속 후 개발 테스트 하면 됨
```

# Windows

```
윈도우즈에서 사용 할 때는 OpenCV_DIR , Torch_DIR을 시스템 환경 변수에 등록 해줘야 함
OpenCV_DIR 는 OpenCV 다운 받고 설치 파일에 BUILD 폴더를 경로로 설정 해야 하고
Torch_DIR의 경우 Libtorch 다운 받고 압축 푼 폴더를 설정 하면 됨
시스템 환경 변수 PATH에 아래 두개 설정 해야함
%OpenCV_DIR%\x64\vc14\bin
%Torch_DIR%\lib
```
