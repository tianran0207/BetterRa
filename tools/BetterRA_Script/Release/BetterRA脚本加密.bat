@echo off
echo =====================================
echo = BetterRA 脚本加密程序 欢迎使用... =
echo =====================================
if not exist Encrypt (
	mkdir Encrypt
)
set Script_File=0
set format=BIT
for %%i in (*.txt) do (
	set /a Script_File+=1
)
echo [提示] 目录下共有 %Script_File% 个txt文件待加密...
if %Script_File% == 0 (
	echo [提示] 请把脚本文件放入本文件同目录中...
	goto end
)
echo;
set/p key=请输入秘钥(必须为8位):
set/p format=输出格式(HEX\BIT)(默认BIT):

for %%i in (*.txt) do (
	echo [提示] 正在加密 %%i 到 Encrypt\%%~ni.txt 输出格式 %format%
	BetterRA_Script.exe -e %%~ni.txt Encrypt\%%~ni.txt %key% %format%
)

echo;
echo =====================================
echo = 加密完毕！                        =
echo =====================================
:end
pause