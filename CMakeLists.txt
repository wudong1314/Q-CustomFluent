cmake_minimum_required(VERSION 3.16)

project(UniversalItem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 引入标准安装路径模块 (定义了 CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR 等)
include(GNUInstallDirs)

# 1. 查找 Qt (删除了 UiPlugin，因为不再构建设计器插件)
find_package(Qt6 COMPONENTS Network Widgets Svg WinExtras Xml QUIET )
if(Qt6_FOUND)
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 REQUIRED COMPONENTS Network Widgets Svg WinExtras Xml)
    set(QT_PACKAGE Qt5)
endif()

# 2. 文件列表定义
set(HEADER_FILES
    mainwindow.h
    QCusFluentStyleSheet.h
    QCusFluentMaximumSizeButton_p.h
    QCusFluentApplication.h
    QCusFluentUtil_p.h
    QCusFluentFramelessDelegate_p.h
    QCusFluentFramelessDelegate_win.h
    QCusFluentWidgetItem_p.h
    QCusFluentTitleBar.h
    QCusFluentDialog.h
    QCusFluentSeparator_p.h
    QCusFluentToolButton_p.h
    QCusFluentWindow.h
    QCusFluentNavigationBar.h
    QCusFluentNavigationSidePane.h 
    QCusFluentNavigationImageButton_p.h
    QCusFluentUserButton_p.h
    QCusFluentMessageTipButton.h
    QCusFluentMessageTipHelper_p.h
    QCusFluentAbstractMessageTipModel.h
    QCusFluentMessageTipView.h
    QCusFluentAbstractMessageTipView.h
    QCusFluentMessageViewDelegate_p.h
    QCusFluentMessageTipData.h
    
    Fluent/Router.h
    Fluent/GalleryInterface.h
    Fluent/IconInterface.h
    Fluent/HomeInterface.h
    Fluent/Pivot.h
    Fluent/NavigationWidget.h
    Fluent/NavigationToolTip.h
    Fluent/NavigationPanel.h
    Fluent/NavigationBar.h
    Fluent/FluentIcon.h
    Fluent/FluentGlobal.h
    Fluent/ScrollArea.h
    Fluent/PushButton.h
    Fluent/ToolTip.h
    Fluent/Animation.h
    Fluent/Theme.h
    Fluent/Screen.h
    Fluent/StyleSheet.h
    Fluent/ImageLabel.h
    Fluent/Flyout.h
    Fluent/AnimationPrivate.h
    Fluent/ScrollBar.h
    Fluent/TextEdit.h
    Fluent/ToolButton.h
    Fluent/RoundMenu.h
    Fluent/MenuActionListWidget.h
    Fluent/ThemePrivate.h
    Fluent/TextWrapPrivate.h
    Fluent/TextWrap.h
    Fluent/Label.h
    Fluent/ScrollBarStyle.h 
    Fluent/ScrollBarPrivate.h
    Fluent/MenuAnimation.h
    Fluent/MenuItemDelegate.h
    Fluent/RoundMenuPrivate.h
    Fluent/SubMenuItemWidget.h
    Fluent/StackedWidget.h
    Fluent/FlowLayout.h 
    Fluent/IconWidget.h
    Fluent/LineEdit.h
    Fluent/LinkCard.h
    Fluent/SampleCard.h
    Fluent/CardWidget.h 
)

