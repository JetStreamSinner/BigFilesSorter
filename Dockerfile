FROM gcc:latest as build
RUN apt update && apt install -y cmake
COPY CMakeLists.txt /app/CMakeLists.txt
COPY source /app/source
WORKDIR /app/build
RUN cmake .. && cmake --build .
FROM ubuntu:latest
WORKDIR /app
COPY --from=build /app/build/source/BigFilesSorter .
COPY resources/data.txt /app/resources/data.txt
COPY resources/out.txt /app/resources/out.txt
ENTRYPOINT ["./BigFilesSorter"]