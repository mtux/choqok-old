set(choqok_SRCS
   mainwindow.cpp
   main.cpp
   statuswidget.cpp
   backend.cpp
   statustextedit.cpp
   mediamanagement.cpp
   systrayicon.cpp
   quicktwit.cpp
 )

kde4_add_ui_files(choqok_SRCS mainwindow_base.ui prefs_base.ui status_base.ui accounts_base.ui)

kde4_add_kcfg_files(choqok_SRCS settings.kcfgc )

kde4_add_executable(choqok ${choqok_SRCS})

target_link_libraries(choqok ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS choqok DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES choqok.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES choqok.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES choqokui.rc  DESTINATION  ${DATA_INSTALL_DIR}/choqok )
