cmake_minimum_required(VERSION 3.0)
project(my)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(mysocks4a mysocks4a.cc)
target_link_libraries(mysocks4a PRIVATE muduo_net muduo_base pthread)

add_executable(mynginx mynginx.cc HttpContext.cc HttpResponse.cc HttpServer.cc)
target_link_libraries(mynginx  PRIVATE muduo_net muduo_base pthread)
