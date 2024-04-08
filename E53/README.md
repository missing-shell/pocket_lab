## 待排查错误

错误代码102通常表示一个资源不可用的错误，这可能是由于硬件资源（如GPIO引脚或I2C总线）被其他任务占用，或者是由于配置错误。
要解决这个问题，你需要进一步调查：

检查硬件资源：确保你的LCD逻辑分析器需要的所有硬件资源（如GPIO引脚或I2C总线）都没有被其他任务占用。
检查配置：确保你的LCD逻辑分析器的配置是正确的，包括GPIO引脚、I2C总线和其他相关的配置。
检查代码：查看你的代码，特别是与LCD逻辑分析器相关的部分，看看是否有任何错误的配置或初始化。
如果你能提供更多的上下文信息，如代码片段或更详细的日志，我可以提供更具体的建议。

## 前置设置

- menuconfig

```txt
Flash size：选择`16MB`
Partition Table  选择`Custom partition table CSV`
Custom partition CSV file  选择`partitions.csv`

# lvgl相关
Color depth.  选择`RGB565`
勾选`Swap the 2 bytes of RGB565 color. Useful if the display has an 8-bit interface (e.g. SPI).`
勾选`Enable Montserrat 16`

```

## 如何使用该项目

该存储库包含子模块，您应该添加“--recursive”来克隆整个项目。

此代码针对esp32s3 (N16R8) 构建

将其他正常运行项目的.vscode文件夹复制到该项目的根目录中，重新选择串口和芯片
