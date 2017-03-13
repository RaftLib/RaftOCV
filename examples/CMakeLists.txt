foreach(main video_capture video_save stereo_capture stereo_calibrate video_playback)
    add_executable(${main} ${main}.cpp)
    target_link_libraries(${main} RaftOCV)
endforeach()
