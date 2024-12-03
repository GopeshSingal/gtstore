set(ABSL_PROPAGATE_CXX_STD ON)

include(FetchContent)
FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG        v1.45.0
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)

add_subdirectory(proto)

add_executable(client client.cpp)
target_link_libraries(client
        PRIVATE
        project_options
        project_warnings
        demo_grpc_proto
        )

add_executable(manager manager.cpp)
target_link_libraries(manager
        PRIVATE
        project_options
        project_warnings
        demo_grpc_proto
        )

add_executable(storage storage.cpp)
target_link_libraries(storage
        PRIVATE
        project_options
        project_warnings
        demo_grpc_proto
        )

add_executable(test_app test_app.cpp)
target_link_libraries(test_app
        PRIVATE
        project_options
        project_warnings
        demo_grpc_proto
        )