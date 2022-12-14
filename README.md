# ServerMain

SM 服务器启动器，用于 Minecraft Java Edition 以及其他命令行下的服务器。

## 如何使用

将二进制文件 `ServerMain.exe` 与配置文件 `config.txt` 放入同一目录下，执行程序 `ServerMain.exe` 即可。

## 关于配置文件

配置文件会在进程每次重启时重载。

### 配置文件 V2（V1.4 及以后）

以下是配置文件的默认内容：

```json
{
    "readme": [
        "在 readme 内的内容将被忽略。若要了解更多信息，请访问 https://github.com/HJFunnyMinecraft/ServerMain/README.md",
        "Contents inside readme will be ignore. To learn more, please visit https://github.com/HJFunnyMinecraft/ServerMain/README.md"
    ],
    "command": "",
    "timeout": 5
}
```

#### 参数名解释：

- `command`：命令行启动参数，默认为空。
- `timeout`：服务器崩溃后等待几秒重启，设为 -1 为不自动重启，默认为 5。

语法内容同 JSON 文件

### 配置文件 V1（V1.3 及以前）

以下是配置文件的默认内容：

```
#Config_Start.
command= java -Xmx2g -Xms1g -jar paper-1.17.1.jar
timeout= 5
#Config_End.

更多内容请访问；https://github.com/HJFunnyMinecraft/ServerMain/README.md
```

“Config_Start.”之前、“Config_End.”之后的内容将被忽略。

#### 参数名解释：

- [command=] 命令行启动参数，默认为空。
- [timeout=] 服务器崩溃后等待几秒重启，设为 -1 为不自动重启，默认为 5。

#### 语法解释：

1. 每一行的格式为 “[参数名]= [参数值]”，注意等号前无空格，等号后有空格。
2. 配置文件每行一条。
3. 配置文件必须包含至少一个 “Config_Start.” 和 “Config_End“（称之为配置组），且 “Config_Start.” 必须在 “Config_End.” 之前。
4. 如果填入了多个配置组，只会载入第一个。
5. 如果找不到某个参数，将以默认值替代。


