#include "ServerMain.h"
using namespace std;

int RestartCount;
tm* NowTime;
tm* LastRestartTime;
LocalConfig MainConfig;

void ReadConfigV1() {
    // 读取配置文件 V1
    MainConfig.Command = "";
    MainConfig.RestartTimeout = 5;
    color(COLOR_PROCESS);
    puts("[PROC] 读取配置文件 config.txt ...");
    /*if(!fileExits("config.txt")) {
        color(COLOR_ERROR);
        puts("[ERR!] 配置文件不存在！将在10秒后重试 ……");
        timeout(10);
        ReadConfig();
        return;
    }
    if(!fileRead("config.txt")) {
        color(COLOR_ERROR);
        puts("[ERR!] 配置文件无法读取！将在10秒后重试 ……");
        timeout(10);
        ReadConfig();
        return;
    }*/
    ifstream in("config.txt");
    string reader;
    color(COLOR_SUCCESS);
    puts("[SUCC] 配置文件V1打开成功！");
    while(reader != "#Config_Start.") {
        in >> reader;
        if(in.eof()) {
            color(COLOR_ERROR);
            puts("[ERR!] 配置文件没有任何配置组！将在10秒后重试 ……");
            timeout(10);
            ReadConfigV1();
            return;
        }
    }
    while(reader != "#Config_End.") {
        in >> reader;
        if(in.eof()) {
            color(COLOR_ERROR);
            puts("[ERR!] 配置文件没有有效结尾！将在10秒后重试 ……");
            timeout(10);
            ReadConfigV1();
            return;
        }
        if(reader == "command=") {
            stringstream command;
            in >> reader;
            while(reader != "#Config_End." && reader != "timeout=") {
                command << reader << " ";
                in >> reader;
            }
            MainConfig.Command = command.str();
        }
        if(reader == "timeout=") {
            int tmout;
            in >> tmout;
            MainConfig.RestartTimeout = tmout;
        }
    }
    in.close();
    color(COLOR_SUCCESS);
    puts("[SUCC] 配置文件V1读取成功：");
    cout << "\tCommand: " << MainConfig.Command << endl;
    cout << "\tRestartTimeout: " << MainConfig.RestartTimeout << endl;
    puts("");
}

void ReadConfig() {
    // 读取配置文件 V2
    MainConfig.Command = "";
    MainConfig.RestartTimeout = 5;
    color(COLOR_PROCESS);
    puts("[PROC] 读取配置文件 config.json ...");
    if(!fileExits("config.json")) {
        if(fileExits("config.txt") && fileRead("config.txt")) {
            puts("[PROC] 检测到配置文件V1 (config.txt)，转换为V2。");
            ReadConfigV1();
            puts("[PROC] 开始转换为V2。");
            ofstream out("config.json");
            out << "{" << endl;
            out << "    \"readme\": [" << endl;
            out << "        \"在 readme 内的内容将被忽略。若要了解更多信息，请访问 https://github.com/HJFunnyMinecraft/ServerMain/README.md\"," << endl;
            out << "        \"Contents inside readme will be ignore. To learn more, please visit https://github.com/HJFunnyMinecraft/ServerMain/README.md\"" << endl;
            out << "    ]," << endl;
            out << "    \"command\": \"" << MainConfig.Command << "\"," << endl;
            out << "    \"timeout\": " << MainConfig.RestartTimeout << endl;
            out << "}" << endl;
            out.close();
            color(7);
            puts("配置文件V1转V2已结束：config.json");
            puts("请访问 https://github.com/HJFunnyMinecraft/ServerMain/README.md，检查转换结果。");
            puts("当配置完成时，请关闭编辑器……");
            system("notepad config.json");
        } else {
            puts("[PROC] config.json 不存在，加载默认配置模板。");
            ofstream out("config.json");
            out << "{" << endl;
            out << "    \"readme\": [" << endl;
            out << "        \"在 readme 内的内容将被忽略。若要了解更多信息，请访问 https://github.com/HJFunnyMinecraft/ServerMain/README.md\"," << endl;
            out << "        \"Contents inside readme will be ignore. To learn more, please visit https://github.com/HJFunnyMinecraft/ServerMain/README.md\"" << endl;
            out << "    ]," << endl;
            out << "    \"command\": \"\"," << endl;
            out << "    \"timeout\": 5" << endl;
            out << "}" << endl;
            out.close();
            color(7);
            puts("默认配置模板已创建：config.json");
            puts("请访问 https://github.com/HJFunnyMinecraft/ServerMain/README.md，配置此程序。");
            puts("当配置完成时，请关闭编辑器……");
            system("notepad config.json");
        }
        ReadConfig();
        return;
    }
    if(!fileRead("config.json")) {
        color(COLOR_ERROR);
        puts("[ERR!] 配置文件无法读取！将在10秒后重试 ……");
        timeout(10);
        ReadConfig();
        return;
    }
    ifstream in("config.json");
    Json::Reader reader;
    Json::Value root;
    color(COLOR_SUCCESS);
    puts("[SUCC] 配置文件打开成功！");
    if(reader.parse(in, root)) {
        MainConfig.Command = root["command"].asString();
        MainConfig.RestartTimeout = root["timeout"].asInt();
    }
    in.close();
    color(COLOR_SUCCESS);
    puts("[SUCC] 配置文件读取成功：");
    cout << "\tCommand: " << MainConfig.Command << endl;
    cout << "\tRestartTimeout: " << MainConfig.RestartTimeout << endl;
    puts("");
}

int main() {
    system("chcp 65001"); // 使用 UTF-8 格式化控制台
    system("echo off");
    system("cls");
    setTitle("SM服务器启动器 V1.4");
    color(COLOR_SUCCESS);
    puts("[SUCC] 软件启动成功！");
    puts("");
    color(COLOR_TIPS);
    puts("========== SM服务器启动器 ==========");
    puts("版本：V1.4");
    puts("构建日期：2022-12-14");
    puts("作者：CodeZhangBorui");
    puts("");
    puts("");
    while(1) {
        ReadConfig();
        NowTime = GetNowTime();
        LastRestartTime = NowTime;
        stringstream title;
        title
                << "SM服务器启动器 V1.4 [ 进程退出次数："
                << RestartCount
                << " 上次重启："
                << GetDateString(LastRestartTime)
                << " "
                << GetTimeString(LastRestartTime)
                << " ]";
        setTitle(title.str());
        color(COLOR_TIPS);
        puts("> 启动进程……");
        puts("");
        color(COLOR_DEFAULT);
        system(MainConfig.Command.c_str());
        puts("");
        color(COLOR_PROCESS);
        puts("检测到进程终止，即将重启 ……");
        puts("若希望停止进程，请直接关闭此窗口。");
        timeout(MainConfig.RestartTimeout);
        RestartCount++;
        puts("");
    }
}
