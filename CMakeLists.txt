cmake_minimum_required(VERSION 3.15)

project(SHM_BL LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.project_importers.stm32.cubemx SOURCE_PROJECT SHM_BL.ioc)

add_bsp_based_executable(NAME SHM_BL
	GENERATE_BIN
	GENERATE_MAP)

