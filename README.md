# OV5640 定时拍照上传

基于 ESP32-S3 + OV5640 的定时拍照并通过 HTTP 上传至本地服务器的小项目，主要是淘宝店家没给我OV5640相关配置文件，苦苦弄了一个晚上，现在写出来。  

## 硬件

- **MCU**: ESP32-S3（8MB PSRAM, 16MB Flash）
- **摄像头**: OV5640
- **引脚映射**：

| GPIO | 信号 | GPIO | 信号 |
|------|------|------|------|
| 4 | SDA | 11 | D0 |
| 5 | SCL | 9 | D1 |
| 6 | VSYNC | 8 | D2 |
| 7 | HREF | 10 | D3 |
| 13 | PCLK | 12 | D4 |
| 15 | XCLK | 18 | D5 |
| - | - | 17 | D6 |
| - | - | 16 | D7 |

## 功能

- 每 5 秒自动拍摄一张 VGA (640×480) JPEG 照片
- 通过 WiFi 将照片 POST 到局域网服务器
- 垂直翻转（OV5640 默认图像倒置）

## 项目结构

```
main/
├── main.c          # 入口，定时调度
├── wifi_mgr.h/c    # WiFi STA 连接（15s 超时）
├── cam.h/c         # OV5640 初始化、拍照、vflip
├── upload.h/c      # HTTP POST JPEG 上传
└── CMakeLists.txt
components/
└── BSP/camera/     # OV5640 驱动封装
```

## 配置

复制 `main/secrets_example.h` 为 `main/secrets.h` 并填入实际值：

```bash
cp main/secrets_example.h main/secrets.h
```

编辑 `main/secrets.h`：
```c
#define WIFI_SSID     "你的WiFi名"
#define WIFI_PASSWORD "你的WiFi密码"
#define UPLOAD_URL    "http://你的IP:8888/upload"
```

## 照片接收

运行 Flask 接收服务：

```bash
python3 save_images.py
```

浏览器访问 `http://<IP>:8888` 查看照片。
