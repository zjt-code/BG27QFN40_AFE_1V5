################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.c \
D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.c 

OBJS += \
./gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.o \
./gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.o 

C_DEPS += \
./gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.d \
./gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.o: D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.c gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32BG27C230F768IM40=1' '-DSL_APP_PROPERTIES=1' '-DBOOTLOADER_APPLOADER=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DRTT_USE_ASM=0' '-DSEGGER_RTT_SECTION="SEGGER_RTT"' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\include" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\config" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\config\btconf" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\autogen" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/Device/SiliconLabs/EFR32BG27/Include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/common/util/app_assert" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/common/util/app_log" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/common/util/app_timer" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//protocol/bluetooth/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/common/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//protocol/bluetooth/bgcommon/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//protocol/bluetooth/bgstack/ll/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/bootloader" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/bootloader/api" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/CMSIS/Core/Include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_cryptoacc_library/include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_cryptoacc_library/src" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/device_init/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/emdrv/common/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/emlib/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/bluetooth/common/gatt_service_device_information" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/bluetooth/common/in_place_ota_dfu" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/iostream/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_mbedtls_support/config" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_mbedtls_support/config/preset" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_mbedtls_support/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/third_party/mbedtls/include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/third_party/mbedtls/library" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/mpu/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/emdrv/nvm3/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/power_manager/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_psa_driver/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/common" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/ble" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/ieee802154" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/wmbus" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/zwave" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/sidewalk" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/pa-conversions" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/pa-conversions/efr32xg27" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/rail_util_power_manager_init" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/rail_util_pti" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/se_manager/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/se_manager/src" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/third_party/segger/systemview/SEGGER" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/silicon_labs/silabs_core/memory_manager" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/common/toolchain/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/system/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/sleeptimer/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_protocol_crypto/src" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.o: D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.c gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32BG27C230F768IM40=1' '-DSL_APP_PROPERTIES=1' '-DBOOTLOADER_APPLOADER=1' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DRTT_USE_ASM=0' '-DSEGGER_RTT_SECTION="SEGGER_RTT"' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\include" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\config" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\config\btconf" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5\autogen" -I"E:\SimplicityStudio\v5_workspace\BG27QFN40_AFE_1V5" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/Device/SiliconLabs/EFR32BG27/Include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/common/util/app_assert" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/common/util/app_log" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/common/util/app_timer" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//protocol/bluetooth/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/common/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//protocol/bluetooth/bgcommon/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//protocol/bluetooth/bgstack/ll/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/bootloader" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/bootloader/api" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/CMSIS/Core/Include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_cryptoacc_library/include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_cryptoacc_library/src" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/device_init/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/emdrv/common/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/emlib/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/bluetooth/common/gatt_service_device_information" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//app/bluetooth/common/in_place_ota_dfu" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/iostream/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_mbedtls_support/config" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_mbedtls_support/config/preset" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_mbedtls_support/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/third_party/mbedtls/include" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/third_party/mbedtls/library" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/mpu/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/emdrv/nvm3/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/power_manager/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_psa_driver/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/common" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/ble" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/ieee802154" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/wmbus" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/zwave" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/protocol/sidewalk" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/pa-conversions" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/pa-conversions/efr32xg27" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/rail_util_power_manager_init" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/radio/rail_lib/plugin/rail_util_pti" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/se_manager/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/se_manager/src" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/third_party/segger/systemview/SEGGER" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//util/silicon_labs/silabs_core/memory_manager" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/common/toolchain/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/system/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/service/sleeptimer/inc" -I"D:/GSDK440/gecko_sdk4.4.0.zip/gecko_sdk4.4.0//platform/security/sl_component/sl_protocol_crypto/src" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.0/util/third_party/segger/systemview/SEGGER/SEGGER_RTT_printf.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


