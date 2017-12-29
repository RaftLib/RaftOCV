SET(MAINS video_capture video_save blur
        video_playback camera_calibrate video_stabilization video_bgmodel
        dense_optical_flow)

if(USE_PCL)
    LIST(APPEND MAINS stereo_capture stereo_calibrate)
ENDIF()

if(USE_OPENPOSE)
    LIST(APPEND MAINS pose_tracker)
ENDIF()


FILE(COPY ps4eye_calib.yaml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
foreach(main ${MAINS})
    add_executable(${main} ${main}.cpp)
    target_link_libraries(${main} RaftOCV)
endforeach()