set(SOURCES_FILES
    main.cpp
    mainwindow.cpp
    QCusFluentApplication.cpp
    QCusFluentStyleSheet.cpp
    QCusFluentFramelessDelegate_win.cpp
    QCusFluentWidgetItem.cpp
    QCusFluentMaximumSizeButton.cpp
    QCusFluentUtil.cpp
    QCusFluentTitleBar.cpp
    QCusFluentDialog.cpp
    QCusFluentSeparator.cpp
    QCusFluentNavigationImageButton.cpp
    QCusFluentToolButton.cpp
    QCusFluentWindow.cpp
    QCusFluentNavigationBar.cpp
    QCusFluentNavigationSidePane.cpp
    QCusFluentUserButton.cpp
    QCusFluentMessageTipButton.cpp
    QCusFluentMessageTipHelper.cpp
    QCusFluentAbstractMessageTipModel.cpp 
    QCusFluentMessageTipView.cpp 
    QCusFluentAbstractMessageTipView.cpp
    QCusFluentMessageViewDelegate.cpp
    QCusFluentMessageTipData.cpp 

    Fluent/GalleryInterface.cpp
    Fluent/IconInterface.cpp
    Fluent/HomeInterface.cpp
    Fluent/Pivot.cpp
    Fluent/NavigationWidget.cpp
    Fluent/NavigationToolTip.cpp
    Fluent/NavigationPanel.cpp
    Fluent/NavigationBar.cpp
    Fluent/FluentIcon.cpp
    Fluent/ScrollArea.cpp
    Fluent/PushButton.cpp
    Fluent/ToolTip.cpp
    Fluent/Animation.cpp
    Fluent/Theme.cpp
    Fluent/Screen.cpp
    Fluent/StyleSheet.cpp
    Fluent/ImageLabel.cpp
    Fluent/Flyout.cpp
    Fluent/AnimationPrivate.cpp
    Fluent/ScrollBar.cpp
    Fluent/TextEdit.cpp
    Fluent/ToolButton.cpp
    Fluent/RoundMenu.cpp
    Fluent/MenuActionListWidget.cpp
    Fluent/ThemePrivate.cpp
    Fluent/TextWrapPrivate.cpp
    Fluent/TextWrap.cpp
    Fluent/Label.cpp
    Fluent/ScrollBarStyle.cpp 
    Fluent/ScrollBarPrivate.cpp
    Fluent/MenuAnimation.cpp
    Fluent/MenuItemDelegate.cpp
    Fluent/RoundMenuPrivate.cpp
    Fluent/SubMenuItemWidget.cpp
    Fluent/StackedWidget.cpp
    Fluent/FlowLayout.cpp
    Fluent/IconWidget.cpp
    Fluent/LineEdit.cpp
    Fluent/Router.cpp
    Fluent/LinkCard.cpp
    Fluent/SampleCard.cpp
    Fluent/CardWidget.cpp

)

# 获取 UI 文件
file(GLOB CUS_FLUENT_UIS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

set(RCCS
    Resource.qrc
)


# 3. 主程序目标
add_executable(UniversalItem WIN32
    ${CUS_FLUENT_UIS}
    ${HEADER_FILES}
    ${SOURCES_FILES}
    ${RCCS}
)

set_target_properties(UniversalItem PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

# IDE 源码分组
source_group("Ui" FILES ${CUS_FLUENT_UIS})
source_group("Res" FILES ${RCCS})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCES_FILES})

target_include_directories(UniversalItem
    PRIVATE
    # 项目内部的包含路径
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Fluent
)


target_link_libraries(UniversalItem PRIVATE 
    ${QT_PACKAGE}::Core 
    ${QT_PACKAGE}::Gui
    ${QT_PACKAGE}::Widgets 
    ${QT_PACKAGE}::Svg
    ${QT_PACKAGE}::Network 
    ${QT_PACKAGE}::WinExtras
    ${QT_PACKAGE}::Xml
)

# ==============================================================================
# 4. 安装部分 (INSTALL)
# ==============================================================================

# 安装主程序可执行文件
install(TARGETS UniversalItem
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
)

# (Windows 专用) 自动化部署 Qt 依赖库
if(WIN32)
    # 查找 windeployqt 工具
    if(Qt6_FOUND)
        set(deploy_tool_target Qt6::windeployqt)
    else()
        # Qt5 需要手动查找路径
        get_target_property(_qmake_executable ${QT_PACKAGE}::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
        set(deploy_tool_target ${WINDEPLOYQT_EXECUTABLE})
    endif()

    if(TARGET ${deploy_tool_target} OR EXISTS "${deploy_tool_target}")
        install(CODE "
            execute_process(COMMAND \"${deploy_tool_target}\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/CustomFluent.exe\")
        ")
    endif()
endif()

# ==============================================================================

# 5. 演示运行/调试配置
option(QTFLUENT_DEMO_SUPPRESS_KNOWN_PAINT_WARNINGS "Suppress known Qt viewport paint warnings" ON)

if (QTFLUENT_DEMO_SUPPRESS_KNOWN_PAINT_WARNINGS)
    add_custom_target(run-UniversalItem
        COMMAND ${CMAKE_COMMAND} -E env QTFLUENT_SUPPRESS_KNOWN_PAINT_WARNINGS=1 $<TARGET_FILE:UniversalItem>
        DEPENDS UniversalItem
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
    )
endif()

# Visual Studio 调试环境变量设置
set_property(TARGET UniversalItem PROPERTY VS_DEBUGGER_ENVIRONMENT "QTFLUENT_SUPPRESS_KNOWN_PAINT_WARNINGS=$<IF:$<BOOL:${QTFLUENT_DEMO_SUPPRESS_KNOWN_PAINT_WARNINGS}>,1,0>")