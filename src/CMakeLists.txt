set(kwitter_SRCS
   mainwindow.cpp
   main.cpp
   statuswidget.cpp
   backend.cpp
   statustextedit.cpp
   mediamanagement.cpp
   systrayicon.cpp
 )

kde4_add_ui_files(kwitter_SRCS mainwindow_base.ui prefs_base.ui status_base.ui accounts_base.ui)

kde4_add_kcfg_files(kwitter_SRCS settings.kcfgc )

kde4_add_executable(kwitter ${kwitter_SRCS})

target_link_libraries(kwitter ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS kwitter DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES kwitter.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kwitter.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kwitterui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kwitter )
