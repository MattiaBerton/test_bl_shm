cmake_minimum_required(VERSION 3.15)

project(SHM_BL LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.project_importers.stm32.cubemx
	SOURCE_PROJECT SHM_BL.ioc
	HWREGISTER_LIST_FILE STM32G030.mcudef)

bsp_include_directories(inc)
add_bsp_based_executable(
	NAME SHM_BL
	GENERATE_BIN
	GENERATE_MAP
	SOURCES
		${BSP_ROOT}/src/app_execute.c
		${BSP_ROOT}/src/common.c
		${BSP_ROOT}/src/crc.c
		${BSP_ROOT}/src/flash_if.c
		${BSP_ROOT}/src/menu.c
		${BSP_ROOT}/src/ymodem.c)

